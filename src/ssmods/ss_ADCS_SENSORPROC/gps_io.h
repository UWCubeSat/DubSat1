/*
 * gps_io.h
 *
 *  Created on: Feb 11, 2018
 *      Author: David Dupre
 */

#ifndef GPS_IO_H_
#define GPS_IO_H_

#define GPS_TRACE_LEVEL 0
#define MS_IN_WEEK 604800000

#include <stdint.h>

#include "sensors/gps/gps.h"

typedef enum {
    State_Off = 0,
    State_EnablingBuck = 1,
    State_EnablingGPS = 2,
    State_AwaitingGPSOn = 3,
    State_On = 4,
    State_ShuttingDown = 5
} gps_power_state_code;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t unknownMsg;
    uint8_t unknownEvt;
    gps_health health;
} gpshealth_segment;

// TODO add reset status entry
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t gpsEnabled;
    uint8_t buckEnabled;
    uint8_t state;
} gpspower_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t error;
    uint32_t status;
    uint32_t aux1;
    uint32_t aux2;
    uint32_t aux3;
} rxstatus_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    gps_enum posStatus;
    GPSVectorD pos;
    GPSVectorF posStdDev;
    gps_enum velStatus;
    GPSVectorD vel;
    GPSVectorF velStdDev;
    uint16_t week;
    int32_t ms;
} bestxyz_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    double offset;
    int32_t ms;
    uint16_t week;
    gps_enum clockStatus;
    gps_enum utcStatus;
} time_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float temp;
    float antCurrent;
    float supVolt;
    float antVolt;
    float digCoreVolt;
    float secTemp;
} hwmonitor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t numGPS;
    uint32_t numGLONASS;
    uint32_t numSBAS;
} satvis2_segment;

typedef struct PACKED_STRUCT
{
    uint16_t prn;
    float cToNo;
    float locktime;
} range_segMember;

#define NUM_RANGE_SEGMENT_MEMBERS 6

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    range_segMember obs[NUM_RANGE_SEGMENT_MEMBERS];
} range_segment;

CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

void gpsioInit();
void gpsioConfig();
void gpsioUpdate();

void gpsioPowerOn();
void gpsioPowerOff();

void gpsioSendPowerStatus();
void gpsioSendStatus();
void gpsioSendHealth();

bool gpsioHandlePackage(GPSPackage *p);
bool gpsioHandleCommand(uint8_t * cmdstr);

#endif /* GPS_IO_H_ */
