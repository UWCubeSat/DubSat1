#define GPS_TRACE_LEVEL 0
#define MS_IN_WEEK 604800000

#include <adcs_sensorproc.h>
#include <msp430.h> 

#include <math.h>

#include "bsp/bsp.h"
#include "core/debugtools.h"

#include "sensors/gps/gps.h"
#include "sensors/gps/GPSIDs.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state    = State_GPSOff;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerGPSPoweringOn;
FILE_STATIC flag_t triggerGPSOn;
FILE_STATIC flag_t triggerGPSOff;

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC rxstatus_segment rxstatusSeg;
FILE_STATIC bestxyz_segment bestxyzSeg;
FILE_STATIC time_segment timeSeg;
FILE_STATIC hwmonitor_segment hwmonitorSeg;
FILE_STATIC satvis2_segment satvis2Seg;

// called on every gps message
FILE_STATIC void handleGPSMessage(const GPSHeader *header);

// gps message handlers
FILE_STATIC void handleRxStatus(const GPSPackage *package);
FILE_STATIC void handleTime(const GPSPackage *package);
FILE_STATIC void handleBestXYZ(const GPSPackage *package);
FILE_STATIC void handleHwMonitor(const GPSPackage *package);
FILE_STATIC void handleSatvis2(const GPSPackage *package);

// gps status event handlers
FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e);
FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e);

// util functions
FILE_STATIC void toUtc(uint16_t *week, gps_ec *ms, double offset);

// send commands to configure the GPS
FILE_STATIC void gpsConfigure()
{
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

FILE_STATIC void gpsSendStatus()
{
    bcbinSendPacket((uint8_t *) &rxstatusSeg, sizeof(rxstatusSeg));
    bcbinSendPacket((uint8_t *) &timeSeg, sizeof(timeSeg));
}

/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up sync pulse handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    // TODO meta segment?
    // TODO what should the opcodes be?
    bcbinPopulateHeader(&rxstatusSeg.header, 123, sizeof(rxstatusSeg));
    bcbinPopulateHeader(&bestxyzSeg.header, 124, sizeof(bestxyzSeg));
    bcbinPopulateHeader(&timeSeg.header, 125, sizeof(timeSeg));
    bcbinPopulateHeader(&hwmonitorSeg.header, 126, sizeof(hwmonitorSeg));
    bcbinPopulateHeader(&satvis2Seg.header, 127, sizeof(satvis2Seg));

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, handleDebugInfoCallback,
                                     handleDebugStatusCallback,
                                     handleDebugActionCallback);

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize GPS data structures and UART read callback
    gpsInit(handleGPSMessage);

    // register GPS message handlers
    gpsRegisterMessageHandler(MSGID_RXSTATUS, handleRxStatus);
    gpsRegisterMessageHandler(MSGID_TIME, handleTime);
    gpsRegisterMessageHandler(MSGID_BESTXYZ, handleBestXYZ);
    gpsRegisterMessageHandler(MSGID_HWMONITOR, handleHwMonitor);
    gpsRegisterMessageHandler(MSGID_SATVIS2, handleSatvis2);

    // register GPS event handlers
    gpsRegisterEventHandler(EVTID_POSITION, handlePositionStatusEvent,
                                    EVTTYPE_CLEAR);
    gpsRegisterEventHandler(EVTID_UTC, handleClockStatusEvent, EVTTYPE_CLEAR);
    gpsRegisterEventHandler(EVTID_CLOCK, handleClockStatusEvent, EVTTYPE_CLEAR);

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
    while (1)
    {
        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
        case State_GPSOff:
            if (triggerGPSPoweringOn)
            {
                // switch the GPS on
                gpsPowerOn();

                triggerGPSPoweringOn = 0;
                ss_state = State_GPSPoweringOn;
            }
            break;
        case State_GPSPoweringOn:
            if (triggerGPSOn)
            {
                // now that the GPS is confirmed to be on, send commands to configure it
                gpsConfigure();

                triggerGPSOn = 0;
                ss_state = State_GPSOn;
            }
            break;
        case State_GPSOn:
            // read from the GPS and trigger message handlers
            if (gpsUpdate())
            {
                // send status telemetry on update
                // TODO send meta and health segments
                gpsSendStatus();
            }

            if (triggerGPSOff)
            {
                // switch the GPS off
                gpsPowerOff();

                triggerGPSOff = 0;
                ss_state = State_GPSOff;
            }
            break;
        default:
            mod_status.state_transition_errors++;
            mod_status.in_unknown_state++;
            break;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

/* ----- SYNC PULSE INTERRUPT HANDLERS ----- */
// Both of these handlers are INTERRUPT HANDLERS, and run as such, which means that all OTHER
// interrupts are blocked while they are running.  This can cause all sorts of issues, so
// MAKE SURE TO MINIMIZE THE CODE RUNNING IN THESE FUNCTIONS.
// Sync pulse 1:  typically raised every 2 seconds while PPT firing, to help each subsystem module
// do the "correct thing" around the firing sequence.  This timing might
// not be exact, and may even change - don't rely on it being 2 seconds every time, and it may
// be shut off entirely during early or late stages of mission, so also do NOT use as a "heartbeat"
// for other, unrelated functionality.
void handleSyncPulse1()
{
    __no_operation();
}

// Sync pulse 2:  typically every 1-2 minutes, but again, don't count on any length.
// General semanatics are that this pulse means all subsystems should share accumulated
// status data on the CAN bus.  It is also the cue for the PPT to ascertain whether it
// will use the following period as an active or suspended firing period.  All subsystems
// will assume active until they are notified that firing has been suspended, but
// this determination will be reset (back to active) at each sync pulse 2.
void handleSyncPulse2()
{
    __no_operation();
}

// Optional callback for the debug system.  "Info" is considered static information
// that doesn't change about the subsystem module code/executable, so this is most
// often left off.
uint8_t handleDebugInfoCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // debugPrintF information in a user-friendly, formatted way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // debugPrintF information without field names, as CSV
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // debugPrintF into a ground segment-friendly "packet" mode
    }
    return 1;
}

// Optional callback for the debug system.  "Status" is considered the
// current state of dynamic information about the subsystem module, and is the most
// common to be surfaced, particularly as "streaming telemetry".
uint8_t handleDebugStatusCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // debugPrintF status in a user-friendly, formatted way
        debugPrintF("GPS Status: 0x%08lx\r\n", rxstatusSeg.status);
        uint8_t i = 32;
        while (i--)
        {
            if (rxstatusSeg.status & ((uint32_t) 1 << i))
            {
                debugPrintF("\terror #%u - %s\r\n", i, GPS_STATUS_DESC[i]);
            }
        }
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // debugPrintF status without field names, as CSV
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // debugPrintF status a ground segment-friendly "packet" format
    }
    return 1;
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // handle actions in a user-friendly way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // handle actions in a low-output way
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // handle actions, any output should be ground-segment friendly
        // "packet" format
        switch(cmdstr[0])
        {
            case OPCODE_SENDASCII:
                // send the part of the command that comes after the opcode
                gpsSendCommand(cmdstr + 1);
                gpsSendCommand("\r\n");
                break;
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
}

// --- gps message handlers ---

FILE_STATIC void handleGPSMessage(const GPSHeader *header)
{
    rxstatusSeg.status = header->rxStatus;

    timeSeg.week = header->week;
    timeSeg.ms = header->ms;
    toUtc(&timeSeg.week, &timeSeg.ms, timeSeg.offset);
}

FILE_STATIC void handleRxStatus(const GPSPackage *package)
{
    const GPSRXStatus m = package->message.rxstatus;

    debugTraceF(GPS_TRACE_LEVEL, "RXSTATUS error word: %X \r\n", m.error);

    rxstatusSeg.error = m.error;
    rxstatusSeg.aux1 = m.aux1stat.word;
    rxstatusSeg.aux2 = m.aux2stat.word;
    rxstatusSeg.aux3 = m.aux3stat.word;
}

FILE_STATIC void handleTime(const GPSPackage *package)
{
    const GPSTime m = package->message.time;

    debugTraceF(GPS_TRACE_LEVEL, "TIME offset: %f\r\n", timeSeg.offset);

    // update the time offset
    // used in pos/vel messages and updating MET
    timeSeg.offset = m.offset + m.utcOffset;
    timeSeg.clockStatus = m.clockStatus;
    timeSeg.utcStatus = m.utcStatus;
}

FILE_STATIC void handleBestXYZ(const GPSPackage *package)
{
    const GPSBestXYZ m = package->message.bestXYZ;

    uint16_t week = package->header.week;
    gps_ec ms = package->header.ms;
    toUtc(&week, &ms, timeSeg.offset - m.velLatency);

    debugTraceF(GPS_TRACE_LEVEL,
                "BESTXYZ (%u)\r\n\tx: %f\r\n\ty: %f\r\n\tz: %f\r\n",
                m.pSolStatus, m.pos.x, m.pos.y, m.pos.z);

    bestxyzSeg.posStatus = m.pSolStatus;
    bestxyzSeg.pos = m.pos;
    bestxyzSeg.posStdDev = m.posStdDev;
    bestxyzSeg.velStatus = m.vSolStatus;
    bestxyzSeg.vel = m.vel;
    bestxyzSeg.velStdDev = m.velStdDev;
    bestxyzSeg.week = week;
    bestxyzSeg.ms = ms;

    bcbinSendPacket((uint8_t *) &bestxyzSeg, sizeof(bestxyzSeg));
}

FILE_STATIC void handleHwMonitor(const GPSPackage *package)
{
    const GPSHWMonitor monLog = package->message.hwMonitor;
    debugTraceF(GPS_TRACE_LEVEL, "HWMonitor:\r\n");
    debugTraceF(GPS_TRACE_LEVEL, "\ttemp: %f C\r\n", monLog.temp.reading);

    // TODO add other measurements to COSMOS
    hwmonitorSeg.temp = monLog.temp.reading;
    hwmonitorSeg.tempStatus = monLog.temp.status;

    bcbinSendPacket((uint8_t *) &hwmonitorSeg, sizeof(hwmonitorSeg));
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
            satvis2Seg.numGPS = satvis2.numSats;
            break;
        case SATSYSTEM_GLONASS:
            satvis2Seg.numGLONASS = satvis2.numSats;
            break;
        case SATSYSTEM_SBAS:
            satvis2Seg.numSBAS = satvis2.numSats;
            break;
    }

    bcbinSendPacket((uint8_t *) &satvis2Seg, sizeof(satvis2Seg));
}

// --- gps status event handlers ---

FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log bestxyzb\r\n");
}

FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log timeb\r\n");
}

// --- util functions ---

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
