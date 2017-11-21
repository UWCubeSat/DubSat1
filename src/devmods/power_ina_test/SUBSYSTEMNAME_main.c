#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    P1DIR |= BIT0;
    P1OUT |= BIT0;


    for(;;) {
        volatile unsigned int i;

        P1OUT ^= BIT0;

        i = 10000;
        do i--;
        while(i > 0);
    }
}
