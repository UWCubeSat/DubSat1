#include <mettest.h>
#include <msp430.h> 

#include "bsp/bsp.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    // Setup RTC Timer
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC with magic value

    //RTCCTL0_L = RTCTEVIE_L;                 // RTC event interrupt enable
    //RTCCTL13 = RTCSSEL_2 | RTCTEV_0 | RTCHOLD; // Counter Mode, RTC1PS, 8-bit ovf
    //RTCPS0CTL = RT0PSDIV1;                  // ACLK, /8
    //RTCPS1CTL = RT1SSEL1 | RT1PSDIV0 | RT1PSDIV1; // out from RT0PS, /16
    RTCCTL13 = RTCSSEL_2 | RTCTEV_3 | RTCHOLD;  // Counter mode implied



    RTCCTL13 &= ~(RTCHOLD);                 // Start RTC

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    return 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_C_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_C_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(RTCIV, RTCIV__RT1PSIFG))
    {
        case RTCIV__NONE:      break;       // No interrupts
        case RTCIV__RTCOFIFG:  break;       // RTCOFIFG
        case RTCIV__RTCRDYIFG: break;       // RTCRDYIFG
        case RTCIV__RTCTEVIFG:              // RTCEVIFG
            P1OUT ^= BIT0;                  // Toggle P1.0 LED
            break;
        case RTCIV__RTCAIFG:   break;       // RTCAIFG
        case RTCIV__RT0PSIFG:  break;       // RT0PSIFG
        case RTCIV__RT1PSIFG:  break;       // RT1PSIFG
        default: break;
    }
}

