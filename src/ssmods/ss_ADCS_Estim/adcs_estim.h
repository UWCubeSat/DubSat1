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
} input_tle_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    double met;
} input_met_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float sc2gs_unit[3];
    float sc2sun_unit[3];
    float mag_unit_vector_eci[3];
    float mag_vector_eci[3];
    float vel_eci_mps[3];
    uint8_t sc_above_gs;
    uint8_t sc_in_fov;
    uint8_t sc_in_sun;
} output_segment;

void handlePPTFiringNotification();
void handleRollCall();

void canRxCallback(CANPacket *packet);

#endif /* ADCS_ESTIM_H_ */
