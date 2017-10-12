#define cast(TYPE, PTR) (*((TYPE*) (PTR)))
#define GPS_BUFFER_LENGTH 256

#include <msp430.h>

#include "ADCS_GPS.h"
#include "bsp/bsp.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "core/uart.h"
#include "core/utils.h"
#include "crc.h"
#include "queue.h"

typedef enum message_id
{
    Message_BestXYZ = 241,
    Message_Time = 101,
} message_id;

FILE_STATIC uint8_t buf[GPS_BUFFER_LENGTH];
FILE_STATIC uint8_t bytesRead = 0;
FILE_STATIC uint8_t synced = 0;

FILE_STATIC Queue *queue;

FILE_STATIC uint32_t rxStatus = 0;

FILE_STATIC hBus uartHandle;

#ifdef __DEBUG__
FILE_STATIC const char *GPS_ERROR[] = { "Error (use RXSTATUS for details)",
                                        "Temperature warning",
                                        "Voltage supply warning",
                                        "Antenna not powered", "LNA Failure",
                                        "Antenna open", "Antenna shorted",
                                        "CPU overload", "COM1 buffer overrun",
                                        "COM2 buffer overrun",
                                        "COM3 buffer overrun", "Link overrun",
                                        "", "Aux transmit overrun",
                                        "AGC out of range", "", "INS reset", "",
                                        "GPS Almanac/UTC invalid",
                                        "Position invalid", "Position fixed",
                                        "Clock steering disabled",
                                        "Clock model invalid",
                                        "External oscillator locked",
                                        "Software resource warning", "", "", "",
                                        "", "Aux 3 status event",
                                        "Aux 2 status event",
                                        "Aux 1 status event" };
#else
const char *GPS_ERROR[];
#endif /* __DEBUG__ */

 // TODO send CAN packets
FILE_STATIC void parseMessage(uint8_t* buf)
 {
    GPSHeader *header = (GPSHeader*) buf;
    switch (header->messageId)
    {
    case Message_BestXYZ:
    {
        GPSBestXYZ *m = (GPSBestXYZ*) buf;
        debugTraceF(4, "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                    m->pSolStatus, m->pos.x, m->pos.y, m->pos.z);
        break;
    }
    case Message_Time:
    {
        GPSTime *m = (GPSTime*) buf;

        // year and ms are officially unsigned longs, but in practice they can never exceed the max
        // value of a short. I cast them to uint16_t to fit into one CAN packet.

        // TODO real error handling
        if (!m->clockStatus)
        {
            debugTraceF(4, "bad clock status: %u\r\n", m->clockStatus);
        }
        if (m->utcStatus != 1)
        {
            debugTraceF(4, "bad utc status: %u\r\n", m->utcStatus);
        }

        debugTraceF(
                4,
                "TIME \r\n\tyear: %u \r\n\tmonth: %u\r\n\tday: %u\r\n\thour: %u\r\n\tmin: %u\r\n\tms: %u\r\n",
                m->utcYear, m->utcMonth, m->utcDay, m->utcHour, m->utcMin, m->utcMs);
        break;
    }
    default:
        debugTraceF(4, "unsupported message ID: %u\r\n", header->messageType);
        break;
    }
}

FILE_STATIC void readCallback(uint8_t rcvdbyte)
{
    if (synced == 0)
    {
        // sync using magic sync bytes 170, 68, 18
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = rcvdbyte;
        if (buf[0] == 170 && buf[1] == 68 && buf[2] == 18)
        {
            bytesRead = 3;
            synced = 1;
        }
    }
    else
    {
        buf[bytesRead] = rcvdbyte;
        bytesRead++;

        if (bytesRead > GPS_BUFFER_LENGTH)
        {
            // TODO handle buffer overflow
            debugPrintF("buffer overflow\r\n");
        }

        // if the header, message, and crc have been read, copy them into the
        // queue and reset the buffer
        GPSHeader *header = (GPSHeader*) buf;
        if (bytesRead >= 4
                && bytesRead >= header->headerLength
                && bytesRead >= header->headerLength + header->messageLength + 4)
        {
            // allocate buffer and node
            const size_t bufSize = header->headerLength + header->messageLength + 4;
            uint8_t *bufCopy = (uint8_t*) malloc(bufSize);
            if (bufCopy == NULL) {
                // TODO out of memory
                // running out of memory is probably a sign that the read is
                // consistently happening much slower than write. This shouldn't
                // be a problem since we're writing at 1Hz, but it should be
                // addressed.
                debugPrintF("out of memory\r\n");
            }
            memcpy(bufCopy, buf, bufSize); // copy buffer
            if (!PushQueue(queue, bufCopy))
            {
                // TODO out of memory
                debugPrintF("out of memory\r\n");
            }

            // reset to initial state
            bytesRead = 0;
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            synced = 0;
        }
    }
}

uint8_t gpsStatus(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        debugPrintF("GPS Status: %X\r\n", rxStatus);
        uint8_t i = 0;
        while (i < 32)
        {
            if (rxStatus & (1 << i))
            {
                debugPrintF("\terror #%u - %s\r\n", i, GPS_ERROR[i]);
            }
            i++;
        }
    }
    return 1;
}

void gpsSendCommand(uint8_t *command)
{
    uartTransmit(uartHandle, command, strlen((char*) command));
}

uint8_t actionHandler(DebugMode mode, uint8_t *command)
{
    gpsSendCommand(command);
    return 1;
}

int main(void)
{
    bspInit(Module_Test);
    __bis_SR_register(GIE);

    queue = AllocateQueue();

    uartHandle = uartInit(ApplicationUART, 0, Speed_115200);

#if defined(__DEBUG__)

    debugRegisterEntity(Entity_Test, NULL, gpsStatus, actionHandler);

    // send configuration and commands to receiver
    gpsSendCommand("interfacemode com1 novatel novatelbinary on\n\r");
    gpsSendCommand("unlogall\n\r");
    gpsSendCommand("log bestxyzb ontime 3\n\r");

#endif

    uartRegisterRxCallback(uartHandle, readCallback);

    while (1)
    {
        // wait for next message
        while (!QueueHasNext(queue));

        // get the next message
        uint8_t *message;
        PopQueue(queue, (void **) &message);

        // extract header from message
        GPSHeader *header = (GPSHeader*) message;
        uint16_t messageLength = header->headerLength + header->messageLength;

        // check the CRC
        const uint32_t crcExpected = cast(uint32_t, message + messageLength);
        const uint32_t crcActual = calculateBlockCrc32(messageLength, message);
        if (crcActual != crcExpected)
        {
            // TODO real error handling
            debugPrintF(
                        "invalid CRC\r\n\texpected: %X\r\n\tactual:   %X\r\n",
                        crcExpected, crcActual);
        }

        // set rxStatus for status callback
        rxStatus = header->rxStatus;

        // filter out response messages
        if (header->messageType & 0b1000000)
        {
            // the receiver responds to commands with a confirmation message,
            // which can be ignored
            debugPrintF("received command: %u", header->messageId);
        }
        else
        {
            parseMessage(message);
        }

        free(message);
    }

    // TODO unreachable code. Resolve this memory leak.
    FreeQueue(queue, free);

    return EXIT_SUCCESS;
}
