#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/timers.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    // Enable for output
    LED_PORT_DIR |= LED_BITS;

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    while(1)
    {
        LED_PORT_OUT ^= LED_BITS;
        __delay_cycles(0.1 * SEC);
    }
	
	return 0;
}
