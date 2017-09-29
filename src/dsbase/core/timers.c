/*
 * timers.c
 *
 *  Created on: Aug 22, 2017
 *      Author: jeffc
 */
#include "timers.h"

uint32_t timerCycleDiff16(uint16_t earlierTime, uint16_t laterTime)
{
    if (laterTime > earlierTime)
    {
        return laterTime - earlierTime;
    }
    else if (earlierTime > laterTime)
    {
        return laterTime + (0xFFFF - earlierTime);
    }
    else
        return 0;
}

// This function serves to both encapsulate ms->cycles conversion, and because __delay_cycles()
// can only take a constant (determinable at compile time)
void spinWait_ms(uint16_t ms)
{
    for (; ms>0; ms--)
    {
        __delay_cycles(MSEC);
    }
}
