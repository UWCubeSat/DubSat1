/*
 * general_timer.c
 *
 *  Created on: Nov 28, 2017
 *      Author: Thu Phan
 */

#include <msp430.h>
#include <stdint.h>
#include "general_timer.h"

#define MAX (2^16)
static uint64_t counter = 0;    // 1 counter = 65.53 ms --> counter overflow at 71.58 minutes

void initialize_Timer()
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | ID__8;
    PM5CTL0 &= ~LOCKLPM5;
}
uint64_t checkTime_microSeconds()
{
    return (counter * MAX + TA1R);
}

uint64_t checkTime_seconds()
{
    return checkTime_microSeconds() * (10^-6);
}

uint64_t setTimertoZero()
{
    return checkTime_microSeconds();
}

uint64_t checkTimer(uint64_t startTime)
{
    uint64_t currentTime = checkTime_microSeconds();
    __delay_cycles(10);
    if(currentTime < startTime)
    {
        return MAX * MAX - startTime + currentTime;
    }
    uint64_t difference = currentTime - startTime;
    return difference;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    counter++;
    if(counter == 0xFFFF)
    {
        counter = 0;
    }
}

