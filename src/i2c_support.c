/*
 * i2c_support.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include <stdint.h>
#include "i2c_support.h"

void i2cInit(uint8_t slaveaddr)
{
    // USCI Configuration:  Common for both TX and RX
    i2cHoldReset();                                             // Software reset enabled
    UCB2CTLW0 |= UCMODE_3 | UCMST | UCSYNC | UCSSEL__SMCLK;     // I2C, master, synchronous, use SMCLK
    UCB2BRW = 12;                                               // Baudrate = SMCLK / 12
    UCB2I2CSA = slaveaddr;                                      // Slave address
    UCB2CTLW1 |= UCASTP_2;                                      // Automatic stop generated after transmission complete
}

// Primary interrupt vector for I2C on module B2 on the 430
#pragma vector = EUSCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{
    switch(__even_in_range(UCB2IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:          break;     // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:   break;     // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG:            // Vector 4: NACKIFG
            UCB2CTL1 |= UCTXSTT;            // Received NACK ... try again, start
            break;
        case USCI_I2C_UCSTTIFG:  break;     // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:  break;     // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:  break;     // Vector 10: RXIFG3
        case USCI_I2C_UCTXIFG3:  break;     // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:  break;     // Vector 14: RXIFG2
        case USCI_I2C_UCTXIFG2:  break;     // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:  break;     // Vector 18: RXIFG1
        case USCI_I2C_UCTXIFG1:  break;     // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:  break;     // Vector 22: RXIFG0
        case USCI_I2C_UCTXIFG0:
//            if (indexBuff < szBuff)
//            {
//                UCB2TXBUF = i2cBuff[indexBuff++];   // Automatically resets UCTXIFG0 flag
//            }
//            else
//            {
//                UCB2CTLW0 |= UCTXSTP;       // I2C stop condition
//                UCB2IFG &= ~UCTXIFG;        // Clear USCI_B2 TX int flag
//                __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
//            }
            break;                          // Vector 24: TXIFG0
        case USCI_I2C_UCBCNTIFG:            // Vector 26: BCNTIFG
//            P1OUT ^= BIT0;                  // Toggle LED on P1.0
            break;
        case USCI_I2C_UCCLTOIFG: break;     // Vector 28: clock low timeout
        case USCI_I2C_UCBIT9IFG: break;     // Vector 30: 9th bit
        default: break;
    }
}

