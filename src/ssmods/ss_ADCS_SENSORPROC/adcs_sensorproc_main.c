#define GPS_TRACE_LEVEL 0
#define MS_IN_WEEK 604800000

#include <adcs_sensorproc.h>
#include <msp430.h> 

#include <math.h>

#include "bsp/bsp.h"
#include "core/debugtools.h"

#include "sensors/gps/gps.h"
#include "sensors/sun_sensor.h"
#include "sensors/photodiode.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state    = State_GPSOff;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerGPSOn;
FILE_STATIC flag_t triggerGPSOff;

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC sunsensor_segment sunsensorSeg;
FILE_STATIC photodiode_segment photodiodeSeg;
FILE_STATIC gpshealth_segment gpshealthSeg;
FILE_STATIC gpspower_segment gpspowerSeg;
FILE_STATIC rxstatus_segment rxstatusSeg;
FILE_STATIC time_segment timeSeg;
FILE_STATIC shared_segment sharedSeg;

// photodiode handles
FILE_STATIC uint8_t pdCenter;
FILE_STATIC uint8_t pdRight;
FILE_STATIC uint8_t pdLeft;

int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    asensorInit(Ref_2p5V);

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR, sizeof(sunsensorSeg));
    bcbinPopulateHeader(&photodiodeSeg.header, TLM_ID_PHOTODIODE, sizeof(photodiodeSeg));
    bcbinPopulateHeader(&gpshealthSeg.header, TLM_ID_GPSHEALTH, sizeof(gpshealthSeg));
    bcbinPopulateHeader(&gpspowerSeg.header, TLM_ID_GPSPOWER, sizeof(gpspowerSeg));
    bcbinPopulateHeader(&rxstatusSeg.header, TLM_ID_RXSTATUS, sizeof(rxstatusSeg));
    bcbinPopulateHeader(&timeSeg.header, TLM_ID_TIME, sizeof(timeSeg));

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

    // initialize sensors
    gpsInit();
    sunSensorInit();
    photodiodeInitAll();

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
    while (1)
    {
        static uint32_t i = 0;
        i++;

        if (i % 65536 == 0)
        {
            LED_OUT ^= LED_BIT;
            sendGPSPowerStatus();
            sendHealthSegment();
        }

        if (i % 524288 == 0)
        {
            sendMetaSegment();
        }

//        sunSensorUpdate(); // TODO don't let these block
//        photodiodeUpdate();
        if (i % 16384 == 0)
        {
            sendSunSensorData();
            sendPhotodiodeData();
        }

        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
        case State_GPSOff:
            if (triggerGPSOn)
            {
                // enable the buck converter
                gpsBuckOn();

                triggerGPSOn = 0;
                ss_state = State_BuckWaitOn;
            }
            break;
        case State_BuckWaitOn:
            if (gpsBuckGood())
            {
                // buck converter signal is good, enable the GPS
                gpsPowerOn();
                ss_state = State_GPSWait;
            }
            break;
        case State_GPSWait:
            // periodically poll the GPS until it responds
            // TODO replace this with timers?
            if (i % 65536 == 0)
            {
                gpsSendCommand("log rxstatusb\r\n");
            }
            if (handleGPSPackage(gpsRead()))
            {
                // now that the GPS is confirmed to be on, send commands to configure it
                gpsConfigure();
                ss_state = State_GPSOn;
            }
            break;
        case State_GPSOn:
            // read from the GPS and trigger message handlers
            handleGPSPackage(gpsRead());

            if (i % 65536 == 0)
            {
                sendGPSStatus();
            }

            if (triggerGPSOff)
            {
                // switch the GPS off
                gpsPowerOff();

                triggerGPSOff = 0;
                ss_state = State_BuckWaitOff;
            }
            break;
        case State_BuckWaitOff:
        {
            // wait a while before disabling the buck converter
            static uint16_t count = 0;
            if (count++ > 50000)
            {
                gpsBuckOff();
                ss_state = State_GPSOff;
                count = 0;
            }
            break;
        }
        default:
            mod_status.state_transition_errors++;
            mod_status.in_unknown_state++;
            break;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}

// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
void handleRollCall()
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
        enable_segment *enableSegment;

        // handle actions, any output should be ground-segment friendly
        // "packet" format
        switch(cmdstr[0])
        {
            case OPCODE_SENDASCII:
                // send the part of the command that comes after the opcode
                gpsSendCommand(cmdstr + 1);
                gpsSendCommand("\r\n");
                break;
            case OPCODE_ENABLE:
                enableSegment = (enable_segment *) (cmdstr + 1);
                if (enableSegment->enable && ss_state == State_GPSOff)
                {
                    triggerGPSOn = 1;
                }
                else if (enableSegment->enable == 0 && ss_state == State_GPSOn)
                {
                    triggerGPSOff = 1;
                }
                else
                {
                    // TODO log error
                }
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
void sendHealthSegment()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // send gps-specific health segment
    gpsGetHealth(&gpshealthSeg.health);
    bcbinSendPacket((uint8_t *) &gpshealthSeg, sizeof(gpshealthSeg));

    // determine overall health based on querying sensors for their health
    // TODO query sun sensor and photodiodes' health
    // TODO check if this is being used correctly
    hseg.oms = OMS_Nominal;
    if (gpshealthSeg.unknownEvt || gpshealthSeg.unknownMsg
            || gpshealthSeg.health.bad_crc || gpshealthSeg.health.skipped)
    {
        hseg.oms = OMS_MinorFaults;
    }
    if (rxstatusSeg.error)
    {
        hseg.oms = OMS_MajorFaults;
    }

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);
}

void sendMetaSegment()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

// send commands to configure the GPS
void gpsConfigure()
{
    // configure to reply in binary only
    gpsSendCommand("interfacemode thisport novatel novatelbinary on\r\n");

    gpsSendCommand("statusconfig clear status 004c0000\r\n");

    // log the status once. Status word will be updated at every message.
    gpsSendCommand("log rxstatusb\r\n");
    gpsSendCommand("log timeb\r\n");

    // monitor hardware
    gpsSendCommand("log hwmonitorb ontime 3\r\n");

    // monitor position
    gpsSendCommand("log bestxyzb ontime 1\r\n");
}

void sendGPSPowerStatus()
{
    gpspowerSeg.buckEnabled = gpsBuckEnabled();
    gpspowerSeg.gpsEnabled = gpsPowerEnabled();
    gpspowerSeg.state = ss_state;
    bcbinSendPacket((uint8_t *) &gpspowerSeg, sizeof(gpspowerSeg));
}

void sendGPSStatus()
{
    bcbinSendPacket((uint8_t *) &rxstatusSeg, sizeof(rxstatusSeg));
    bcbinSendPacket((uint8_t *) &timeSeg, sizeof(timeSeg));
}

bool handleGPSPackage(GPSPackage *p)
{
    if (p == NULL)
    {
        return false;
    }

    // process header
    rxstatusSeg.status = p->header.rxStatus;
    timeSeg.week = p->header.week;
    timeSeg.ms = p->header.ms;
    toUtc(&timeSeg.week, &timeSeg.ms, timeSeg.offset);

    // handle message-specific data
    switch (p->header.messageId)
    {
    case MSGID_RXSTATUS:
        handleRxStatus(p);
        break;
    case MSGID_TIME:
        handleTime(p);
        break;
    case MSGID_BESTXYZ:
        handleBestXYZ(p);
        break;
    case MSGID_HWMONITOR:
        handleHwMonitor(p);
        break;
    case MSGID_SATVIS2:
        handleSatvis2(p);
        break;
    case MSGID_RANGE:
        handleRange(p);
        break;
    case MSGID_RXSTATUSEVENT:
        handleRxStatusEvent(p);
        break;
    default:
        gpshealthSeg.unknownMsg++;
        break;
    }

    return true;
}

void handleRxStatus(const GPSPackage *package)
{
    const GPSRXStatus *m = &(package->message.rxstatus);

    debugTraceF(GPS_TRACE_LEVEL, "RXSTATUS error word: %X \r\n", m->error);

    rxstatusSeg.error = m->error;
    rxstatusSeg.aux1 = m->aux1stat.word;
    rxstatusSeg.aux2 = m->aux2stat.word;
    rxstatusSeg.aux3 = m->aux3stat.word;
}

void handleTime(const GPSPackage *package)
{
    const GPSTime *m = &(package->message.time);

    // update the time offset
    // used in pos/vel messages and updating MET
    timeSeg.offset = m->offset + m->utcOffset;
    timeSeg.clockStatus = m->clockStatus;
    timeSeg.utcStatus = m->utcStatus;

    debugTraceF(GPS_TRACE_LEVEL, "TIME offset: %f\r\n", timeSeg.offset);
}

void handleBestXYZ(const GPSPackage *package)
{
    const GPSBestXYZ *m = &(package->message.bestXYZ);

    uint16_t week = package->header.week;
    gps_ec ms = package->header.ms;
    toUtc(&week, &ms, timeSeg.offset - m->velLatency);

    bestxyz_segment *bestxyzSeg = &sharedSeg.bestxyz;
    bestxyzSeg->posStatus = m->pSolStatus;
    bestxyzSeg->pos = m->pos;
    bestxyzSeg->posStdDev = m->posStdDev;
    bestxyzSeg->velStatus = m->vSolStatus;
    bestxyzSeg->vel = m->vel;
    bestxyzSeg->velStdDev = m->velStdDev;
    bestxyzSeg->week = week;
    bestxyzSeg->ms = ms;

    bcbinPopulateHeader(&sharedSeg.bestxyz.header, TLM_ID_BESTXYZ, sizeof(bestxyz_segment));
    bcbinSendPacket((uint8_t *) &sharedSeg, sizeof(bestxyz_segment));
}

void handleHwMonitor(const GPSPackage *package)
{
    const GPSHWMonitor *monLog = &(package->message.hwMonitor);
    hwmonitor_segment *hwmonitorSeg = &sharedSeg.hwmonitor;

    uint32_t i = monLog->numMeasurements;
    while (i-- != 0)
    {
        float reading = monLog->mes[i].reading;
        uint8_t type = monLog->mes[i].type;
        switch (type)
        {
        case HW_TEMP:
            hwmonitorSeg->temp = reading;
            break;
        case HW_ANTCUR:
            hwmonitorSeg->antCurrent = reading;
            break;
        case HW_SUPVOLT:
            hwmonitorSeg->supVolt = reading;
            break;
        case HW_ANTVOLT:
            hwmonitorSeg->antVolt = reading;
            break;
        case HW_DIGCORE:
            hwmonitorSeg->digCoreVolt = reading;
            break;
        case HW_TEMP2:
            hwmonitorSeg->secTemp = reading;
            break;
        default:
            break;
        }
    }

    bcbinPopulateHeader(&sharedSeg.hwmonitor.header, TLM_ID_HWMONITOR, sizeof(hwmonitor_segment));
    bcbinSendPacket((uint8_t *) &sharedSeg, sizeof(hwmonitor_segment));
}

void handleSatvis2(const GPSPackage *package)
{
    const GPSSatvis2 *satvis2 = &(package->message.satvis2);
    debugTraceF(GPS_TRACE_LEVEL, "Satvis2:\r\n");
    debugTraceF(GPS_TRACE_LEVEL, "\tsystem: %u\r\n", satvis2->system);
    debugTraceF(GPS_TRACE_LEVEL, "\tnum:    %u\r\n", satvis2->numSats);

    satvis2_segment *satvis2Seg = &sharedSeg.satvis2;
    switch (satvis2->system)
    {
        case SATSYSTEM_GPS:
            satvis2Seg->numGPS = satvis2->numSats;
            break;
        case SATSYSTEM_GLONASS:
            satvis2Seg->numGLONASS = satvis2->numSats;
            break;
        case SATSYSTEM_SBAS:
            satvis2Seg->numSBAS = satvis2->numSats;
            break;
    }

    bcbinPopulateHeader(&sharedSeg.satvis2.header, TLM_ID_SATVIS2, sizeof(satvis2_segment));
    bcbinSendPacket((uint8_t *) &sharedSeg, sizeof(satvis2_segment));
}

void handleRange(const GPSPackage *package)
{
    const GPSRange *range = &(package->message.range);

    sharedSeg.range = (const range_segment){ 0 };

    range_segment *rangeSeg = &sharedSeg.range;
    uint32_t i;
    uint32_t numObs = range->numObs < NUM_RANGE_SEGMENT_MEMBERS ? range->numObs : NUM_RANGE_SEGMENT_MEMBERS;
    for (i = 0; i < numObs; i++)
    {
        rangeSeg->obs[i].prn = range->obs[i].prn;
        rangeSeg->obs[i].cToNo = range->obs[i].cToNo;
        rangeSeg->obs[i].locktime = range->obs[i].locktime;
    }

    bcbinPopulateHeader(&sharedSeg.range.header, TLM_ID_RANGE, sizeof(range_segment));
    bcbinSendPacket((uint8_t *) &sharedSeg, sizeof(range_segment));
}

void handleRxStatusEvent(const GPSPackage *package)
{
    const GPSRXStatusEvent *e = &(package->message.rxstatusEvent);

    // only read the status events
    // TODO read and deal with error/aux events too
    if (e->word != 1)
    {
        return;
    }

    if (e->event == EVTTYPE_CLEAR)
    {
        switch (e->bitPosition)
        {
        case EVTID_POSITION:
            handlePositionStatusEvent(e);
            break;
        case EVTID_UTC:
            handleClockStatusEvent(e);
            break;
        case EVTID_CLOCK:
            handleClockStatusEvent(e);
            break;
        default:
            gpshealthSeg.unknownEvt++;
            break;
        }
    }
    else
    {
        gpshealthSeg.unknownEvt++;
    }
}

void handlePositionStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log bestxyzb\r\n");
}

void handleClockStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log timeb\r\n");
}

void toUtc(uint16_t *week, gps_ec *ms, double offset)
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

// read the sun sensor and update its segments
void sunSensorUpdate()
{
    SunSensorAngle *angle = sunSensorReadAngle();
    if (angle != NULLP)
    {
        sunsensorSeg.alpha = angle->alpha;
        sunsensorSeg.beta = angle->beta;
        sunsensorSeg.error = angle->error;
    }
    else
    {
        // TODO log read error
    }
}

void sendSunSensorData()
{
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));
}

void photodiodeInitAll()
{
    pdCenter = photodiodeInit(PD_ADDR_HH);
    pdRight = photodiodeInit(PD_ADDR_FF);
    pdLeft = photodiodeInit(PD_ADDR_HF);
    // TODO init other photodiodes when we have them
}

void photodiodeUpdate()
{
    photodiodeSeg.center = photodiodeRead(defaultWrite, pdCenter);
    photodiodeSeg.right = photodiodeRead(defaultWrite, pdRight);
    photodiodeSeg.left = photodiodeRead(defaultWrite, pdLeft);
}

void sendPhotodiodeData()
{
    bcbinSendPacket((uint8_t *) &photodiodeSeg, sizeof(photodiodeSeg));
}
