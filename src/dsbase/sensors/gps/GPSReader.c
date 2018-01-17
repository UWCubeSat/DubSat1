#define PARSER_BUFFER_LENGTH 10

#include <stdint.h>
#include <stdbool.h>

#include "core/debugtools.h"
#include "core/utils.h"
#include "core/uart.h"

#include "GPSReader.h"
#include "GPSPackage.h"
#include "GPSIDs.h"
#include "queue.h"
#include "crc.h"

typedef enum gps_state
{
    State_Sync, State_Header, State_Message, State_CRC
} gps_state;

typedef struct parser_entity
{
    gps_parser handler;
    gps_message_id id;
} parser_entity;

FILE_STATIC parser_entity parsers[PARSER_BUFFER_LENGTH];
FILE_STATIC uint8_t numParsers = 0;

FILE_STATIC gpsreader_health health;

// message queue
FILE_STATIC Queue queue;

// uart read callback
FILE_STATIC void readCallback(uint8_t rcvdbyte);

// custom parsing callbacks
FILE_STATIC void parseSatvis2(GPSMessage *message,
                              uint8_t rcvdbyte,
                              uint8_t bytesRead);

void GPSReaderInit(hBus uartHandle, GPSPackage *buffer, uint32_t bufferLength)
{
    health.bad_crc = 0;
    health.registration_errors = 0;
    health.skipped = 0;

    queue = CreateQueue((uint8_t *) buffer, sizeof(GPSPackage), bufferLength);
    uartRegisterRxCallback(uartHandle, readCallback);

    // register default parsers
    gpsRegisterParser(MSGID_SATVIS2, parseSatvis2);
}

FILE_STATIC void readCallback(uint8_t rcvdbyte)
{
    static uint8_t bytesRead = 0;
    static gps_state state = State_Sync;
    static uint32_t crc = 0;
    static uint32_t readCrc = 0;

    GPSPackage *p = (GPSPackage *) WriteQueue(&queue);
    if (p == NULL)
    {
        debugPrintF("GPS buffer out of memory\r\n");
        health.skipped++;

        // skipping a single byte this way will (probably) skip the whole
        // message. If some space is freed up in the queue, this should be able
        // to pick up the next message's sync bytes and carry on.
        bytesRead = 0;
        state = State_Sync;
        return;
    }

    switch (state)
    {
    case State_Sync:
    {
        // sync using magic sync bytes 170, 68, 18
        uint8_t *buf = p->header.sync;
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = rcvdbyte;
        if (buf[0] == 170 && buf[1] == 68 && buf[2] == 18)
        {
            bytesRead = 3;
            crc = calculateBlockCrc32(3, buf);
            state = State_Header;
        }
        break;
    }
    case State_Header:
    {
        // write directly into header
        uint8_t *buf = (uint8_t *) &p->header;
        buf[bytesRead] = rcvdbyte;
        crc = continueCrc32(crc, rcvdbyte);
        bytesRead++;

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
        // check if there is some custom parser for this message id
        uint8_t i = numParsers;
        bool usedParser = false;
        while (i-- != 0)
        {
            if (parsers[i].id == p->header.messageId)
            {
                (parsers[i].handler)(&p->message, rcvdbyte, bytesRead);
                usedParser = true;
                break;
            }
        }

        // otherwise write directly into the message struct
        if (!usedParser)
        {
            uint8_t *buf = (uint8_t *) &p->message;
            buf[bytesRead] = rcvdbyte;
        }

        crc = continueCrc32(crc, rcvdbyte);
        bytesRead++;

        // once the message is read, move on to the crc
        if (bytesRead >= p->header.messageLength)
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
        buf[bytesRead] = rcvdbyte;
        bytesRead++;

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
                PushQueue(&queue);
            }
            crc = 0;
            readCrc = 0;
            bytesRead = 0;
            state = State_Sync;
        }
        break;
    }
    }
}

GPSPackage *ReadGPSPackage()
{
    return (GPSPackage *) ReadQueue(&queue);
}

void PopGPSPackage()
{
    PopQueue(&queue);
}

bool gpsRegisterParser(gps_message_id messageId,
                       gps_parser parser)
{
    if (numParsers >= PARSER_BUFFER_LENGTH)
    {
        debugPrintF("too many parsers\r\n");
        health.registration_errors++;
        return false;
    }
    parsers[numParsers++] = (parser_entity){ parser, messageId };
    return true;
}

void GPSReaderGetHealth(gpsreader_health *h)
{
    *h = health;
}

FILE_STATIC void parseSatvis2(GPSMessage *message,
                              uint8_t rcvdbyte,
                              uint8_t bytesRead)
{
    if (bytesRead < 16) {
        uint8_t *buf = (uint8_t *) message;
        buf[bytesRead] = rcvdbyte;
    }
    // ignore everything after the first 16 bytes, which only includes
    // satellite-specific information
}
