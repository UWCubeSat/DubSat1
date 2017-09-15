#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

/*
 * main.c
 */

uint8_t *outstr = "Hi there, said %d times!\r\n";

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    debugInit();
    hBus h = uartInit(ApplicationUART);

    uint8_t outlen = strlen(outstr);
    uint8_t count = 0;

    while(1)
    {
        debugPrintF(outstr, count);
        uartTransmit(h, outstr, outlen);
        count++;
        __delay_cycles(8000000);
    }
	
	return 0;
}
