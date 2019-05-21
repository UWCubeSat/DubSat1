/*
 * gps_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

/*
 * minimum number of milliseconds between switching on/off the buck converter
 * and switching on/off the GPS.
 */
#define DELAY_BUCK_MS 300

/*
 * number of milliseconds to hold the RESET pin low after switching on the GPS.
 * The OEM719 manual recommends > 150 ms.
 */
#define DELAY_GPS_RESET_MS 150

/*
 * number of milliseconds between each query of the GPS to check if it is
 * responsive after turning it on.
 */
#define DELAY_GPS_POLL_MS 1000

/*
 * number of milliseconds between each manual log while the GPS is on.
 */
#define DELAY_MANUAL_LOG_MS 3000

#define M_PI acos(-1.0)

#include <math.h>

#include "gps_io.h"
#include "adcs_sensorproc_ids.h"
#include "core/debugtools.h"
#include "core/utils.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"

//------------------------------------------------------------------
// outgoing telemetry
//------------------------------------------------------------------

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC gpshealth_segment gpshealthSeg;
FILE_STATIC gpspower_segment gpspowerSeg;
FILE_STATIC rxstatus_segment rxstatusSeg;
FILE_STATIC time_segment timeSeg;

//------------------------------------------------------------------
// power state management
//------------------------------------------------------------------

/*
|---------------+--------+---------+------+----------|
| Name          | Inputs | Outputs |      |          |
|---------------+--------+---------+------+----------|
|               | PGOOD  | GPS     | Buck | RESET    |
|---------------+--------+---------+------+----------|
| Off           | N/A    | Off     | Off  | N/A      |
|---------------+--------+---------+------+----------|
| EnablingBuck  | Bad    | Off     | On   | Active   |
|---------------+--------+---------+------+----------|
| EnablingGPS   | Good   | On      | On   | Active   |
|---------------+--------+---------+------+----------|
| AwaitingGPSOn | Good   | On      | On   | Inactive |
|---------------+--------+---------+------+----------|
| On            | Good   | On      | On   | Inactive |
|---------------+--------+---------+------+----------|
| ShuttingDown  | N/A    | Off     | On   | Inactive |
|---------------+--------+---------+------+----------|
 */

typedef enum {
    PowerCmd_Enable,
    PowerCmd_Disable,
    PowerCmd_None
} gps_power_cmd;

FILE_STATIC volatile gps_power_state_code gpsPowerState;

typedef gps_power_state_code (* state_fn)(gps_power_cmd);
typedef void (* state_entry_fn)(void);

typedef struct {
    /**
     * The start function is called when entering this state. It adjusts outputs
     * so that all the states constraints are met.
     */
    state_entry_fn start;

    /**
     * The run function of the current state is called on every update. It
     * processes commands and other inputs to return the next state.
     */
    state_fn run;
} state;

FILE_STATIC void enterStateOff();
FILE_STATIC gps_power_state_code stateOff(gps_power_cmd cmd);
FILE_STATIC void enterStateEnablingBuck();
FILE_STATIC gps_power_state_code stateEnablingBuck(gps_power_cmd cmd);
FILE_STATIC void enterStateEnablingGPS();
FILE_STATIC gps_power_state_code stateEnablingGPS(gps_power_cmd cmd);
FILE_STATIC void enterStateAwaitingGPSOn();
FILE_STATIC gps_power_state_code stateAwaitingGPSOn(gps_power_cmd cmd);
FILE_STATIC void enterStateOn();
FILE_STATIC gps_power_state_code stateOn(gps_power_cmd cmd);
FILE_STATIC void enterStateShuttingDown();
FILE_STATIC gps_power_state_code stateShuttingDown(gps_power_cmd cmd);

// keep in sync with codes in header file!
FILE_STATIC const state states[] = {
        { enterStateOff, stateOff },
        { enterStateEnablingBuck, stateEnablingBuck },
        { enterStateEnablingGPS, stateEnablingGPS },
        { enterStateAwaitingGPSOn, stateAwaitingGPSOn },
        { enterStateOn, stateOn },
        { enterStateShuttingDown, stateShuttingDown },
};

FILE_STATIC int timerHandle;

//------------------------------------------------------------------
// command handling
//------------------------------------------------------------------

FILE_STATIC flag_t triggerGPSOn;
FILE_STATIC flag_t triggerGPSOff;

FILE_STATIC void canRxCallback(CANPacket *packet);

//------------------------------------------------------------------
// incoming telemetry handlers
//------------------------------------------------------------------

// message handlers
FILE_STATIC void handleRxStatus(const GPSPackage *package);
FILE_STATIC void handleTime(const GPSPackage *package);
FILE_STATIC void handleBestXYZ(const GPSPackage *package);
FILE_STATIC void handleHwMonitor(const GPSPackage *package);
FILE_STATIC void handleSatvis2(const GPSPackage *package);
FILE_STATIC void handleRange(const GPSPackage *package);
FILE_STATIC void handleRxStatusEvent(const GPSPackage *package);

// status event handlers
FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e);
FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e);

// util functions
FILE_STATIC void toUtc(uint16_t *week, gps_ec *ms, double offset);
FILE_STATIC void sendBestXYZOverBackchannel(const GPSBestXYZ *m, uint16_t week, gps_ec ms);
FILE_STATIC void sendBestXYZOverCAN(const GPSBestXYZ *m, uint16_t week, gps_ec ms);

void gpsioInit()
{
    setCANPacketRxCallback(canRxCallback);

    gpshealthSeg = (gpshealth_segment) { 0 };
    gpspowerSeg = (gpspower_segment) { 0 };
    rxstatusSeg = (rxstatus_segment) { 0 };
    timeSeg = (time_segment) { 0 };

    bcbinPopulateHeader(&gpshealthSeg.header, TLM_ID_GPSHEALTH, sizeof(gpshealthSeg));
    bcbinPopulateHeader(&gpspowerSeg.header, TLM_ID_GPSPOWER, sizeof(gpspowerSeg));
    bcbinPopulateHeader(&rxstatusSeg.header, TLM_ID_RXSTATUS, sizeof(rxstatusSeg));
    bcbinPopulateHeader(&timeSeg.header, TLM_ID_TIME, sizeof(timeSeg));

    triggerGPSOn = FALSE;
    triggerGPSOff = FALSE;

    gpsPowerState = State_Off;

    initializeTimer();

    gpsInit();
}

void gpsioConfig()
{
    // configure to reply in binary only
    gpsSendCommand("interfacemode thisport novatel novatelbinary on\r\n");

    gpsSendCommand("statusconfig clear status 004c0000\r\n");

    gpsSendCommand("log rxstatusb\r\n");
    gpsSendCommand("log timeb\r\n");

    // monitor hardware
    // TODO this only logs once for some reason
    gpsSendCommand("log hwmonitorb ontime 3\r\n");

    // monitor position
    gpsSendCommand("log bestxyzb ontime 1\r\n");

    // TODO remove before flight
    gpsSendCommand("log satvis2b ontime 3\r\n");
    gpsSendCommand("log rangeb ontime 3\r\n");
}

void gpsioUpdate()
{
    // convert the trigger flags into an enum
    gps_power_cmd cmd;
    if (triggerGPSOn)
    {
        triggerGPSOn = FALSE;
        cmd = PowerCmd_Enable;
    }
    else if (triggerGPSOff)
    {
        triggerGPSOff = FALSE;
        cmd = PowerCmd_Disable;
    }
    else
    {
        cmd = PowerCmd_None;
    }

    // run the current state
    gps_power_state_code prevState = gpsPowerState;
    gpsPowerState = states[gpsPowerState].run(cmd);

    // if the state changed, start the next state
    if (prevState != gpsPowerState)
    {
        states[gpsPowerState].start();
    }
}

FILE_STATIC void freeTimer()
{
    if (timerHandle != -1)
    {
        endPollingTimer(timerHandle);
        timerHandle = -1;
    }
}

FILE_STATIC void enterStateOff()
{
    gpsSetPower(FALSE);
    gpsSetBuck(FALSE);
}

FILE_STATIC gps_power_state_code stateOff(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Enable)
    {
        return State_EnablingBuck;
    }

    return State_Off;
}

FILE_STATIC void enterStateEnablingBuck()
{
    gpsSetPower(FALSE);
    gpsSetBuck(TRUE);
    gpsSetReset(TRUE);
    timerHandle = timerPollInitializer(DELAY_BUCK_MS);
}

FILE_STATIC gps_power_state_code stateEnablingBuck(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Disable)
    {
        freeTimer();
        return State_Off;
    }

    if (gpsIsBuckGood())
    {
        freeTimer();
        return State_EnablingGPS;
    }

    if (gpsioIsBuckOverride() && checkTimer(timerHandle))
    {
        return State_EnablingGPS;
    }

    return State_EnablingBuck;
}

FILE_STATIC void enterStateEnablingGPS()
{
    gpsSetPower(TRUE);
    gpsSetBuck(TRUE);
    gpsSetReset(TRUE);
    timerHandle = timerPollInitializer(DELAY_GPS_RESET_MS);
}

FILE_STATIC gps_power_state_code stateEnablingGPS(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Disable)
    {
        freeTimer();
        return State_ShuttingDown;
    }

    if (!gpsIsBuckGood() && !gpsioIsBuckOverride())
    {
        freeTimer();
        return State_EnablingBuck;
    }

    if (checkTimer(timerHandle))
    {
        return State_AwaitingGPSOn;
    }

    return State_EnablingGPS;
}

FILE_STATIC void enterStateAwaitingGPSOn()
{
    gpsSetPower(TRUE);
    gpsSetBuck(TRUE);
    gpsSetReset(FALSE);
    timerHandle = -1;
}

FILE_STATIC gps_power_state_code stateAwaitingGPSOn(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Disable)
    {
        freeTimer();
        return State_ShuttingDown;
    }

    if (!gpsIsBuckGood() && !gpsioIsBuckOverride())
    {
        freeTimer();
        return State_EnablingBuck;
    }

    if (gpsioHandlePackage(gpsRead()))
    {
        freeTimer();
        return State_On;
    }

    // periodically poll the GPS until it responds
    if (timerHandle == -1 || checkTimer(timerHandle))
    {
        gpsSendCommand("log rxstatusb\r\n");
        timerHandle = timerPollInitializer(DELAY_GPS_POLL_MS);
    }

    return State_AwaitingGPSOn;
}

FILE_STATIC void enterStateOn()
{
    gpsSetPower(TRUE);
    gpsSetBuck(TRUE);
    gpsSetReset(FALSE);
    gpsioConfig();
    timerHandle = timerPollInitializer(DELAY_MANUAL_LOG_MS);
}

FILE_STATIC gps_power_state_code stateOn(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Disable)
    {
        freeTimer();
        return State_ShuttingDown;
    }

    if (!gpsIsBuckGood() && !gpsioIsBuckOverride())
    {
        freeTimer();
        return State_EnablingBuck;
    }

    // handle incoming telemetry
    gpsioHandlePackage(gpsRead());

    /*
     * Periodically do a manual log.
     * This would be unnecessary if "log hwmonitorb ontime 3" worked.
     */
    if (checkTimer(timerHandle))
    {
        gpsSendCommand("log hwmonitorb\r\n");
        timerHandle = timerPollInitializer(DELAY_MANUAL_LOG_MS);
    }

    return State_On;
}

FILE_STATIC void enterStateShuttingDown()
{
    gpsSetPower(FALSE);
    gpsSetBuck(TRUE);
    gpsSetReset(FALSE);
    timerHandle = timerPollInitializer(DELAY_BUCK_MS);
}

FILE_STATIC gps_power_state_code stateShuttingDown(gps_power_cmd cmd)
{
    if (cmd == PowerCmd_Enable)
    {
        freeTimer();
        return State_EnablingBuck;
    }

    if (checkTimer(timerHandle))
    {
        return State_Off;
    }

    return State_ShuttingDown;
}

void gpsioPowerOn()
{
    triggerGPSOn = TRUE;
    triggerGPSOff = FALSE;
}

void gpsioPowerOff()
{
    triggerGPSOn = FALSE;
    triggerGPSOff = TRUE;
}

void gpsioSetBuckOverride(uint8_t override)
{
    gpspowerSeg.buckOverride = override ? TRUE : FALSE;
}

uint8_t gpsioIsBuckOverride()
{
    return gpspowerSeg.buckOverride;
}

void gpsioSendPowerStatus()
{
    gpspowerSeg.buckEnabled = gpsIsBuckEnabled();
    gpspowerSeg.gpsEnabled = gpsIsPowerEnabled();
    gpspowerSeg.resetStatus = gpsIsResetActive();
    gpspowerSeg.state = gpsPowerState;
    bcbinSendPacket((uint8_t *) &gpspowerSeg, sizeof(gpspowerSeg));
}

void gpsioSendStatus()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &rxstatusSeg, sizeof(rxstatusSeg));
    bcbinSendPacket((uint8_t *) &timeSeg, sizeof(timeSeg));

    // TODO send CAN packets
}

void gpsioSendHealth()
{
    gpsGetHealth(&gpshealthSeg.health);
    bcbinSendPacket((uint8_t *) &gpshealthSeg, sizeof(gpshealthSeg));
}

bool gpsioHandlePackage(GPSPackage *p)
{
#ifdef __SKIP_GPS_TRAFFIC__
    return TRUE;
#endif // __SKIP_GPS_TRAFFIC__

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

bool gpsioHandleCommand(uint8_t *cmdstr)
{
    enable_segment *enableSegment;
    buck_override_segment *overrideSegment;


    switch(cmdstr[0])
    {
        case OPCODE_SENDASCII:
            // send the part of the command that comes after the opcode
            gpsSendCommand(cmdstr + 1);
            gpsSendCommand("\r\n");
            break;
        case OPCODE_ENABLE:
            enableSegment = (enable_segment *) (cmdstr + 1);

            // set enable/disable trigger and overwrite previous command
            if (enableSegment->enable)
            {
                triggerGPSOn = TRUE;
                triggerGPSOff = FALSE;
            }
            else
            {
                triggerGPSOn = FALSE;
                triggerGPSOff = TRUE;
            }
            break;
        case OPCODE_OVERRIDE_BUCK:
            overrideSegment = (buck_override_segment *) (cmdstr + 1);
            gpsioSetBuckOverride(overrideSegment->enable);
            break;
        case OPCODE_TEST_RESET:
            enableSegment = (enable_segment *) (cmdstr + 1);
            gpsSetReset(enableSegment->enable);
            break;
        case OPCODE_TEST_BUCK:
            enableSegment = (enable_segment *) (cmdstr + 1);
            gpsSetBuck(enableSegment->enable);
            break;
        case OPCODE_TEST_GPS_SWITCH:
            enableSegment = (enable_segment *) (cmdstr + 1);
            gpsSetPower(enableSegment->enable);
            break;
        default:
            return 0;
    }
    return 1;
}

FILE_STATIC void handleRxStatus(const GPSPackage *package)
{
    const GPSRXStatus *m = &(package->message.rxstatus);

    debugTraceF(GPS_TRACE_LEVEL, "RXSTATUS error word: %X \r\n", m->error);

    rxstatusSeg.error = m->error;
    rxstatusSeg.aux1 = m->aux1stat.word;
    rxstatusSeg.aux2 = m->aux2stat.word;
    rxstatusSeg.aux3 = m->aux3stat.word;
}

FILE_STATIC void handleTime(const GPSPackage *package)
{
    const GPSTime *m = &(package->message.time);

    // update the time offset
    // used in pos/vel messages and updating MET
    timeSeg.offset = m->offset + m->utcOffset;
    timeSeg.clockStatus = m->clockStatus;
    timeSeg.utcStatus = m->utcStatus;

    debugTraceF(GPS_TRACE_LEVEL, "TIME offset: %f\r\n", timeSeg.offset);
}

FILE_STATIC void sendBestXYZOverBackchannel(const GPSBestXYZ *m, uint16_t week, gps_ec ms)
{
    bestxyz_segment bestxyzSeg;
    bestxyzSeg.posStatus = m->pSolStatus;
    bestxyzSeg.pos = m->pos;
    bestxyzSeg.posStdDev = m->posStdDev;
    bestxyzSeg.velStatus = m->vSolStatus;
    bestxyzSeg.vel = m->vel;
    bestxyzSeg.velStdDev = m->velStdDev;
    bestxyzSeg.week = week;
    bestxyzSeg.ms = ms;
    bcbinPopulateHeader(&bestxyzSeg.header, TLM_ID_BESTXYZ, sizeof(bestxyzSeg));
    bcbinSendPacket((uint8_t *) &bestxyzSeg, sizeof(bestxyz_segment));
}

// TODO this is kind of a lot to do. Check if the RX buffer doesn't overflow here
FILE_STATIC void sendBestXYZOverCAN(const GPSBestXYZ *m, uint16_t week, gps_ec ms)
{
    CANPacket canPacket;

    // send position packets
    sensorproc_gps_pos_x pos_x = { m->pos.x };
    encodesensorproc_gps_pos_x(&pos_x, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_pos_y pos_y = { m->pos.y };
    encodesensorproc_gps_pos_y(&pos_y, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_pos_z pos_z = { m->pos.z };
    encodesensorproc_gps_pos_z(&pos_z, &canPacket);
    canSendPacket(&canPacket);

    // send velocity packets
    sensorproc_gps_vel_x vel_x = { m->vel.x };
    encodesensorproc_gps_vel_x(&vel_x, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_vel_y vel_y = { m->vel.y };
    encodesensorproc_gps_vel_y(&vel_y, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_vel_z vel_z = { m->vel.z };
    encodesensorproc_gps_vel_z(&vel_z, &canPacket);
    canSendPacket(&canPacket);

    // send uncertainty packets
    sensorproc_gps_x_u x_u = { m->velStdDev.x, m->posStdDev.x };
    encodesensorproc_gps_x_u(&x_u, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_y_u y_u = { m->velStdDev.y, m->posStdDev.y };
    encodesensorproc_gps_y_u(&y_u, &canPacket);
    canSendPacket(&canPacket);
    sensorproc_gps_z_u z_u = { m->velStdDev.z, m->posStdDev.z };
    encodesensorproc_gps_z_u(&z_u, &canPacket);
    canSendPacket(&canPacket);

    // send time packet
    sensorproc_gps_time time = { ms, week };
    encodesensorproc_gps_time(&time, &canPacket);
    canSendPacket(&canPacket);
}

FILE_STATIC GPSVectorD gps_pos = {0};

FILE_STATIC void handleBestXYZ(const GPSPackage *package)
{
    const GPSBestXYZ *m = &(package->message.bestXYZ);

    gps_pos.x = m->pos.x;
    gps_pos.y = m->pos.y;
    gps_pos.z = m->pos.z;

    // adjust reported time with UTC offset and velocity latency
    uint16_t week = package->header.week;
    gps_ec ms = package->header.ms;
    toUtc(&week, &ms, timeSeg.offset - m->velLatency);

    // send backchannel telemetry
    sendBestXYZOverBackchannel(m, week, ms);

    // send CAN packets
    sendBestXYZOverCAN(m, week, ms);
}

void getLonLat(GPSLonLat* l) {
    double R = gps_pos.x * gps_pos.x + gps_pos.y * gps_pos.y + gps_pos.z * gps_pos.z;
    l->lat = 180 / M_PI * asin(gps_pos.z / R);
    l->lon = 180 / M_PI * atan2(gps_pos.y, gps_pos.x);
}

void getDM(GPSLonLat *l, GPSDegMin *dm) {
    double gpsLat = l->lat;
    dm->degLat = (int)gpsLat;
    dm->minLat = 60*(gpsLat - dm->degLat);

    double gpsLon = l->lon;
    dm->degLon = (int)gpsLon;
    dm->minLon = 60 * (gpsLon - dm->degLon);
}

FILE_STATIC void handleHwMonitor(const GPSPackage *package)
{
    const GPSHWMonitor *monLog = &(package->message.hwMonitor);
    hwmonitor_segment hwmonitorSeg;

    uint32_t i = monLog->numMeasurements;
    while (i-- != 0)
    {
        GPSMeasurement inMes = monLog->mes[i];
        hwmonitor_segMember *outMes;
        switch (inMes.type)
        {
        case HW_TEMP:
            outMes = &hwmonitorSeg.temp;
            break;
        case HW_ANTCUR:
            outMes = &hwmonitorSeg.antCurrent;
            break;
        case HW_SUPVOLT:
            outMes = &hwmonitorSeg.supVolt;
            break;
        case HW_ANTVOLT:
            outMes = &hwmonitorSeg.antVolt;
            break;
        case HW_DIGCORE:
            outMes = &hwmonitorSeg.digCoreVolt;
            break;
        case HW_TEMP2:
            outMes = &hwmonitorSeg.secTemp;
            break;
        default:
            continue;
        }
        outMes->reading = inMes.reading;
        outMes->status = inMes.status;
    }

    bcbinPopulateHeader(&hwmonitorSeg.header, TLM_ID_HWMONITOR, sizeof(hwmonitorSeg));
    bcbinSendPacket((uint8_t *) &hwmonitorSeg, sizeof(hwmonitor_segment));
}

FILE_STATIC void handleSatvis2(const GPSPackage *package)
{
    const GPSSatvis2 *satvis2 = &(package->message.satvis2);
    debugTraceF(GPS_TRACE_LEVEL, "Satvis2:\r\n");
    debugTraceF(GPS_TRACE_LEVEL, "\tsystem: %u\r\n", satvis2->system);
    debugTraceF(GPS_TRACE_LEVEL, "\tnum:    %u\r\n", satvis2->numSats);

    /*
     * Set part of the outgoing satvis telemetry segment to match the number
     * visible in the system of the incoming telemetry segment.
     *
     * The number is truncated from a uint32_t to a uint8_t because we can
     * assume no GNSS system will have over 255 satellites.
     */
    satvis2_segment satvis2Seg = { 0 };
    switch (satvis2->system)
    {
        case SATSYSTEM_GPS:
            satvis2Seg.numGPS = satvis2->numSats;
            break;
        case SATSYSTEM_GLONASS:
            satvis2Seg.numGLONASS = satvis2->numSats;
            break;
        case SATSYSTEM_SBAS:
            satvis2Seg.numSBAS = satvis2->numSats;
            break;
        case SATSYSTEM_GALILEO:
            satvis2Seg.numGalileo = satvis2->numSats;
            break;
        case SATSYSTEM_BEIDOU:
            satvis2Seg.numBeiDou = satvis2->numSats;
            break;
        case SATSYSTEM_QZSS:
            satvis2Seg.numQZSS = satvis2->numSats;
            break;
        default:
            break;
    }

    bcbinPopulateHeader(&satvis2Seg.header, TLM_ID_SATVIS2, sizeof(satvis2Seg));
    bcbinSendPacket((uint8_t *) &satvis2Seg, sizeof(satvis2Seg));
}

FILE_STATIC void handleRange(const GPSPackage *package)
{
    const GPSRange *range = &(package->message.range);

    range_segment rangeSeg = (const range_segment){ 0 };

    uint32_t i;
    uint32_t numObs = range->numObs < NUM_RANGE_SEGMENT_MEMBERS ? range->numObs : NUM_RANGE_SEGMENT_MEMBERS;
    for (i = 0; i < numObs; i++)
    {
        rangeSeg.obs[i].prn = range->obs[i].prn;
        rangeSeg.obs[i].cToNo = range->obs[i].cToNo;
        rangeSeg.obs[i].locktime = range->obs[i].locktime;
    }

    bcbinPopulateHeader(&rangeSeg.header, TLM_ID_RANGE, sizeof(range_segment));
    bcbinSendPacket((uint8_t *) &rangeSeg, sizeof(range_segment));
}

FILE_STATIC void handleRxStatusEvent(const GPSPackage *package)
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

FILE_STATIC void handlePositionStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log bestxyzb\r\n");
}

FILE_STATIC void handleClockStatusEvent(const GPSRXStatusEvent *e)
{
    gpsSendCommand("log timeb\r\n");
}

FILE_STATIC void canRxCallback(CANPacket *packet)
{
    switch (packet->id)
    {
//    case CAN_ID_SENSORPROC_GPS_ENABLE:
//        // TODO process GPS enable command once it is implemented
//        break;
    default:
        // TODO log an error?
        break;
    }
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
