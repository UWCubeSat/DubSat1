#define MS_IN_WEEK 604800000
#define PACKAGE_CAPACITY 3

#include <msp430.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "ADCS_GPS.h"
#include "queue.h"

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/uart.h"
#include "crc.h"

typedef enum message_id
{
    Message_RXStatus = 93,
    Message_RXStatusEvent = 94,
    Message_Time = 101,
    Message_BestXYZ = 241,
    Message_HWMonitor = 963,
} message_id;

typedef enum gps_state
{
    State_Sync,
    State_Header,
    State_Message,
    State_CRC
} gps_state;

// IO related variables
FILE_STATIC uint8_t bytesRead = 0;
FILE_STATIC gps_state state;
FILE_STATIC Queue queue;
FILE_STATIC hBus uartHandle;

// these should probably go in so status struct
FILE_STATIC uint32_t rxStatus = 0;
FILE_STATIC double timeOffset = 0;

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

FILE_STATIC void toUtc(uint16_t *week, gps_ec *ms, double offset) {
    double tmp = *ms;
    tmp += offset * 1000;
    if (tmp < 0) {
        (*week)--;
        tmp += MS_IN_WEEK;
    } else if (tmp > MS_IN_WEEK) {
        (*week)++;
        tmp -= MS_IN_WEEK;
    }
    *ms = (gps_ec) round(tmp);
}

FILE_STATIC void parseMessage(GPSPackage *package)
 {
    switch (package->header.messageId)
    {
    case Message_BestXYZ:
    {
        const GPSBestXYZ m = package->message.bestXYZ;
        debugTraceF(4, "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                    m.pSolStatus, m.pos.x, m.pos.y, m.pos.z);
        // TODO send CAN packet
        break;
    }
    case Message_Time:
    {
        const GPSTime m = package->message.time;

        // TODO real error handling
        if (!m.clockStatus)
        {
            debugTraceF(4, "bad clock status: %u\r\n", m.clockStatus);
        }
        if (m.utcStatus != 1)
        {
            debugTraceF(4, "bad utc status: %u\r\n", m.utcStatus);
        }

        // update the time offset
        // used in pos/vel messages and updating MET
        timeOffset = m.offset + m.utcOffset;

        debugTraceF(4, "TIME offset: %f\r\n", timeOffset);
        break;
    }
    case Message_RXStatus:
    {
        const GPSRXStatus m = package->message.rxstatus;
        debugTraceF(4, "RXSTATUS error word: %X \r\n", m.error);
        break;
    }
    case Message_RXStatusEvent:
    {
        const GPSRXStatusEvent e = package->message.rxstatusEvent;

        // only read the status events
        // TODO read and deal with error/aux events too
        if (e.word != 1) {
            return;
        }

        debugPrintF("GPS status event: %s\r\n", e.description);
        switch(e.bitPosition) {
        case 19: // position valid flag
            if (e.event == 0) { // if the position became valid
                gpsSendCommand("log bestxyzb ontime 1\r\n");
            }
            break;
        case 18: // almanac/UTC valid flag
        case 22: // clock model flag
            if (e.event == 0) {
                // if either the clock model or UTC became valid, log time
                gpsSendCommand("log timeb\r\n");
            }
            break;
        }
        break;
    }
    case Message_HWMonitor:
    {
        const GPSHWMonitor m = package->message.hwMonitor;
        debugTraceF(4, "HWMonitor:\r\n");
        unsigned int i = m.numMeasurements;
        while (i--) {
            debugTraceF(4, "\tm: %f\r\n", m.measurements[i].reading);
        }
        break;
    }
    default:
        debugTraceF(4, "unsupported message ID: %u\r\n",
                    package->header.messageType);
        break;
    }
}

FILE_STATIC void readCallback(uint8_t rcvdbyte)
{
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

uint8_t gpsStatus(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        debugPrintF("GPS Status: %08lx\r\n", rxStatus);
        uint8_t i = 32;
        while (i--)
        {
            if (rxStatus & (uint32_t) 1 << i)
            {
                debugPrintF("\terror #%u - %s\r\n", i, GPS_ERROR[i]);
            }
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

    // make that stack frame extra thicc
    uint8_t queueContents[PACKAGE_CAPACITY * sizeof(GPSPackage)];
    queue = CreateQueue(queueContents, sizeof(GPSPackage), PACKAGE_CAPACITY);

    uartHandle = uartInit(ApplicationUART, 0, Speed_115200);

#if defined(__DEBUG__)

    debugRegisterEntity(Entity_Test, NULL, gpsStatus, actionHandler);

    // configure to reply in binary only
    gpsSendCommand("iterfacemode com2 novatel novatelbinary on\r\n");

    // stop logging defaults
    gpsSendCommand("unlogall\r\n");

    // configure to send an RXSTATUSEVENT when either the position, clock model,
    // or utc/almanac becomes valid
    gpsSendCommand("statusconfig clear status 004c0000\r\n");
    gpsSendCommand("log rxstatuseventb onnew\r\n");

    // monitor hardware
    gpsSendCommand("log hwmonitorb ontime 10\r\n");

    // TODO do we need to log bestxyz? The log will be triggered when the
    // position becomes valid, but what if it was valid to being with?

#endif

    uartRegisterRxCallback(uartHandle, readCallback);

    while (1)
    {
        // wait for next message
        while (isQueueEmpty(&queue));

        // get the next message
        GPSPackage *package = (GPSPackage *) ReadQueue(&queue);

        // check the CRC
        const uint32_t crcActual = calculateBlockCrc32(
                package->header.headerLength + package->header.messageLength,
                (uint8_t *) package);
        if (crcActual != package->crc)
        {
            debugPrintF(
                    "invalid CRC\r\n\texpected: %X\r\n\tactual:   %X\r\n",
                    package->crc, crcActual);
            // skip the corrupted message
            // TODO log the error somehow
            PopQueue(&queue);
            continue;
        }

        // filter out response messages
        // TODO start checking these for confirmation (see LOG command)
        if (package->header.messageType & 0b1000000)
        {
            // the receiver responds to commands with a confirmation message,
            // which can be ignored
            debugPrintF("received command: %u", package->header.messageId);
        }
        else
        {
            parseMessage(package);
        }

        // set rxStatus for status callback
        rxStatus = package->header.rxStatus;

        // TODO send a CAN packet updating the MET
        uint16_t week = package->header.week;
        gps_ec ms = package->header.ms;
        toUtc(&week, &ms, timeOffset);

        PopQueue(&queue);
    }

    return EXIT_SUCCESS;
}
