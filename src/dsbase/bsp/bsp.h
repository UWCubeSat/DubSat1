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

// Various helper functions
void bspInit(SubsystemModule mod);
//void bspUARTInit(bus_instance_UART instance);
SubsystemModule bspGetModule();

// Hard-wired assignments for a given board are stashed in these #if defined(...)
// blocks
#if defined(__BSP_Board_MSP430FR5994LaunchPad__)
#include <msp430.h>

// LaunchPad-specific pins for built-in LEDs
#define LP5994_LED_PORT_DIR    P1DIR
#define LP5994_LED_PORT_OUT    P1OUT
#define LP5994_LED0_BIT        BIT0
#define LP5994_LED1_BIT        BIT1
#define LP5994_LED_BITS        (LP5994_LED0_BIT | LP5994_LED1_BIT)

#define LP5994_A1_UART_SEL0     P2SEL0
#define LP5994_A1_UART_SEL1     P2SEL1
#define LP5994_A1_UART_BITS     (BIT5 | BIT6)

#define LP5994_BACKCHANNEL_UART_SEL0    P2SEL0  // Maps to USCI A0
#define LP5994_BACKCHANNEL_UART_SEL1    P2SEL1  // Maps to USCI A0
#define LP5994_BACKCHANNEL_UART_BITS    (BIT0 | BIT1)

// NOTE:  I2C1 (mapping to P5.0/5.1) is not accessible on LaunchPad
//#define LP5994_I2C1_PORTSEL0        P5SEL0
//#define LP5994_I2C1_PORTSEL1        P5SEL1
//#define LP5994_I2C1_SDA_BIT         BIT0
//#define LP5994_I2C1_SCL_BIT         BIT1

#define LP5994_I2C2_PORTSEL0        P7SEL0
#define LP5994_I2C2_PORTSEL1        P7SEL1
#define LP5994_I2C2_SDA_BIT         BIT0
#define LP5994_I2C2_SCL_BIT         BIT1

// Generic assignments
#define LED_PORT_DIR        LP5994_LED_PORT_DIR
#define LED_PORT_OUT        LP5994_LED_PORT_OUT
#define LED0_BIT            LP5994_LED0_BIT
#define LED1_BIT            LP5994_LED1_BIT
#define LED_BITS            LP5994_LED_BITS

// TX=P2.5, RX=P2.6
#define APP_UART_SEL0   LP5994_A1_UART_SEL0
#define APP_UART_SEL1   LP5994_A1_UART_SEL1
#define APP_UART_BITS   LP5994_A1_UART_BITS

// TX=P2.0, RX=P2.1 - Exposed on LaunchPad in jumper J101 TXD and RXD
#define BACKCHANNEL_UART_SEL0   LP5994_BACKCHANNEL_UART_SEL0
#define BACKCHANNEL_UART_SEL1   LP5994_BACKCHANNEL_UART_SEL1
#define BACKCHANNEL_UART_BITS   LP5994_BACKCHANNEL_UART_BITS

// NOTE:  I2C1 (mapping to P5.0/5.1) is not accessible on LaunchPad
//#define I2C1_PORTSEL0        LP5994_I2C1_PORTSEL0
//#define I2C1_PORTSEL1        LP5994_I2C1_PORTSEL1
//#define I2C1_SDA_BIT         LP5994_I2C1_SDA_BIT
//#define I2C1_SCL_BIT         LP5994_I2C1_SCL_BIT

#define I2C2_PORTSEL0        LP5994_I2C2_PORTSEL0
#define I2C2_PORTSEL1        LP5994_I2C2_PORTSEL1
#define I2C2_SDA_BIT         LP5994_I2C2_SDA_BIT
#define I2C2_SCL_BIT         LP5994_I2C2_SCL_BIT

#elif defined(__BSP_Board_MSP430FR5994_CANMSPBlockv24__)

#define CANMSP_BLOCKV24_LED_PORT_DIR    PJDIR
#define CANMSP_BLOCKV24_LED_PORT_OUT    PJOUT
#define CANMSP_BLOCKV24_LED0_BIT        BIT0
#define CANMSP_BLOCKV24_LED1_BIT        BIT1
#define CANMSP_BLOCKV24_LED2_BIT        BIT2
#define CANMSP_BLOCKV24_LED_BITS        (CANMSP_BLOCKV24_LED0_BIT | CANMSP_BLOCKV24_LED1_BIT | CANMSP_BLOCKV24_LED2_BIT)

#define CANMSP_BLOCKV24_2ND_UART_SEL0     P2SEL0
#define CANMSP_BLOCKV24_2ND_UART_SEL1     P2SEL1
#define CANMSP_BLOCKV24_2ND_UART_BITS     (BIT5 | BIT6)

#define CANMSP_BLOCKV24_BACKCHANNEL_UART_SEL0    P2SEL0  // Maps to USCI A0
#define CANMSP_BLOCKV24_BACKCHANNEL_UART_SEL1    P2SEL1  // Maps to USCI A0
#define CANMSP_BLOCKV24_BACKCHANNEL_UART_BITS    (BIT0 | BIT1)

#define CANMSP_BLOCKV24_I2C1_PORTSEL0        P5SEL0
#define CANMSP_BLOCKV24_I2C1_PORTSEL1        P5SEL1
#define CANMSP_BLOCKV24_I2C1_SDA_BIT         BIT0
#define CANMSP_BLOCKV24_I2C1_SCL_BIT         BIT1

#define CANMSP_BLOCKV24_I2C2_PORTSEL0        P7SEL0
#define CANMSP_BLOCKV24_I2C2_PORTSEL1        P7SEL1
#define CANMSP_BLOCKV24_I2C2_SDA_BIT         BIT0
#define CANMSP_BLOCKV24_I2C2_SCL_BIT         BIT1

// Generic assignments
#define LED_PORT_DIR        CANMSP_BLOCKV24_LED_PORT_DIR
#define LED_PORT_OUT        CANMSP_BLOCKV24_LED_PORT_OUT
#define LED0_BIT            CANMSP_BLOCKV24_LED0_BIT
#define LED1_BIT            CANMSP_BLOCKV24_LED1_BIT
#define LED2_BIT            CANMSP_BLOCKV24_LED2_BIT
#define LED_BITS            CANMSP_BLOCKV24_LED_BITS

// TX=P2.5, RX=P2.6
#define APP_UART_SEL0   CANMSP_BLOCKV24_2ND_UART_SEL0
#define APP_UART_SEL1   CANMSP_BLOCKV24_2ND_UART_SEL1
#define APP_UART_BITS   CANMSP_BLOCKV24_2ND_UART_BITS

// TX=P2.0, RX=P2.1 - Exposed on LaunchPad in jumper J101 TXD and RXD
#define BACKCHANNEL_UART_SEL0   CANMSP_BLOCKV24_BACKCHANNEL_UART_SEL0
#define BACKCHANNEL_UART_SEL1   CANMSP_BLOCKV24_BACKCHANNEL_UART_SEL1
#define BACKCHANNEL_UART_BITS   CANMSP_BLOCKV24_BACKCHANNEL_UART_BITS

#define I2C1_PORTSEL0        CANMSP_BLOCKV24_I2C1_PORTSEL0
#define I2C1_PORTSEL1        CANMSP_BLOCKV24_I2C1_PORTSEL1
#define I2C1_SDA_BIT         CANMSP_BLOCKV24_I2C1_SDA_BIT
#define I2C1_SCL_BIT         CANMSP_BLOCKV24_I2C1_SCL_BIT

#define I2C2_PORTSEL0        CANMSP_BLOCKV24_I2C2_PORTSEL0
#define I2C2_PORTSEL1        CANMSP_BLOCKV24_I2C2_PORTSEL1
#define I2C2_SDA_BIT         CANMSP_BLOCKV24_I2C2_SDA_BIT
#define I2C2_SCL_BIT         CANMSP_BLOCKV24_I2C2_SCL_BIT

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



#endif /* BSP_BSP_H_ */
