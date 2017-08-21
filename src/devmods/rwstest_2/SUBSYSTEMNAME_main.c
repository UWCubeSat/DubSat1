#include <msp430.h>

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

uint16_t lastTime = 0, currentTime = 0;
uint16_t currentPeriod = 0;
uint16_t timerOverflows = 0;
/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    // Set pin P1.2 for input, to monitor frequency (and thus RPM) of motor
    // This is using TA1.CCI1A - set to '0 0 1'
    P1DIR &= ~BIT2;
    P1SEL1 &= ~BIT2;
    P1SEL0 |= BIT2;

    // Timer1_A1 Setup:  rising edge, CCIA input, sync mode, capture mode and interrupts enabled
    TA1CCTL1 = CM__RISING | CCIS__CCIA | SCS | CAP | CCIE;

    // SMCLK as clk source, continuous mode
    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | TACLR | TAIE;

    __bis_SR_register(LPM0_bits | GIE);
    __no_operation();


#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

	return 0;
}

// Timer1_A1 CC1-4, TA Interrupt Handler
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
{
    switch (__even_in_range(TA1IV, TAIV__TAIFG))
    {
        case TAIV__TACCR1:
            currentTime = TA1CCR1;
            currentPeriod = currentTime - lastTime;
            lastTime = currentTime;
            __no_operation();
            break;
        case TAIV__TACCR2: break;           // not used
        case TAIV__TACCR3: break;           // not used
        case TAIV__TACCR4: break;           // not used
        case TAIV__TACCR5: break;           // not used
        case TAIV__TACCR6: break;           // not used
        case TAIV__TAIFG:                   // TimerA1 overflow
            timerOverflows++;
            break;
        default:
            break;
    }
}
