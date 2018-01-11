/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef EPS_GEN_H_
#define EPS_GEN_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

// Solar panel configuration
#define NUM_PANELS  3
#define CENTER_PANEL_INDEX  2

// Resistance (ohms) of the shunt resistors used by the INA219 on each panel circuit
#define SHUNT_SIDE_PANELS       0.09f
#define SHUNT_CENTER_PANEL      0.11f

// Various control lines
#define ENABLE_PANEL1_DIR       P1DIR
#define ENABLE_PANEL1_OUT       P1OUT
#define ENABLE_PANEL1_BIT       BIT6

#define ENABLE_PANEL2_DIR       P3DIR
#define ENABLE_PANEL2_OUT       P3OUT
#define ENABLE_PANEL2_BIT       BIT6

#define ENABLE_PANEL3_DIR       P2DIR
#define ENABLE_PANEL3_OUT       P2OUT
#define ENABLE_PANEL3_BIT       BIT6

// Input lines indicating state of MPT
#define CHARGING_PANEL1_DIR     P1DIR
#define CHARGING_PANEL1_OUT     P1OUT
#define CHARGING_PANEL1_BIT     BIT7

#define CHARGING_PANEL2_DIR     P3DIR
#define CHARGING_PANEL2_OUT     P3OUT
#define CHARGING_PANEL2_BIT     BIT7

#define CHARGING_PANEL3_DIR     P2DIR
#define CHARGING_PANEL3_OUT     P2OUT
#define CHARGING_PANEL3_BIT     BIT2

// Analog temperature sensing definitions
#define CHAN_TEMP1              CHAN_A0
#define CHAN_TEMP2              CHAN_A1
#define CHAN_TEMP3              CHAN_A2

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

// Various INA219 settings
#define PCVI2C_PANEL1   0x42
#define PCVI2C_PANEL2   0x43
#define PCVI2C_PANEL3   0x44

#define PANEL_CURRENT_LIMIT  1.50f   // in amps

typedef struct {
    uint8_t panelnum;
    hDev hpcvsensor;
    float shuntresistance;
} panel_info;

TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

} general_segment;

TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float paneltempsC[NUM_PANELS];


} sensordat_segment;

CMD_SEGMENT {

} gencmd_segment;

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

uint8_t genActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* EPS_GEN_H_ */
