#include <coretest.h>
#include <msp430.h> 

#include <stdint.h>

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
	
    while (1)
    {
        debugPrintF("Testing ...\r\n");
        __delay_cycles(1 * SEC);
    }

	return 0;
}
