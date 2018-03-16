/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_MTQ_H_
#define ADCS_MTQ_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

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

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

//---------------------
// MTQ specific 
//---------------------
typedef enum _tumble_state {Tumbling=1,Idle=0} TumbleState;
// PWM pins 
#define PWM_PERIOD 10000-1 //(10000-1) // pwm period = 1000 us
#define CCR_PERIOD 100
#define X1 1 // for set_pwm function which_pin 
#define X2 2 
#define Y1 3  
#define Y2 4  
#define Z1 5 
#define Z2 6 


#endif /* ADCS_MTQ_H_ */
