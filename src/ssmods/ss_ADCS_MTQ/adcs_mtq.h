/* MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ 

file: adcs_mtq.h
author: Eloise Perrochet
description: header file for magnetorquer subsystem 
			 contains function prototypes, 
			 global variables, and macros

*/

#ifndef ADCS_MTQ_H_
#define ADCS_MTQ_H_

//==================================================================
// function prototypes 
//==================================================================

// state machine

FILE_STATIC void resetMTQ();
FILE_STATIC void measurement();
FILE_STATIC void fsw_actuation();
FILE_STATIC void bdot_actuation();
FILE_STATIC void stabilize();

// helper 

FILE_STATIC void start_actuation_timer(void);
FILE_STATIC void start_measurement_timer(void);
FILE_STATIC void start_stabilize_timer(void);
FILE_STATIC void start_telem_timer(void);
FILE_STATIC void start_bdot_death_timer(void); 
FILE_STATIC void start_LED_timer(void); 
FILE_STATIC void start_cosmos_commands_timer(void); 
FILE_STATIC void manage_telemetry(void);
FILE_STATIC uint8_t fsw_is_valid(void);
FILE_STATIC uint8_t command_dipole_valid(int command_x, int command_y, int command_z);
FILE_STATIC void turn_off_coils(void);
FILE_STATIC void blink_LED(void);
FILE_STATIC void set_pwm(char axis, int pwm_percent);
FILE_STATIC void degauss_lol(void);
FILE_STATIC int is_bdot_still_alive(void); 

// CAN 

FILE_STATIC void can_init(void);
FILE_STATIC void rc_agg_init();
FILE_STATIC void can_packet_rx_callback(CANPacket *packet);
FILE_STATIC void send_CAN_health_packet(void);
FILE_STATIC void send_CAN_ack_packet(void);
FILE_STATIC void send_CAN_rollCall(); 
FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulate0(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC void rcPopulate5(CANPacket *out);
FILE_STATIC void update_rollcall_aggregates();

// COSMOS

FILE_STATIC uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
FILE_STATIC void cosmos_init(void);
FILE_STATIC void send_COSMOS_health_packet(void); 
FILE_STATIC void send_COSMOS_meta_packet(void);
FILE_STATIC void send_COSMOS_commands_packet(void);
FILE_STATIC void send_COSMOS_dooty_packet(void);

// SFR initialization 

FILE_STATIC void mtq_sfr_init(void);

//==================================================================
// globals
//==================================================================

//--------------- general status ------------------

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;

//------------- state machine -------------

// index of states for state machine 
typedef enum MTQState {
	MEASUREMENT = 0,
	FSW_ACTUATION,
	BDOT_ACTUATION,
	STABILIZE,
} eMTQState;
// This table contains a pointer to the function to call in each state 
void (* const state_table[])() = {measurement, fsw_actuation, bdot_actuation, stabilize}; 
eMTQState curr_state; // current state declaration 

//----------- CAN mtq/bdot/fsw handshake ------------------

// bdot commands packet 
FILE_STATIC volatile int8_t bdot_command_x, bdot_command_y, bdot_command_z;
 
// fsw commands 
FILE_STATIC volatile int8_t fsw_command_x, fsw_command_y, fsw_command_z;  
FILE_STATIC volatile int8_t sc_mode;

// acknowledgement packet 
FILE_STATIC volatile int8_t command_source = ELOISE_UNKNOWN; 
FILE_STATIC volatile int8_t which_phase = ELOISE_UNKNOWN; 

// interrupt received flag 
FILE_STATIC uint8_t bdot_interrupt_received = 0; 

//----------- CAN mtq/ground handshake ------------------

// health packet 
FILE_STATIC meta_segment metaSeg;
FILE_STATIC health_segment healthSeg;

// roll call packet 
FILE_STATIC aggVec_f mspTemp_agg;
FILE_STATIC aggVec_i bdot_x_agg;
FILE_STATIC aggVec_i bdot_y_agg;
FILE_STATIC aggVec_i bdot_z_agg;
FILE_STATIC aggVec_i fsw_x_agg;
FILE_STATIC aggVec_i fsw_y_agg;
FILE_STATIC aggVec_i fsw_z_agg;
FILE_STATIC aggVec_i duty_x1_agg;
FILE_STATIC aggVec_i duty_x2_agg;
FILE_STATIC aggVec_i duty_y1_agg;
FILE_STATIC aggVec_i duty_y2_agg;
FILE_STATIC aggVec_i duty_z1_agg;
FILE_STATIC aggVec_i duty_z2_agg;
FILE_STATIC const rollcall_fn rollcallFunctions[] =
	{rcPopulate1, rcPopulate0, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5};

// ignore flight software 
FILE_STATIC volatile uint8_t fsw_ignore = 1;
#pragma PERSISTENT(fsw_ignore) // persist value of fsw_ignore on reboot

// POP "Polarity Override Protocol" 
FILE_STATIC volatile int8_t pop_x = 0, pop_y = 0, pop_z = 0; 
#pragma PERSISTENT(pop_x)
#pragma PERSISTENT(pop_y)
#pragma PERSISTENT(pop_z)

// PMS "Permanent Magnet Setting"
FILE_STATIC volatile int8_t pms_x = 0, pms_y = 0, pms_z = 0; 
FILE_STATIC volatile int8_t pms_enable = 0;
#pragma PERSISTENT(pms_x)
#pragma PERSISTENT(pms_y)
#pragma PERSISTENT(pms_z)
#pragma PERSISTENT(pms_enable)

//----------- COSMOS ------------------

// dooty packet 
FILE_STATIC volatile uint8_t duty_x1, duty_x2, duty_y1, duty_y2, duty_z1, duty_z2 = 0;
TLM_SEGMENT {
    BcTlmHeader header;  
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
    uint8_t z1;
    uint8_t z2;
} duty_percent;
FILE_STATIC duty_percent cosmos_dooty; 

// command packet
FILE_STATIC volatile uint8_t last_pwm_percent_executed_x, last_pwm_percent_executed_y, last_pwm_percent_executed_z = 0; 
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
FILE_STATIC bdot_fsw_commands cosmos_commandy_commands; // commands 

//------------ timers ----------------

// state machine timers 
#pragma PERSISTENT(telem_time_ms)
FILE_STATIC int actuation_timer = 0;
FILE_STATIC int actuation_time_ms = 2000;
#pragma PERSISTENT(actuation_time_ms)
FILE_STATIC int measurement_timer = 0;
FILE_STATIC int measurement_time_ms = 2000;
#pragma PERSISTENT(measurement_time_ms)
FILE_STATIC int stabilize_timer = 0;
FILE_STATIC int stabilize_time_ms = 100;
#pragma PERSISTENT(stabilize_time_ms)
// other timers 
FILE_STATIC int telem_timer; 
FILE_STATIC int telem_time_ms = 1000;
FILE_STATIC int LED_timer = 0;
FILE_STATIC int LED_time_ms = 200;
FILE_STATIC int cosmos_commands_timer = 0; 
FILE_STATIC int cosmos_commands_time_ms = 100; 



//==================================================================
// defines 
//==================================================================

// --- PWM ----

#define PWM_PERIOD 10000-1 //(10000-1) // pwm period = 1000 us
#define CCR_PERIOD 100

// --- SFR API ----

#define SET_X1_PWM TB0CCR4 =
#define SET_X2_PWM TB0CCR3 =
#define SET_Y1_PWM TB0CCR6 =
#define SET_Y2_PWM TB0CCR5 =
#define SET_Z1_PWM TB0CCR2 =
#define SET_Z2_PWM TB0CCR1 =

// --- COSMOS -----

#define TLM_ID_BDOT_FSW_COMMANDS 127 
#define TLM_ID_DUTY_PERCENT 126


#endif /* ADCS_MTQ_H_ */
