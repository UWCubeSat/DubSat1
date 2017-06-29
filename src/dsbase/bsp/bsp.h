/*
 * bsp.h
 *
 *  Created on: May 5, 2017
 *      Author: jeffc
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include "../core/i2c.h"
#include "../core/debugtools.h"
#include "../core/uart.h"
#include "../core/utils.h"
#include "../interfaces/systeminfo.h"

// Hard-wired assignments for a given board are stashed in these #if defined(...)
// blocks
#if defined(__BSP_Board_MSP430FR5994LaunchPad__)
#include <msp430.h>

// LaunchPad-specific pins for built-in LEDs
#define LP5994_LED_PORT_DIR    P1DIR
#define LP5994_LED_PORT_OUT    P1OUT
#define LP5994_LED1_BIT        BIT0
#define LP5994_LED2_BIT        BIT1
#define LP5994_LED_BITS        (LED1_BIT | LED2_BIT)

// Generic assignments
#define LED_PORT_DIR        LP5994_LED_PORT_DIR
#define LED_PORT_OUT        LP5994_LED_PORT_OUT
#define LED1_BIT            LP5994_LED1_BIT
#define LED2_BIT            LP5994_LED2_BIT
#define LED_BITS            LP5994_LED_BITS

#define LP5994_BACKCHANNEL_UART_SEL0    P2SEL0  // Maps to USCI A0
#define LP5994_BACKCHANNEL_UART_SEL1    P2SEL1  // Maps to USCI A0
#define LP5994_BACKCHANNEL_UART_BITS    (BIT0 | BIT1)

#define BACKCHANNEL_UART_SEL0   LP5994_BACKCHANNEL_UART_SEL0
#define BACKCHANNEL_UART_SEL1   LP5994_BACKCHANNEL_UART_SEL1
#define BACKCHANNEL_UART_BITS   LP5994_BACKCHANNEL_UART_BITS

#elif defined(__BSP_Board_MSP430FR5994_CANMSPBlockv1__)

#include <msp430.h>

// LaunchPad-specific pins for built-in LEDs
#define CANMSP_BLOCKV1_LED_PORT_DIR    P1DIR
#define CANMSP_BLOCKV1_LED_PORT_OUT    P1OUT
#define CANMSP_BLOCKV1_LED1_BIT        BIT0
#define CANMSP_BLOCKV1_LED2_BIT        BIT1
#define CANMSP_BLOCKV1_LED_BITS        (LED1_BIT | LED2_BIT)

// Generic assignments
#define LED_PORT_DIR        CANMSP_BLOCKV1_LED_PORT_DIR
#define LED_PORT_OUT        CANMSP_BLOCKV1_LED_PORT_OUT
#define LED1_BIT            CANMSP_BLOCKV1_LED1_BIT
#define LED2_BIT            CANMSP_BLOCKV1_LED2_BIT
#define LED_BITS            CANMSP_BLOCKV1_LED_BITS

#define CANMSP_BLOCKV1_BACKCHANNEL_UART_SEL0    P2SEL0  // Maps to USCI A0
#define CANMSP_BLOCKV1_BACKCHANNEL_UART_SEL1    P2SEL1  // Maps to USCI A0
#define CANMSP_BLOCKV1_BACKCHANNEL_UART_BITS    (BIT0 | BIT1)

#define BACKCHANNEL_UART_SEL0   CANMSP_BLOCKV1_BACKCHANNEL_UART_SEL0
#define BACKCHANNEL_UART_SEL1   CANMSP_BLOCKV1_BACKCHANNEL_UART_SEL1
#define BACKCHANNEL_UART_BITS   CANMSP_BLOCKV1_BACKCHANNEL_UART_BITS

#else
#error Unspecified board hardware, unable to determine correct BSP implementation.  Please specify board.
#endif

// Hardware assignments that will (hopefully) NOT vary between boards goes below this point ...
// If that changes, they get "bumped" back into the #if defined(...) blocks above
// Currently valid for following boards:
//     MSP4305994 LaunchPad
//     CAN-MSP Block v1
#define I2C_PORT_SEL0       P7SEL0
#define I2C_PORT_SEL1       P7SEL1
#define I2C_SDA_BIT         BIT0
#define I2C_SCL_BIT         BIT1

void bspInit(SubsystemModule mod);
SubsystemModule bspGetModule();

#endif /* BSP_BSP_H_ */
