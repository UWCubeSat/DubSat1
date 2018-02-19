/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef EPS_BATT_H_
#define EPS_BATT_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

// Configure battery balancer control pins
#define BATT_BALANCER_ENABLE_DIR  P4DIR
#define BATT_BALANCER_ENABLE_OUT  P4OUT
#define BATT_BALANCER_ENABLE_BIT  BIT3

#define LED_DIR  P2DIR
#define LED_OUT  P2OUT
#define LED_BIT  BIT2

typedef enum {
    BBCmd_InitialDisable,
    BBCmd_AutoEnable,
    BBCmd_ExplicitDisable,
    BBCmd_ExplicitEnable,
    BBCmd_NoChange,
} BattBalancerCmd;

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


uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* EPS_BATT_H_ */
