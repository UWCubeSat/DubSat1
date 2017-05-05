/*
 * bsp.c
 *
 *  Created on: May 5, 2017
 *      Author: jeffc
 */

#ifdef __BSP_Board_MSP430FR5994LaunchPad__

#include <msp430.h>

void bspInit()
{
    // Stop watchdog timer
        WDTCTL = WDTPW | WDTHOLD;

        // Clock System Setup
        CSCTL0_H = CSKEY_H;                     // Unlock CS registers
        CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
        // Set SMCLK = MCLK = DCO, ACLK = VLOCLK
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;

        // Per Device Errata set divider to 4 before changing frequency to
        // prevent out of spec operation from overshoot transient
        CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all corresponding clk sources to divide by 4 for errata
        CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz

        // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
        __delay_cycles(60);
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 for 8MHz operation
        CSCTL0_H = 0;                           // Lock CS Registers

}

#else

#error Unspecified board hardware, unable to determine correct BSP implementation.  Please specify board.

#endif

