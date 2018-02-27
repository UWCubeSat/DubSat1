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
// defines 
//------------------------------------------------------------------
#define PWM_PERIOD (1000-1) // pwm period = 1000 us  
#define X1 1 // for set_pwm function which_pin 
#define X2 2 
#define Y1 3  
#define Y2 4  
#define Z1 5 
#define Z2 6 

//------------------------------------------------------------------
// global variables 
//------------------------------------------------------------------
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile TumbleState tumble_status = Idle; // static volatile enum _tumble_state {Tumbling=1,Idle=0} TumbleState tumble_status = Idle;
FILE_STATIC volatile int duty_X1, duty_X2, duty_Y1, duty_Y2, duty_Z1, duty_Z2; 
FILE_STATIC volatile int received_command_packet; // flag to protect against weird behavior if in tumbling state but no command packet has been received

//------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------
void mtq_gpio_init(void);
void mtq_gpio_debug_init(void); // DEBUG
void timer_b_init(void); 
void interpret_command_dipole(int x_command, int y_command, int z_command);
void set_pwm(int which_pin, int ccr_value); 
void can_packet_rx_handler(CANPacket *packet);
void send_ping_packet(void);

//------------------------------------------------------------------
// Main 
//------------------------------------------------------------------
int main(void)
{	
	bspInit(__SUBSYSTEM_MODULE__);
	//mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);
	// port initialization 
	mtq_gpio_init();
	timer_b_init();
	// CAN initialization 
	canWrapInit();
	setCANPacketRxCallback(can_packet_rx_handler);
	
    while (1)
	{
		// control for turning on coils based on command dipole signals 
        switch (tumble_status) 
		{
			case Tumbling: 
				send_ping_packet();
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
// Interrupt handler definitions 
//------------------------------------------------------------------

void can_packet_rx_handler(CANPacket *packet)
{
	// check if packet is tumble status 
	if (packet->id == CAN_ID_BDOT_TUMBLE_STATUS)
	{
		// initialize tumble status packet
		bdot_tumble_status tumble_status_packet = {0}; 
		// use tumble status to set tumble state 
		decodebdot_tumble_status(packet, &tumble_status_packet);
		if (tumble_status_packet.bdot_tumble_status_status) //tumbling
		    tumble_status = Tumbling;
	}
	 
	// check if packet is command dipole
	if (packet->id == CAN_ID_BDOT_COMMAND_DIPOLE)
	{
		received_command_packet = 1; // set received_command_packet
		// initialize command dipole packet
		bdot_command_dipole command_dipole_packet = {0};
		decodebdot_command_dipole(packet, &command_dipole_packet);
		// use dipole commands to update values for commands 
		interpret_command_dipole(command_dipole_packet.bdot_command_dipole_x,
								 command_dipole_packet.bdot_command_dipole_y, 
								 command_dipole_packet.bdot_command_dipole_z);
	}
}	

//------------------------------------------------------------------
// functions definitions 
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

void interpret_command_dipole(int x_command, int y_command, int z_command)  
{
	// valid command range : (-100 - 100)
	if (x_command > 100 | x_command < -100 | y_command > 100 | y_command < -100 | z_command > 100 | z_command < -100)
		; // TODO add error condition  
	duty_X1 = (x_command >= 0) ? (x_command) : 0;
	duty_X2 = (x_command >= 0) ? 0: -(x_command);
	duty_Y1 = (y_command >= 0) ? (y_command) : 0;
	duty_Y2 = (y_command >= 0) ? 0: -(y_command);	
	duty_Z1 = (z_command >= 0) ? (z_command) : 0;
	duty_Z2 = (z_command >= 0) ? 0: -(z_command);
}

void set_pwm(int which_pin, int pwm_percent)  
{
	const int ccr_value = pwm_percent*10;
	switch(which_pin)
	{
		case X1: // MTQ: P1_7 Launchpad: P3_5
			TB0CCR4 = 0xccr_value;
			break; 
		case X2: // MTQ: P1_6 Launchpad: P3_4
			TB0CCR3 = 0xccr_value;
			break;
		case Y1: // MTQ: P3_7 Launchpad: P3_7
			TB0CCR6 = 0xccr_value;
			break;
		case Y2: // MTQ: P3_6 Launchpad: P3_6
			TB0CCR5 = 0xccr_value;
			break;	
		case Z1: // MTQ: P2_2 Launchpad: P1_5
			TB0CCR2 = 0xccr_value;
			break;	
		case Z2: // MTQ: P2_6 Launchpad: P2_6
			TB0CCR1 = 0xccr_value;
			break;
		default: // unknown state 
			break;
	}
}

void send_ping_packet(void)
{
	CANPacket p = {0};
	p.id = 1234;
	p.length = 0;
	canSendPacket(&p);
}






