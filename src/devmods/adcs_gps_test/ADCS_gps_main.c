#include <interfaces/systeminfo.h>
#include <intrinsics.h>
#include <msp430fr5994.h>

#define __DEBUG__ 1

#include "ADCS_GPS.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "core/uart.h"
#include "bsp/bsp.h"

uint8_t syncBuf[3] = { 0, 0, 0 };

uint8_t headerBuf[32];
uint8_t messageBuf[128];
uint8_t bytesRead = 0;

unsigned short messageId;
unsigned short messageLength;

typedef enum status {
    Status_Sync,
    Status_Header,
    Status_Message,
} Status;

Status status = Status_Sync;

int main(void) {
    bspInit(Module_Test);
    __bis_SR_register(GIE); // Enter LPM3, interrupts enabled

    // override debugtools' callback
    uartInit();
    uartRegisterRxCallback(readCallback);

    // send configuration and commands to receiver
    uint8_t *commandIntMode = "interfacemode com1 novatel novatelbinary on\n\r";
    uartTransmit(commandIntMode, sizeof(*commandIntMode));
    uint8_t *commandUnlog = "unlogall\n\r";
    uartTransmit(commandUnlog, sizeof(*commandUnlog));
    uint8_t *command = "log timeb ontime 1\n\r";
    uartTransmit(command, sizeof(*command));

    for (;;) {
        // TODO this empty forever-loop seems bad
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
            syncBuf[0] = 0;
            syncBuf[1] = 0;
            syncBuf[2] = 0;
            status = Status_Header;
        }
        break;
    case Status_Header:
        headerBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        // read through the header (messageBuf[0] is the header length)
        // Below assumes header length includes the response ID but not the sync bits
        // TODO confirm that assumption
        if (bytesRead == headerBuf[0]) {
            messageId = (uint16_t) headerBuf[1];
            messageLength = (uint16_t) headerBuf[5];
            if ((GPS_ENUM) headerBuf[25] != RESPONSE_ID_OK) {
                // TODO error handling
            }
            bytesRead = 0;
            status = Status_Message;
        }
        break;
    case Status_Message:
        messageBuf[bytesRead] = rcvdbyte;
        bytesRead++;

        if (bytesRead == messageLength) {
            parseMessage();
            bytesRead = 0;
            status = Status_Sync;
        }
        break;
    }
}

/**
 * Parse a message, assuming that message is stored in the messageBuf and messageId is set
 * TODO do something with the data (log it or send CAN packets)
 */
void parseMessage(void) {
    switch (messageId) {
    case ID_BESTPOS: {
        GPS_ENUM solStat = (GPS_ENUM) messageBuf[0];
        double lat = (double) messageBuf[8];
        double lon = (double) messageBuf[16];
        double hgt = (double) messageBuf[24];
        break;
    }
    case ID_BESTVEL: {
        GPS_ENUM solStat = (GPS_ENUM) messageBuf[0];
        double horspd = (double) messageBuf[16];
        double trkgnd = (double) messageBuf[24];
        double vertspd = (double) messageBuf[32];
        break;
    }
    case ID_BESTXYZ: {
        GPS_ENUM pSolStat = (GPS_ENUM) messageBuf[0];
        double px = (double) messageBuf[8];
        double py = (double) messageBuf[16];
        double pz = (double) messageBuf[24];
        GPS_ENUM vSolStatus = (GPS_ENUM) messageBuf[44];
        double vx = (double) messageBuf[52];
        double vy = (double) messageBuf[60];
        double vz = (double) messageBuf[68];
    }
    case ID_TIME: {
        // read basic time from header
        unsigned short week = (unsigned short) headerBuf[11];
        GPSec ms = (GPSec) headerBuf[13];

        // read offset from message
        GPS_ENUM clockStatus = (GPS_ENUM) messageBuf[0];
        double offset = (double) messageBuf[4];
        break;
    }
    default:
        // TODO handle unsupported IDs
        break;
    }
}
