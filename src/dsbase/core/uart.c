/*
 * uart.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include <msp430.h>
#include <stdio.h>
#include "utils.h"
#include "uart.h"
#include "config/config.h"

FILE_STATIC uint8_t txBuff[CONFIGM_uart_txbuffsize];
FILE_STATIC volatile uint8_t currentTxIndex = 0;
FILE_STATIC uint8_t currentTxNumBytes;
FILE_STATIC volatile uint8_t currentRxIndex = 0;

FILE_STATIC bus_status_UART uart_status;
FILE_STATIC void (*rxCallback)(uint8_t) = 0;

FILE_STATIC bus_status_UART bus_status[CONFIGM_uart_maxperipheralinstances];

// TODO:  Add configuration parameters for speed
hBus uartInit(bus_instance_UART instance)
{
    hBus handle = (uint8_t)instance;
    bus_status_UART *bus = &bus_status[handle];

    // Only initialize each instance once
    if (bus->initialized != 0)
        return handle;

    bus->initialized = 1;
    bus->echo_on = 1;
    bus->tx_in_use = 0;
    bus->rx_in_use = 0;

    bspUARTInit(instance);

    // TODO:  Add logic to rejigger the dividers based on current clock
    // setting ... these currently ASSUME A 8MHz CLOCK!
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                    // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
    // ORIGINAL Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    //UCA0BRW = 52;                           // 8000000/16/9600
    //UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;

    // Now using 115.2kbps for baud rate (see table 24-5 in Family Guide)
    // Seems relatively stable
    UCA0BRW = 4;
    UCA0MCTLW |= UCOS16 | UCBRF_5 | 0x55;

    UCA0CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
    UCA0IE |= UCRXIE | UCTXIE;              // Enable USCI_A0 RX interrupt

    return handle;
}

void uartTransmit(hBus handle, uint8_t * srcBuff, uint8_t szBuff)
{
    // Check for overlong transmission or transmission in progress
    if (uart_status.tx_in_use != 0)
    {
        uart_status.tx_error_count++;
        uart_status.tx_error_overrun_count++;
        return;
    }
    if (szBuff > CONFIGM_uart_txbuffsize)
    {
        uart_status.tx_error_count++;
        uart_status.tx_error_buffer_overflow_count++;
        return;
    }

    // Transmit is good, cache the data to be sent
    memcpy(txBuff, srcBuff, szBuff);
    currentTxIndex = 0;
    currentTxNumBytes = szBuff;
    uart_status.tx_in_use = 1;

    // Start write process
    UCA0TXBUF = txBuff[currentTxIndex++];
    uart_status.tx_bytes_sent++;

    __bis_SR_register(GIE);     // Interrupts enabled

}

void uartRegisterRxCallback(hBus handle, void (*callback)(uint8_t rcvdbyte))
{
    rxCallback = callback;
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    uint8_t rcvdbyte;

    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            rcvdbyte = UCA0RXBUF;
            uart_status.rx_bytes_rcvd++;

            if (uart_status.echo_on == 1)
            {
                UCA0TXBUF = rcvdbyte;
            }

            if (rxCallback == 0)
            {
                uart_status.rx_error_count++;
                uart_status.rx_error_missinghandler_count++;
                return;
            }
            (*rxCallback)(rcvdbyte);
            break;
        case USCI_UART_UCTXIFG:     // Set when tx buff is empty
            if (uart_status.tx_in_use == 0)
            {
                uart_status.tx_error_count++;
                uart_status.tx_error_underrun_count++;
            }

            if (currentTxIndex < currentTxNumBytes)
            {
                UCA0TXBUF = txBuff[currentTxIndex++];
                uart_status.tx_bytes_sent++;
            }
            else
            {
                currentTxIndex++;
                uart_status.tx_in_use = 0;
            }
            break;
        case USCI_UART_UCSTTIFG: break;  // Set after start received
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}
