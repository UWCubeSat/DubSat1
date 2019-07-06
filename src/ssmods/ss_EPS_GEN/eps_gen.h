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
#include "interfaces/canwrap.h"

// Solar panel configuration
#define NUM_PANELS  3
#define CENTER_PANEL_INDEX  2

// Resistance (ohms) of the shunt resistors used by the INA219 on each panel circuit
#define SHUNT_SIDE_PANELS       0.10f
#define SHUNT_CENTER_PANEL      0.10f

// Various control lines
#define DISABLE_PTRACKER1_DIR       P1DIR
#define DISABLE_PTRACKER1_OUT       P1OUT
#define DISABLE_PTRACKER1_BIT       BIT6

#define DISABLE_PTRACKER2_DIR       P3DIR
#define DISABLE_PTRACKER2_OUT       P3OUT
#define DISABLE_PTRACKER2_BIT       BIT6

#define DISABLE_PTRACKER3_DIR       P2DIR
#define DISABLE_PTRACKER3_OUT       P2OUT
#define DISABLE_PTRACKER3_BIT       BIT6

// Input lines indicating state of MPT
#define CHARGING_PTRACKER1_DIR     P1DIR
#define CHARGING_PTRACKER1_IN      P1IN
#define CHARGING_PTRACKER1_BIT     BIT7

#define CHARGING_PTRACKER2_DIR     P3DIR
#define CHARGING_PTRACKER2_IN      P3IN
#define CHARGING_PTRACKER2_BIT     BIT7

#define CHARGING_PTRACKER3_DIR     P2DIR
#define CHARGING_PTRACKER3_IN      P2IN
#define CHARGING_PTRACKER3_BIT     BIT2

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

typedef enum
{
    Panel1,
    Panel2,
    Panel3,
} PanelNum;

typedef enum
{
    PowerTracker1,
    PowerTracker2,
    PowerTracker3,
} PowTrackerNum;

typedef enum
{
    InitialEnable,
    ExplicitEnable,
    ExplicitDisable,
} PowerTrackerCmd;

typedef struct {
    uint8_t panelnum;
    hDev hpcvsensor;
    float shuntresistance;
} panel_info;

// COSMOS telem and cmd packets
#define TLM_ID_EPS_GEN_GENERAL    TLM_ID_SHARED_SSGENERAL  // == 0x02  <--- standard message ID
#define TLM_ID_EPS_GEN_SENSORDAT  0x03



TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    uint8_t ptlastcmds[NUM_PANELS];
    uint8_t ptchargingactual[NUM_PANELS];   // Indicates the state of the power trackers actual charging pin
    uint8_t ptchargingenablesw[NUM_PANELS]; // Indicates the state of the enable pin to the power trackers

} general_segment;

TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float paneltempsC[NUM_PANELS];
    float panelpowers[NUM_PANELS];
    float panelcurrents[NUM_PANELS];
    float panelvoltages[NUM_PANELS];

} sensordat_segment;

#define OPCODE_CHARGECMD          0x64  // Dec '100', ASCII 'd'
#define CMD_NOCHANGE  2
CMD_SEGMENT {
    uint8_t enablecharge[NUM_PANELS];
} chargecmd_segment;

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

FILE_STATIC void rcPopulateH1(CANPacket *out);
FILE_STATIC void rcPopulateH2(CANPacket *out);
FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC void rcPopulate5(CANPacket *out);
FILE_STATIC void rcPopulate6(CANPacket *out);
FILE_STATIC void rcPopulate7(CANPacket *out);
FILE_STATIC void rcPopulate8(CANPacket *out);
FILE_STATIC void rcPopulate9(CANPacket *out);
FILE_STATIC void rcPopulate10(CANPacket *out);

#endif /* EPS_GEN_H_ */
