/*
 * config_bus_uart.h
 *
 *  Created on: Jun 17, 2017
 *      Author: jeffc
 */
#ifndef _CONFIG_BUS_UART_H_
#define _CONFIG_BUS_UART_H_

#include "config.h"

// CONFIGM... configuration values are defined as MACROS
// Use these for values that will be needed in other initializers (best example:  buffer sizes)
#define CONFIGM_uart_txbuffsize  512
#define CONFIGM_uart_rxbuffsize  512
#define CONFIGM_uart_maxperipheralinstances   2

// CONFIGRO... configuration values are similarly READONLY (in the sense of unchanging, not C sense)
// These are variables, but enforced with const when actually defined in the source file
extern const uint8_t CONFIGRO_uart_example1_read_only;
extern const uint8_t CONFIGRO_uart_example2_read_only;

// CONFIGRW... configuration values
extern uint8_t CONFIGRW_uart_example3_configurable_at_runtime;

#endif /* _CONFIG_BUS_UART_H_ */
