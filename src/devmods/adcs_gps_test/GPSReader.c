#include <stdint.h>

#include "core/debugtools.h"
#include "core/utils.h"
#include "core/uart.h"

#include "GPSReader.h"
#include "GPSPackage.h"
#include "queue.h"

typedef enum gps_state
{
    State_Sync,
    State_Header,
    State_Message,
    State_CRC
} gps_state;

FILE_STATIC Queue queue;

FILE_STATIC void readCallback(uint8_t rcvdbyte);

void GPSReaderInit(hBus uartHandle, GPSPackage *buffer, uint32_t bufferLength)
{
    queue = CreateQueue((uint8_t *) buffer, sizeof(GPSPackage), bufferLength);
    uartRegisterRxCallback(uartHandle, readCallback);
}

FILE_STATIC void readCallback(uint8_t rcvdbyte)
{
    static uint8_t bytesRead = 0;
    static gps_state state = State_CRC;

    GPSPackage *p = (GPSPackage *) WriteQueue(&queue);
    if (p == NULL) {
        debugPrintF("GPS buffer out of memory\r\n");

        // skipping a single byte this way will (probably) skip the whole
        // message. If some space is freed up in the queue, this should be able
        // to pick up the next message's sync bytes and carry on.
        return;
    }

    switch (state)
    {
        case State_Sync: {
            // sync using magic sync bytes 170, 68, 18
            uint8_t *buf = p->header.sync;
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = rcvdbyte;
            if (buf[0] == 170 && buf[1] == 68 && buf[2] == 18) {
                bytesRead = 3;
                state = State_Header;
            }
            break;
        }
        case State_Header: {
            // write directly into header
            uint8_t *buf = (uint8_t *) &p->header;
            buf[bytesRead] = rcvdbyte;
            bytesRead++;

            // once the header is read, move on to the message
            if (bytesRead >= sizeof(GPSHeader)) {
                bytesRead = 0;
                state = State_Message;
            }
            break;
        }
        case State_Message: {
            // write directly into message
            uint8_t *buf = (uint8_t *) &p->message;
            buf[bytesRead] = rcvdbyte;
            bytesRead++;

            // once the message is read, move on to the crc
            if (bytesRead >= p->header.messageLength)
            {
                bytesRead = 0;
                state = State_CRC;
            }
            break;
        }
        case State_CRC: {
            // write directly into the crc
            uint8_t *buf = (uint8_t *) &p->crc;
            buf[bytesRead] = rcvdbyte;
            bytesRead++;

            // once the crc is read, restart the process
            if (bytesRead >= sizeof(p->crc)) {
                bytesRead = 0;
                PushQueue(&queue);
                state = State_Sync;
            }
            break;
        }
    }
}

GPSPackage *ReadGPSPackage() {
    return (GPSPackage *) ReadQueue(&queue);
}

void PopGPSPackage() {
    PopQueue(&queue);
}
