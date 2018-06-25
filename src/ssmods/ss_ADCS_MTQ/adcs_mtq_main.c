/*
file: adcs_mtq_main.c 
author: Eloise Perrochet
description: software for magnetorquer subsystem 

coil  pin   timer 
-----------------
X1 - P1_7 - TB0.4
X2 - P1_6 - TB0.3
Y1 - P3_7 - TB0.6
Y2 - P3_6 - TB0.5
Z1 - P2_2 - TB0.2
Z2 - P2_6 - TB0.1
*/

//--------includes--------
#include <msp430.h>
#include <stdint.h>
#include "core/utils.h"
#include "core/timer.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"
#include "sensors/analogsensor.h"
#include "adcs_mtq.h"


//--------state functions----------------
void restartMTQ();
void measurement();
void fsw_actuation();
void bdot_actuation();
void stabilize();

//--------helper functions---------------
void start_actuation_timer(void);
void start_measurement_timer(void);
void start_stabilize_timer(void);
void start_telem_timer(void);
void start_bdot_death_timer(void);
void start_LED_timer(void);
void start_cosmos_commands_timer(void);
void manage_telemetry(void);
uint8_t fsw_is_valid(void);
uint8_t command_dipole_valid(int command_x, int command_y, int command_z);
void turn_off_coils(void);
void blink_LED(void);
void set_pwm(char axis, int pwm_percent);
void degauss_lol(void);
int is_bdot_still_alive(void);

//--------CAN functions---------------
void can_init(void);
void can_packet_rx_callback(CANPacket *packet);
void send_CAN_health_packet(void);
void send_CAN_ack_packet(void);
void send_CAN_rollCall();

//---------COSMOS functions--------------
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
void cosmos_init(void);
void send_COSMOS_health_packet(void);
void send_COSMOS_meta_packet(void);
void send_COSMOS_commands_packet(void);
void send_COSMOS_dooty_packet(void);

//--------SFR initialization------
void mtq_sfr_init(void);

//-----------inputs-----------------
FILE_STATIC volatile int8_t bdot_command_x, bdot_command_y, bdot_command_z;
FILE_STATIC volatile int8_t fsw_command_x, fsw_command_y, fsw_command_z;
FILE_STATIC volatile uint8_t fsw_ignore = 1;
#pragma PERSISTENT(fsw_ignore) // persist value of fsw_ignore on reboot
FILE_STATIC volatile int8_t sc_mode;

//-----------internal control -------------
FILE_STATIC uint8_t bdot_interrupt_received = 0;

//-----------backchannel and CAN------------------
// CAN health packet
FILE_STATIC meta_segment metaSeg;
FILE_STATIC health_segment healthSeg;
// cosmos
FILE_STATIC volatile uint8_t last_pwm_percent_executed_x, last_pwm_percent_executed_y, last_pwm_percent_executed_z = 0;
FILE_STATIC duty_percent cosmos_dooty;
FILE_STATIC volatile uint8_t duty_x1, duty_x2, duty_y1, duty_y2, duty_z1, duty_z2 = 0;
// CAN ack packet
FILE_STATIC bdot_fsw_commands cosmos_commandy_commands; // commands
FILE_STATIC volatile int8_t command_source = ELOISE_UNKNOWN; // source
FILE_STATIC volatile int8_t which_phase = ELOISE_UNKNOWN; // phase

FILE_STATIC int telem_timer;
FILE_STATIC int telem_time_ms = 1000;
#pragma PERSISTENT(telem_time_ms)
FILE_STATIC int actuation_timer = 0;
FILE_STATIC int actuation_time_ms = 2000;
#pragma PERSISTENT(actuation_time_ms)
FILE_STATIC int measurement_timer = 0;
FILE_STATIC int measurement_time_ms = 1;
#pragma PERSISTENT(measurement_time_ms)
FILE_STATIC int stabilize_timer = 0;
FILE_STATIC int stabilize_time_ms = 100;
#pragma PERSISTENT(stabilize_time_ms)
FILE_STATIC int LED_timer = 0;
FILE_STATIC int LED_time_ms = 200;
FILE_STATIC int cosmos_commands_timer = 0;
FILE_STATIC int cosmos_commands_time_ms = 100;
FILE_STATIC uint64_t sent = 0;

//------- state machine -----------

// index of states
typedef enum MTQState {
    MEASUREMENT = 0,
    FSW_ACTUATION,
    BDOT_ACTUATION,
    STABILIZE,
} eMTQState;

// This table contains a pointer to the function to call in each state
void (* const state_table[])() = {measurement, fsw_actuation, bdot_actuation, stabilize};

// camera state declaration
eMTQState curr_state;

//------------------------------------------------------------------
// Main
// TODO
// add dipole to CAN ack packet
// add fsw timeout
// fix manage telem function
// cntrl f DEBUG to see commented out sections
// add error messages for invalid commands
// ack packet not sending commands properly
// get rid of enable command update flag
// take out timer stuff from rx handler
//------------------------------------------------------------------

int main(void)
{
    // general initialization
    bspInit(__SUBSYSTEM_MODULE__); // BSP initialization
    mtq_sfr_init();                // MTQ specific SFR initialization
    turn_off_coils();              // make sure coils are off post initialization
    initializeTimer();             // timer A initialization
    cosmos_init();                 // COSMOS backchannel initialization
    can_init();                    // CAN initialization

    restartMTQ(); // restart

    while (1)
    {
        blink_LED();

        // mtq control loop
        state_table[curr_state]();

        // send periodic telemetry
        manage_telemetry();
    }

    return 0;
}

//------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------

//---------- state machine ------------------

void measurement()
{

    if(checkTimer(measurement_timer)) // finished measurement phase
    {
        send_CAN_ack_packet();
        start_measurement_timer();
    }
}

void fsw_actuation()
{
    if (command_dipole_valid(fsw_command_x, fsw_command_y, fsw_command_z))
    {
        set_pwm('x', fsw_command_x);
        set_pwm('y', fsw_command_y);
        set_pwm('z', fsw_command_z);
    } else // invalid xyz command
    {
        turn_off_coils();
    }
    if(checkTimer(actuation_timer)) // finished actuation phase
    {
        curr_state = STABILIZE;
        send_CAN_ack_packet();
        start_stabilize_timer();
    }
}

void bdot_actuation()
{
    if (command_dipole_valid(bdot_command_x, bdot_command_y, bdot_command_z) && is_bdot_still_alive())
    {
        set_pwm('x', bdot_command_x);
        set_pwm('y', bdot_command_y);
        set_pwm('z', bdot_command_z);
    } else // invalid xyz command
    {
        turn_off_coils();
    }
    if(checkTimer(actuation_timer)) // finished actuation phase
    {
        bdot_interrupt_received = 0;
        curr_state = STABILIZE;
        send_CAN_ack_packet();
        start_stabilize_timer();
    }
}

void stabilize()
{
    turn_off_coils();

    if(checkTimer(stabilize_timer)) // finished wait phase
    {
        curr_state = MEASUREMENT;
        send_CAN_ack_packet();
        start_measurement_timer();
    }
}

//------------- helper functions --------------

void restartMTQ()
{
    // turn off coils
    turn_off_coils();
    // set input signals to unknown
    sc_mode = ELOISE_UNKNOWN;
    bdot_command_x = ELOISE_UNKNOWN;
    bdot_command_y = ELOISE_UNKNOWN;
    bdot_command_z = ELOISE_UNKNOWN;
    fsw_command_x = ELOISE_UNKNOWN;
    fsw_command_y = ELOISE_UNKNOWN;
    fsw_command_z = ELOISE_UNKNOWN;
    // reset state
    curr_state = MEASUREMENT;
    // kick off all the timers
    start_LED_timer();
    start_cosmos_commands_timer();
    start_measurement_timer();
}

void start_actuation_timer(void)
{
    actuation_timer = timerPollInitializer(actuation_time_ms);
}
void start_measurement_timer(void)
{
    measurement_timer = timerPollInitializer(measurement_time_ms);
}
void start_stabilize_timer(void)
{
    stabilize_timer = timerPollInitializer(stabilize_time_ms);
}
void start_telem_timer(void)
{
    telem_timer = timerPollInitializer(telem_time_ms);
}
void start_LED_timer(void)
{
    LED_timer = timerPollInitializer(LED_time_ms);
}

void start_cosmos_commands_timer(void)
{
    cosmos_commands_timer = timerPollInitializer(cosmos_commands_time_ms);
}

void manage_telemetry(void)
{
    // send_CAN_rollCall(); commented out because of potential memory leaks

    if (checkTimer(cosmos_commands_timer)){
        send_COSMOS_commands_packet();
        start_cosmos_commands_timer();
    }

    if (checkTimer(telem_timer))
    {
        send_CAN_health_packet();
        send_COSMOS_health_packet();
        // commented out for DEBUG
        //send_COSMOS_dooty_packet();
        send_COSMOS_meta_packet();
        start_telem_timer(); // reset timer
    }
}

uint8_t fsw_is_valid(void)
{
    if(fsw_ignore == 1)
    {
        return 0;
    } else
    {
        return 1;
    }
}

uint8_t command_dipole_valid(int command_x, int command_y, int command_z)
{
    // check to make sure commands are in the range -100 - 100
    if (command_x > 100 || command_y > 100 || command_z > 100 || command_x < -100 || command_y < -100 || command_z < -100)
    {
        return 0; // invalid
    } else
    {
        return 1; // valid
    }
}

void turn_off_coils(void)
{
    set_pwm('x', 0);
    set_pwm('y', 0);
    set_pwm('z', 0);
}

void blink_LED(void)
{
    if (checkTimer(LED_timer)){
        PJDIR |= BIT1;
        PJOUT ^= BIT1; // toggle LED
        start_LED_timer();
    }
}

// sets the PWM duty cycles for each of the outputs based
// on the A3903 driver chip data sheet description for chopping mode
void set_pwm(char axis, int pwm_percent)
{
    // set duty cycles based on driver chopping mode
    int duty_1 = (pwm_percent >= 0) ? (100-pwm_percent) : 100;
    int duty_2 = (pwm_percent < 0) ? (100-(-pwm_percent)): 100;

    // set CCR values to assign pwm to output pins
    int ccr_value_1 = duty_1*CCR_PERIOD;
    int ccr_value_2 = duty_2*CCR_PERIOD;

    // set SFRs and update globals
    switch(axis)
    {
        case 'x':
            SET_X1_PWM ccr_value_1;
            SET_X2_PWM ccr_value_2;
            duty_x1 = duty_1; // for COSMOS
            duty_x2 = duty_2;
            last_pwm_percent_executed_x = pwm_percent; // for CAN ack
            break;
        case 'y':
            SET_Y1_PWM ccr_value_1;
            SET_Y2_PWM ccr_value_2;
            duty_y1 = duty_1;
            duty_y2 = duty_2;
            last_pwm_percent_executed_y = pwm_percent;
            break;
        case 'z':
            SET_Z1_PWM ccr_value_1;
            SET_Z2_PWM ccr_value_2;
            duty_z1 = duty_1;
            duty_z2 = duty_2;
            last_pwm_percent_executed_z = pwm_percent;
            break;
        default: // unknown state
            break;
    }
}

// outputs a (very shitty) discreet sine wave of decreasing amplitude with frequency 1/(delay_cycles*2)
// note: HuskySat1 has only aircores so this function is never used; It's just for future reference.
void degauss_lol(void)
{
    int sine_table_ish[] = {0,50,100,50,0,-50,-100,-50,
                        0,30,75,30,0,-30,-75,-30,
                        0,20,50,20,0,-20,-50,-20,
                        0,10,20,10,-10,-20,-10,0};
    int sine_count = 0;

    while (sine_count < sizeof(sine_table_ish))
    {
        set_pwm('x', sine_table_ish[sine_count]);
        set_pwm('y', sine_table_ish[sine_count]);
        set_pwm('z', sine_table_ish[sine_count]);
        sine_count++;
        _delay_cycles(100000);
    }
}

int is_bdot_still_alive(void)
{
    if (bdot_interrupt_received){
        return 1;
    } else {
        return 0;
    }
}

//-------- CAN --------

// can initialization
void can_init(void)
{
    canWrapInitWithFilter();
    setCANPacketRxCallback(can_packet_rx_callback);
}

// Interrupt service routine callback
void can_packet_rx_callback(CANPacket *packet)
{
    if (packet->id == CAN_ID_CMD_MTQ_BDOT){
        bdot_interrupt_received = 1;
        command_source = FROM_BDOT;
        // update global bdot command variables
        cmd_mtq_bdot bdot_packet = {0};
        decodecmd_mtq_bdot(packet, &bdot_packet);
        bdot_command_x = bdot_packet.cmd_mtq_bdot_x;
        bdot_command_y = bdot_packet.cmd_mtq_bdot_y;
        bdot_command_z = bdot_packet.cmd_mtq_bdot_z;
        // for rollcall
    }
    if (packet->id == CAN_ID_CMD_MTQ_FSW){
        command_source = FROM_FSW;
        // update global fsw command variables
        cmd_mtq_fsw fsw_packet = {0};
        decodecmd_mtq_fsw(packet, &fsw_packet);
        fsw_command_x = fsw_packet.cmd_mtq_fsw_x;
        fsw_command_y = fsw_packet.cmd_mtq_fsw_y;
        fsw_command_z = fsw_packet.cmd_mtq_fsw_z;
        sc_mode = fsw_packet.cmd_mtq_fsw_sc_mode;
        // for rollcall
    }
    if (packet->id == CAN_ID_CMD_IGNORE_FSW){
        cmd_ignore_fsw ignore = {0};
        decodecmd_ignore_fsw(packet, &ignore);
        fsw_ignore = ignore.cmd_ignore_fsw_ignore;
    }
    if(packet->id == CAN_ID_CMD_ROLLCALL)
    {
    }
}

void reverseArrah(uint8_t arr[], uint8_t start, uint8_t end) {
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArrah(arr, start+1, end-1);
}
uint8_t errorcount=0;
void send_CAN_health_packet(void)
{
    // send CAN packet of temperature (in deci-Kelvin)
    CANPacket out;
    CANPacket* output = &out;
    output -> id = 304677442;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    errorcount += canRxErrorCheck();
    fullPacketData = ((uint64_t)sent) ;
    fullPacketData = (((uint64_t)sent) | ((uint64_t)errorcount <<56)) ;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArrah((output->data), 0, 7);
    canSendPacket(output);

}

void send_CAN_ack_packet(void)
{
    if (curr_state == MEASUREMENT){
        which_phase = MEASUREMENT_PHASE;
    } else {
        which_phase = ACTUATION_PHASE;
    }
    mtq_ack ack;
    ack.mtq_ack_source = command_source;
    ack.mtq_ack_last_bdot_x = bdot_command_x;
    ack.mtq_ack_last_bdot_y = bdot_command_y;
    ack.mtq_ack_last_bdot_z = bdot_command_z;
    ack.mtq_ack_last_fsw_x = fsw_command_x;
    ack.mtq_ack_last_fsw_y = fsw_command_y;
    ack.mtq_ack_last_fsw_z = fsw_command_z;
    CANPacket mtq_ack_packet;
    encodemtq_ack(&ack, &mtq_ack_packet);
    while(canTxCheck()==CAN_TX_BUSY){}
    canSendPacket(&mtq_ack_packet);
    sent++;
}

void send_CAN_rollCall()
{
}
//-------- COSMOS backchannel --------

// cosmos initialization
void cosmos_init(void)
{
    initializeTimer();
    bcbinPopulateHeader(&(cosmos_commandy_commands.header), TLM_ID_BDOT_FSW_COMMANDS, sizeof(cosmos_commandy_commands));
    bcbinPopulateHeader(&(healthSeg.header), TLM_ID_SHARED_HEALTH, sizeof(healthSeg));
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));
    bcbinPopulateHeader(&cosmos_dooty.header, TLM_ID_DUTY_PERCENT, sizeof(duty_percent));
    asensorInit(Ref_2p5V); // initialize temperature sensor
    telem_timer = timerPollInitializer(telem_time_ms);
}

void send_COSMOS_health_packet()
{
    healthSeg.oms = OMS_Unknown;
    healthSeg.inttemp = asensorReadIntTempC();
    healthSeg.reset_count = bspGetResetCount();
    bcbinSendPacket((uint8_t *) &healthSeg, sizeof(healthSeg));
    debugInvokeStatusHandler(Entity_UART); // send uart bus status over backchannel
}

void send_COSMOS_commands_packet()
{
    cosmos_commandy_commands.last_bdot_x = bdot_command_x;
    cosmos_commandy_commands.last_bdot_y = bdot_command_y;
    cosmos_commandy_commands.last_bdot_z = bdot_command_z;
    cosmos_commandy_commands.last_fsw_x = fsw_command_x;
    cosmos_commandy_commands.last_fsw_y = fsw_command_y;
    cosmos_commandy_commands.last_fsw_z = fsw_command_z;
    cosmos_commandy_commands.last_mtq_executed_x = last_pwm_percent_executed_x;
    cosmos_commandy_commands.last_mtq_executed_y = last_pwm_percent_executed_y;
    cosmos_commandy_commands.last_mtq_executed_z = last_pwm_percent_executed_z;
    bcbinSendPacket((uint8_t *) &cosmos_commandy_commands, sizeof(cosmos_commandy_commands));
}

void send_COSMOS_dooty_packet()
{
    cosmos_dooty.x1 = duty_x1;
    cosmos_dooty.x2 = duty_x2;
    cosmos_dooty.y1 = duty_y1;
    cosmos_dooty.y2 = duty_y2;
    cosmos_dooty.z1 = duty_z1;
    cosmos_dooty.z2 = duty_z2;
    bcbinSendPacket((uint8_t *) &cosmos_dooty, sizeof(cosmos_dooty));
}

void send_COSMOS_meta_packet(void)
{
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
}

//-------- special function registers config --------

// used to configure SFRs for mtq
void mtq_sfr_init(void)
{
}







