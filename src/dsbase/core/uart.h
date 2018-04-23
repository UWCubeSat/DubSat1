/*
 * uart.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "utils.h"
//#include "bsp/bsp.h"
#include "config/config_bus_uart.h"
#include "debugtools.h"


typedef enum _bus_instance_UART {
    BackchannelUART = 0,
    ApplicationUART = 1,
} bus_instance_UART;

typedef enum {
    Speed_9600,
    Speed_38400,
    Speed_115200,
} UARTSpeed;

hBus uartInit(bus_instance_UART instance, uint8_t echoenable, UARTSpeed speed);
void uartTransmit(hBus handle, uint8_t * buff, uint8_t szBuff);
void uartRegisterRxCallback(hBus handle, void (*rxcallback)(uint8_t rcvdbyte));

void uartSetEchoOn(hBus handle);
void uartSetEchoOff(hBus handle);

// Global error tallies (not per-bus)
uint8_t error_bus_not_initialized;

typedef struct {

    // NOTE:  keep some of these fields in sync with bus_status_UART_packet below
    uint8_t busnum;
    uint8_t initialized;
    bus_health health;
    uint8_t tx_in_use;
    uint8_t rx_in_use;

    uint16_t tx_bytes_sent;
    uint16_t rx_bytes_rcvd;

    uint8_t tx_error_count;
    uint8_t tx_error_overrun_count;
    uint8_t tx_error_underrun_count;
    uint8_t tx_overlapped_requests_fulfilled;  // not an error

    uint8_t rx_error_count;
    uint8_t rx_error_missinghandler_count;
    uint8_t txBuff[CONFIGM_uart_txbuffsize];
    uint8_t echo_on;

    // --------- tear line:  above is shared as a COSMOS packet -----

    volatile uint16_t currentTxIndex;
    uint16_t currentTxNumBytes;
    volatile uint16_t currentRxIndex;
    void (*rxCallback)(uint8_t);

} bus_context_UART;

// Non-buffer portion of context struct used to structure outgoing debug info to COSMOS
TLM_SEGMENT {
    BcTlmHeader header;   // All COSMOS TLM packets must have this

    uint8_t busnum;
    uint8_t initialized;
    uint8_t bushealth;

    uint16_t tx_bytes_sent;
    uint16_t rx_bytes_rcvd;

    uint8_t tx_error_count;
    uint8_t rx_error_count;

} bus_status_UART_packet;



// Definitions for the supported speeds for the UART bus
#define UCAxBRW_9600 52
#define UCAxMCTLW_9600 (UCOS16 | UCBRF_1 | 0x49)
#define UCAxBRW_38400 13
#define UCAxMCTLW_38400 (UCOS16 | UCBRF_0 | 0x84)
#define UCAxBRW_115200 4
#define UCAxMCTLW_115200 (UCOS16 | UCBRF_5 | 0x55)


#endif /* UART_H_ */
