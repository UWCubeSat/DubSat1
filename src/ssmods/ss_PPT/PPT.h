/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef PPT_H_
#define PPT_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    State_Undetermined,
    State_Uncommissioned,
    State_Main_Charging,
    State_Igniter_Charging,
    State_Firing,
    State_Cooldown,
    State_InitializingFire,
} SubsystemState;

// Additional, it can be helpful if states are grouped into higher level
// "modes" in a hierarchical way for certain kinds of decision making and
// reporting.  These are not mandatory, however.  State transitions will need
// to explicitly transition the mode as well
typedef enum _subsystem_mode {
    Mode_Undetermined,
    Mode_Uncommissioned,
    Mode_Safe,
    Mode_WaitingToFire,
    Mode_Firing,
} SubsystemMode;

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    SubsystemMode ss_mode;
    SubsystemState ss_state;

    StartupType startup_type;

    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;

void handleSyncPulse1();
void handleSyncPulse2();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

BOOL readyToFire();

#endif /* PPT_H_ */
