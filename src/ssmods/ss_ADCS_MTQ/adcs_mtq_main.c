/* MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ MTQ 

file: adcs_mtq_main.c 
author: Eloise Perrochet
description: main software for magnetorquer subsystem 

*/

// TODO  
// confirm update_rollcall_aggregates() functionality and check for agg overflow

#include <msp430.h>
#include "core/utils.h"
#include <stdint.h>
#include "core/timer.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"
#include "sensors/analogsensor.h"
#include "interfaces/rollcall.h"
#include "core/agglib.h"
#include "adcs_mtq.h"

//==================================================================
// Main 
//==================================================================

int main(void)
{	
    // initialization
    bspInit(__SUBSYSTEM_MODULE__); // BSP initialization
    mtq_sfr_init();                // MTQ specific SFR initialization
    turn_off_coils();              // make sure coils are off (must come after sfr initialization)
    initializeTimer();             // timer A initialization
    cosmos_init();                 // COSMOS backchannel initialization
    can_init();                    // CAN initialization
    rc_agg_init();				   // aggregate initialization 
	
	// reset mtq parameters
    resetMTQ();  				    
	
	// main loop 
    while (1)
    {
		blink_LED(); // TODO comment out for flight 
        state_table[curr_state](); // call a function in the the state machine function pointer table 
		manage_telemetry(); // send periodic cosmos telemetry
		update_rollcall_aggregates(); // add to rollcall aggregates
		rollcallUpdate(); // handle rollcall updates 
    }
		
	return 0;
}

//==================================================================
// Function definitions 
//==================================================================

//---------- state machine ------------------

// transitions to either bdot actuation or fsw actiation phase when 
// the measurement timer has finished. 
FILE_STATIC void measurement()
{
	
    if(checkTimer(measurement_timer)) // finished measurement phase
    {

		if((sc_mode ==0||sc_mode ==1) && fsw_is_valid())
		{
			curr_state = FSW_ACTUATION;
		} else 
		{
			curr_state = BDOT_ACTUATION;
		}  
		send_CAN_ack_packet();
        start_actuation_timer();
    }
}

// executes last received commands from fsw. 
// transitions to stabalize phase when the actuation timer has finished.
FILE_STATIC void fsw_actuation()
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

// executes last received commands from bdot or set values from ground if in pms mode. 
// transitions when actuation timer is finished to stabalize phase or actuation if in pms mode.
FILE_STATIC void bdot_actuation() 
{
    if (command_dipole_valid(bdot_command_x, bdot_command_y, bdot_command_z) 
		&& is_bdot_still_alive() && pms_enable != 1)
    {
        set_pwm('x', bdot_command_x);
        set_pwm('y', bdot_command_y);
        set_pwm('z', bdot_command_z);
    } else if (pms_enable == 1 && command_dipole_valid(pms_x, pms_y, pms_z)) 
	{
        set_pwm('x', pms_x);
        set_pwm('y', pms_y);
        set_pwm('z', pms_z);
    } else // invalid xyz command
    {
		turn_off_coils(); 
    }
    
	if(checkTimer(actuation_timer)) // finished actuation phase
    {
		bdot_interrupt_received = 0; 
		if (pms_enable == 1) {
			curr_state = BDOT_ACTUATION; 
			send_CAN_ack_packet();
	        start_actuation_timer();
		} else {
			curr_state = STABILIZE;
			send_CAN_ack_packet();
	        start_stabilize_timer();
		}
    }
}

// turns off the coils.
// transitions to measurement phase when the stabalize timer has finished. 
FILE_STATIC void stabilize()
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

// initializes mtq parameters at startup 
FILE_STATIC void resetMTQ()
{ 
	// turn off coils 
	turn_off_coils();
	// set input signals to unknown
	sc_mode = ELOISE_UNKNOWN;
	/*bdot_command_x = ELOISE_UNKNOWN; //because of these initializations, the min would always be -128
	bdot_command_y = ELOISE_UNKNOWN; 
	bdot_command_z = ELOISE_UNKNOWN; 
	fsw_command_x = ELOISE_UNKNOWN; 
	fsw_command_y = ELOISE_UNKNOWN; 
	fsw_command_z = ELOISE_UNKNOWN; */
	// reset state 
	curr_state = MEASUREMENT;
	// kick off all the timers 
	start_LED_timer();
	start_cosmos_commands_timer(); 
	start_measurement_timer(); 
}

// functions for starting various timers 
FILE_STATIC void start_actuation_timer(void)
{
    actuation_timer = timerPollInitializer(actuation_time_ms);
}
FILE_STATIC void start_measurement_timer(void)
{
    measurement_timer = timerPollInitializer(measurement_time_ms);
}
FILE_STATIC void start_stabilize_timer(void)
{
    stabilize_timer = timerPollInitializer(stabilize_time_ms);
}
FILE_STATIC void start_telem_timer(void)
{
    telem_timer = timerPollInitializer(telem_time_ms);
}
FILE_STATIC void start_LED_timer(void)
{
    LED_timer = timerPollInitializer(LED_time_ms);
}

FILE_STATIC void start_cosmos_commands_timer(void)
{
	cosmos_commands_timer = timerPollInitializer(cosmos_commands_time_ms);
}

// returns 0 to indicate fsw commands should not be executed 
// returns 1 if fsw should not be ignored 
FILE_STATIC uint8_t fsw_is_valid(void)
{
	if(fsw_ignore == 1)
	{
		return 0; 
	} else 
	{
		return 1; 
	}
}

// returns 0 to indicate invalid if cmmands are outside of the valid -100 to 100 range
// returns 1 for valid commands 
FILE_STATIC uint8_t command_dipole_valid(int command_x, int command_y, int command_z)
{
	if (command_x > 100 || command_y > 100 || command_z > 100 || 
		command_x < -100 || command_y < -100 || command_z < -100)
	{
		return 0; // invalid
	} else 
	{
		return 1; // valid
	}
}

// turns off all mtq coils 
FILE_STATIC void turn_off_coils(void) 
{
	set_pwm('x', 0); 
	set_pwm('y', 0); 
	set_pwm('z', 0); 
}

// toggles LED
FILE_STATIC void blink_LED(void)
{
	if (checkTimer(LED_timer)){
		P3OUT ^= BIT5; // toggle LED 
		start_LED_timer();
	}
}

// sets the PWM duty cycles for each of the outputs based 
// on the A3903 driver chip data sheet description for chopping mode 
FILE_STATIC void set_pwm(char axis, int pwm_percent)  
{
	// POP "polarity override protocol"
	if (axis == 'x' && pop_x || axis == 'y' && pop_y || axis == 'z' && pop_z){
		pwm_percent = -pwm_percent; 
	}
	
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
			aggVec_push_i(&dipole_cmd_x_agg, pwm_percent);
			break;
		case 'y': 
			SET_Y1_PWM ccr_value_1; 
			SET_Y2_PWM ccr_value_2; 
			duty_y1 = duty_1;
			duty_y2 = duty_2;
			last_pwm_percent_executed_y = pwm_percent;
			aggVec_push_i(&dipole_cmd_y_agg, pwm_percent);
			break;	
		case 'z': 
			SET_Z1_PWM ccr_value_1; 
			SET_Z2_PWM ccr_value_2; 
			duty_z1 = duty_1;
			duty_z2 = duty_2;
			last_pwm_percent_executed_z = pwm_percent;
			aggVec_push_i(&dipole_cmd_z_agg, pwm_percent);
			break;
		default: // unknown state 
			break;
	}
}

// outputs a (very shitty) discreet sine wave of decreasing 
// amplitude with frequency 1/(delay_cycles*2)
// note: HuskySat1 has only aircores so this function is 
// never used; It's just for future reference. 
FILE_STATIC void degauss_lol(void)
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

// returns a 1 if the received interrupt flag is high to indidcate that 
// bdot is still properly sending commands during measurement phase. 
// otherwise returns a 0. 
FILE_STATIC int is_bdot_still_alive(void)
{
	if (bdot_interrupt_received){   
		return 1; 
	} else {
		return 0; 
	}
}

//-------- CAN --------

// ISR callback for receiving CAN packets 
FILE_STATIC void can_packet_rx_callback(CANPacket *packet)
{  
	// bdot command packet 
	if (packet->id == CAN_ID_CMD_MTQ_BDOT){
		bdot_interrupt_received = 1; 
		command_source = FROM_BDOT; 
		// update global bdot command variables
		cmd_mtq_bdot bdot_packet = {0};
        decodecmd_mtq_bdot(packet, &bdot_packet);
        bdot_command_x = bdot_packet.cmd_mtq_bdot_x;
        aggVec_push_i(&bdot_x_agg, bdot_command_x);
        bdot_command_y = bdot_packet.cmd_mtq_bdot_y;
        aggVec_push_i(&bdot_y_agg, bdot_command_y);
        bdot_command_z = bdot_packet.cmd_mtq_bdot_z;
        aggVec_push_i(&bdot_z_agg, bdot_command_z);
	}
	// flight software command packet 
	if (packet->id == CAN_ID_CMD_MTQ_FSW){
		command_source = FROM_FSW; 
		// update global fsw command variables
		cmd_mtq_fsw fsw_packet = {0};
        decodecmd_mtq_fsw(packet, &fsw_packet);
        fsw_command_x = fsw_packet.cmd_mtq_fsw_x;
        fsw_command_y = fsw_packet.cmd_mtq_fsw_y;
        fsw_command_z = fsw_packet.cmd_mtq_fsw_z;
        sc_mode = fsw_packet.cmd_mtq_fsw_sc_mode;
	}
	// ignore flight software packet 
	if (packet->id == CAN_ID_CMD_IGNORE_FSW){
		cmd_ignore_fsw ignore = {0};
	    decodecmd_ignore_fsw(packet, &ignore);
		fsw_ignore = ignore.cmd_ignore_fsw_ignore;
	} 
	// rollcall packet 
	if(packet->id == CAN_ID_CMD_ROLLCALL)
    {
		rollcallStart();
    }
	// polarity override protocol packet 
	if(packet->id == CAN_ID_GCMD_MTQ_POP)
	{
		gcmd_mtq_pop pop_packet = {0};
		decodegcmd_mtq_pop(packet, &pop_packet);
		pop_x = pop_packet.gcmd_mtq_pop_x; 
		pop_y = pop_packet.gcmd_mtq_pop_y; 
		pop_z = pop_packet.gcmd_mtq_pop_z; 
	}
	// permanent magnet setting packet 
	if(packet->id == CAN_ID_GCMD_MTQ_PMS)
	{
		gcmd_mtq_pms pms_packet = {0};
		decodegcmd_mtq_pms(packet, &pms_packet);
		pms_enable = pms_packet.gcmd_mtq_pms_enable; 
		pms_x = pms_packet.gcmd_mtq_pms_x; 
		pms_y = pms_packet.gcmd_mtq_pms_y; 
		pms_z = pms_packet.gcmd_mtq_pms_z; 
	}
	if(packet->id == CAN_ID_GCMD_RESET_MINMAX)
	{
        gcmd_reset_minmax pktRst;
        decodegcmd_reset_minmax(packet, &pktRst);
        if(pktRst.gcmd_reset_minmax_mtq)
        {
            aggVec_reset((aggVec *)&mspTemp_agg);
            aggVec_reset((aggVec *)&bdot_x_agg);
            aggVec_reset((aggVec *)&bdot_y_agg);
            aggVec_reset((aggVec *)&bdot_z_agg);
            aggVec_reset((aggVec *)&fsw_x_agg);
            aggVec_reset((aggVec *)&fsw_y_agg);
            aggVec_reset((aggVec *)&fsw_z_agg);
            aggVec_reset((aggVec *)&duty_x1_agg);
            aggVec_reset((aggVec *)&duty_x2_agg);
            aggVec_reset((aggVec *)&duty_y1_agg);
            aggVec_reset((aggVec *)&duty_y2_agg);
            aggVec_reset((aggVec *)&duty_z1_agg);
            aggVec_reset((aggVec *)&duty_z2_agg);
            aggVec_reset((aggVec *)&dipole_cmd_x_agg);
            aggVec_reset((aggVec *)&dipole_cmd_y_agg);
            aggVec_reset((aggVec *)&dipole_cmd_z_agg);
        }
	}
	if(packet->id == CAN_ID_GCMD_DIST_RESET_MISSION)
	{
		//clear persistent flags here
        bspClearResetCount();
	}
}

// can initialization 
FILE_STATIC void can_init(void)
{
	canWrapInitWithFilter();
	setCANPacketRxCallback(can_packet_rx_callback);
}

// roll call aggregate initialization 
FILE_STATIC void rc_agg_init() {
    aggVec_init_f(&mspTemp_agg);
    aggVec_init_i(&bdot_x_agg);
    aggVec_init_i(&bdot_y_agg);
    aggVec_init_i(&bdot_z_agg);
    aggVec_init_i(&fsw_x_agg);
    aggVec_init_i(&fsw_y_agg);
    aggVec_init_i(&fsw_z_agg);
    aggVec_init_i(&duty_x1_agg);
    aggVec_init_i(&duty_x2_agg);
    aggVec_init_i(&duty_y1_agg);
    aggVec_init_i(&duty_y2_agg);
    aggVec_init_i(&duty_z1_agg);
    aggVec_init_i(&duty_z2_agg);
    aggVec_init_i_Var(&dipole_cmd_x_agg);
    aggVec_init_i_Var(&dipole_cmd_y_agg);
    aggVec_init_i_Var(&dipole_cmd_z_agg);
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));
}	

// sends acknowledgement to CAN 
// contains information about the current mtq phase, the 
// source of the last received command, and the last command
// executed by mtq 
FILE_STATIC void send_CAN_ack_packet(void)
{ 
	if (pms_enable == 1){
		which_phase = PMS_PHASE; 
	} else if (curr_state == MEASUREMENT){
		which_phase = MEASUREMENT_PHASE; 
	} else { // either in actuation or stabalize state 
		which_phase = ACTUATION_PHASE;
	}
	mtq_ack ack = {0};
	ack.mtq_ack_phase = which_phase;
	ack.mtq_ack_source = command_source;
	ack.mtq_ack_last_bdot_x = bdot_command_x;
	ack.mtq_ack_last_bdot_y = bdot_command_y;
	ack.mtq_ack_last_bdot_z = bdot_command_z;
	ack.mtq_ack_last_fsw_x = fsw_command_x;
	ack.mtq_ack_last_fsw_y = fsw_command_y;
	ack.mtq_ack_last_fsw_z = fsw_command_z;
	CANPacket mtq_ack_packet; 
	encodemtq_ack(&ack, &mtq_ack_packet);
	canSendPacket(&mtq_ack_packet);
}

// TODO: description 
void rcPopulate1(CANPacket *out){
    rc_adcs_mtq_h1 rc = {0};
    rc.rc_adcs_mtq_h1_sysrstiv = bspGetResetReason();
    rc.rc_adcs_mtq_h1_reset_count = bspGetResetCount();
    rc.rc_adcs_mtq_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTemp_agg));
    rc.rc_adcs_mtq_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTemp_agg));
    rc.rc_adcs_mtq_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTemp_agg));
    encoderc_adcs_mtq_h1(&rc, out);
    aggVec_as_reset((aggVec *)&mspTemp_agg);
}

void rcPopulate0(CANPacket *out){
    rc_adcs_mtq_h2 rc;
    rc.rc_adcs_mtq_h2_canrxerror = canRxErrorCheck();
    encoderc_adcs_mtq_h2(&rc, out);
}

// TODO: description 
void rcPopulate2(CANPacket *out){
    rc_adcs_mtq_2 rc = {0};
    rc.rc_adcs_mtq_2_bdot_x_min = aggVec_min_i(&bdot_x_agg);
    rc.rc_adcs_mtq_2_bdot_x_max = aggVec_max_i(&bdot_x_agg);
    rc.rc_adcs_mtq_2_bdot_x_avg = aggVec_avg_i_i(&bdot_x_agg);
    rc.rc_adcs_mtq_2_bdot_y_min = aggVec_min_i(&bdot_y_agg);
    rc.rc_adcs_mtq_2_bdot_y_max = aggVec_max_i(&bdot_y_agg);
    rc.rc_adcs_mtq_2_bdot_y_avg = aggVec_avg_i_i(&bdot_y_agg);
    rc.rc_adcs_mtq_2_bdot_z_max = aggVec_max_i(&bdot_z_agg);
    rc.rc_adcs_mtq_2_bdot_z_avg = aggVec_avg_i_i(&bdot_z_agg);
    aggVec_as_reset((aggVec *) &bdot_x_agg);
    aggVec_as_reset((aggVec *) &bdot_y_agg);
    aggVec_as_reset((aggVec *) &bdot_z_agg);
    encoderc_adcs_mtq_2(&rc, out);
}

// TODO: description 
void rcPopulate3(CANPacket *out){
    rc_adcs_mtq_3 rc = {0};
    rc.rc_adcs_mtq_3_bdot_z_min = aggVec_min_i(&bdot_z_agg);
    rc.rc_adcs_mtq_3_fsw_x_min = aggVec_min_i(&fsw_x_agg);
    rc.rc_adcs_mtq_3_fsw_x_max = aggVec_max_i(&fsw_x_agg);
    rc.rc_adcs_mtq_3_fsw_x_avg = aggVec_avg_i_i(&fsw_x_agg);
    rc.rc_adcs_mtq_3_fsw_y_min = aggVec_min_i(&fsw_y_agg);
    rc.rc_adcs_mtq_3_fsw_y_max = aggVec_max_i(&fsw_y_agg);
    rc.rc_adcs_mtq_3_fsw_y_avg = aggVec_avg_i_i(&fsw_y_agg);
    rc.rc_adcs_mtq_3_fsw_z_avg = aggVec_avg_i_i(&fsw_z_agg);
    aggVec_as_reset((aggVec *) &fsw_x_agg);
    aggVec_as_reset((aggVec *) &fsw_y_agg);
    aggVec_as_reset((aggVec *) &fsw_z_agg);
    encoderc_adcs_mtq_3(&rc, out);

}

// TODO: description 
void rcPopulate4(CANPacket *out){
    rc_adcs_mtq_4 rc = {0};
    rc.rc_adcs_mtq_4_fsw_z_min = aggVec_min_i(&fsw_z_agg);
    rc.rc_adcs_mtq_4_fsw_y_max = aggVec_max_i(&fsw_z_agg);
    rc.rc_adcs_mtq_4_duty_x1_avg = aggVec_avg_i_i(&duty_x1_agg);
    rc.rc_adcs_mtq_4_duty_x2_avg = aggVec_avg_i_i(&duty_x2_agg);
    rc.rc_adcs_mtq_4_duty_y1_avg = aggVec_avg_i_i(&duty_y1_agg);
    rc.rc_adcs_mtq_4_duty_y2_avg = aggVec_avg_i_i(&duty_y2_agg);
    rc.rc_adcs_mtq_4_duty_z1_avg = aggVec_avg_i_i(&duty_z1_agg);
    rc.rc_adcs_mtq_4_duty_z2_avg = aggVec_avg_i_i(&duty_z2_agg);
    aggVec_as_reset((aggVec *) &duty_x1_agg);
    aggVec_as_reset((aggVec *) &duty_x2_agg);
    aggVec_as_reset((aggVec *) &duty_y1_agg);
    aggVec_as_reset((aggVec *) &duty_y2_agg);
    aggVec_as_reset((aggVec *) &duty_z1_agg);
    aggVec_as_reset((aggVec *) &duty_z2_agg);
    encoderc_adcs_mtq_4(&rc, out);
}

// TODO: description 
void rcPopulate5(CANPacket *out){
    rc_adcs_mtq_5 rc = {0};
    rc.rc_adcs_mtq_5_fsw_ignore = fsw_ignore;
    rc.rc_adcs_mtq_5_reset_counts = bspGetResetCount();
    rc.rc_adcs_mtq_5_cmds_x_var = compressVariance(aggVec_var_i_f(&dipole_cmd_x_agg));
    rc.rc_adcs_mtq_5_cmds_y_var = compressVariance(aggVec_var_i_f(&dipole_cmd_y_agg));
    rc.rc_adcs_mtq_5_cmds_z_var = compressVariance(aggVec_var_i_f(&dipole_cmd_z_agg));
    encoderc_adcs_mtq_5(&rc, out);
    aggVec_as_reset((aggVec *)&dipole_cmd_x_agg);
    aggVec_as_reset((aggVec *)&dipole_cmd_y_agg);
    aggVec_as_reset((aggVec *)&dipole_cmd_z_agg);
}

// TODO: description 
FILE_STATIC void update_rollcall_aggregates()
{
	//aggVec_push_i(&bdot_x_agg, bdot_command_x);
	//aggVec_push_i(&bdot_y_agg, bdot_command_y);
	//aggVec_push_i(&bdot_z_agg, bdot_command_z);
	aggVec_push_i(&fsw_x_agg, fsw_command_x);
	aggVec_push_i(&fsw_y_agg, fsw_command_y);  
	aggVec_push_i(&fsw_z_agg, fsw_command_z); 
	aggVec_push_i(&duty_x1_agg, duty_x1);	  
	aggVec_push_i(&duty_x2_agg, duty_x2);      
	aggVec_push_i(&duty_y1_agg, duty_y1);      
	aggVec_push_i(&duty_y2_agg, duty_y2);      
	aggVec_push_i(&duty_z1_agg, duty_z1);      
	aggVec_push_i(&duty_z2_agg, duty_z2);	 
	// TODO: remove old stuff  
	//
	// if (aggVec_push_i(&bdot_x_agg, bdot_command_x) |
	// 	aggVec_push_i(&bdot_y_agg, bdot_command_y) |
	// 	aggVec_push_i(&bdot_z_agg, bdot_command_z) |
	// 	aggVec_push_i(&fsw_x_agg, fsw_command_x)   |
	// 	aggVec_push_i(&fsw_y_agg, fsw_command_y)   |
	// 	aggVec_push_i(&fsw_z_agg, fsw_command_z)   |
	// 	aggVec_push_i(&duty_x1_agg, duty_x1)	   |
	// 	aggVec_push_i(&duty_x2_agg, duty_x2)       |
	// 	aggVec_push_i(&duty_y1_agg, duty_y1)       |
	// 	aggVec_push_i(&duty_y2_agg, duty_y2)       |
	// 	aggVec_push_i(&duty_z1_agg, duty_z1)       |
	// 	aggVec_push_i(&duty_z2_agg, duty_z2)	   | ) // aggregate overflow
	//  	{
	//
	// }
		
}

//-------- COSMOS --------

// cosmos initialization 
FILE_STATIC void cosmos_init(void)
{
	initializeTimer();
    bcbinPopulateHeader(&(cosmos_commandy_commands.header), TLM_ID_BDOT_FSW_COMMANDS, sizeof(cosmos_commandy_commands));
    bcbinPopulateHeader(&(healthSeg.header), TLM_ID_SHARED_HEALTH, sizeof(healthSeg));
	bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));
	bcbinPopulateHeader(&(cosmos_dooty.header), TLM_ID_DUTY_PERCENT, sizeof(cosmos_dooty));
    asensorInit(Ref_2p5V); // initialize temperature sensor
    telem_timer = timerPollInitializer(telem_time_ms);
}

// periodically sends telemenry over cosmos
FILE_STATIC void manage_telemetry(void)
{ 
	
	if (checkTimer(cosmos_commands_timer)){
		send_COSMOS_commands_packet(); 
		start_cosmos_commands_timer(); 
	}
	
    if (checkTimer(telem_timer))
    {
		send_COSMOS_health_packet();
		send_COSMOS_meta_packet();
        start_telem_timer(); // reset timer 
    }
}

// send mtq temperature and reset count over COSMOS 
FILE_STATIC void send_COSMOS_health_packet()
{
    healthSeg.oms = OMS_Unknown;
    healthSeg.inttemp = asensorReadIntTempC();
    aggVec_push_f(&mspTemp_agg, healthSeg.inttemp);
	healthSeg.reset_count = bspGetResetCount(); 
    bcbinSendPacket((uint8_t *) &healthSeg, sizeof(healthSeg));
    //debugInvokeStatusHandler(Entity_UART); // send uart bus status over backchannel
}

// send last bdot commands and fsw commands received as well as last 
// executed command by MTQ over COSMOS 
FILE_STATIC void send_COSMOS_commands_packet()
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

// sends the current duty percent for the MTQ coils over COSMOS 
FILE_STATIC void send_COSMOS_dooty_packet()
{
    cosmos_dooty.x1 = duty_x1;
    cosmos_dooty.x2 = duty_x2;
    cosmos_dooty.y1 = duty_y1;
    cosmos_dooty.y2 = duty_y2;
    cosmos_dooty.z1 = duty_z1;
    cosmos_dooty.z2 = duty_z2;
	bcbinSendPacket((uint8_t *) &cosmos_dooty, sizeof(cosmos_dooty));
}

// sends a mtq meta packet over COSMOS 
FILE_STATIC void send_COSMOS_meta_packet(void)
{
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
}

//-------- sfr config --------	

/*
coil  pin   timer 
-----------------
X1 - P1_7 - TB0.4
X2 - P1_6 - TB0.3
Y1 - P3_7 - TB0.6
Y2 - P3_6 - TB0.5
Z1 - P2_2 - TB0.2
Z2 - P2_6 - TB0.1
*/		
// Configures special function registers to set up 
// timers, pwm, and gpios for mtq 
FILE_STATIC void mtq_sfr_init(void)
{	
	//---------GPIO initialization--------------------------
	// P3.5 - LED - board leds 
	P3OUT &= ~BIT5; // power on state
	P3DIR |= BIT5;
	P3SEL0 &= ~BIT5;
	P3SEL1 &= ~BIT5;
	// P3.4 - uart pin 
	P3OUT |= BIT4; // power on state
	P3DIR |= BIT4;
	P3SEL0 &= ~BIT4;
	P3SEL1 &= ~BIT4;
	// P1.7 - TB0.4 - x1
	P1DIR |= BIT7;
    P1SEL0 |= BIT7;
    P1SEL1 &= ~BIT7;
	// P1.6 - TB0.3 - x2
	P1DIR |= BIT6;
    P1SEL0 |= BIT6;
    P1SEL1 &= ~BIT6;
	// P3.7 - TB0.6 - y1
	P3DIR |= BIT7;
    P3SEL0 |= BIT7;
    P3SEL1 &= ~BIT7;
	// P3.6 - TB0.5 - y2
	P3DIR |= BIT6;
    P3SEL0 |= BIT6;
    P3SEL1 &= ~BIT6;
	// P2.2 - TB0.2 - z1
	P2DIR |= BIT2;
    P2SEL0 |= BIT2;
    P2SEL1 &= ~BIT2;
	// P2.6 - TB0.1 - z2
	P2DIR |= BIT6;
    P2SEL0 |= BIT6;
    P2SEL1 &= ~BIT6;
	// Disable the GPIO power-on default high-impedance 
	// mode to activate previously configured port settings 
	PM5CTL0 &= ~LOCKLPM5;  
	//---------Timer B initialization--------------------------
	// capture control 0 (to define PWM period)
    TB0CCR0 = PWM_PERIOD; 
  	// capture control value (to define PWM duty cycles)
	TB0CCR4 = 0; // P1.7 - TB0.4 - x1
	TB0CCR3 = 0; // P1.6 - TB0.3 - x2
	TB0CCR6 = 0; // P3.7 - TB0.6 - y1
	TB0CCR5 = 0; // P3.6 - TB0.5 - y2 
	TB0CCR2 = 0; // P2.2 - TB0.2 - z1
	TB0CCR1 = 0; // P2.6 - TB0.1 - z2
	// capture control mode 
	TB0CCTL4 = OUTMOD_7;
	TB0CCTL3 = OUTMOD_7;
	TB0CCTL6 = OUTMOD_7;
	TB0CCTL5 = OUTMOD_7;
	TB0CCTL2 = OUTMOD_7;
	TB0CCTL1 = OUTMOD_7;
	// TB0 control 
	TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR; 
}








