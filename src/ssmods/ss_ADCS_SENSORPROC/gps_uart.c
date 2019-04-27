/*
 * gps_uart.c
 *
 *  Created on: Apr 27, 2019
 *      Author: Adithya Suresh
 */

#include <msp430.h>

void initUART(void){
    //9600 8N1
    UCA3CTLW0 |= UCSWRST;//has to be set to initialize board

    //set clock source to SMCLK
    UCA3CTLW0 |= UCSSEL__SMCLK;

    //set baud rate using table from data sheet
    UCA3BR0 = 104;
    UCA3BR1 = 0;
    UCA3MCTLW = UCBRS1;

    //UCA3STATW |=  UCLISTEN; //Internal loopback mode

    //enable the USCI peripheral
    UCA3CTLW0 &= ~UCSWRST;
    //enables interrupts
    UCA3IE |= (UCTXIE|UCRXIE);
}

void putChar(char c){
    //wait for the buffer to be ready
    while(!(UCA3IFG & UCTXIFG));

    //transmit data
    UCA3TXBUF = c;
    __bis_SR_register(LPM0_bits | GIE); // Enter LPM0, interrupts enabled
}

int getChar(void){
    int chr = -1;

    if(UCA3IFG & UCRXIFG){
        chr = UCRXBUF3;
    }
    return chr;
}

/**
 * main.c
 */
int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //set ports for UART
    P6SEL1 &= ~(BIT0 | BIT1);
    P6SEL0 |= (BIT0 | BIT1);
    P1DIR |= (BIT0|BIT1);
    P1OUT &= ~(BIT0|BIT1);
    PM5CTL0 &= ~LOCKLPM5;

    //sets DCO to 1mHz
    CSCTL0 = CSKEY;
    CSCTL1 |= DCOFSEL_0;
    //sets SMCLK to use DCO
    CSCTL2 = SELS__DCOCLK;

    initUART();
    while(1){
        putChar('A');
    }
    return 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = EUSCI_A3_VECTOR             // eUSCI ISR
__interrupt void USCI_A3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_A3_VECTOR))) USCI_A3_ISR(void)
#else
#error Compiler not supported!
#endif
{
   switch(__even_in_range(UCA3IV,USCI_UART_UCTXCPTIFG)) {
      case USCI_NONE:
         P1OUT &= ~(0x01|0x02);
         break; // Vector 0: No interrupts
      case USCI_UART_UCRXIFG:
         P1OUT |= (0x01);

         UCA3IFG &= ~UCRXIFG;
         __bic_SR_register_on_exit(LPM0_bits);

         break; // Vector 2: UCRXIFG
      case USCI_UART_UCTXIFG:
          P1OUT |= (0x02);
          //__delay_cycles(10000);
         break; // Vector 4: UCTXIFG
      case USCI_UART_UCSTTIFG: break;
      case USCI_UART_UCTXCPTIFG: break;
      default:
         break;
   }
}



