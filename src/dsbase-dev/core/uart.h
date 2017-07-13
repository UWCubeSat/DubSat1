/*
 * uart.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "bsp/bsp.h"
#include "config/config_bus_uart.h"

void uartInit();
void uartTransmit(uint8_t * buff, uint8_t szBuff);
void uartRegisterRxCallback(void (*rxcallback)(uint8_t rcvdbyte));

typedef struct {
    uint8_t initialized;
    uint8_t echo_on;

    uint8_t tx_in_use;
    uint8_t rx_in_use;

    uint16_t tx_bytes_sent;
    uint8_t tx_error_count;
    uint8_t tx_error_underrun_count;
    uint8_t tx_error_buffer_overflow;
    uint8_t tx_overlapped_requests_fulfilled;   // NOTE:  NOT an error

    uint16_t rx_bytes_rcvd;
    uint8_t rx_error_count;
    uint8_t rx_error_missinghandler_count;
} bus_status_UART;


#endif /* UART_H_ */
