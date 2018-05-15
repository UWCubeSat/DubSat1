/*
file: adcs_mtq.h
*/

#ifndef ADCS_MTQ_H_
#define ADCS_MTQ_H_

//==============================================
// prototypes 
//==============================================

// state functions

FILE_STATIC void restartMTQ();
FILE_STATIC void measurement();
FILE_STATIC void fsw_actuation();
FILE_STATIC void bdot_actuation();
FILE_STATIC void stabilize();

// helper functions

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

// CAN functions

FILE_STATIC void can_init(void);
FILE_STATIC void can_packet_rx_callback(CANPacket *packet);
FILE_STATIC void send_CAN_health_packet(void);
FILE_STATIC void send_CAN_ack_packet(void);
FILE_STATIC void send_CAN_rollCall(); 
// TODO Garrett enter rollcall functions here 

// COSMOS functions 

FILE_STATIC uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
FILE_STATIC void cosmos_init(void);
FILE_STATIC void send_COSMOS_health_packet(void); 
FILE_STATIC void send_COSMOS_meta_packet(void);
FILE_STATIC void send_COSMOS_commands_packet(void);
FILE_STATIC void send_COSMOS_dooty_packet(void);

// SFR initialization 

FILE_STATIC void mtq_sfr_init(void);

//==============================================
// globals
//==============================================

//--------------- status ------------------

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
eMTQState curr_state; // camera state declaration 

//-----------internal control -------------

FILE_STATIC uint8_t bdot_interrupt_received = 0; 
FILE_STATIC volatile uint8_t fsw_ignore = 1;
#pragma PERSISTENT(fsw_ignore) // persist value of fsw_ignore on reboot

//----------- CAN ------------------

// input packet 
FILE_STATIC volatile int8_t bdot_command_x, bdot_command_y, bdot_command_z; 
FILE_STATIC volatile int8_t fsw_command_x, fsw_command_y, fsw_command_z;  
FILE_STATIC volatile int8_t sc_mode;

// health packet 
FILE_STATIC meta_segment metaSeg;
FILE_STATIC health_segment healthSeg;

// acknowledgement packet 
FILE_STATIC volatile int8_t command_source = ELOISE_UNKNOWN; 
FILE_STATIC volatile int8_t which_phase = ELOISE_UNKNOWN; 

// roll call packet 
// TODO Garrett enter rollcall globals here. Delete old ones. 
/*
FILE_STATIC uint16_t mspTempArray[60] = {0};
#pragma PERSISTENT(mspTempArray);
FILE_STATIC uint8_t bdot_xArray[60] = {0};
FILE_STATIC uint8_t bdot_yArray[60] = {0};
FILE_STATIC uint8_t bdot_zArray[60] = {0};
FILE_STATIC uint8_t fsw_xArray[60] = {0};
FILE_STATIC uint8_t fsw_yArray[60] = {0};
FILE_STATIC uint8_t fsw_zArray[60] = {0};
FILE_STATIC uint8_t duty_x1Array[60] = {0};
FILE_STATIC uint8_t duty_x2Array[60] = {0};
FILE_STATIC uint8_t duty_y1Array[60] = {0};
FILE_STATIC uint8_t duty_y2Array[60] = {0};
FILE_STATIC uint8_t duty_z1Array[60] = {0};
FILE_STATIC uint8_t duty_z2Array[60] = {0};
FILE_STATIC uint16_t mspTemp;
FILE_STATIC uint16_t bdot_x;
FILE_STATIC uint16_t bdot_y;
FILE_STATIC uint16_t bdot_z;
FILE_STATIC uint16_t fsw_x;
FILE_STATIC uint16_t fsw_y;
FILE_STATIC uint16_t fsw_z;
FILE_STATIC uint16_t duty_x1Handle;
FILE_STATIC uint16_t duty_x2Handle;
FILE_STATIC uint16_t duty_y1Handle;
FILE_STATIC uint16_t duty_y2Handle;
FILE_STATIC uint16_t duty_z1Handle;
FILE_STATIC uint16_t duty_z2Handle;
FILE_STATIC int rcFlag = 0;
*/

//-----------COSMOS------------------

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

FILE_STATIC int telem_timer; 
FILE_STATIC int telem_time_ms = 1000;
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
FILE_STATIC int LED_timer = 0;
FILE_STATIC int LED_time_ms = 200;
FILE_STATIC int cosmos_commands_timer = 0; 
FILE_STATIC int cosmos_commands_time_ms = 100; 

//==============================================
// defines 
//==============================================

// ---PWM ----

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
