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

FILE_STATIC bus_context_UART buses[CONFIGM_uart_maxperipheralinstances];
FILE_STATIC buses_registered_with_debug = 0;  // Only need to register once with debug service for both buses

uint8_t uartReportStatus(DebugMode mode)
{
    int i;
    bus_context_UART *bus_ctx;

    // Run through report for each initialized UART
    for (i=0; i<2; i++)
    {
        bus_ctx = &buses[i];
        if (bus_ctx->initialized == 1)
        {
            if (mode == InteractiveMode)
            {

                debugPrintF("**UART %d Status:\r\n", i);
                debugPrintF("*TX:\r\nBytes sent: %d\r\nErrors: %d\r\nBuffer OF: %d\r\nBuff UF: %d\r\nBuff overlapped: %d\r\n",
                        bus_ctx->tx_bytes_sent, bus_ctx->tx_error_count, bus_ctx->tx_error_buffer_overflow_count,
                        bus_ctx->tx_error_underrun_count, bus_ctx->tx_overlapped_requests_fulfilled);
                __delay_cycles(1000000);
                debugPrintF("*RX:\r\nBytes rcvd: %d\r\nErrors: %d\r\nMissing handlers: %d\r\n\r\n", bus_ctx->rx_bytes_rcvd,
                            bus_ctx->rx_error_count, bus_ctx->rx_error_missinghandler_count);
            }
            else
            {
                // Output just CSV fields, for telemetry consumption
            }
        }
    }
    return 1;
}

// TODO:  Add configuration parameters for speed
hBus uartInit(bus_instance_UART instance)
{
    hBus handle = (uint8_t)instance;
    bus_context_UART *bus_ctx = &buses[handle];

    // Only initialize each instance once
    if (bus_ctx->initialized != 0)
        return handle;

    bus_ctx->initialized = 1;
    bus_ctx->echo_on = 1;
    bus_ctx->tx_in_use = 0;
    bus_ctx->rx_in_use = 0;

    bspUARTInit(instance);
    if (buses_registered_with_debug == 0)
    {
        buses_registered_with_debug = 1;
        debugRegisterEntity(Entity_UART, 'u', NULL, uartReportStatus, NULL);
    }
    
    // TODO:  Add logic to rejigger the dividers based on current clock
    // setting ... these currently ASSUME A 8MHz CLOCK!
    // Now using 115.2kbps for baud rate (see table 24-5 in Family Guide)
    // Seems relatively stable
    // Configure USCI_An for UART mode, generally looks like:
    /*
     * UCA0CTLW0 = UCSWRST;                    // Put eUSCI in reset
       UCA0CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
       UCA0BRW = 4;
       UCA0MCTLW |= UCOS16 | UCBRF_5 | 0x55;
       UCA0CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
       UCA0IE |= UCRXIE | UCTXIE;              // Enable USCI_A0 RX interrupt
     */
    if (instance == BackchannelUART)
    {
        UCA0CTLW0 = UCSWRST;
        UCA0CTLW0 |= UCSSEL__SMCLK;
        UCA0BRW = 4;
        UCA0MCTLW |= UCOS16 | UCBRF_5 | 0x55;
        UCA0CTLW0 &= ~UCSWRST;
        UCA0IE |= UCRXIE | UCTXIE;
    }
    else if (instance == ApplicationUART)
    {
        UCA1CTLW0 = UCSWRST;
        UCA1CTLW0 |= UCSSEL__SMCLK;
        UCA1BRW = 4;
        UCA1MCTLW |= UCOS16 | UCBRF_5 | 0x55;
        UCA1CTLW0 &= ~UCSWRST;
        UCA1IE |= UCRXIE | UCTXIE;
    }

    return handle;
}

void uartTransmit(hBus handle, uint8_t * srcBuff, uint8_t szBuff)
{
    bus_context_UART *bus_ctx = &buses[handle];
    
    if (bus_ctx->initialized != 1)
    {
        error_bus_not_initialized++;
        return;
    }
    
    // Are we adding more to the current transmit buffer, or starting fresh?
    // TODO:  make this buffer truly circular
    if (bus_ctx->tx_in_use != 0)
    {
        // Check if we can use the buffer or we don't have enough total space
    	if ((bus_ctx->currentTxNumBytes + szBuff) >= CONFIGM_uart_txbuffsize)
    	{
    	    bus_ctx->tx_error_count++;
    	    bus_ctx->tx_error_overrun_count++;
    	    return;
        }
        else
        {
            // Leave currentTxIndex where it is, but modify everything else
            // and add new stuff to buffer
            bus_ctx->tx_overlapped_requests_fulfilled++;
            memcpy(bus_ctx->txBuff+bus_ctx->currentTxNumBytes, srcBuff, szBuff);
            bus_ctx->currentTxNumBytes += szBuff;
        }
    }
    else
    {
        // If not currently in use, we start over at start of buffer
        bus_ctx->currentTxIndex = 0;
        bus_ctx->currentTxNumBytes = szBuff;
        memcpy(bus_ctx->txBuff, srcBuff, szBuff);
    }

    bus_ctx->tx_in_use = 1;

    // Start adding characters to transmit buffer if this is a new transmission
    if (bus_ctx->currentTxIndex == 0)
    {
        // Start write process
        if (handle == BackchannelUART)
            UCA0TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];
        else
            UCA1TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];
        bus_ctx->tx_bytes_sent++;

        __bis_SR_register(GIE);     // Make sure interrupts enabled
    }
}

void uartRegisterRxCallback(hBus handle, void (*callback)(uint8_t rcvdbyte))
{
    bus_context_UART *bus_ctx = &buses[handle];

    if (bus_ctx->initialized != 1)
    {
        error_bus_not_initialized++;
        return;
    }

    bus_ctx->rxCallback = callback;
}

// Helpers for handling interrupts (shared code so less copy/pasting to support multiple UARTS)
void handleUCRXIFG(bus_context_UART *bus_ctx, bus_instance_UART instance)
{
    uint8_t rcvdbyte;

    if (instance == BackchannelUART)
        rcvdbyte = UCA0RXBUF;
    else
        rcvdbyte = UCA1RXBUF;

    bus_ctx->rx_bytes_rcvd++;

    if (bus_ctx->echo_on == 1)
    {
        if (instance == BackchannelUART)
            UCA0TXBUF = rcvdbyte;
        else
            UCA1TXBUF = rcvdbyte;
    }

    if (bus_ctx->rxCallback == 0)
    {
        bus_ctx->rx_error_count++;
        bus_ctx->rx_error_missinghandler_count++;
        return;
    }

    (*(bus_ctx->rxCallback))(rcvdbyte);
}

void handleUCTXIFG(bus_context_UART *bus_ctx, bus_instance_UART instance)
{
    if (bus_ctx->tx_in_use == 0)
    {
        bus_ctx->tx_error_count++;
        bus_ctx->tx_error_underrun_count++;
    }

    if (bus_ctx->currentTxIndex < bus_ctx->currentTxNumBytes)
    {
        if (instance == BackchannelUART)
            UCA0TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];
        else
            UCA1TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];
        bus_ctx->tx_bytes_sent++;
    }
    else
    {
        bus_ctx->currentTxIndex++;
        bus_ctx->tx_in_use = 0;
    }
}


// Interrupt vector for BACKCHANNEL UART (A0-based)
#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    bus_context_UART *bus_ctx = &buses[BackchannelUART];

    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            handleUCRXIFG(bus_ctx, BackchannelUART);
            break;
        case USCI_UART_UCTXIFG:     // Set when tx buff is empty
            handleUCTXIFG(bus_ctx, BackchannelUART);
            break;
        case USCI_UART_UCSTTIFG: break;  // Set after start received
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

// Interrupt vector for APPLICATION UART (A1-based)
#pragma vector=EUSCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    bus_context_UART *bus_ctx = &buses[ApplicationUART];

    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            handleUCRXIFG(bus_ctx, ApplicationUART);
            break;
        case USCI_UART_UCTXIFG:     // Set when tx buff is empty
            handleUCTXIFG(bus_ctx, ApplicationUART);
            break;
        case USCI_UART_UCSTTIFG: break;  // Set after start received
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

