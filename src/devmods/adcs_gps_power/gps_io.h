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
    State_GPSOff = 0,
    State_BuckWaitOn = 1,
    State_GPSWait = 2,
    State_GPSOn = 3,
    State_BuckWaitOff = 4,
} gps_power_state;

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
    uint8_t state;
} gpspower_segment;

CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

void gpsioInit();
void gpsioUpdate();

void gpsioPowerOn();
void gpsioPowerOff();

void gpsioSendPowerStatus();
void gpsioSendStatus();
void gpsioSendHealth();

bool gpsioHandleCommand(uint8_t * cmdstr);

#endif /* GPS_IO_H_ */
