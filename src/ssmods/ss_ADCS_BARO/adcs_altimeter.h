/*
 * adcs_altimeter.h
 *
 *  Created on: Jul 12, 2017
 *      Author: Eli. R
 */

#ifndef ADCS_ALTIMETER_H_
#define ADCS_ALTIMETER_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

#define TLM_ID_ALTIM_BAROMETER 127

TLM_SEGMENT {
    BcTlmHeader header;
    float temperature;
    float pressure;
    float altitude;

} altimeter_segment;


// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    State_FirstState,
    State_SecondState,
    State_ThirdState,
} SubsystemState;

// Additional, it can be helpful if states are grouped into higher level
// "modes" in a hierarchical way for certain kinds of decision making and
// reporting.  These are not mandatory, however.  State transitions will need
// to explicitly transition the mode as well
typedef enum _subsystem_mode {
    Mode_FirstMode,
    Mode_SecondMode,
    Mode_ThirdMode,
} SubsystemMode;

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;

    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;

FILE_STATIC void initial_setup();
void read_altimeter_data();
void send_altimetry_reading_cosmos();

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* ADCS_ALTIMETER_H_ */
