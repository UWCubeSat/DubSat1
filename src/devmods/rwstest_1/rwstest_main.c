/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//*******************************************************************************
//  MSP430FR5x9x Demo - TimerB, PWM TB0.1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P1.4,P1.5 using
//  TimerB configured for up mode. The value in CCR0, 1000-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1MHz
//  SMCLK as TACLK, the timer period is ~1ms with a 75% duty cycle on P1.4
//  and 25% on P1.5.
//  ACLK = n/a, SMCLK = MCLK = TACLK = 1MHz
//
//           MSP430FR5994
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |     P1.4/TB0.1|--> CCR1 - 75% PWM
//        |     P1.5/TB0.2|--> CCR2 - 25% PWM
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

#include "bsp/bsp.h"

#define INCR_STEP   1
#define TIME_STEP   10000

#define PWM_TIMER(reg)          T##B0##reg
#define PWM_ROOT_TIMER(bits)    T##B##bits

#define SEC     8000000

int main(void)
{
    bspInit(Module_Test);

    // Configure GPIO - Timer B - for PWM
    P1DIR |= BIT4 | BIT5 | BIT7;                   // P1.4 and P1.5 output
    P1SEL0 |= BIT4 | BIT5 | BIT7;                  // P1.4 and P1.5 options select
    P1SEL1 &= ~(BIT4 | BIT5 | BIT7);

    // Configure GPIO - Timer A (to drive LEDs)
    P1DIR |= BIT0 | BIT1;                   // P1.0 and P1.1 output
    P1SEL0 |= BIT0 | BIT1;                  // P1.0 and P1.1 options select

    // Setup pin P7.3 to control direction, P7.4 to read FG pin
    P7DIR |= BIT3;
    P7DIR &= ~BIT4;
    P7SEL0 &= ~(BIT3 | BIT4);
    P7SEL1 &= ~(BIT3 | BIT4);

    // LED pin
    //P1DIR |= 0x01;

//    TIMER(CCR0) = 65500;                       // PWM Period
//    TIMER(CCTL1) = OUTMOD_7;                    // CCR1 reset/set
//    TIMER(CCR1) = 55000;                          // CCR1 PWM duty cycle
//    TIMER(CCTL2) = OUTMOD_7;                    // CCR2 reset/set
//    TIMER(CCR2) = 45000;                          // CCR2 PWM duty cycle
//    TIMER(CTL) = ROOT_TIMER(SSEL__SMCLK) | MC__UP | ROOT_TIMER(CLR);  // SMCLK, up mode, clear TAR
//
//    int incr1 = INCR_STEP, incr2 = INCR_STEP;
//    for (;;)
//    {
//        __delay_cycles(TIME_STEP);
//        if (TIMER(CCR1) >= 65000)
//            incr1 = -1 * INCR_STEP;
//        else if (TIMER(CCR1) <= 0)
//            incr1 = INCR_STEP;
//
//        if (TIMER(CCR2) >= 65000)
//            incr2 = -1 * INCR_STEP;
//        else if (TIMER(CCR2) <= 0)
//            incr2 = INCR_STEP;
//
//        TIMER(CCR1) += incr1;
//        TIMER(CCR2) += incr2;
//    }

    PWM_TIMER(CCR0) = 1000;
    PWM_TIMER(CCTL1) = OUTMOD_7;
    PWM_TIMER(CCTL2) = OUTMOD_7;
    PWM_TIMER(CCTL4) = OUTMOD_7;
    PWM_TIMER(CCR1) = 200;
    PWM_TIMER(CCR2) = 200;
    PWM_TIMER(CCR4) = 200;
    PWM_TIMER(CTL) = PWM_ROOT_TIMER(SSEL__SMCLK) | MC__UP | PWM_ROOT_TIMER(CLR);  // SMCLK, up mode, clear TAR

    debugPrintF("\r\nBeginning spin tests ...\r\n");

    while(1)
    {
        debugPrintF("Direction set to CW (FR = HIGH) ...\r\n");
        P7OUT |= BIT3;
        PWM_TIMER(CCR4) = 15;
        __delay_cycles(5 * SEC);
        debugPrintF("Coasting to a stop ...\r\n");
        PWM_TIMER(CCR4) = 0;
        __delay_cycles(10 * SEC);
        debugPrintF("Direction set to CCW (FR = LOW) ...\r\n");
        P7OUT &= ~ BIT3;
        PWM_TIMER(CCR4) = 15;
        __delay_cycles(5 * SEC);
        debugPrintF("Coasting to a stop ...\r\n");
        PWM_TIMER(CCR4) = 0;
        __delay_cycles(10 * SEC);
    }

    __bis_SR_register(LPM0_bits);           // Enter LPM0
    __no_operation();                       // For debugger

}


