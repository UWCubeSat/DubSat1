#include <adcs_gps_switch.h>
#include <msp430.h> 

#include "bsp/bsp.h"

FILE_STATIC uint8_t flagOn;
FILE_STATIC uint8_t flagOff;

/*
 * main.c
 */
int main(void)
{
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)
    debugSetMode(Mode_ASCIIInteractive);
#endif  //  __DEBUG__

    // initialize flags
    flagOn = 0;
    flagOff = 0;

    // configure GPIO pins
    LED_DIR |= LED_BIT;                // output
    GPS_POWER_DIR |= GPS_POWER_BIT;    // output
    INPUT_GPIO_DIR &= ~INPUT_GPIO_BIT; // input
    INPUT_GPIO_IE |= INPUT_GPIO_BIT;   // enable interrupt
    INPUT_GPIO_IFG &= ~INPUT_GPIO_BIT; // clear interrupt flag

    // print initial state
    debugPrintF("initial input ");
    if (INPUT_GPIO_IN & INPUT_GPIO_BIT)
    {
        debugPrintF("high\r\n");
    }
    else
    {
        debugPrintF("low\r\n");
    }

    // turn the GPS on
    GPS_POWER_OUT |= GPS_POWER_BIT;

    while (1)
    {
        __delay_cycles(0.1 * SEC);

        // blink LED
        LED_OUT ^= LED_BIT;

        // print changes based on interrupt
        if (flagOn)
        {
            debugPrintF("input high\r\n");
            flagOn = 0;
        }
        if (flagOff)
        {
            debugPrintF("input low\r\n");
            flagOff = 0;
        }
    }
	
	return 0;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    INPUT_GPIO_IFG &= ~INPUT_GPIO_BIT; // clear interrupt
    INPUT_GPIO_IES ^= INPUT_GPIO_BIT;  // toggle interrupt edge

    // trigger a flag depending current input
    if (INPUT_GPIO_IN & INPUT_GPIO_BIT)
    {
        flagOn = 1;
    }
    else
    {
        flagOff = 1;
    }
}
