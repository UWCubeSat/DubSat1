#include <coretest.h>
#include <msp430.h> 

#include <stdint.h>

#include "bsp/bsp.h"

/*
 * main.c
 */

typedef struct PACKED_STRUCT {
    uint32_t lotwaferid;
    uint32_t dieXYpos;
} die_record;


int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    die_record test = *((die_record *)0x1A0A);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
    while (1)
    {
        debugPrintF("Test value:  %d\r\n", test.lotwaferid);
        __delay_cycles(1 * SEC);
    }

	return 0;
}
