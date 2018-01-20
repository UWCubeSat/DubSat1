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
    LED_DIR |= LED_BIT;                 // output
    GPS_ENABLE_DIR |= GPS_ENABLE_BIT;   // output
    BUCK_ENABLE_DIR |= BUCK_ENABLE_BIT; // output
    BUCK_GOOD_DIR &= ~BUCK_GOOD_BIT; // input
    BUCK_GOOD_IE |= BUCK_GOOD_BIT;   // enable interrupt
    BUCK_GOOD_IFG &= ~BUCK_GOOD_BIT; // clear interrupt flag

    // print initial state
    debugPrintF("initial input ");
    if (BUCK_GOOD_IN & BUCK_GOOD_BIT)
    {
        debugPrintF("high\r\n");
    }
    else
    {
        debugPrintF("low\r\n");
    }

    // enable buck converter
    debugPrintF("enabling buck converter\r\n");
    BUCK_ENABLE_OUT |= BUCK_ENABLE_BIT;

    __delay_cycles(1 * SEC);

    // enable GPS
    debugPrintF("enabling GPS\r\n");
    GPS_ENABLE_OUT |= GPS_ENABLE_BIT;

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
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{
    BUCK_GOOD_IFG &= ~BUCK_GOOD_BIT; // clear interrupt
    BUCK_GOOD_IES ^= BUCK_GOOD_BIT;  // toggle interrupt edge

    // trigger a flag depending current input
    if (BUCK_GOOD_IN & BUCK_GOOD_BIT)
    {
        flagOn = 1;
    }
    else
    {
        flagOff = 1;
    }
}
