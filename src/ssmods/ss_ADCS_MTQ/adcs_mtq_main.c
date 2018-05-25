/*
file: adcs_mtq_main.c 
author: Eloise Perrochet
description: software for magnetorquer subsystem 

coil  pin   timer 
-----------------
X1 - P1_7 - TB0.4
X2 - P1_6 - TB0.3
Y1 - P3_7 - TB0.6
Y2 - P3_6 - TB0.5
Z1 - P2_2 - TB0.2
Z2 - P2_6 - TB0.1
*/

//--------includes--------
#include <msp430.h>
#include <stdint.h>
#include "core/utils.h"
#include "core/timer.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"
#include "sensors/analogsensor.h"

TIMER_HANDLE testTimer1;
uint32_t testTimer1_ms = 100;
int main(void)
{	
    // general initialization
    bspInit(__SUBSYSTEM_MODULE__); // BSP initialization
    initializeTimer();
    testTimer1 = timerPollInitializer(testTimer1_ms);
    P1DIR |= BIT7;
    P1DIR |= BIT6;
    P3DIR |= BIT7;
    P3DIR |= BIT6;
    P2DIR |= BIT2;
    PJDIR |= BIT0;
    while (1)
    {
        if(checkTimer(testTimer1)){
            P1OUT ^= BIT7;
            PJOUT ^= BIT0;
            testTimer1 = timerPollInitializer(testTimer1_ms);
        }
    }
		
	return 0;
}
