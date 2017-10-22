#define MS_IN_WEEK 604800000
#define PACKAGE_BUFFER_LENGTH 3

#include <msp430.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/uart.h"

#include "ADCS_GPS.h"
#include "GPSPackage.h"
#include "GPSReader.h"

typedef enum message_id
{
    Message_RXStatus = 93,
    Message_RXStatusEvent = 94,
    Message_Time = 101,
    Message_BestXYZ = 241,
    Message_HWMonitor = 963,
} message_id;

FILE_STATIC hBus uartHandle;

// these should probably go in some status struct
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
FILE_STATIC const char *GPS_ERROR[];
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

        if (m.pSolStatus || m.vSolStatus)
        {
            // Skip the invalid message. An RXSTATUSEVENT should trigger another
            // log once position becomes valid
            // TODO log the error
            debugTraceF(4, "BESTXYZ invalid\r\n");
            break;
        }

        debugTraceF(4, "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                    m.pSolStatus, m.pos.x, m.pos.y, m.pos.z);
        // TODO send CAN packet
        break;
    }
    case Message_Time:
    {
        const GPSTime m = package->message.time;

        if (!m.clockStatus || m.utcStatus != 1)
        {
            // Skip the invalid message. An RXSTATUSEVENT should trigger another
            // log once either becomes valid
            // TODO log the error
            debugTraceF(4, "TIME invalid\r\n");
            break;
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
        const GPSHWMonitor monLog = package->message.hwMonitor;
        debugTraceF(4, "HWMonitor:\r\n");

        // read only the measurements supported by this model (615)
        // TODO revisit this for the OEM719
        if (monLog.numMeasurements < 2) {
            debugPrintF("HWMonitor missing measurements!\r\n");
            break;
        }
        const GPSMeasurement m = monLog.measurements[1];
        debugTraceF(4, "\ttemp: %f, status: %u\r\n", m.reading, m.status);
        break;
    }
    default:
        debugTraceF(4, "unsupported message ID: %u\r\n",
                    package->header.messageType);
        break;
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

    uartHandle = uartInit(ApplicationUART, 0, Speed_115200);

    // initialize the reader
    GPSPackage packageBuffer[PACKAGE_BUFFER_LENGTH];
    GPSReaderInit(uartHandle, packageBuffer, PACKAGE_BUFFER_LENGTH);

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

    GPSPackage *package;

    while (1)
    {
        // wait for next package
        while ((package = ReadGPSPackage()) == NULL);

        // validate package using crc
        if (!isGPSPackageValid(package))
        {
            debugPrintF("invalid CRC\r\n");
            // skip the corrupted message
            // TODO log the error somehow
            PopGPSPackage();
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

        PopGPSPackage();
    }

    return EXIT_SUCCESS;
}
