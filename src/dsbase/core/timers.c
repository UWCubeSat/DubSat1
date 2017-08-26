/*
 * timers.c
 *
 *  Created on: Aug 22, 2017
 *      Author: jeffc
 */
#include "timers.h"

uint16_t timerCycleDiff16(uint16_t earlierTime, uint16_t laterTime)
{
    if (laterTime > earlierTime)
    {
        return laterTime - earlierTime;
    }
    else
    {
        return laterTime + (0xFFFF - earlierTime);
    }
}
