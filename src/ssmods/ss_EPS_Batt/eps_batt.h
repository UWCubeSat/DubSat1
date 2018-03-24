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
#include "sensors/analogsensor.h"

// Configure battery balancer control pins
#define BATTERY_BALANCER_ENABLE_DIR  P4DIR
#define BATTERY_BALANCER_ENABLE_OUT  P4OUT
#define BATTERY_BALANCER_ENABLE_BIT  BIT3

//Heater switch pins
#define HEATER_ENABLE_DIR       P2DIR
#define HEATER_ENABLE_OUT       P2OUT
#define HEATER_ENABLE_BIT       BIT5

// LED pins
#define LED_DIR  P2DIR
#define LED_OUT  P2OUT
#define LED_BIT  BIT2

// COSMOS telem and cmd packets
#define TLM_ID_EPS_BATT_GENERAL    TLM_ID_SHARED_SSGENERAL  // == 0x02  <--- standard message ID
#define TLM_ID_EPS_BATT_SENSORDAT  0x03

TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    uint8_t lastbalancercmd;
    uint8_t lastheatercmd;

} general_segment;

// SensorDat packet:  high-frequency sends that capture state of sensors
TLM_SEGMENT {            //Add sensor data here
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float battVolt;
    float SOC;
    float battCurr;
    float accumulatedCharge;
    float battNodeVolt;
    float battNodeCurr;
    float battTemp; //Not sure yet
    uint8_t heaterState;
    uint8_t balancerState; //enable state
    float battCharge;


} sensordat_segment;

#define OPCODE_BATTMGMT          0x64  // Dec '100', ASCII 'd'
#define NOCHANGE  2
CMD_SEGMENT {                          //no need to add anything unless change to CC (e.g. reset)
    uint8_t enablebattbal;
    uint8_t enablebattheater;
} battmgmt_segment;


typedef enum {
    Cmd_InitialDisable,
    Cmd_AutoEnable,
    Cmd_ExplicitDisable,
    Cmd_ExplicitEnable,
    Cmd_NoChange,
} Cmds;

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

