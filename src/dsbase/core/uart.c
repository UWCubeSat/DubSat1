/*
 * uart.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifdef __MSP432P401R__
#include <msp.h>
#elif defined __MSP430FR5994__
#include <msp430.h>
#endif
#include <stdio.h>
#include "utils.h"
#include "uart.h"
#include "config/config.h"

FILE_STATIC bus_context_UART buses[CONFIGM_uart_maxperipheralinstances];
FILE_STATIC uint8_t buses_registered_with_debug = 0;  // Only need to register once with debug service for both buses

uint8_t uartReportStatus(DebugMode mode)
{
    int i;
    bus_context_UART *bus_ctx;
    bus_status_UART_packet bpacket;

    static BOOL statusSent[2];

    bcbinPopulateHeader(&(bpacket.header), TLM_ID_SHARED_BUS_UART, sizeof(bpacket));

    // Run through report for each initialized UART
    for (i=0; i<2; i++)
    {
        bus_ctx = &buses[i];

        if (bus_ctx->initialized == 1)
        {
            if (mode == Mode_ASCIIInteractive)
            {
                debugPrintF("**UART %d Status:\r\n", i);
                debugPrintF("*TX:\r\nBytes sent: %d\r\nErrors: %d\r\nBuffer OF: %d\r\nBuff UF: %d\r\nBuff overlapped: %d\r\n",
                        bus_ctx->tx_bytes_sent, bus_ctx->tx_error_count, bus_ctx->tx_error_overrun_count,
                        bus_ctx->tx_error_underrun_count, bus_ctx->tx_overlapped_requests_fulfilled);
                __delay_cycles(1000000);
                debugPrintF("*RX:\r\nBytes rcvd: %d\r\nErrors: %d\r\nMissing handlers: %d\r\n\r\n", bus_ctx->rx_bytes_rcvd,
                            bus_ctx->rx_error_count, bus_ctx->rx_error_missinghandler_count);
            }
            else if (mode == Mode_BinaryStreaming)
            {
                bpacket.busnum = i;
                bpacket.initialized = bus_ctx->initialized;
                bpacket.bushealth = bus_ctx->health;
                bpacket.tx_bytes_sent = bus_ctx->tx_bytes_sent;
                bpacket.rx_bytes_rcvd = bus_ctx->rx_bytes_rcvd;
                bpacket.tx_error_count = bus_ctx->tx_error_count;
                bpacket.rx_error_count = bus_ctx->rx_error_count;

                bcbinSendPacket((uint8_t *) &bpacket, sizeof(bpacket));
            }
        }
        else
        {
            // Make sure un-initialized UART buses are marked as such
            if (statusSent[i] == FALSE && mode == Mode_BinaryStreaming)
            {
                bpacket.busnum = i;
                bpacket.initialized = bus_ctx->initialized;
                bpacket.bushealth = Bus_Uninitialized;
                bpacket.tx_bytes_sent = 0;
                bpacket.rx_bytes_rcvd = 0;
                bpacket.tx_error_count = 0;
                bpacket.rx_error_count = 0;
                bcbinSendPacket((uint8_t *) &bpacket, sizeof(bpacket));
            }
        }

        statusSent[i] = TRUE;
    }
    return 1;
}

// TODO:  Add configuration parameters for speed
hBus uartInit(bus_instance_UART instance, uint8_t echoenable, UARTSpeed speed)
{
    hBus handle = (uint8_t)instance;
    bus_context_UART *bus_ctx = &buses[handle];

    // Only initialize each instance once
    if (bus_ctx->initialized != 0)
        return handle;

    bus_ctx->initialized = 1;
    bus_ctx->echo_on = echoenable;
    bus_ctx->tx_in_use = 0;
    bus_ctx->rx_in_use = 0;

    bspUARTInit(instance);
    if (buses_registered_with_debug == 0)
    {
        buses_registered_with_debug = 1;
        debugRegisterEntity(Entity_UART, NULL, uartReportStatus, NULL);
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
    // Also:  default UART settings are parity disabled,
    // LSB first, 8-bit data, and 1 stop bit
    if (instance == BackchannelUART)
    {
        UCA0CTLW0 = UCSWRST;
        UCA0CTLW0 |= UCSSEL__SMCLK;

        switch (speed)
        {
            case Speed_9600:
                UCA0BRW = UCAxBRW_9600;
                UCA0MCTLW |= UCAxMCTLW_9600;
                break;
            case Speed_38400:
                UCA0BRW = UCAxBRW_38400;
                UCA0MCTLW |= UCAxMCTLW_38400;
                break;
            case Speed_115200:
                UCA0BRW = UCAxBRW_115200;
                UCA0MCTLW |= UCAxMCTLW_115200;
                break;
        }

        UCA0CTLW0 &= ~UCSWRST;
        UCA0IE |= UCRXIE | UCTXIE;
    }
    else if (instance == ApplicationUART)
    {
        UCA1CTLW0 = UCSWRST;
        UCA1CTLW0 |= UCSSEL__SMCLK;

        switch (speed)
        {
            case Speed_9600:
                UCA1BRW = UCAxBRW_9600;
                UCA1MCTLW |= UCAxMCTLW_9600;
                break;
            case Speed_38400:
                UCA1BRW = UCAxBRW_38400;
                UCA1MCTLW |= UCAxMCTLW_38400;
                break;
            case Speed_115200:
                UCA1BRW = UCAxBRW_115200;
                UCA1MCTLW |= UCAxMCTLW_115200;
                break;
        }

        UCA1CTLW0 &= ~UCSWRST;
        UCA1IE |= UCRXIE | UCTXIE;
    }

    bus_ctx->health = Bus_Healthy;
    return handle;
}

void uartSetEchoOn(hBus handle)
{
    bus_context_UART *bus_ctx = &buses[handle];

    // Only initialize each instance once
    if (bus_ctx->initialized == 1)
    {
        bus_ctx->echo_on = 1;
    }
}

void uartSetEchoOff(hBus handle)
{
    bus_context_UART *bus_ctx = &buses[handle];

    // Only initialize each instance once
    if (bus_ctx->initialized == 1)
    {
        bus_ctx->echo_on = 0;
    }
}

void enableUARTInterrupts(handle)
{
    if (handle == BackchannelUART)
    {
        UCA0IE |= UCRXIE | UCTXIE;
    }
    else if (handle == ApplicationUART)
    {
        UCA1IE |= UCRXIE | UCTXIE;
    }
}

void disableUARTInterrupts(handle)
{
    if (handle == BackchannelUART)
    {
        UCA0IE &= ~(UCRXIE | UCTXIE);
    }
    else if (handle == ApplicationUART)
    {
        UCA1IE &= ~(UCRXIE | UCTXIE);
    }
}

void uartTransmit(hBus handle, uint8_t * srcBuff, uint8_t szBuff)
{
    bus_context_UART *bus_ctx = &buses[handle];
    
    if (bus_ctx->initialized != 1)
    {
        error_bus_not_initialized++;
        return;
    }
    
    // MUST turn off interrupts so we don't get the buffer changes out from under us
    disableUARTInterrupts(handle);

    // Are we adding more to the current transmit buffer, or starting fresh?
    // TODO:  make this buffer truly circular
    if (bus_ctx->tx_in_use == 1)
    {

        // Check if we can use the buffer or we don't have enough total space
    	if ((bus_ctx->currentTxNumBytes + szBuff) >= CONFIGM_uart_txbuffsize)
    	{
    	    bus_ctx->tx_error_count++;
    	    bus_ctx->tx_error_overrun_count++;

    	    bus_ctx->health = Bus_BuffOverrunHandled;

    	    enableUARTInterrupts(handle);
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
        bus_ctx->tx_bytes_sent++;

        // Start write process
        if (handle == BackchannelUART)
            UCA0TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];
        else
            UCA1TXBUF = bus_ctx->txBuff[bus_ctx->currentTxIndex++];


        __bis_SR_register(GIE);     // Make sure interrupts enabled
    }

    enableUARTInterrupts(handle);
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
    disableUARTInterrupts(instance);

    if (bus_ctx->tx_in_use == 0)
    {
        bus_ctx->tx_error_count++;
        bus_ctx->tx_error_underrun_count++;
        bus_ctx->health = Bus_Underrun;
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

    enableUARTInterrupts(instance);
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

