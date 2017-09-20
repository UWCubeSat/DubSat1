#include <interfaces/systeminfo.h>
#include <intrinsics.h>
#include <msp430fr5994.h>

#define cast(TYPE, PTR) (*((TYPE*) (PTR)))

#include "ADCS_GPS.h"
#include <stdint.h>
#include <cstring>
#include "core/uart.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"

FILE_STATIC uint8_t headerBuf[32];
FILE_STATIC uint8_t messageBuf[128];
FILE_STATIC uint16_t bytesRead = 0;

FILE_STATIC uint16_t messageLength;

FILE_STATIC uint32_t rxStatus = 0;

typedef enum status
{
    Status_Sync, Status_Header, Status_Message,
} Status;

FILE_STATIC Status status = Status_Sync;

FILE_STATIC hBus uartHandle;

/**
 * Parse a message, assuming that message is stored in the messageBuf and messageId is set
 * TODO send CAN packets
 */
void parseMessage(uint16_t messageId)
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

        debugTraceF(4, "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n", pSolStat,
                    px, py, pz);
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
        if(!clockStatus) {
            debugTraceF(4, "bad clock status: %u\r\n", clockStatus);
        }
        if(utcStatus != 1) {
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
        headerBuf[0] = headerBuf[1];
        headerBuf[1] = headerBuf[2];
        headerBuf[2] = rcvdbyte;
        // synced when read \xAA, \x44, \x12
        if (headerBuf[0] == 170 && headerBuf[1] == 68 && headerBuf[2] == 18)
        {
            debugTraceF(4, "synced\r\n");
            headerBuf[0] = 0;
            headerBuf[1] = 0;
            headerBuf[2] = 0;
            bytesRead = 3;
            status = Status_Header;
        }
        break;
    case Status_Header:
    {
        headerBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        // read through the header + the responseId that follows
        const uint8_t headerLength = headerBuf[3];
        if (bytesRead == headerLength + sizeof(GPS_ENUM))
        {
            debugTraceF(4, "read header, length: %u\r\n", headerLength);

            messageLength = cast(uint16_t, headerBuf + 8);
            rxStatus = cast(uint32_t, headerBuf + 20);

            const GPS_ENUM responseId = cast(GPS_ENUM,
                                             headerBuf + headerLength);
            if (responseId != RESPONSE_ID_OK)
            {
                // TODO error handling
                debugTraceF(4, "bad response ID: %u\r\n", responseId);
            }
            bytesRead = 0;
            status = Status_Message;
        }
        break;
    }
    case Status_Message:
        messageBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        if (bytesRead == messageLength)
        {
            debugTraceF(4, "read message, length: %u\r\n", messageLength);

            const uint16_t messageId = cast(uint16_t, headerBuf + 4);

            const uint8_t messageType = headerBuf[6];
            if (messageType & 0b1000000) {
                // the receiver responds to commands with a confirmation message,
                // which can be ignored
                debugTraceF(4, "received command: %u", messageId);
            } else {
                parseMessage(messageId);
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

uint8_t actionHandler(DebugMode mode, uint8_t *command)
{
    sendCommand(command);
    return 1;
}

uint8_t statusHandler(DebugMode mode)
{
    if(mode == InteractiveMode) {
        debugPrintF("GPS Status: %X\r\n", rxStatus);
        uint8_t i = 0;
        while (i < 32) {
            if (rxStatus & (1 << i)) {
                debugPrintF("\terror #%u - %s\r\n", i, GPS_ERROR[i]);
            }
            i++;
        }
    }
    return 1;
}

int main(void)
{
    bspInit(Module_Test);
    __bis_SR_register(GIE); // Enter LPM3, interrupts enabled

    uartHandle = uartInit(ApplicationUART, 0);

    // TODO what is pathchar?
    debugRegisterEntity(Entity_Test, 'g', NULL, statusHandler, actionHandler);

    // send configuration and commands to receiver
    sendCommand("interfacemode com1 novatel novatelbinary on\n\r");
    sendCommand("unlogall\n\r");
    sendCommand("log bestxyzb ontime 3\n\r");

    uartRegisterRxCallback(uartHandle, readCallback);

    for (;;)
    {
        // TODO this empty forever-loop seems bad
    }
}
