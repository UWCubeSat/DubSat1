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
#include "bsp/bsp.h"
#include "config/config_bus_uart.h"

typedef enum _bus_instance_UART {
    BackchannelUART = 0,
    ApplicationUART = 1,
} bus_instance_UART;

hBus uartInit(bus_instance_UART instance);
void uartTransmit(hBus handle, uint8_t * buff, uint8_t szBuff);
void uartRegisterRxCallback(hBus handle, void (*rxcallback)(uint8_t rcvdbyte));

// Global error tallies (not per-bus)
uint8_t error_bus_not_initialized;

typedef struct {
    uint8_t initialized;
    uint8_t echo_on;

    uint8_t tx_in_use;
    uint8_t rx_in_use;

    uint16_t tx_bytes_sent;
    uint8_t tx_error_count;
    uint8_t tx_error_overrun_count;
    uint8_t tx_error_underrun_count;
    uint8_t tx_error_buffer_overflow_count;

    uint16_t rx_bytes_rcvd;
    uint8_t rx_error_count;
    uint8_t rx_error_missinghandler_count;

    uint8_t txBuff[CONFIGM_uart_txbuffsize];
    volatile uint8_t currentTxIndex;
    uint8_t currentTxNumBytes;
    volatile uint8_t currentRxIndex;
    void (*rxCallback)(uint8_t);

} bus_context_UART;


#endif /* UART_H_ */
