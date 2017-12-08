#define MS_IN_WEEK 604800000
#define PACKAGE_BUFFER_LENGTH 3
#define GPS_TRACE_LEVEL 0

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "core/uart.h"
#include "core/debugtools.h"

#include "gps.h"
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

FILE_STATIC RXStatusInfo rxStatusInfo;
FILE_STATIC BestXYZInfo bestXYZInfo;
FILE_STATIC TimeInfo timeInfo;
FILE_STATIC HWMonitorInfo hwMonitorInfo;

FILE_STATIC GPSPackage packageBuffer[PACKAGE_BUFFER_LENGTH];

#ifdef __DEBUG__

#if defined(__BSP_HW_GPS_OEM615__)

FILE_STATIC const char *GPS_STATUS_DESC[] =
{
    "Error (use RXSTATUS for details)", "Temperature warning",
    "Voltage supply warning", "Antenna not powered", "LNA Failure",
    "Antenna open", "Antenna shorted", "CPU overload",
    "COM1 buffer overrun", "COM2 buffer overrun", "COM3 buffer overrun",
    "Link overrun", "", "Aux transmit overrun", "AGC out of range", "",
    "INS reset", "", "GPS Almanac/UTC invalid", "Position invalid",
    "Position fixed", "Clock steering disabled", "Clock model invalid",
    "External oscillator locked", "Software resource warning", "", "", "",
    "", "Aux 3 status event", "Aux 2 status event", "Aux 1 status event"};

#elif defined(__BSP_HW_GPS_OEM719__)

FILE_STATIC const char *GPS_STATUS_DESC[] = {
        "Error (use RXSTATUS for details)", "Temperature warning",
        "Voltage supply warning", "Antenna not powered", "LNA Failure",
        "Antenna open", "Antenna shorted", "CPU overload",
        "COM1 buffer overrun", "", "", "Link overrun", "Input overrun",
        "Aux transmit overrun", "AGC out of range", "Jammer detected",
        "INS reset", "IMU communication failure", "GPS Almanac/UTC invalid",
        "Position invalid", "Position fixed", "Clock steering disabled",
        "Clock model invalid", "External oscillator locked",
        "Software resource warning", "Version bit 0", "Version bit 1",
        "HDR tracking", "Digital filtering enabled", "Aux 3 status event",
        "Aux 2 status event", "Aux 1 status event" };

#else

#error GPS hardware unspecified

#endif /* __BPS_HW_GPS_*__ */

#else

FILE_STATIC const char *GPS_STATUS_DESC[];

#endif /* __DEBUG__ */

FILE_STATIC void toUtc(uint16_t *week, gps_ec *ms, double offset)
{
    double tmp = *ms;
    tmp += offset * 1000;
    if (tmp < 0)
    {
        (*week)--;
        tmp += MS_IN_WEEK;
    }
    else if (tmp > MS_IN_WEEK)
    {
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

        uint16_t week = package->header.week;
        gps_ec ms = package->header.ms;
        toUtc(&week, &ms, timeInfo.offset - m.velLatency);

        debugTraceF(GPS_TRACE_LEVEL,
                    "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                    m.pSolStatus, m.pos.x, m.pos.y, m.pos.z);

        // TODO send CAN packet

        bestXYZInfo.posStatus = m.pSolStatus;
        bestXYZInfo.pos = m.pos;
        bestXYZInfo.posStdDev = m.posStdDev;
        bestXYZInfo.velStatus = m.vSolStatus;
        bestXYZInfo.vel = m.vel;
        bestXYZInfo.velStdDev = m.velStdDev;
        bestXYZInfo.week = week;
        bestXYZInfo.ms = ms;
        break;
    }
    case Message_Time:
    {
        const GPSTime m = package->message.time;

        debugTraceF(GPS_TRACE_LEVEL, "TIME offset: %f\r\n", timeInfo.offset);

        // update the time offset
        // used in pos/vel messages and updating MET
        timeInfo.offset = m.offset + m.utcOffset;
        timeInfo.clockStatus = m.clockStatus;
        timeInfo.utcStatus = m.utcStatus;
        break;
    }
    case Message_RXStatus:
    {
        const GPSRXStatus m = package->message.rxstatus;

        debugTraceF(GPS_TRACE_LEVEL, "RXSTATUS error word: %X \r\n", m.error);

        rxStatusInfo.error = m.error;
        rxStatusInfo.aux1 = m.aux1stat.word;
        rxStatusInfo.aux2 = m.aux2stat.word;
        rxStatusInfo.aux3 = m.aux3stat.word;
        break;
    }
    case Message_RXStatusEvent:
    {
        const GPSRXStatusEvent e = package->message.rxstatusEvent;

        // only read the status events
        // TODO read and deal with error/aux events too
        if (e.word != 1)
        {
            return;
        }

        debugPrintF("GPS status event: %s\r\n", e.description);
        switch (e.bitPosition)
        {
        case 19: // position valid flag
            if (e.event == 0) // if the position became valid
            {
                gpsSendCommand("log bestxyzb\r\n");
            }
            break;
        case 18: // almanac/UTC valid flag
        case 22: // clock model flag
            if (e.event == 0)
            {
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
        debugTraceF(GPS_TRACE_LEVEL, "HWMonitor:\r\n");
        debugTraceF(GPS_TRACE_LEVEL, "\ttemp: %f C\r\n", monLog.temp.reading);

        // TODO add other measurements to COSMOS
        hwMonitorInfo.temp = monLog.temp.reading;
        hwMonitorInfo.tempStatus = monLog.temp.status;

        break;
    }
    default:
        debugTraceF(GPS_TRACE_LEVEL, "unsupported message ID: %u\r\n",
                    package->header.messageId);
        break;
    }
}

void bcbinSendTlm()
{
    // These packets are updated in the header of every message, so they should
    // be sent at every update
    bcbinSendPacket((uint8_t *) &rxStatusInfo, sizeof(rxStatusInfo));
    bcbinSendPacket((uint8_t *) &timeInfo, sizeof(timeInfo));

    // These packets are only updated when the message comes in from the GPS.
    // Sending them could be postponed until they are updated.
    bcbinSendPacket((uint8_t *) &bestXYZInfo, sizeof(bestXYZInfo));
    bcbinSendPacket((uint8_t *) &hwMonitorInfo, sizeof(hwMonitorInfo));
}

uint8_t gpsStatusCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        debugPrintF("GPS Status: 0x%08lx\r\n", rxStatusInfo.status);
        uint8_t i = 32;
        while (i--)
        {
            if (rxStatusInfo.status & ((uint32_t) 1 << i))
            {
                debugPrintF("\terror #%u - %s\r\n", i, GPS_STATUS_DESC[i]);
            }
        }
    }
    else if (mode == Mode_BinaryStreaming)
    {
        bcbinSendTlm();
    }
    return 1;
}

void gpsSendCommand(uint8_t *command)
{
    uartTransmit(uartHandle, command, strlen((char*) command));
}

uint8_t gpsActionCallback(DebugMode mode, uint8_t *command)
{
    gpsSendCommand(command);
    return 1;
}

void gpsInit()
{
    uartHandle = uartInit(ApplicationUART, 0, Speed_115200);

    // initialize the reader
    GPSReaderInit(uartHandle, packageBuffer, PACKAGE_BUFFER_LENGTH);

    // TODO what should the opcodes be?
    bcbinPopulateHeader(&rxStatusInfo.header, 123, sizeof(rxStatusInfo));
    bcbinPopulateHeader(&bestXYZInfo.header, 124, sizeof(bestXYZInfo));
    bcbinPopulateHeader(&timeInfo.header, 125, sizeof(timeInfo));
    bcbinPopulateHeader(&hwMonitorInfo.header, 126, sizeof(hwMonitorInfo));
    debugRegisterEntity(Entity_Test, NULL, gpsStatusCallback,
                        gpsActionCallback);
}

void gpsPowerOn()
{
    // TODO switch on the power
    /*
     * Using timers, periodically poll the GPS with a log to see if its on.
     * Consider doing this asynchronously with some powered on callback that
     * configures the GPS.
     */

    // configure to reply in binary only
    gpsSendCommand("interfacemode thisport novatel novatelbinary on\r\n");

    // configure to send an RXSTATUSEVENT when either the position, clock model,
    // or utc/almanac becomes valid
    gpsSendCommand("statusconfig clear status 004c0000\r\n");

    // log the status once. Status word will be updated at every message.
    gpsSendCommand("log rxstatusb\r\n");

    // monitor hardware
    gpsSendCommand("log hwmonitorb ontime 10\r\n");

    // monitor position
    gpsSendCommand("log bestxyzb ontime 1\r\n");
}

void gpsPowerOff()
{
    // TODO switch off the power
    gpsFlush();
}

bool gpsUpdate()
{
    GPSPackage *package = ReadGPSPackage();
    if (package == NULL)
    {
        return false;
    }

    // filter out response messages
    // TODO start checking these for confirmation (see LOG command)
    if (package->header.messageType & 0b1000000)
    {
        // the receiver responds to commands with a confirmation message,
        // which can be ignored
        debugPrintF("received command: %u\r\n", package->header.messageId);
    }
    else
    {
        parseMessage(package);
    }

    // set rxStatus for status callback
    rxStatusInfo.status = package->header.rxStatus;

    // TODO send a CAN packet updating the MET
    timeInfo.week = package->header.week;
    timeInfo.ms = package->header.ms;
    toUtc(&timeInfo.week, &timeInfo.ms, timeInfo.offset);

    PopGPSPackage();

    return true;
}

bool gpsFlush()
{
    bool updated = false;
    while (gpsUpdate())
    {
        updated = true;
    }
    return updated;
}
