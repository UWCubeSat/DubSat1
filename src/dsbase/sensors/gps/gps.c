#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "core/uart.h"
#include "core/debugtools.h"

#include "gps.h"
#include "BufferedReader.h"
#include "crc.h"

typedef enum reader_state
{
    State_Sync, State_Header, State_Message, State_ArrayLength,
    State_ArrayMessage, State_CRC
} reader_state;

FILE_STATIC hBus uartHandle;
FILE_STATIC uint8_t buffer[GPS_RX_BUFFER_LENGTH];
FILE_STATIC gps_health health;
FILE_STATIC GPSPackage package;

void gpsInit()
{
    // zero out health struct
    health.bad_crc = 0;
    health.skipped = 0;
    health.oversized = 0;
    health.buck_status = 0;

    // initialize the reader
    uartHandle = uartInit(ApplicationUART, 0, Speed_9600);
    BufferedReaderInit(uartHandle, buffer, GPS_RX_BUFFER_LENGTH);

    // set GPS switch and buck converter to be off initially
    GPS_ENABLE_OUT &= ~GPS_ENABLE_BIT;
    gpsSetBuck(FALSE);

    // configure GPIO pins
    GPS_ENABLE_DIR |= GPS_ENABLE_BIT;   // output
    BUCK_ENABLE_DIR |= BUCK_ENABLE_BIT; // output
    RESET_DIR |= RESET_BIT;             // output
    BUCK_GOOD_DIR &= ~BUCK_GOOD_BIT;    // input
}

void gpsSetBuck(uint8_t enable)
{
    if (enable)
    {
        BUCK_ENABLE_OUT |= BUCK_ENABLE_BIT;
    }
    else
    {
        BUCK_ENABLE_OUT &= ~BUCK_ENABLE_BIT;
    }
}

uint8_t gpsIsBuckEnabled()
{
    if (BUCK_ENABLE_OUT & BUCK_ENABLE_BIT)
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t gpsIsBuckGood()
{
    if (BUCK_GOOD_IN & BUCK_GOOD_BIT)
    {
        return TRUE;
    }
    return FALSE;
}

void gpsSetPower(uint8_t on)
{
    uint8_t isOn = gpsIsPowerEnabled();
    if (on && !isOn)
    {
        BufferedReaderFlush();
        GPS_ENABLE_OUT |= GPS_ENABLE_BIT;
    }
    else if (!on && isOn)
    {
        GPS_ENABLE_OUT &= ~GPS_ENABLE_BIT;
        BufferedReaderFlush();
    }
}

uint8_t gpsIsPowerEnabled()
{
    if (GPS_ENABLE_OUT & GPS_ENABLE_BIT)
    {
        return TRUE;
    }
    return FALSE;
}

void gpsSetReset(uint8_t active)
{
    if (active)
    {
        // active low
        RESET_OUT &= ~RESET_BIT;
    }
    else
    {
        RESET_OUT |= RESET_BIT;
    }
}

uint8_t gpsIsResetActive()
{
    if (RESET_OUT & RESET_BIT)
    {
        return FALSE;
    }
    return TRUE;
}

// read from the BufferedReader into another buffer while also doing a crc
// returns the total bytes read
FILE_STATIC uint8_t readAndCrc(uint8_t *buf, uint8_t bytesRead, uint8_t numToRead, uint32_t *crc)
{
    reader_index numRead = BufferedReaderRead(buf,
                                              numToRead - bytesRead,
                                              bytesRead);
    *crc = continueCrc32(*crc, numRead, buf + bytesRead);
    return bytesRead + numRead;
}

GPSPackage *gpsRead()
{
#ifdef __SKIP_GPS_TRAFFIC__
    return NULL;
#endif // __SKIP_GPS_TRAFFIC__

    static uint8_t bytesRead = 0;
    static reader_state state = State_Sync;
    static uint32_t crc = 0;

    // if we missed some bytes, revert back to sync state
    if (BufferedReaderOverrun())
    {
        health.skipped++;
        state = State_Sync;
        bytesRead = 0;
        BufferedReaderFlush();
    }

    switch (state)
    {
    case State_Sync:
    {
        // sync using magic sync bytes 170, 68, 18
        uint8_t byte[1];
        if(BufferedReaderRead(byte, 1, 0))
        {
            uint8_t *buf = package.header.sync;
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = byte[0];
            if (buf[0] == 170 && buf[1] == 68 && buf[2] == 18)
            {
                bytesRead = 3;
                crc = calculateBlockCrc32(3, buf);
                state = State_Header;
            }
        }
        break;
    }
    case State_Header:
    {
        // write incoming bytes into the header
        bytesRead = readAndCrc((uint8_t *) &package.header, bytesRead, sizeof(GPSHeader), &crc);

        // once the header is read, move on to the message
        if (bytesRead >= sizeof(GPSHeader))
        {
            bytesRead = 0;

            /*
             * Check if the message fits in the length limit. Messages with a
             * flexible array member don't always fit.
             */
            if (package.header.messageLength > sizeof(GPSMessage) + FLEX_ARRAY_BUFFER_LENGTH)
            {
                debugPrintF("message too big: %u\r\n", package.header.messageLength);
                health.oversized++;
                state = State_Sync;
            }
            else
            {
                state = State_Message;
            }
        }
        break;
    }
    case State_Message:
    {
        // write incoming bytes into the message
        bytesRead = readAndCrc((uint8_t *) &package.message, bytesRead,
                               package.header.messageLength, &crc);

        // once the message is read, move on to the crc
        if (bytesRead >= package.header.messageLength)
        {
            bytesRead = 0;
            state = State_CRC;
        }
        break;
    }
    case State_CRC:
    {
        // write directly into the crc
        static uint32_t readCrc = 0;
        uint8_t *buf = (uint8_t *) &readCrc;
        bytesRead += BufferedReaderRead(buf, sizeof(readCrc) - bytesRead, bytesRead);

        // once the crc is read, restart the process and return the package
        if (bytesRead >= sizeof(readCrc))
        {
            bytesRead = 0;
            state = State_Sync;

            if (readCrc == crc)
            {
                // process response messages
                if (package.header.messageType & 0b1000000)
                {
                    // check if the response is OK
                    gps_enum responseId = *((gps_enum *) &package.message);
                    if (responseId != 1) {
                        // TODO use this to confirm a message came through
                    }
                }
                else
                {
                    return &package;
                }
            }

            debugPrintF("invalid crc\r\n");
            health.bad_crc++;
        }
        break;
    }
    }

    return NULL;
}

void gpsSendCommand(uint8_t *command)
{
#ifdef __SKIP_GPS_TRAFFIC__
    return;
#endif // __SKIP_GPS_TRAFFIC__

    uartTransmit(uartHandle, command, strlen((char*) command));
}

void gpsSendBinaryCommand(gps_message_id messageId,
                          uint8_t *message,
                          uint16_t messageLength)
{
#ifdef __SKIP_GPS_TRAFFIC__
    return;
#endif // __SKIP_GPS_TRAFFIC__

    // populate header
    GPSHeader header;
    header.sync[0] = 0xAA;
    header.sync[1] = 0x44;
    header.sync[2] = 0x12;
    header.headerLength = sizeof(header);
    header.messageId = messageId;
    header.messageType = 0; // ???
    header.portAddress = 0xc0; // 0xc0: THISPORT
    header.messageLength = messageLength;
    // all other header fields are either ignored or recommended 0 on input
    header.sequence = 0;
    header.idleTime = 0;
    header.timeStatus = 0;
    header.week = 0;
    header.ms = 0;
    header.rxStatus = 0;
    header.reserved = 0;
    header.rxVersion = 0;

    // calculate crc
    uint32_t crc = calculateBlockCrc32(sizeof(header), (uint8_t *) &header);
    crc = continueCrc32(crc, messageLength, message);

    // transmit command
    uartTransmit(uartHandle, (uint8_t *) &header, sizeof(header));
    uartTransmit(uartHandle, message, messageLength);
    uartTransmit(uartHandle, (uint8_t *) &crc, sizeof(crc));
}

void gpsGetHealth(gps_health *h)
{
    health.buck_status = gpsIsBuckGood();
    *h = health;
}
