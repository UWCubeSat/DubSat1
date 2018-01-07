#define MS_IN_WEEK 604800000
#define PACKAGE_BUFFER_LENGTH 3
#define GPS_TRACE_LEVEL 0
#define HANDLER_BUFFER_LENGTH 10

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "core/uart.h"
#include "core/debugtools.h"

#include "gps.h"
#include "GPSPackage.h"
#include "GPSReader.h"
#include "GPSIDs.h"
#include "crc.h"

typedef struct message_handler_entity
{
    gps_message_handler handler;
    gps_message_id id;
} message_handler_entity;

FILE_STATIC message_handler_entity messageHandlers[HANDLER_BUFFER_LENGTH];
FILE_STATIC uint8_t numMessageHandlers = 0;

typedef struct event_handler_entity
{
    gps_event_handler handler;
    gps_event_id eventId;
    gps_enum eventType;
} event_handler_entity;

FILE_STATIC event_handler_entity eventHandlers[HANDLER_BUFFER_LENGTH];
FILE_STATIC uint8_t numEventHandlers = 0;

FILE_STATIC GPSPackage packageBuffer[PACKAGE_BUFFER_LENGTH];

FILE_STATIC hBus uartHandle;

FILE_STATIC RXStatusInfo rxStatusInfo;
FILE_STATIC BestXYZInfo bestXYZInfo;
FILE_STATIC TimeInfo timeInfo;
FILE_STATIC HWMonitorInfo hwMonitorInfo;
FILE_STATIC Satvis2Info satvis2Info;

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

// debug callbacks
FILE_STATIC uint8_t statusCallback(DebugMode mode);
FILE_STATIC uint8_t actionCallback(DebugMode mode, uint8_t *command);

// message handlers
FILE_STATIC void handleRxStatusEvent(const GPSPackage *package);
FILE_STATIC void handleRxStatus(const GPSPackage *package);
FILE_STATIC void handleTime(const GPSPackage *package);
FILE_STATIC void handleBestXYZ(const GPSPackage *package);
FILE_STATIC void handleHwMonitor(const GPSPackage *package);
FILE_STATIC void handleSatvis2(const GPSPackage *package);

// status event handlers
FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e);
FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e);

// utility functions
FILE_STATIC void toUtc(uint16_t *week, gps_ec *ms, double offset);

void gpsInit()
{
    uartHandle = uartInit(ApplicationUART, 0, Speed_9600);

    // initialize the reader
    GPSReaderInit(uartHandle, packageBuffer, PACKAGE_BUFFER_LENGTH);

    // configure default message event handlers
    gpsRegisterMessageHandler(MSGID_RXSTATUS, handleRxStatus);
    gpsRegisterMessageHandler(MSGID_RXSTATUSEVENT, handleRxStatusEvent);
    gpsRegisterMessageHandler(MSGID_TIME, handleTime);
    gpsRegisterMessageHandler(MSGID_BESTXYZ, handleBestXYZ);
    gpsRegisterMessageHandler(MSGID_HWMONITOR, handleHwMonitor);
    gpsRegisterMessageHandler(MSGID_SATVIS2, handleSatvis2);

    // TODO what should the opcodes be?
    bcbinPopulateHeader(&rxStatusInfo.header, 123, sizeof(rxStatusInfo));
    bcbinPopulateHeader(&bestXYZInfo.header, 124, sizeof(bestXYZInfo));
    bcbinPopulateHeader(&timeInfo.header, 125, sizeof(timeInfo));
    bcbinPopulateHeader(&hwMonitorInfo.header, 126, sizeof(hwMonitorInfo));
    bcbinPopulateHeader(&satvis2Info.header, 127, sizeof(satvis2Info));

    debugRegisterEntity(Entity_Test, NULL, statusCallback, actionCallback);
}

void gpsPowerOn()
{
    // TODO switch on the power
    /*
     * Using timers, periodically poll the GPS with a log to see if its on.
     * Consider doing this asynchronously with some powered on callback that
     * configures the GPS.
     */

    // configure default status event handlers
    gpsRegisterEventHandler(EVTID_POSITION, handlePositionStatusEvent,
                            EVTTYPE_CLEAR);
    gpsRegisterEventHandler(EVTID_UTC, handleClockStatusEvent, EVTTYPE_CLEAR);
    gpsRegisterEventHandler(EVTID_CLOCK, handleClockStatusEvent, EVTTYPE_CLEAR);

    // configure to reply in binary only
    gpsSendCommand("interfacemode thisport novatel novatelbinary on\r\n");

    // log the status once. Status word will be updated at every message.
    gpsSendCommand("log rxstatusb\r\n");
    gpsSendCommand("log timeb\r\n");

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
    /*
     * TODO start checking these for confirmation (see LOG command).
     * Replies are in the same form as the command, so we need to switch to
     * sending binary commands if we want confirmation messages.
     */
    if (package->header.messageType & 0b1000000)
    {
        // the receiver responds to commands with a confirmation message,
        // which can be ignored
        debugPrintF("received command: %u\r\n", package->header.messageId);
    }
    else
    {
        // delegate the package to its handlers
        uint8_t i = numMessageHandlers;
        uint8_t numRegistered = 0;
        while (i-- != 0)
        {
            if (messageHandlers[i].id == package->header.messageId)
            {
                (messageHandlers[i].handler)(package);
                numRegistered++;
            }
        }
        if (numRegistered == 0)
        {
            debugTraceF(GPS_TRACE_LEVEL, "unregistered message ID: %u\r\n",
                        package->header.messageId);
        }
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

void gpsSendCommand(uint8_t *command)
{
    uartTransmit(uartHandle, command, strlen((char*) command));
}

void gpsSendBinaryCommand(gps_message_id messageId,
                          uint8_t *message,
                          uint16_t messageLength)
{
    // populate header
    GPSHeader header;
    header.sync[0] = 0xAA;
    header.sync[1] = 0x44;
    header.sync[2] = 0x12;
    header.headerLength = sizeof(header);
    header.messageId = messageId;
    header.messageType = 0; // ???
    header.portAddress = 0xc0; // 0xc0: THISPORT
    header.messageLength = messageLength;
    // all other header fields are either ignored or recommended 0 on input
    header.sequence = 0;
    header.idleTime = 0;
    header.timeStatus = 0;
    header.week = 0;
    header.ms = 0;
    header.rxStatus = 0;
    header.reserved = 0;
    header.rxVersion = 0;

    // calculate crc
    uint32_t crc = calculateBlockCrc32(sizeof(header), (uint8_t *) &header);
    uint16_t i;
    for (i = 0; i < messageLength; i++)
    {
        crc = continueCrc32(crc, message[i]);
    }

    // transmit command
    uartTransmit(uartHandle, (uint8_t *) &header, sizeof(header));
    uartTransmit(uartHandle, message, messageLength);
    uartTransmit(uartHandle, (uint8_t *) &crc, sizeof(crc));
}

bool gpsRegisterMessageHandler(gps_message_id messageId,
                               gps_message_handler handler)
{
    if (numMessageHandlers >= HANDLER_BUFFER_LENGTH)
    {
        debugPrintF("too many message handlers\r\n");
        return false;
    }
    messageHandlers[numMessageHandlers++]
                    = (message_handler_entity){ handler, messageId };
    return true;
}

bool gpsRegisterEventHandler(gps_event_id eventId,
                             gps_event_handler handler,
                             gps_enum eventType)
{
    if (numEventHandlers >= HANDLER_BUFFER_LENGTH)
    {
        debugPrintF("too many event handlers\r\n");
        return false;
    }

    // configure the GPS to send a status event message
    // TODO send the statusconfig command again without using sprintf
    StatusConfig cmd;
    cmd.type = eventType;
    cmd.word = 1; // for status
    cmd.mask = ((uint32_t) 1) << eventId;
    gpsSendBinaryCommand(CMDID_STATUSCONFIG, (uint8_t *) &cmd, sizeof(cmd));

    eventHandlers[numEventHandlers++]
                  = (event_handler_entity){ handler, eventId, eventType };
    return true;
}

FILE_STATIC uint8_t statusCallback(DebugMode mode)
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
        // These packets are updated in the header of every message, so they
        // should be sent at every update
        bcbinSendPacket((uint8_t *) &rxStatusInfo, sizeof(rxStatusInfo));
        bcbinSendPacket((uint8_t *) &timeInfo, sizeof(timeInfo));

        // These packets are only updated when the message comes in from the
        // GPS. Sending them could be postponed until they are updated.
        bcbinSendPacket((uint8_t *) &bestXYZInfo, sizeof(bestXYZInfo));
        bcbinSendPacket((uint8_t *) &hwMonitorInfo, sizeof(hwMonitorInfo));
        bcbinSendPacket((uint8_t *) &satvis2Info, sizeof(satvis2Info));
    }
    return 1;
}

FILE_STATIC uint8_t actionCallback(DebugMode mode, uint8_t *command)
{
    uint8_t len = strlen((const char *) command);

    if (mode == Mode_ASCIIInteractive)
    {
        uartTransmit(uartHandle, command, len);
        uartTransmit(uartHandle, "\r\n", 2);
    }
    else
    {
        // switch based on the OPCODE of the command
        switch(command[0])
        {
            case OPCODE_SEND_ASCII:
                if (len > 1) {
                    // send the part of the command that comes after the opcode
                    uartTransmit(uartHandle, command + 1, len - 1);

                    // execute the command
                    uartTransmit(uartHandle, "\r\n", 2);
                } else {
                    return 0;
                }
                break;
        }
    }
    return 1;
}

FILE_STATIC void handleRxStatusEvent(const GPSPackage *package)
{
    const GPSRXStatusEvent e = package->message.rxstatusEvent;

    // only read the status events
    // TODO read and deal with error/aux events too
    if (e.word != 1)
    {
        return;
    }

    uint8_t i = numEventHandlers;
    uint8_t foundAtLeastOne = 0;
    while (i-- != 0)
    {
        event_handler_entity h = eventHandlers[i];
        if (h.eventId == e.bitPosition && h.eventType == e.event)
        {
            (eventHandlers[i].handler)(&e);
            foundAtLeastOne = 1;
        }
    }
    if (!foundAtLeastOne)
    {
        debugTraceF(GPS_TRACE_LEVEL, "unregistered event ID: %u\r\n",
                            package->header.messageId);
    }
}

FILE_STATIC void handleRxStatus(const GPSPackage *package)
{
    const GPSRXStatus m = package->message.rxstatus;

    debugTraceF(GPS_TRACE_LEVEL, "RXSTATUS error word: %X \r\n", m.error);

    rxStatusInfo.error = m.error;
    rxStatusInfo.aux1 = m.aux1stat.word;
    rxStatusInfo.aux2 = m.aux2stat.word;
    rxStatusInfo.aux3 = m.aux3stat.word;
}

FILE_STATIC void handleTime(const GPSPackage *package)
{
    const GPSTime m = package->message.time;

    debugTraceF(GPS_TRACE_LEVEL, "TIME offset: %f\r\n", timeInfo.offset);

    // update the time offset
    // used in pos/vel messages and updating MET
    timeInfo.offset = m.offset + m.utcOffset;
    timeInfo.clockStatus = m.clockStatus;
    timeInfo.utcStatus = m.utcStatus;
}

FILE_STATIC void handleBestXYZ(const GPSPackage *package)
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
}

FILE_STATIC void handleHwMonitor(const GPSPackage *package)
{
    const GPSHWMonitor monLog = package->message.hwMonitor;
    debugTraceF(GPS_TRACE_LEVEL, "HWMonitor:\r\n");
    debugTraceF(GPS_TRACE_LEVEL, "\ttemp: %f C\r\n", monLog.temp.reading);

    // TODO add other measurements to COSMOS
    hwMonitorInfo.temp = monLog.temp.reading;
    hwMonitorInfo.tempStatus = monLog.temp.status;
}

FILE_STATIC void handleSatvis2(const GPSPackage *package)
{
    const GPSSatvis2 satvis2 = package->message.satvis2;
    debugTraceF(GPS_TRACE_LEVEL, "Satvis2:\r\n");
    debugTraceF(GPS_TRACE_LEVEL, "\tsystem: %u\r\n", satvis2.system);
    debugTraceF(GPS_TRACE_LEVEL, "\tnum:    %u\r\n", satvis2.numSats);

    switch (satvis2.system)
    {
        case SATSYSTEM_GPS:
            satvis2Info.numGPS = satvis2.numSats;
            break;
        case SATSYSTEM_GLONASS:
            satvis2Info.numGLONASS = satvis2.numSats;
            break;
        case SATSYSTEM_SBAS:
            satvis2Info.numSBAS = satvis2.numSats;
            break;
    }
}

FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log bestxyzb\r\n");
}

FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log timeb\r\n");
}

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
