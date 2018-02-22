//------------------------------------------------------------------
// includes
//------------------------------------------------------------------
#include <msp430.h> 
#include "adcs_mtq.h"
#include "interfaces/canwrap.h" // TODO add to dsbase bsp.h? 
#include "bsp/bsp.h"

//------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------
void mtq_gpio_init(void);
void mtq_gpio_debug_init(void); // DEBUG
void timer_b_init(void); 
void interpret_command_dipole(int pwm_percent, int *duty_1, int *duty_2);
void set_pwm(int which_pin, int duty_cycle);
void receive_packet(CANPacket *packet);
void send_ping_packet(void);

//------------------------------------------------------------------
// static variables 
//------------------------------------------------------------------
FILE_STATIC volatile TumbleState tumble_status = Idle; // static volatile enum _tumble_state {Tumbling=1,Idle=0} TumbleState tumble_status = Idle;

//------------------------------------------------------------------
// Main 
//------------------------------------------------------------------
int main(void)
{	
	// initialization 
	bspInit(__SS_ADCS_MTQ__);
	mtq_gpio_debug_init();
	timer_b_init();
	canWrapInit();
	setCANPacketRxCallback(receive_packet);
	
    while (1){}
	return 0;
}

//------------------------------------------------------------------
// Interrupt handler definitions 
//------------------------------------------------------------------
void receive_packet(CANPacket *packet)
{
	// check if packet is tumble status 
	if (packet->id == CAN_ID_BDOT_TUMBLE_STATUS)
	{
		// initialize packet
		bdot_tumble_status tumble_status_packet = {0}; 
		decodebdot_tumble_status(packet, &tumble_status_packet);
		tumble_status = (tumble_status_packet.bdot_tumble_status_status == 1);
	}
	
	// check if packet is command dipole 
	if (packet->id == CAN_ID_BDOT_COMMAND_DIPOLE)
	{
		// initialize packet
		bdot_command_dipole command_dipole_packet = {0};
		decodebdot_command_dipole(packet, &command_dipole_packet);
		// used to set duty cycles for all outputs 
		int duty_x1, duty_x2, duty_y1, duty_y2, duty_z1, duty_z2; 
		// control for turning on coils based on command dipole signals 
        switch (tumble_status) 
		{
			case Tumbling: 
				PJOUT |= BIT0|BIT1|BIT2; // debug lights 
				// control for turning on coils based on command dipole signals 
				interpret_command_dipole(command_dipole_packet.bdot_command_dipole_x, &duty_x1, &duty_x2); // interpret dipole commands
				interpret_command_dipole(command_dipole_packet.bdot_command_dipole_y, &duty_y1, &duty_y2); 
				interpret_command_dipole(command_dipole_packet.bdot_command_dipole_z, &duty_z1, &duty_z2); 
				// set duty cycles 
				set_pwm(X1, duty_x1); 
				set_pwm(X2, duty_x2); 
				set_pwm(Y1, duty_y1); 
				set_pwm(Y2, duty_y2);
				set_pwm(Z1, duty_z1); 
				set_pwm(Z2, duty_z2); 		
				break;
			case Idle: 
				PJOUT &= ~(BIT0|BIT1|BIT2); // debug lights 
				// set all duty cycles to 0 
				TB0CCR4 = PWM_0; // x1
				TB0CCR3 = PWM_0; // x2
				TB0CCR6 = PWM_0; // y1
				TB0CCR5 = PWM_0; // y2 
				TB0CCR2 = PWM_0; // z1
				TB0CCR1 = PWM_0; // z2
				break;
        	default:
				// unknown state 
            	break;
        }
	
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
	TB0CCR4 = PWM_0; // P1.7 - TB0.4 - x1
	TB0CCR3 = PWM_0; // P1.6 - TB0.3 - x2
	TB0CCR6 = PWM_0; // P3.7 - TB0.6 - y1
	TB0CCR5 = PWM_0; // P3.6 - TB0.5 - y2 
	TB0CCR2 = PWM_0; // P2.2 - TB0.2 - z1
	TB0CCR1 = PWM_0; // P2.6 - TB0.1 - z2
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

void interpret_command_dipole(int pwm_percent, int* duty_1, int* duty_2)  
{
	if (pwm_percent > 0) 
	{
		*duty_1 = pwm_percent;
		*duty_2 = 0;
	}
	else if (pwm_percent < 0)
	{
		*duty_1 = 0;
		*duty_2 = -pwm_percent;
	}
	else // pwm_percent == 0
	{
		*duty_1 = 0;
		*duty_2 = 0;
	}	
}

void set_pwm(int which_pin, int duty_cycle)  
{
	// PWM duty cycle should always be between 0 100 TODO add range check
	int ccr_value = ((duty_cycle*10) - 1); // Assigning |duty cycle| = (pwm_percent*10)-1 --> 100 = 1000-1 = 1000 us duty 
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

void send_ping_packet(void)
{
	CANPacket p = {0};
	p.id = 1234;
	p.length = 0;
	canSendPacket(&p);
}






