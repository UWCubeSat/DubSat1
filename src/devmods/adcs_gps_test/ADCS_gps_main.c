#include <interfaces/systeminfo.h>
#include <intrinsics.h>
#include <msp430fr5994.h>

#define __DEBUG__ 1
#define cast(TYPE, PTR) (*((TYPE*) (PTR)))

#include "ADCS_GPS.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "core/uart.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"

FILE_STATIC uint8_t syncBuf[3] = { 0, 0, 0 };

FILE_STATIC uint8_t headerBuf[32];
FILE_STATIC uint8_t messageBuf[128];
FILE_STATIC uint16_t bytesRead = 0;

FILE_STATIC uint16_t messageLength;

typedef enum status {
    Status_Sync,
    Status_Header,
    Status_Message,
} Status;

FILE_STATIC Status status = Status_Sync;

/**
 * Parse a message, assuming that message is stored in the messageBuf and messageId is set
 * TODO send CAN packets
 */
void parseMessage(uint16_t messageId) {
    switch (messageId) {
    case ID_BESTXYZ: {
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

        debugTraceF(4, "BESTXYZ (%u)\n\tx: %e\n\ty: %e\n\tz: %e", pSolStat, px, py, pz);
        break;
    }
    case ID_TIME: {
        // read basic time from header
        const uint16_t week = cast(uint16_t, headerBuf + 11);
        const GPSec ms = cast(GPSec, headerBuf + 13);

        // read offset from message
        const GPS_ENUM clockStatus = cast(GPS_ENUM, messageBuf);
        const double offset = cast(double, messageBuf + 4);

        debugTraceF(4, "TIME \n\tweek: %u \n\tms: %u", week, ms);
        break;
    }
    default:
        debugTraceF(4, "unsupported message ID: %u\n", messageId);
        break;
    }
}

void readCallback(uint8_t rcvdbyte) {
    switch(status) {
    case Status_Sync:
        // read until synced (at the start of a new header)
        syncBuf[0] = syncBuf[1];
        syncBuf[1] = syncBuf[2];
        syncBuf[2] = rcvdbyte;
        if (syncBuf[0] == '\xAA' && syncBuf[1] == '\x44' && syncBuf[2] == '\x12') {
            debugTraceF(4, "synced");

            syncBuf[0] = 0;
            syncBuf[1] = 0;
            syncBuf[2] = 0;
            status = Status_Header;
        }
        break;
    case Status_Header:
        headerBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        // read through the header (headerBuf[0] is the header length)
        // Below assumes header length includes the response ID but not the sync bits
        // TODO confirm that assumption
        if (bytesRead == headerBuf[0]) {
            debugTraceF(4, "read header, length: %u", headerBuf[0]);

            messageLength = cast(uint16_t, headerBuf + 5);
            const GPS_ENUM responseId = cast(GPS_ENUM, headerBuf + 25);
            if (responseId != RESPONSE_ID_OK) {
                // TODO error handling
                debugTraceF(4, "bad response ID: %u", responseId);
            }
            bytesRead = 0;
            status = Status_Message;
        }
        break;
    case Status_Message:
        messageBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        if (bytesRead == messageLength) {
            debugTraceF(4, "read message, length: %u", messageLength);

            const uint16_t messageId = cast(uint16_t, headerBuf + 1);
            parseMessage(messageId);
            bytesRead = 0;
            status = Status_Sync;
        }
        break;
    }
}

void sendCommand(hBus uartHandle, const char *command) {
    uartTransmit(uartHandle, (uint8_t*) command, strlen(command));
}

int main(void) {
    bspInit(Module_Test);
    __bis_SR_register(GIE); // Enter LPM3, interrupts enabled

    const hBus uartHandle = uartInit(ApplicationUART);

    // send configuration and commands to receiver
    sendCommand(uartHandle, "interfacemode com1 novatel novatelbinary on\n\r");
    sendCommand(uartHandle, "unlogall\n\r");
    sendCommand(uartHandle, "log timeb ontime 1\n\r");

    uartRegisterRxCallback(uartHandle, readCallback);

    for (;;) {
        // TODO this empty forever-loop seems bad
    }
}
