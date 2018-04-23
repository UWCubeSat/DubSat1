#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

/*
 * main.c
 */

uint8_t *outstr = "Hi there, said %d times!\r\n";

// Use pins TX=2.5 and RX=2.6 to talk to application UART
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    debugInit();
    hBus h = uartInit(ApplicationUART, 1);

    uint8_t outlen = strlen(outstr);
    uint8_t count = 0;

    while(1)
    {
        debugTraceF(2,outstr, count);
        uartTransmit(h, outstr, outlen);
        count++;
        __delay_cycles(8000000);
    }
	
	return 0;
}
