#include <stdio.h>

#ifdef __MSP432P401R__
#include <msp.h>
#elif defined __MSP430FR5994__
#include <msp430.h>
#endif

#include <stdint.h>
#include "spi.h"

void spiInit(uint8_t csPins)
{
	/******************************************** UCB0CTLW0 ************************************************ 
	/  [15]   UCCKPH:  0b = Data is changed on the first UCLK edge and captured on the following edge.
	/  [14]   UCCKPL:  0b = The inactive state is low.
	/  [13]   UCMSB:   MSB first select. Controls the direction of the receive and transmit shift register.
	/ 		   		   1b = MSB first.
	/  [12]   UC7BIT:  Character length. Selects 7-bit or 8-bit character length. 0b = 8-bit data.
	/  [11]   UCMST:   Master mode select. 1b = Master mode.
	/  [10-9] UCMODEx: eUSCI mode. The UCMODEx bits select the synchronous mode when UCSYNC = 1.
	/ 				   00b = 3-pin SPI
	/  [8] 	  UCSYNC:  Synchronous mode enable. 1b = Synchronous mode. 
	/  [7-6]  UCSSELx: 10b = SMCLK in master mode. Don't use in slave mode.
	/  [1]    UCSTEM:  STE mode select in master mode. This byte is ignored in slave or 3-wire mode.
	/  [0]    UCSWRST: Software reset enable. 0b = Disabled. eUSCI reset released for operation.
	********************************************************************************************************/

	/* 1. Set UCSWRST. */
	UCA2CTLW0 = UCSWRST;
	UCA2CTLW0 |= UCSSEL_2;
	UCA2CTLW0 |= UCMODE_THREE_WIRE_SPI | UCMSB | UCSYNC | UCMST | UCCKPH;
	//UCCKPH shifts bits to rising edge of clock.

	/* 3. Configure ports. 
	P5SEL0.0 = 1: UCB1SIMO
	P5SEL0.1 = 1: UCB1SOMI
	P5SEL0.2 = 1: UCB1CLK */
	P5SEL0 |= BIT4 | BIT5 | BIT6;
		
	/* Dividing SMCLK by SPI_CLOCK_DIV. */
	/* SMCLK is originally running at 8 MHz. */
	UCA2BRW = SPI_CLOCK_DIV;

	/* 4. Ensure that any input signals into the SPI module such as UCxSOMI (in master mode) 
	or UCxSIMO and UCxCLK (in slave mode) have settled to their final voltage levels 
	before clearing UCSWRST and avoid any unwanted transitions during operation. */
	uint16_t i;
	for (i = 0; i < WAIT_TIME; i++)
	{
	}
	
	/* 5. Clear UCSWRST to start device. */
	UCA2CTLW0 &= ~UCSWRST;

	/* 6. Set P8 Direction to OUT */
    if(csPins & 0x01){
        P2DIR |= 0x10;
    }
    if(csPins & 0x02){
        P4DIR |= 0x10;
    }
    if(csPins & 0x04){
        P4DIR |= 0x20;
    }
	// Set CS (P8.0) high
    if(csPins & 0x01){
        P2OUT |= 0x10;
    }
    if(csPins & 0x02){
        P4OUT |= 0x10;
    }
    if(csPins & 0x04){
        P4OUT |= 0x20;
    }
}
void spiTransceive(uint8_t *pTxBuf, uint8_t *pRxBuf, size_t num, uint8_t csPin){

	// Clear the MSP430's rxBuffer of any junk data left over from previous transactions.
	//*pRxBuf = UCB1RXBUF;

	uint8_t dummydata = UCB1RXBUF;

	// TX all data from the provided register over the SPI bus by adding it to the txbuffer one byte at a time.
	// Store all data received from the slave in pRxBuf.
	uint8_t i;
	for(i = 0; i < num; i++) {

		// Wait for any previous tx to finish.
		while (!(UCA2IFG & UCTXIFG));

		// Drop CS Pin if it hasn't been dropped yet.
		if (i == 0){
            if(csPin & 0x01){
                P2OUT &= ~0x10;
            }
            else if(csPin & 0x02){
                P4OUT &= ~0x10;
            }
            else if(csPin & 0x04){
                P4OUT &= ~0x20;
            }
		}

		// Write to tx buffer.
		UCA2TXBUF = *pTxBuf;

		// Bring CS High again.
		if (i == num - 1){
            if(csPin & 0x01){
                P2OUT |= 0x10;
            }
            else if(csPin & 0x02){
                P4OUT |= 0x10;
            }
            else if(csPin & 0x04){
                P4OUT |= 0x20;
            }
		}

		// Wait for any previous rx to finish rx-ing.
		while (!(UCA2IFG & UCRXIFG));

		// Store data transmitted from the slave.
		*pRxBuf = UCA2RXBUF;

		// Increment the pointer to send and store the next byte.
		pTxBuf++;
		pRxBuf++;

	}
}
