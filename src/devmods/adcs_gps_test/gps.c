#define cast(TYPE, PTR) (*((TYPE*) (PTR)))

#include "gps.h"
#include <stdint.h>
#include <cstring>
#include "core/uart.h"
#include "crc.h"

FILE_STATIC uint8_t buf[256];
FILE_STATIC uint8_t bytesRead = 0;

FILE_STATIC uint8_t headerLength;
FILE_STATIC uint16_t messageLength;

FILE_STATIC uint32_t rxStatus = 0;

typedef enum status
{
    Status_Sync, Status_Header, Status_Message,
} Status;

FILE_STATIC Status status = Status_Sync;

FILE_STATIC hBus uartHandle;

#ifdef __DEBUG__
const char *GPS_ERROR[] = { "Error (use RXSTATUS for details)",
                            "Temperature warning", "Voltage supply warning",
                            "Antenna not powered", "LNA Failure",
                            "Antenna open", "Antenna shorted", "CPU overload",
                            "COM1 buffer overrun", "COM2 buffer overrun",
                            "COM3 buffer overrun", "Link overrun", "",
                            "Aux transmit overrun", "AGC out of range", "",
                            "INS reset", "", "GPS Almanac/UTC invalid",
                            "Position invalid", "Position fixed",
                            "Clock steering disabled", "Clock model invalid",
                            "External oscillator locked",
                            "Software resource warning", "", "", "", "",
                            "Aux 3 status event", "Aux 2 status event",
                            "Aux 1 status event" };
#else
const char *GPS_ERROR[];
#endif /* __DEBUG__ */

/**
 * Parse a message, assuming that message is stored in the messageBuf and messageId is set
 * TODO send CAN packets
 */
void parseMessage(uint16_t messageId, uint8_t* messageBuf)
{
    switch (messageId)
    {
    case ID_BESTXYZ:
    {
        // position solution status
        const GPS_ENUM pSolStat = cast(GPS_ENUM, messageBuf);

        // position
        const double px = cast(double, messageBuf + 8);
        const double py = cast(double, messageBuf + 16);
        const double pz = cast(double, messageBuf + 24);

        // position std dev
        const float pxd = cast(float, messageBuf + 32);
        const float pyd = cast(float, messageBuf + 36);
        const float pzd = cast(float, messageBuf + 40);

        // velocity solution status
        const GPS_ENUM vSolStat = cast(GPS_ENUM, messageBuf + 44);

        // velocity
        const double vx = cast(double, messageBuf + 52);
        const double vy = cast(double, messageBuf + 60);
        const double vz = cast(double, messageBuf + 68);

        // velocity std dev
        const float vxd = cast(float, messageBuf + 76);
        const float vyd = cast(float, messageBuf + 80);
        const float vzd = cast(float, messageBuf + 84);

        debugTraceF(4, "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                    pSolStat, px, py, pz);
        break;
    }
    case ID_TIME:
    {
        const GPS_ENUM clockStatus = cast(GPS_ENUM, messageBuf);

        // utc time
        // year and ms are officially unsigned longs, but in practice they can never exceed the max
        // value of a short. I cast them to uint16_t to fit into one CAN packet.
        const uint16_t year = cast(uint16_t, messageBuf + 28);
        const uint8_t month = messageBuf[32];
        const uint8_t day = messageBuf[33];
        const uint8_t hour = messageBuf[34];
        const uint8_t min = messageBuf[35];
        const uint16_t ms = cast(uint16_t, messageBuf + 36);
        const GPS_ENUM utcStatus = cast(GPS_ENUM, messageBuf + 40);

        // TODO real error handling
        if (!clockStatus)
        {
            debugTraceF(4, "bad clock status: %u\r\n", clockStatus);
        }
        if (utcStatus != 1)
        {
            debugTraceF(4, "bad utc status: %u\r\n", utcStatus);
        }

        debugTraceF(
                4,
                "TIME \r\n\tyear: %u \r\n\tmonth: %u\r\n\tday: %u\r\n\thour: %u\r\n\tmin: %u\r\n\tms: %u\r\n",
                year, month, day, hour, min, ms);
        break;
    }
    default:
        debugTraceF(4, "unsupported message ID: %u\r\n", messageId);
        break;
    }
}

void readCallback(uint8_t rcvdbyte)
{
    switch (status)
    {
    case Status_Sync:
        // read until synced (at the start of a new header)
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = rcvdbyte;
        // synced when read \xAA, \x44, \x12
        if (buf[0] == 170 && buf[1] == 68 && buf[2] == 18)
        {
            debugTraceF(4, "synced\r\n");
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            bytesRead = 3;
            status = Status_Header;
        }
        break;
    case Status_Header:
    {
        buf[bytesRead] = rcvdbyte;
        bytesRead++;

        // read through the header + the responseId that follows
        headerLength = buf[3];
        if (bytesRead == headerLength)
        {
            debugTraceF(4, "read header, length: %u\r\n", bytesRead);

            messageLength = cast(uint16_t, buf + 8);
            rxStatus = cast(uint32_t, buf + 20);

            status = Status_Message;
        }
        break;
    }
    case Status_Message:
        buf[bytesRead] = rcvdbyte;
        bytesRead++;

        if (bytesRead == headerLength + messageLength + 4) // + 4 to get the CRC too
        {
            debugTraceF(4, "read message, length: %u\r\n", messageLength);

            // check the CRC
            const uint32_t crcExpected = cast(uint32_t, buf + bytesRead - 4);
            const uint32_t crcActual = calculateBlockCrc32(bytesRead - 4, buf);
            if (crcExpected != crcActual)
            {
                // TODO real error handling
                debugTraceF(3, "invalid CRC");
            }

            const uint16_t messageId = cast(uint16_t, buf + 4);

            const uint8_t messageType = buf[6];
            if (messageType & 0b1000000)
            {
                // the receiver responds to commands with a confirmation message,
                // which can be ignored
                debugTraceF(4, "received command: %u", messageId);
            }
            else
            {
                parseMessage(messageId, buf + headerLength);
            }

            bytesRead = 0;
            status = Status_Sync;
        }
        break;
    }
}

void sendCommand(uint8_t *command)
{
    uartTransmit(uartHandle, command, strlen((char*) command));
}

uint8_t gpsStatus(DebugMode mode)
{
    if (mode == InteractiveMode)
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

void gpsInit(void)
{
    uartHandle = uartInit(ApplicationUART, 0);
    uartRegisterRxCallback(uartHandle, readCallback);
}
