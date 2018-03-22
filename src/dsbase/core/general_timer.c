#ifdef __MSP432P401R__
#include <msp.h>
#elif defined __MSP430FR5994__
#include <msp430.h>
#endif


#include <stdint.h>
#include "general_timer.h"

#define MAX 2e16
static uint64_t counter = 0;    // 1 counter = 65.53 ms --> counter overflow at 71.58 minutes

//
//int main(void)
//{
//    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
//    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | ID__8;
//    while(1)
//    {
//    }
//}

uint64_t checkTime_microSeconds()
{
    return counter * MAX + TA1R;
}

uint64_t checkTime_seconds()
{
    return checkTime_microSeconds() * 10e-6;
}

uint64_t setTimertoZero()
{
    return checkTime_microSeconds();
}

uint64_t checkTimer(uint64_t startTime)
{
    uint64_t currentTime = checkTime_microSeconds();
    if(currentTime < startTime)
    {
        return MAX * MAX - startTime + currentTime;
    }
    return currentTime - startTime;
}


#ifdef __MSP430FR5994__
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    counter++;
    if(counter == 0xFFFF)
    {
        counter = 0;
    }
}
#elif defined __MSP432P401R__
__interrupt void Timer1_A0_ISR(void)
{}
#endif

//#pragma vector = TIMER1_A0_VECTOR
//__interrupt void Timer1_A0_ISR(void)
//{
//    counter++;
//    if(counter == 0xFFFF)
//    {
//        counter = 0;
//        overflow++;
//    }
//    if(overflow == 0xFFFF)
//    {
//        overflow = 0;
//    }
//}
