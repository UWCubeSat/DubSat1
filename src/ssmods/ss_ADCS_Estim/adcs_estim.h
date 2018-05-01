/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_ESTIM_H_
#define ADCS_ESTIM_H_

// Debug LED
#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT BIT0

#define TLM_ID_INPUT_TLE 2
#define TLM_ID_INPUT_MET 3
#define TLM_ID_OUTPUT    4

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "interfaces/canwrap.h"

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
} ModuleStatus;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float year;
    float day;
    float bstar;
    float inc;
    float raan;
    float ecc;
    float aop;
    float mna;
    float mnm;
    uint8_t id;
} input_tle_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    double met;
    double epoch;
} input_met_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    double sc2sun_unit[3];
    double mag_eci_unit[3];
    double pos_eci_m[3];
    double vel_eci_mps[3];
    int8_t SGP4_flag;
    uint8_t sc_in_sun;
    uint8_t sc_above_gs;
} output_segment;

void handlePPTFiringNotification();
void handleRollCall();

void canRxCallback(CANPacket *packet);

#endif /* ADCS_ESTIM_H_ */
