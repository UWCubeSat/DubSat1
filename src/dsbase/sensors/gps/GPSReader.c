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

FILE_STATIC array_parser parsers[] = {
    { MSGID_SATVIS2, 16, 40 }
};

void GPSReaderInit(hBus uartHandle)
{
    health.bad_crc = 0;
    health.skipped = 0;

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
    static uint32_t numOfElements = 1;
    static array_parser *parser = NULL;

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
            package.sequence = 0;
            state = State_Message;

            // assign a parser if applicable
            size_t i = sizeof(parsers)/sizeof(array_parser);
            while (i--)
            {
                if (parsers[i].msgId == package.header.messageId)
                {
                    parser = &parsers[i];
                    break;
                }
            }
        }
        break;
    }
    case State_Message:
    {
        // if the message is an array, read up to the array. Otherwise read the entire message.
        uint16_t lengthLimit = parser == NULL ? package.header.messageLength : parser->arrayIndex;

        // write incoming bytes into the message
        bytesRead = readAndCrc((uint8_t *) &package.message, bytesRead, lengthLimit, &crc);

        // once the message is read, move on to the crc
        if (bytesRead >= lengthLimit)
        {
            bytesRead = 0;
            if (parser == NULL)
            {
                state = State_CRC;
            }
            else
            {
                package.sequence = 0;

                // assume the last 4 bytes before the array are the array length
                uint8_t *buf = (uint8_t *) &package.message;
                numOfElements = *((uint32_t *) buf + bytesRead - sizeof(uint32_t));
                bytesRead = 0;

                // may skip to CRC state if the array is empty
                if (numOfElements != 0)
                {
                    state = State_ArrayMessage;

                    // returns non-array part first, without checking CRC
                    return &package;
                }
                else
                {
                    state = State_CRC;
                }
            }

        }
        break;
    }
    case State_ArrayMessage:
    {
        /*
         * Array parsing makes several assumptions:
         *   - arrays are always at the end of a message (this is false for at
         *     least 3 messages, none of which seem important).
         *   - the number of elements in an array is represented as a uint32_t.
         *   - the number of elements in an array always comes just before the
         *     array itself.
         *   - the message is never corrupted (CRC checking is unimplemented for
         *     array messages).
         */

        // write incoming bytes into the array element
        bytesRead = readAndCrc((uint8_t *) &package.message, bytesRead, parser->size, &crc);

        if (bytesRead >= parser->size)
        {
            package.sequence++;
            // check if array is read
            // using > instead of >= because sequence is 1-based
            if (package.sequence > numOfElements)
            {
                // TODO do CRC check and possibly reject previous messages
                state = State_Sync;
                crc = 0;
                bytesRead = 0;
                parser = NULL;
            }
            return &package;
        }
        break;
    }
    case State_CRC:
    {
        // write directly into the crc
        static uint32_t readCrc = 0;
        uint8_t *buf = (uint8_t *) &readCrc;
        bytesRead += BufferedReaderRead(buf, sizeof(readCrc) - bytesRead, bytesRead);

        // once the crc is read, restart the process
        if (bytesRead >= sizeof(readCrc))
        {
            crc = 0;
            bytesRead = 0;
            state = State_Sync;
            parser = NULL;

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
