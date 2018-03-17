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
#include "interfaces/canwrap.h" 

//------------------------------------------------------------------
// global variables 
//------------------------------------------------------------------
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile TumbleState tumble_status = Idle; // static volatile enum _tumble_state {Tumbling=1,Idle=0} TumbleState tumble_status = Idle;
FILE_STATIC volatile int duty_X1, duty_X2, duty_Y1, duty_Y2, duty_Z1, duty_Z2; // for assigning pwm to output 
FILE_STATIC volatile int received_command_packet; // flag to protect against weird behavior if in tumbling state but no command packet has been received

//------------------------------------------------------------------
// function declarations
//------------------------------------------------------------------
void mtq_gpio_init(void);
void mtq_gpio_debug_init(void); // DEBUG pins with gpio function 
void timer_b_init(void); 
void interpret_command_dipole(int x_command, int y_command, int z_command);
void set_pwm(int which_pin, int ccr_value); 
void can_packet_rx_callback(CANPacket *packet);
void send_ping_packet(void);
//void backchannel_send_health(void); 

//------------------------------------------------------------------
// Main 

// TODO 
// add de-gaussing 
// add backchannel UART 
// fix CAN ping packet function 
//------------------------------------------------------------------
int main(void)
{	
	// bsp initialization 
	bspInit(__SUBSYSTEM_MODULE__);
	// port initialization 
	mtq_gpio_init(); // pins are timer b function
	// timer initialization 
	timer_b_init();
	// CAN initialization 
	canWrapInit();
	setCANPacketRxCallback(can_packet_rx_callback);
	// temperature sensor initialization 
	//asensorInit(Ref_2p5V);
	
    while (1)
    {
        /*
		// hardcoded PWM for DEBUG
        cycle = ~cycle;
        interpret_command_dipole(0, 0, cyle*100); // x y z
        set_pwm(X1, duty_X1);
        set_pwm(X2, duty_X2);
        set_pwm(Y1, duty_Y1);
        set_pwm(Y2, duty_Y2);
        set_pwm(Z1, duty_Z1);
        set_pwm(Z2, duty_Z2);
        __delay_cycles(500000);
        */

		// control for turning on coils based on command dipole signals 
        switch (tumble_status) 
		{
			case Tumbling: 
				PJOUT |= BIT0|BIT1|BIT2; // debug lights 
				// set duty cycles 
				if (received_command_packet)
				{
				    received_command_packet = 0; // reset received_command_packet flag
					set_pwm(X1, duty_X1); 
					set_pwm(X2, duty_X2); 
					set_pwm(Y1, duty_Y1); 
					set_pwm(Y2, duty_Y2);
					set_pwm(Z1, duty_Z1); 
					set_pwm(Z2, duty_Z2);
				} 		
				break;
			case Idle: 
				PJOUT &= ~(BIT0|BIT1|BIT2); // debug lights 
				// set all duty cycles to 0 
				set_pwm(X1, 0);
				set_pwm(X2, 0);
				set_pwm(Y1, 0);
				set_pwm(Y2, 0);
				set_pwm(Z1, 0);
				set_pwm(Z2, 0);
				break;
        	default:
				// unknown state 
            	mod_status.state_transition_errors++;
            	mod_status.in_unknown_state++;
            	break;
        }
	}

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
		received_command_packet = 1; // set received_command_packet
		bdot_command_dipole command_dipole_packet = {0}; // initialize command dipole packet
		decodebdot_command_dipole(packet, &command_dipole_packet);
		// use dipole commands to update values for commands 
		interpret_command_dipole(command_dipole_packet.bdot_command_dipole_x,
								 command_dipole_packet.bdot_command_dipole_y, 
								 command_dipole_packet.bdot_command_dipole_z);
	}
}	

//------------------------------------------------------------------
// software functions 
//------------------------------------------------------------------

// this function sets the PWM duty cycles for each of the outputs based 
// on the A3903 driver chip data sheet description for chopping mode 
void interpret_command_dipole(int x_command, int y_command, int z_command)  
{
	// valid command range : (-100 - 100)
	if (x_command > 100 | x_command < -100 | y_command > 100 | y_command < -100 | z_command > 100 | z_command < -100)
		; // TODO add error condition  
	duty_X1 = (x_command >= 0) ? (100-x_command) : 100;
	duty_X2 = (x_command >= 0) ? 100: 100-(-x_command);
	duty_Y1 = (y_command >= 0) ? (100-y_command) : 100;
	duty_Y2 = (y_command >= 0) ? 100: 100-(-y_command);
	duty_Z1 = (z_command >= 0) ? (100-z_command) : 100;
	duty_Z2 = (z_command >= 0) ? 100: 100-(-z_command);
}

// this function sets the CCRn register values to adjust PWM for output
void set_pwm(int which_pin, int pwm_percent)  
{
    int ccr_value;

    if (pwm_percent == 0)
        ccr_value = 0;
    else
        ccr_value = pwm_percent*CCR_PERIOD;

	switch(which_pin)
	{
		case X1: // MTQ: P1_7 Launchpad: P3_5
			TB0CCR4 = ccr_value;
			break; 
		case X2: // MTQ: P1_6 Launchpad: P3_4
			TB0CCR3 = ccr_value;
			break;
		case Y1: // MTQ: P3_7 Launchpad: P3_7
			TB0CCR6 = ccr_value;
			break;
		case Y2: // MTQ: P3_6 Launchpad: P3_6
			TB0CCR5 = ccr_value;
			break;	
		case Z1: // MTQ: P2_2 Launchpad: P1_5
			TB0CCR2 = ccr_value;
			break;	
		case Z2: // MTQ: P2_6 Launchpad: P2_6
			TB0CCR1 = ccr_value;
			break;
		default: // unknown state 
			break;
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

/*
void backchannel_send_health(void); 
{
	int temp;
	temp = asensorReadIntTempC();
}
*/

//------------------------------------------------------------------
// register initialization functions  
//------------------------------------------------------------------

void mtq_gpio_init(void)
{	
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
	//---------------------------------
	// Power mode control (page 73 datasheet)
	//---------------------------------
	// Disable the GPIO power-on default high-impedance mode to activate previously configured port settings 
	PM5CTL0 &= ~LOCKLPM5;  
	// note: the n in px.n refers to the CCRn register that corresponds to the capture compare timer (page 24)
}

void mtq_gpio_debug_init(void)
{	
	// PJ.0,1,2 - gpio - board leds 
	PJOUT &= ~(BIT0|BIT1|BIT2); // power on state
	PJDIR |= BIT0|BIT1|BIT2;
	PJSEL0 &= ~(BIT0|BIT1|BIT2);
	PJSEL1 &= ~(BIT0|BIT1|BIT2);
	// P1.7 - gpio - x1
	P1OUT &= ~BIT7; // power on state
	P1DIR |= BIT7; 
    P1SEL0 &= ~BIT7; 
    P1SEL1 &= ~BIT7;
	// P1.6 - gpio - x2
	P1OUT &= ~BIT6; // power on state
	P1DIR |= BIT6; 
    P1SEL0 &= ~BIT6; 
    P1SEL1 &= ~BIT6;
	// P3.7 - gpio - y1
	P3OUT &= ~BIT7; // power on state
	P3DIR |= BIT7; 
    P3SEL0 &= ~BIT7; 
    P3SEL1 &= ~BIT7;
	// P3.6 - gpio  - y2
	P3OUT &= ~BIT6; // power on state
	P3DIR |= BIT6; 
    P3SEL0 &= ~BIT6; 
    P3SEL1 &= ~BIT6;
	// P2.2 - gpio  - z1
	P2OUT &= ~BIT2; // power on state
	P2DIR |= BIT2; 
    P2SEL0 &= ~BIT2; 
    P2SEL1 &= ~BIT2;
	// P2.6 - gpio  - z2
	P2OUT &= ~BIT6; // power on state
	P2DIR |= BIT6; 
    P2SEL0 &= ~BIT6; 
    P2SEL1 &= ~BIT6;
	//---------------------------------
	// Power mode control (page 73 datasheet)
	//---------------------------------
	// Disable the GPIO power-on default high-impedance mode to activate previously configured port settings 
	PM5CTL0 &= ~LOCKLPM5;  
}

void timer_b_init(void) 
{
	//---------------------------------
	// Timer B0 (page 661 userguide)
	//---------------------------------
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







