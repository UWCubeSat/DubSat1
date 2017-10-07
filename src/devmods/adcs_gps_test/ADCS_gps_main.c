#define cast(TYPE, PTR) (*((TYPE*) (PTR)))
#define GPS_BUFFER_LENGTH 256

#include <msp430.h>

#include "ADCS_GPS.h"
#include "bsp/bsp.h"
#include <stdint.h>
#include <cstring>
#include "core/uart.h"
#include "crc.h"

FILE_STATIC uint8_t buf[GPS_BUFFER_LENGTH];
FILE_STATIC uint8_t bytesRead = 0;
FILE_STATIC uint8_t synced = 0;

FILE_STATIC uint8_t rxStatus = 0;

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

typedef enum message_id
{
    Message_BestXYZ = 241,
    Message_Time = 101,
} message_id;

 // TODO send CAN packets
void parseMessage(uint8_t* buf)
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

// TODO this might get called before the message is parsed.
// This is a problem because then the buffer will write sync bytes at the end and skip the message.
void readCallback(uint8_t rcvdbyte)
{
    if (synced == 0)
    {
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
        // sync
        while (synced == 0);

        // read header length
        while (bytesRead < 4);
        GPSHeader *header = (GPSHeader*) buf;
        debugTraceF(4, "header length: %u\r\n", header->headerLength);

        // read header
        while (bytesRead < header->headerLength);
        rxStatus = header->rxStatus;
        debugTraceF(4, "message length: %u\r\n", header->messageLength);

        // read message (+ 4 to get CRC, too)
        while (bytesRead < header->headerLength + header->messageLength + 4);

        // check the CRC
        const uint32_t crcExpected = cast(uint32_t, buf + bytesRead - 4);
        const uint32_t crcActual = calculateBlockCrc32(bytesRead - 4, buf);
        if (crcExpected != crcActual)
        {
            // TODO real error handling
            debugTraceF(3,
                        "invalid CRC\r\n\texpected: %X\r\n\tactual:   %X\r\n",
                        crcExpected, crcActual);
        }

        if (header->messageType & 0b1000000)
        {
            // the receiver responds to commands with a confirmation message,
            // which can be ignored
            debugTraceF(4, "received command: %u", header->messageId);
        }
        else
        {
            parseMessage(buf);
        }

        // set the sync bytes back to 0
        header->sync[0] = 0;
        header->sync[1] = 0;
        header->sync[2] = 0;

        bytesRead = 0;
        synced = 0;
    }
}
