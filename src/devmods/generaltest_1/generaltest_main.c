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


int main(void)
{
    bspInit(Module_ADCS_BDot);

    __bis_SR_register(GIE);     // Enter LPM3, interrupts enabled

//    debugTraceF(1, "\r\nBSP init complete.\r\n");
//    debugTraceF(1, "And something right after ...\r\n");
//    debugTraceF(1, "And a third thing.\r\n");

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


