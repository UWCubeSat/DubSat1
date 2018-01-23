#define RX_BUFFER_LENGTH 20

#include <stdint.h>
#include <stdbool.h>

#include "core/debugtools.h"
#include "core/utils.h"
#include "core/uart.h"

#include "GPSReader.h"
#include "GPSPackage.h"
#include "GPSIDs.h"
#include "crc.h"
#include "BufferedReader.h"

typedef enum reader_state
{
    State_Sync, State_Header, State_Message, State_ArrayLength,
    State_ArrayMessage, State_CRC
} reader_state;
FILE_STATIC uint8_t buffer[RX_BUFFER_LENGTH];
FILE_STATIC gpsreader_health health;
FILE_STATIC GPSPackage package;

void GPSReaderInit(hBus uartHandle)
{
    health.bad_crc = 0;
    health.skipped = 0;
    health.oversized = 0;

    BufferedReaderInit(uartHandle, buffer, RX_BUFFER_LENGTH);
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

GPSPackage *GPSReaderUpdate()
{
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
                return &package;
            }

            debugPrintF("invalid crc\r\n");
            health.bad_crc++;
        }
        break;
    }
    }

    return NULL;
}

void GPSReaderGetHealth(gpsreader_health *h)
{
    *h = health;
}
