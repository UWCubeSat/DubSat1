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

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t gpsEnabled;
    uint8_t buckEnabled;
    uint8_t buckOverride;
    uint8_t resetStatus;
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

typedef struct PACKED_STRUCT
{
    float reading;
    uint8_t status;
} hwmonitor_segMember;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    hwmonitor_segMember temp;
    hwmonitor_segMember antCurrent;
    hwmonitor_segMember supVolt;
    hwmonitor_segMember antVolt;
    hwmonitor_segMember digCoreVolt;
    hwmonitor_segMember secTemp;
} hwmonitor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t numGPS;
    uint8_t numGLONASS;
    uint8_t numSBAS;
    uint8_t numGalileo;
    uint8_t numBeiDou;
    uint8_t numQZSS;
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

CMD_SEGMENT {
    uint8_t enable;
} buck_override_segment;

void gpsioInit();
void gpsioConfig();
void gpsioUpdate();

void gpsioPowerOn();
void gpsioPowerOff();

/**
 * Enable/disable the buck converter signal override. If enabled, the GPS will
 * be switched on after a delay instead of waiting for a PGOOD buck converter
 * status, and a !PGOOD buck converter status will never cause the GPS to switch
 * off.
 */
void gpsioSetBuckOverride(uint8_t enable);
uint8_t gpsioIsBuckOverride();

void gpsioSendPowerStatus();
void gpsioSendStatus();
void gpsioSendHealth();

bool gpsioHandlePackage(GPSPackage *p);
bool gpsioHandleCommand(uint8_t * cmdstr);

void getLonLat(GPSLonLat* l);
void getDM(GPSLonLat *l, GPSDegMin *dm);

GPSPackage *getGPSData_test();
GPSPackage *gpsioHandlePackage_test(GPSPackage *p);

#endif /* GPS_IO_H_ */
