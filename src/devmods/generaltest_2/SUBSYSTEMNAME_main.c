#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    TA0CCTL0 = CCIE;                        // TACCR0 interrupt enabled
    TA0CCR0 = 50000;
    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS; // SMCLK, continuous mode

	
	return 0;
}
