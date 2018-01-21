#define RX_BUFFER_LENGTH 100

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
    State_Sync, State_Header, State_Message, State_CRC
} reader_state;

FILE_STATIC uint8_t buffer[RX_BUFFER_LENGTH];

FILE_STATIC gpsreader_health health;

FILE_STATIC GPSPackage package;

void GPSReaderInit(hBus uartHandle)
{
    health.bad_crc = 0;
    health.skipped = 0;

    BufferedReaderInit(uartHandle, buffer, RX_BUFFER_LENGTH);
}

GPSPackage *GPSReaderUpdate()
{
    static uint8_t bytesRead = 0;
    static reader_state state = State_Sync;
    static uint32_t crc = 0;
    static uint32_t readCrc = 0;

    static uint8_t readMessage;

    switch (state)
    {
    case State_Sync:
    {
        // sync using magic sync bytes 170, 68, 18
        uint8_t *buf = package.header.sync;
        uint8_t byte[1];
        if(BufferedReaderRead(byte, 1, 0))
        {
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
        // get next header bytes
        uint8_t *buf = (uint8_t *) &(package.header);
        reader_index numRead = BufferedReaderRead(buf,
                                                  sizeof(GPSHeader) - bytesRead,
                                                  bytesRead);

        // continue crc on read bytes
        crc = continueCrc32(crc, numRead, buf + bytesRead);

        bytesRead += numRead;

        // once the header is read, move on to the message
        if (bytesRead >= sizeof(GPSHeader))
        {
            bytesRead = 0;
            state = State_Message;
        }
        break;
    }
    case State_Message:
    {
        // get next message bytes
        uint8_t *buf = (uint8_t *) &(package.message);
        reader_index numRead = BufferedReaderRead(buf,
                                                  package.header.messageLength - bytesRead,
                                                  bytesRead);

        // continue crc on read bytes
        crc = continueCrc32(crc, numRead, buf + bytesRead);

        bytesRead += numRead;

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
        uint8_t *buf = (uint8_t *) &readCrc;
        bytesRead += BufferedReaderRead(buf, sizeof(readCrc) - bytesRead, bytesRead);

        // once the crc is read, restart the process
        if (bytesRead >= sizeof(readCrc))
        {
            if (readCrc != crc)
            {
                debugPrintF("invalid crc\r\n");
                health.bad_crc++;
            }
            else
            {
                readMessage = 1;
            }
            crc = 0;
            readCrc = 0;
            bytesRead = 0;
            state = State_Sync;
        }
        break;
    }
    }

    if (readMessage)
    {
        readMessage = 0;
        return &package;
    }

    return NULL;
}

void GPSReaderGetHealth(gpsreader_health *h)
{
    *h = health;
}
