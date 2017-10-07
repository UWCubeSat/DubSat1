#define cast(TYPE, PTR) (*((TYPE*) (PTR)))

#include <msp430.h>

#include "ADCS_GPS.h"
#include "bsp/bsp.h"
#include <stdint.h>
#include <cstring>
#include "core/uart.h"
#include "crc.h"

FILE_STATIC uint8_t buf[256];
FILE_STATIC uint8_t bytesRead = 0;
FILE_STATIC uint32_t rxStatus = 0;
FILE_STATIC uint8_t synced = 0;

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

/**
 * Parse a message, assuming that message is stored in the messageBuf and messageId is set
 * TODO send CAN packets
 */
void parseMessage(message_type messageType, uint8_t* messageBuf)
{
    switch (messageType)
    {
    case Message_BestXYZ:
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
    case Message_Time:
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
        debugTraceF(4, "unsupported message ID: %u\r\n", messageType);
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
    __bis_SR_register(GIE); // Enter LPM3, interrupts enabled

    uartHandle = uartInit(ApplicationUART, 0, Speed_9600);

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
        const uint8_t headerLength = buf[3];
        debugTraceF(4, "header length: %u\r\n", headerLength);

        // read header
        while (bytesRead < headerLength);
        const uint16_t messageLength = cast(uint16_t, buf + 8);
        rxStatus = cast(uint32_t, buf + 20);
        debugTraceF(4, "message length: %u\r\n", messageLength);

        // read message
        while (bytesRead < headerLength + messageLength + 4); // + 4 to get CRC, too

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
            parseMessage((message_type) messageId, buf + headerLength);
        }

        // set the sync bytes back to 0
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0;

        bytesRead = 0;
        synced = 0;
    }
}
