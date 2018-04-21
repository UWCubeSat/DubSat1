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

//--------globals--------
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC bdot_fsw_commands cosmos_commandy_commands;
FILE_STATIC duty_percent cosmos_dooty;
FILE_STATIC meta_segment metaSeg;
FILE_STATIC health_segment healthSeg;
FILE_STATIC int telemTimer; // timer handle for sending meta/health updates 
FILE_STATIC volatile int8_t sc_mode = UNKNOWN;
FILE_STATIC volatile uint8_t fsw_ignore = OVERRIDE; 
#pragma PERSISTENT(fsw_ignore) // persist value of fsw_ignore on reboot
FILE_STATIC volatile int8_t bdot_command_x, bdot_command_y, bdot_command_z = UNKNOWN;
FILE_STATIC volatile int8_t fsw_command_x, fsw_command_y, fsw_command_z = UNKNOWN;  
FILE_STATIC volatile int fsw_timer = 0; 
FILE_STATIC volatile uint8_t bdot_ack_sent = 1;
FILE_STATIC volatile uint8_t fsw_ack_sent = 0; 
FILE_STATIC volatile uint8_t duty_x1, duty_x2, duty_y1, duty_y2, duty_z1, duty_z2 = 0; // for COSMOS
FILE_STATIC volatile uint8_t enable_command_update = 0;
FILE_STATIC int actuation_timer = 0;
FILE_STATIC int actuation_time_ms = 2000;
#pragma PERSISTENT(actuation_time_ms)
FILE_STATIC int measurement_timer = 0;
FILE_STATIC int measurement_time_ms = 2000;
#pragma PERSISTENT(measurement_time_ms)
FILE_STATIC int wait_timer = 0;
FILE_STATIC int wait_time_ms = 100;
#pragma PERSISTENT(wait_time_ms)
FILE_STATIC volatile uint8_t mtq_state = MEASUREMENT_PHASE;

//--------function declarations--------
void mtq_sfr_init(void);
void cosmos_init(void);
void can_init(void); 
void start_actuation_timer(void);
void start_measurement_timer(void);
void start_wait_timer(void);
uint8_t spacecraft_is_not_tumbling(void);
void manage_telemetry(void);
uint8_t fsw_is_valid(void);
void execute_fsw_command(void);
void execute_bdot_command(void);
uint8_t command_dipole_valid(int command_x, int command_y, int command_z);
uint8_t are_coils_off(int x, int y, int z);
void turn_off_coils(void);
void blink_LED(void);
void set_pwm(char axis, int pwm_percent);
void degauss_lol(void);
void can_packet_rx_callback(CANPacket *packet);
void send_CAN_health_packet(void);
void send_COSMOS_meta_packet(void);
void send_CAN_ack_packet(int command_source, int coil_state);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
void send_COSMOS_commands_packet(void);
void send_COSMOS_telemetry_packet(void);
void send_COSMOS_dooty_packet(void);

 
//------------------------------------------------------------------
// Main 
// TODO 
// add dipole to CAN ack packet
// add acknowledgements for state change
// remove bdot/fsw ack sent flag 
// add timeout if a bdot command that is not 0 remains the same for a few cycles
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


    start_measurement_timer();  // kick off the measurement timer
    while (1)
    {
        // mtq control loop
        switch(mtq_state)
        {
            case MEASUREMENT_PHASE:
                turn_off_coils();
                enable_command_update = 1;
                if(checkTimer(measurement_timer)) // finished measurement phase
                {
                    mtq_state = ACTUATION_PHASE;
                    start_actuation_timer();
                    send_CAN_ack_packet(FROM_BDOT, COILS_ARE_ON); // send acknowledgement
                }
                break;
            case ACTUATION_PHASE:
                enable_command_update = 0;
                if (spacecraft_is_not_tumbling() && fsw_is_valid())
                {
                    execute_fsw_command();
                } else
                {
                    execute_bdot_command();
                }
                if(checkTimer(actuation_timer)) // finished actuation phase
                {
                    mtq_state = WAIT_PHASE;
                    start_wait_timer();
                }
                break;
			case WAIT_PHASE:
				turn_off_coils();
				enable_command_update = 0;
				if(checkTimer(wait_timer))
				{
					mtq_state = MEASUREMENT_PHASE;
					start_measurement_timer();
					send_CAN_ack_packet(FROM_BDOT, COILS_ARE_OFF); // send acknowledgement
				}
            default:
                mod_status.state_transition_errors++;
                mod_status.in_unknown_state++;
                break;
        }
		
		// send periodic telemetry
		manage_telemetry(); 
    }
		
	return 0;
}

//------------------------------------------------------------------
// Function definitions 
//------------------------------------------------------------------

//-------- main software control --------

void start_actuation_timer(void)
{
    actuation_timer = timerPollInitializer(actuation_time_ms);
}
void start_measurement_timer(void)
{
    measurement_timer = timerPollInitializer(measurement_time_ms);
}
void start_wait_timer(void)
{
    wait_timer = timerPollInitializer(wait_time_ms);
}

uint8_t spacecraft_is_not_tumbling(void)
{
    return (sc_mode == IDLE);
}

void manage_telemetry(void)
{
    if (checkTimer(telemTimer))
    {
        send_CAN_health_packet();
        // reset timer
        telemTimer = timerPollInitializer(MTQ_TELEM_DELAY_MS);
    }
}

uint8_t fsw_is_valid(void)
{
	if(fsw_ignore == OVERRIDE || fsw_timer < FSW_TIMEOUT || sc_mode == TUMBLING)
	{
		return 0; 
	} else 
	{
		return 1; 
	}
}

void execute_fsw_command(void)
{
    fsw_timer++; // incrememnt fsw timer
    if (command_dipole_valid(fsw_command_x, fsw_command_y, fsw_command_z))
    {
        set_pwm('x', fsw_command_x);
        set_pwm('y', fsw_command_y);
        set_pwm('z', fsw_command_z);
        if (!fsw_ack_sent) // only send acknowledgement once per interrupt
        {
            send_CAN_ack_packet(FROM_FSW, are_coils_off(fsw_command_x, fsw_command_y, fsw_command_z));
            fsw_ack_sent = 1;
        }
    } else // invalid xyz command
    {
    }

}

void execute_bdot_command(void)
{
    if (command_dipole_valid(bdot_command_x, bdot_command_y, bdot_command_z))
    {
        set_pwm('x', bdot_command_x);
        set_pwm('y', bdot_command_y);
        set_pwm('z', bdot_command_z);
        if (!bdot_ack_sent) // only send acknowledgement once per interrupt
        {
            send_CAN_ack_packet(FROM_BDOT, are_coils_off(bdot_command_x, bdot_command_y, bdot_command_z));
            bdot_ack_sent = 1;
        }
    } else // invalid xyz command
    {
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

uint8_t are_coils_off(int x, int y, int z)
{
	if(x==100 && y==100 && z==100)
	{
		return 1; 
	} else 
	{
		return 0; 
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
	PJOUT |= BIT0|BIT1|BIT2; // debug lights 
	_delay_cycles(50000); 
	PJOUT &= ~(BIT0|BIT1|BIT2); // debug lights 
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
	
	// set SFRs 
	switch(axis)
	{
		case 'x':
			SET_X1_PWM ccr_value_1; // P1_7
			SET_X2_PWM ccr_value_2; // P1_6
			duty_x1 = ccr_value_1 / PWM_PERIOD; // for COSMOS
			duty_x2 = ccr_value_2 / PWM_PERIOD; 
			break;
		case 'y': 
			SET_Y1_PWM ccr_value_1; // P3_7
			SET_Y2_PWM ccr_value_2; // P3_6
			duty_y1 = ccr_value_1 / PWM_PERIOD; 
			duty_y2 = ccr_value_2 / PWM_PERIOD; 
			break;	
		case 'z': 
			SET_Z1_PWM ccr_value_1; // P2_2
			SET_Z2_PWM ccr_value_2; // P2_6
			duty_z1 = ccr_value_1 / PWM_PERIOD; 
			duty_z2 = ccr_value_2 / PWM_PERIOD; 
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

//-------- CAN --------

// can initialization 
void can_init(void)
{
	canWrapInit();
	setCANPacketRxCallback(can_packet_rx_callback);
}

// Interrupt service routine callback 
void can_packet_rx_callback(CANPacket *packet)
{  
    cmd_mtq_bdot bdot_packet = {0};
    cmd_mtq_fsw fsw_packet = {0};
    cmd_ignore_fsw ignore = {0};

	switch(packet->id)
	{
		case CAN_ID_CMD_MTQ_BDOT:
		    if (enable_command_update) // only update commands on low level of master timer
		    {
                decodecmd_mtq_bdot(packet, &bdot_packet);
                // update global bdot command variables
                bdot_command_x = bdot_packet.cmd_mtq_bdot_x;
                bdot_command_y = bdot_packet.cmd_mtq_bdot_y;
                bdot_command_z = bdot_packet.cmd_mtq_bdot_z;
                bdot_ack_sent = 0; // reset acknowledgement sent flag
		    }
			break;
		case CAN_ID_CMD_MTQ_FSW:
		    if (enable_command_update) // only update commands on low level of master timer
		    {
                decodecmd_mtq_fsw(packet, &fsw_packet);
                // update global fsw command variables
                fsw_command_x = fsw_packet.cmd_mtq_fsw_x;
                fsw_command_y = fsw_packet.cmd_mtq_fsw_y;
                fsw_command_z = fsw_packet.cmd_mtq_fsw_z;
                sc_mode = fsw_packet.cmd_mtq_fsw_sc_mode;
                fsw_ack_sent = 0; // reset acknowledgement sent flag
                fsw_timer = 0; // reset fsw timer
		    }
			break;
		case CAN_ID_CMD_IGNORE_FSW:
		    decodecmd_ignore_fsw(packet, &ignore);
			fsw_ignore = ignore.cmd_ignore_fsw_ignore;
			break; 
		default: 
			break;
	}
}	

void send_CAN_health_packet(void)
{
    // TODO determine overall health based on querying sensors for their health
    healthSeg.oms = OMS_Unknown;

    healthSeg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &healthSeg, sizeof(healthSeg));
    debugInvokeStatusHandler(Entity_UART); // send uart bus status over backchannel

    // send CAN packet of temperature (in deci-Kelvin)
    msp_temp temp = { (healthSeg.inttemp + 273.15f) * 10 };
    CANPacket packet;
    encodemsp_temp(&temp, &packet);
    canSendPacket(&packet);
}

void send_CAN_ack_packet(int command_source, int coil_state)
{ 
	mtq_ack ack = {0};
	ack.mtq_ack_coils_state = coil_state;
	ack.mtq_ack_node = command_source;
	
	CANPacket mtq_ack_packet; 
	encodemtq_ack(&ack, &mtq_ack_packet);
	canSendPacket(&mtq_ack_packet);
}

//-------- COSMOS backchannel --------

// cosmos initialization 
void cosmos_init(void)
{
	initializeTimer();
    bcbinPopulateHeader(&(cosmos_commandy_commands.header), TLM_ID_SHARED_SSGENERAL, sizeof(cosmos_commandy_commands));
    debugRegisterEntity(Entity_NONE, NULL, NULL, handleDebugActionCallback);
    asensorInit(Ref_2p5V); // initialize temperature sensor
    telemTimer = timerPollInitializer(MTQ_TELEM_DELAY_MS);
    bcbinPopulateHeader(&(healthSeg.header), TLM_ID_SHARED_HEALTH, sizeof(healthSeg));
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_BinaryStreaming)
    {
        command_segment *myCmdSegment;

        uint8_t opcode = cmdstr[0];
        switch(opcode)
        {
            case 1:
                // cast the payload to our command segment
                myCmdSegment = (command_segment *) (cmdstr + 1);
                set_pwm('x' , myCmdSegment->x);
				set_pwm('y' , myCmdSegment->x);
				set_pwm('z' , myCmdSegment->x);
                // TODO do something based on the command segment
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
}

void send_COSMOS_commands_packet()
{
    cosmos_commandy_commands.bdot_x = bdot_command_x;
    cosmos_commandy_commands.bdot_y = bdot_command_y;
    cosmos_commandy_commands.bdot_z = bdot_command_z;
    cosmos_commandy_commands.fsw_x = fsw_command_x;
    cosmos_commandy_commands.fsw_y = fsw_command_y;
	cosmos_commandy_commands.fsw_z = fsw_command_z;
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
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
}

void send_COSMOS_telemetry_packet()
{
    send_COSMOS_meta_packet();
	send_COSMOS_commands_packet(); 
	send_COSMOS_dooty_packet(); 
}

//-------- special function registers config --------	
		
// used to configure SFRs for mtq
void mtq_sfr_init(void)
{	
	//---------GPIO initialization--------------------------
	// PJ.0,1,2 - gpio - board leds 
	PJOUT &= ~(BIT0|BIT1|BIT2); // power on state
	PJDIR |= BIT0|BIT1|BIT2;
	PJSEL0 &= ~(BIT0|BIT1|BIT2);
	PJSEL1 &= ~(BIT0|BIT1|BIT2);
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








