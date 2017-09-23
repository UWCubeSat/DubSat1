#include <msp430.h> 

/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "core/debugtools.h"
#include "bsp/bsp.h"

char buff[64] = "Hello: %d";
char spew[] = "#: %d\r\n";

uint8_t testInfoCallback(DebugMode mode)
{
    if (mode == InteractiveMode)
    {
        debugPrintF("In test info callback ...\r\n");
    }
    return 1;
}

uint8_t testStatusCallback(DebugMode mode)
{
    if (mode == InteractiveMode)
    {
        debugPrintF("In test status callback ...\r\n");
    }
    return 1;
}

int main(void)
{
    bspInit(Module_ADCS_BDot);
    debugRegisterEntity(Entity_Test, 'q', testInfoCallback, testStatusCallback, 0);

    __bis_SR_register(GIE);     // Enter LPM3, interrupts enabled

    int i = 0;
    for(;;)
    {
        debugTraceF(2, spew, i);
        i++;

        _delay_cycles(500000);
        //debugTraceF(3, "This is trace level 3.\r\n");
    }

    //__bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts


}


