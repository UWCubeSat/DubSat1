#include <adcs_gps_switch.h>
#include <msp430.h> 

#include "bsp/bsp.h"

/*
 * main.c
 */
int main(void)
{
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    // configure GPIO pins
    LED_DIR |= LED_BIT;                 // output
    GPS_ENABLE_DIR |= GPS_ENABLE_BIT;   // output
    BUCK_ENABLE_DIR |= BUCK_ENABLE_BIT; // output

    // turn on the LED solid while booting up
    LED_OUT |= LED_BIT;

    __delay_cycles(DELAY_TIME * SEC);

    BUCK_ENABLE_OUT |= BUCK_ENABLE_BIT;

    __delay_cycles(DELAY_TIME * SEC);

    GPS_ENABLE_OUT |= GPS_ENABLE_BIT;

    // blink LED while in normal operation
    while (1)
    {
        LED_OUT ^= LED_BIT;
        __delay_cycles(SEC);
    }
	
	return 0;
}
