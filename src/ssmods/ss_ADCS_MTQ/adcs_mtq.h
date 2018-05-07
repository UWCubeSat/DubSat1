/*
file: adcs_mtq.h
*/

#ifndef ADCS_MTQ_H_
#define ADCS_MTQ_H_

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

void handlePPTFiringNotification();
void rollCall();

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
// ---COSMOS-----
#define TLM_ID_BDOT_FSW_COMMANDS 127 
#define TLM_ID_DUTY_PERCENT 126


#endif /* ADCS_MTQ_H_ */
