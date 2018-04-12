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

//------------------------------------------------------------------
// includes
//------------------------------------------------------------------
#include <msp430.h> 
#include "adcs_mtq.h"
#include "bsp/bsp.h"
#include "core/timer.h"
#include "interfaces/canwrap.h" 
#include "core/debugtools.h"
#include "sensors/analogsensor.h"

//------------------------------------------------------------------
// global variables 
//------------------------------------------------------------------
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile TumbleState tumble_status = Idle; // state variable 
FILE_STATIC volatile int command_x, command_y, command_z; // commands for assigning pwm to output 
FILE_STATIC volatile int duty_x1, duty_x2, duty_y1, duty_y2, duty_z1, duty_z2; // for cosmos to monitor duty cycles 
FILE_STATIC volatile int8_t received_CAN_command_packet; // flag to exit tumbling state if no command packet has been received
FILE_STATIC volatile int8_t received_cosmos_command_packet; 
FILE_STATIC duty_segment dutyseg;
FILE_STATIC meta_segment metaSeg;
FILE_STATIC health_segment healthSeg;
FILE_STATIC int telemTimer; // timer handle for sending meta/health updates

//------------------------------------------------------------------
// function declarations
//------------------------------------------------------------------
void mtq_init(void);
void set_pwm(char axis, int pwm_percent); 
void can_packet_rx_callback(CANPacket *packet);
void send_ping_packet(void);
void send_meta_packet(void);
void send_health_packet(void);
void blink_LED(void); 
void degauss_lol(void);

//------------------------------------------------------------------
// Main 
// TODO 
// confirm backchannel functionality 
// add ping packet? 
// add status to backchannel 
//------------------------------------------------------------------
int main(void)
{	
	// BSP initialization 
	bspInit(__SUBSYSTEM_MODULE__);
	// MTQ specific SFR initialization 
	mtq_init(); 
	// COSMOS backchannel initialization 
	initializeTimer();
    bcbinPopulateHeader(&(dutyseg.header), TLM_ID_SHARED_SSGENERAL, sizeof(dutyseg));
    debugRegisterEntity(Entity_NONE, NULL, NULL, handleDebugActionCallback);
    int timerID = timerCallbackInitializer(&sendDutyPacket, 500000); // timer will call on blinkLED every 2000 us.
    startCallback(timerID);

    asensorInit(Ref_2p5V); // initialize temperature sensor
    telemTimer = timerPollInitializer(MTQ_TELEM_DELAY_MS);
    bcbinPopulateHeader(&(healthSeg.header), TLM_ID_SHARED_HEALTH, sizeof(healthSeg));

	// CAN initialization 
	canWrapInitWithFilter();
	setCANPacketRxCallback(can_packet_rx_callback);
	
	// DEBUG testing - comment out during normal operation
	while(1)
	{
		set_pwm('x', 50);
	    set_pwm('y', 50);
	    set_pwm('z', -50);
	}
	// end of DEBUG testing
	
	/*
    while (1)
    {
        // send periodic telemetry
        if (checkTimer(telemTimer))
        {
            // reset timer
            telemTimer = timerPollInitializer(MTQ_TELEM_DELAY_MS);

            // send telemetry
            send_health_packet();
            send_meta_packet();
        }

		// control for turning on coils based on command dipole signals 
        switch (tumble_status) 
		{
			case Tumbling: 
				PJOUT |= BIT0|BIT1|BIT2; // turn on debug lights 
				// set duty cycles 
				if (received_CAN_command_packet || received_cosmos_command_packet)
				{
				    received_CAN_command_packet = 0; // reset received command packet flags
				    received_cosmos_command_packet = 0; 
					set_pwm('x', command_x); 
					set_pwm('y', command_y); 
					set_pwm('z', command_z); 
				} 		
				break;
			case Idle: 
				//blink_LED(); // blink debug lights 
				// Turn off coils (according to driver chopping mode)
				set_pwm('x', 100); 
				set_pwm('y', 100); 
				set_pwm('z', 100); 
				break;
        	default:
				// unknown state 
            	mod_status.state_transition_errors++;
            	mod_status.in_unknown_state++;
            	break;
        }
	}
*/
	return 0;
}

//------------------------------------------------------------------
// CAN interrupt handler 
//------------------------------------------------------------------

void can_packet_rx_callback(CANPacket *packet)
{  
	// check if packet is tumble status 
	if (packet->id == CAN_ID_BDOT_TUMBLE_STATUS)
	{ 
		bdot_tumble_status tumble_status_packet = {0}; // initialize tumble status packet
		// use tumble status to set tumble state 
		decodebdot_tumble_status(packet, &tumble_status_packet);
		if (tumble_status_packet.bdot_tumble_status_status) //tumbling
		    tumble_status = Tumbling;
	}
	 
	// check if packet is command dipole
	if (packet->id == CAN_ID_BDOT_COMMAND_DIPOLE)
	{
		received_CAN_command_packet = 1; // set received_CAN_command_packet
		bdot_command_dipole command_dipole_packet = {0}; // initialize command dipole packet
		decodebdot_command_dipole(packet, &command_dipole_packet);
		// update global dipole command variables   
		command_x = command_dipole_packet.bdot_command_dipole_x; 
		command_y = command_dipole_packet.bdot_command_dipole_y; 
		command_z = command_dipole_packet.bdot_command_dipole_z; 
	}
}	

// TODO do not use. Halts Bdot when sent 
void send_ping_packet(void)
{
	static int ping_packet_tx_count;
	ping_packet_tx_count ++; // increment ping packet count 
	CANPacket p = {0};
	p.id = 1234;
	p.length = 0;
	canSendPacket(&p);
}

void send_health_packet(void)
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

void send_meta_packet(void)
{
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
}

//------------------------------------------------------------------
// PWM functions 
//------------------------------------------------------------------

// this function sets the PWM duty cycles for each of the outputs based 
// on the A3903 driver chip data sheet description for chopping mode 
void set_pwm(char axis, int pwm_percent)  
{
	// set duty cycles based on chopping mode 
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
			duty_x1 = ccr_value_1;  // for backchannel
			duty_x2 = ccr_value_2;
			break;
		case 'y': 
			SET_Y1_PWM ccr_value_1; // P3_7
			SET_Y2_PWM ccr_value_2; // P3_6 
			duty_y1 = ccr_value_1;  // for backchannel 
			duty_y2 = ccr_value_2;
			break;	
		case 'z': 
			SET_Z1_PWM ccr_value_1; // P2_2
			SET_Z2_PWM ccr_value_2; // P2_6
			duty_z1 = ccr_value_1;  // for backchannel  
			duty_z2 = ccr_value_2;
			break;
		default: // unknown state 
			break;
	}
}

// outputs a (very shitty) discreet sine wave of decreasing amplitude with frequency 1/(delay_cycles*2)
// note: HuskySat1 has only aircores so this function is never used; It's just for future reference. 
void degauss_lol(void)
{
	int sine_table[] = {0,50,100,50,0,-50,-100,-50,
						0,30,75,30,0,-30,-75,-30,
						0,20,50,20,0,-20,-50,-20,
						0,10,20,10,-10,-20,-10,0};
	int sine_count = 0; 
	
	while (sine_count < sizeof(sine_table))
	{
		set_pwm('x', sine_table[sine_count]);
		set_pwm('y', sine_table[sine_count]);
		set_pwm('z', sine_table[sine_count]);
		sine_count++; 
		_delay_cycles(100000);
	}	
}

//------------------------------------------------------------------
// COSMOS backchannel functions  
//------------------------------------------------------------------

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

void sendDutyPacket(){
    dutyseg.x1 = duty_x1;
    dutyseg.x2 = duty_x2;
    dutyseg.y1 = duty_y1;
    dutyseg.y2 = duty_y2;
    dutyseg.z1 = duty_z1;
	dutyseg.z2 = duty_z2;
    bcbinSendPacket((uint8_t *) &dutyseg, sizeof(dutyseg));
}


//------------------------------------------------------------------
// other functions  
//------------------------------------------------------------------

void blink_LED(void)
{
	PJOUT |= BIT0|BIT1|BIT2; // debug lights 
	_delay_cycles(50000); 
	PJOUT &= ~(BIT0|BIT1|BIT2); // debug lights 
}


//------------------------------------------------------------------
// SFR initialization functions  
//------------------------------------------------------------------

// used to configure SFRs for mtq
void mtq_init(void)
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








