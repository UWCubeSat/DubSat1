/*
file: adcs_mtq.h
author: jeffc
*/

#ifndef ADCS_MTQ_H_
#define ADCS_MTQ_H_

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

// ---- COSMOS------
TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this
    int8_t last_bdot_x;
    int8_t last_bdot_y;
    int8_t last_bdot_z;
    int8_t last_fsw_x;
    int8_t last_fsw_y;
    int8_t last_fsw_z;
    int8_t last_mtq_executed_x;
    int8_t last_mtq_executed_y;
    int8_t last_mtq_executed_z;
} bdot_fsw_commands;

TLM_SEGMENT {
    BcTlmHeader header;  
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
    uint8_t z1;
    uint8_t z2;
} duty_percent;

//TLM_SEGMENT {
//    BcTlmHeader header;
//    uint8_t command_source;
//    uint8_t coil_state;
//} ack_pack;

CMD_SEGMENT {
    int8_t x;
    int8_t y;
    int8_t z;
} command_segment;


void handlePPTFiringNotification();
void handleRollCall();
void sendDutyPacket();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

//---------------------
// MTQ specific 
//---------------------
// ---PWM stuff----
#define PWM_PERIOD 10000-1 //(10000-1) // pwm period = 1000 us
#define CCR_PERIOD 100
// ---SFR APIs----
#define SET_X1_PWM TB0CCR4 =
#define SET_X2_PWM TB0CCR3 =
#define SET_Y1_PWM TB0CCR6 =
#define SET_Y2_PWM TB0CCR5 =
#define SET_Z1_PWM TB0CCR2 =
#define SET_Z2_PWM TB0CCR1 =
// ---other stuffs----
#define FSW_TIMEOUT 1000 // TODO arbitrary change this
#define UNKNOWN -128 // completely arbitrary but outside the -100 - 100 range
#define DONT_OVERRIDE CAN_ENUM_BOOL_FALSE
#define OVERRIDE CAN_ENUM_BOOL_TRUE
#define TUMBLING CAN_ENUM_BOOL_TRUE
#define IDLE CAN_ENUM_BOOL_FALSE
// --- CAN ack packet---
#define FROM_FSW CAN_ENUM_BOOL_FALSE
#define FROM_BDOT CAN_ENUM_BOOL_TRUE
#define COILS_ARE_OFF CAN_ENUM_BOOL_TRUE
#define COILS_ARE_ON CAN_ENUM_BOOL_FALSE
// ---COSMOS-----
#define TLM_ID_BDOT_FSW_COMMANDS 127 
#define TLM_ID_DUTY_PERCENT 126
// ---state machine---
#define MEASUREMENT_PHASE CAN_ENUM_BOOL_FALSE
#define ACTUATION_PHASE CAN_ENUM_BOOL_TRUE


#endif /* ADCS_MTQ_H_ */
