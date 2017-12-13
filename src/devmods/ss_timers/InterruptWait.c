#include <msp430.h>
#include <stdint.h>

int count;
void (*fxPtr)();
uint32_t divide(uint32_t duration);

void createInterruptWithOverflow(void (*waitFunc)(), uint16_t timerVal, int countVal)
{
    count = 0;
    fxPtr = waitFunc;
    TA0CCTL1 = CCIE;

    TA0CCR1 = timerVal;
    count = countVal;

    TA0CTL = TASSEL__ACLK | MC__CONTINOUS | TACLR | TAIE | ID__8; // 32,768 Hz, 16 bit timer -> 4096 Hz (w/ divider)

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3 w/ interrupt
}

void createInterrupt(void (*waitFunc)(), uint16_t timerVal)
{
    createInterruptWithOverflow(waitFunc, timerVal, 0);
}

void wait(void (*waitFunc)(), uint32_t duration)  //262144000 ms max duration (73 hours)
{
    int _count = 0;
    //duration = duration * 5120 / 1000;
    duration = divide(duration);
    while(duration > 65535)
    {
        _count++;
        duration -= 65535;
    }
    TA0CCR1 = duration;

    createInterruptWithOverflow(waitFunc, duration, _count);

}

__attribute((ramfunc))
uint32_t divide(uint32_t duration)
{
    return duration * 5120 / 1000;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    switch(TA0IV)
    {
        case TAIV__TAIFG: //overflow case
            count--;
            break;
        case TAIV__TACCR1: //interrupt flag case
            if(count == 0)
            {
                TA0CTL = MC__STOP;
                (*fxPtr)();
            }
            break;
    }
}
