/*
 * gps_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#include <math.h>

#include "gps_io.h"
#include "adcs_sensorproc_ids.h"
#include "core/debugtools.h"
#include "core/utils.h"
#include "interfaces/canwrap.h"

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC gpshealth_segment gpshealthSeg;
FILE_STATIC gpspower_segment gpspowerSeg;
FILE_STATIC rxstatus_segment rxstatusSeg;
FILE_STATIC time_segment timeSeg;

FILE_STATIC volatile gps_power_state ss_state = State_GPSOff;

FILE_STATIC flag_t triggerGPSOn;
FILE_STATIC flag_t triggerGPSOff;

// callbacks
FILE_STATIC uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
FILE_STATIC void canRxCallback(CANPacket *packet);

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

void gpsioInit()
{
    setCANPacketRxCallback(canRxCallback);

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                          NULL,
                                          handleDebugActionCallback);
#endif  //  __DEBUG__

    bcbinPopulateHeader(&gpshealthSeg.header, TLM_ID_GPSHEALTH, sizeof(gpshealthSeg));
    bcbinPopulateHeader(&gpspowerSeg.header, TLM_ID_GPSPOWER, sizeof(gpspowerSeg));
    bcbinPopulateHeader(&rxstatusSeg.header, TLM_ID_RXSTATUS, sizeof(rxstatusSeg));
    bcbinPopulateHeader(&timeSeg.header, TLM_ID_TIME, sizeof(timeSeg));

    gpsInit();
}

// send commands to configure the GPS
void gpsioConfigure()
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

void gpsioUpdate()
{
    static uint32_t i = 0;
    i++;

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
        if (gpsioHandlePackage(gpsRead()))
        {
            // now that the GPS is confirmed to be on, send commands to configure it
            gpsioConfigure();
            ss_state = State_GPSOn;
        }
        break;
    case State_GPSOn:
        // read from the GPS and trigger message handlers
        gpsioHandlePackage(gpsRead());

        if (i % 65536 == 0)
        {
            gpsioSendStatus();
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
        break;
    }
}

void gpsioSendPowerStatus()
{
    gpspowerSeg.buckEnabled = gpsBuckEnabled();
    gpspowerSeg.gpsEnabled = gpsPowerEnabled();
    gpspowerSeg.state = ss_state;
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

FILE_STATIC void handleBestXYZ(const GPSPackage *package)
{
    const GPSBestXYZ *m = &(package->message.bestXYZ);

    // adjust reported time with UTC offset and velocity latency
    uint16_t week = package->header.week;
    gps_ec ms = package->header.ms;
    toUtc(&week, &ms, timeSeg.offset - m->velLatency);

    // send backchannel telemetry
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

    // send CAN packets
    // this is kind of a lot to do. Check if the RX buffer doesn't overflow here
    sensorproc_gps_z_u z_u = { m->velStdDev.z, m->posStdDev.z };
    sensorproc_gps_y_u y_u = { m->velStdDev.y, m->posStdDev.y };
    sensorproc_gps_x_u x_u = { m->velStdDev.x, m->posStdDev.x };
    sensorproc_gps_vel_z vel_z = { m->vel.z };
    sensorproc_gps_vel_y vel_y = { m->vel.y };
    sensorproc_gps_vel_x vel_x = { m->vel.x };
    sensorproc_gps_pos_z pos_z = { m->pos.z };
    sensorproc_gps_pos_y pos_y = { m->pos.y };
    sensorproc_gps_pos_x pos_x = { m->pos.x };
    sensorproc_gps_time time = { ms, week };
    CANPacket canPacket;
    encodesensorproc_gps_z_u(&z_u, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_y_u(&y_u, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_x_u(&x_u, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_vel_z(&vel_z, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_vel_y(&vel_y, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_vel_x(&vel_x, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_pos_z(&pos_z, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_pos_y(&pos_y, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_pos_x(&pos_x, &canPacket);
    canSendPacket(&canPacket);
    encodesensorproc_gps_time(&time, &canPacket);
    canSendPacket(&canPacket);
}

FILE_STATIC void handleHwMonitor(const GPSPackage *package)
{
    const GPSHWMonitor *monLog = &(package->message.hwMonitor);
    hwmonitor_segment hwmonitorSeg;

    uint32_t i = monLog->numMeasurements;
    while (i-- != 0)
    {
        float reading = monLog->mes[i].reading;
        uint8_t type = monLog->mes[i].type;
        switch (type)
        {
        case HW_TEMP:
            hwmonitorSeg.temp = reading;
            break;
        case HW_ANTCUR:
            hwmonitorSeg.antCurrent = reading;
            break;
        case HW_SUPVOLT:
            hwmonitorSeg.supVolt = reading;
            break;
        case HW_ANTVOLT:
            hwmonitorSeg.antVolt = reading;
            break;
        case HW_DIGCORE:
            hwmonitorSeg.digCoreVolt = reading;
            break;
        case HW_TEMP2:
            hwmonitorSeg.secTemp = reading;
            break;
        default:
            break;
        }
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

    satvis2_segment satvis2Seg;
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

FILE_STATIC uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_BinaryStreaming)
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
