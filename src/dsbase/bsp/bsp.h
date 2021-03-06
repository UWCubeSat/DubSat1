/*
 * bsp.h
 *
 *  Created on: May 5, 2017
 *      Author: jeffc
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include <stdint.h>

#include "../core/debugtools.h"
#include "../core/uart.h"
#include "../core/utils.h"
#include "../interfaces/systeminfo.h"
#include "../core/i2c.h"

extern uint32_t local_reset_count;

// LaunchPad HW keys
#define HWKEY_LP430_A   0x77BEBF44297C215E  // TODO:  get real one
#define HWKEY_LP430_B   0x77BEBF44297C215E  // TODO:  get real one
#define HWKEY_LP430_C   0x002C00432BCB6749

// Engineering board HW keys
#define HWKEY_EPS_DIST_ENG_V3_A  0x001500052BCB6748
#define HWKEY_EPS_DIST_ENG_V3_B  0x0014000F2BCB6748

// Flight board HW keys
// NONE YET

#if defined(__SS_EPS_DIST__)
#define __SUBSYSTEM_MODULE__  Module_EPS_Dist
#define NUM_HWKEYS  2
FILE_STATIC uint64_t hw_keys[] = { HWKEY_EPS_DIST_ENG_V3_A, HWKEY_EPS_DIST_ENG_V3_B };

#elif defined(__SS_EPS_GEN__)
#define __SUBSYSTEM_MODULE__  Module_EPS_Gen
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_EPS_BATT__)
#define __SUBSYSTEM_MODULE__  Module_EPS_Batt
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_PPT__)
#define __SUBSYSTEM_MODULE__  Module_PPT
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_SENSORPROC__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_SensorProc
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_BDOT__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_BDot
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_ESTIM__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_Estim
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_MPC__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_MPC
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_MTQ__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_MTQ
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_RWX__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_RWX
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_RWY__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_RWY
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_ADCS_RWZ__)
#define __SUBSYSTEM_MODULE__  Module_ADCS_RWZ
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#elif defined(__SS_TEST__)
#define __SUBSYSTEM_MODULE__  Module_Test
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#else
#warning No specific module specified via __SS_<subsystemmodule>__ macro at build time, defaulting to Module_Test.
#define __SUBSYSTEM_MODULE__  Module_Test
#define NUM_HWKEYS  0
FILE_STATIC uint64_t hw_keys[] = { };

#endif

typedef enum {
    HWSW_LockNotEnabled,
    HWSW_NoKeysProvided,
    HWSW_LockViolation,
    HWSW_Matched,
} hwsw_match_state;

// Various helper functions
void bspInit(SubsystemModule mod);
SubsystemModule bspGetModule();
hwsw_match_state bspGetHWSWMatchState();
uint64_t bspGetChipID();
uint32_t bspGetResetCount();
uint16_t bspGetResetReason();
void bspClearResetCount();
void bspBackpowerPulldown();

// Hard-wired assignments for a given board are stashed in these #if defined(...)
// blocks
#if defined(__BSP_Board_MSP430FR5994LaunchPad__) || defined(__BSP_Board_SS__)
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
// These map to eUSCI B1
#define LP5994_I2C1_PORTSEL0        P5SEL0
#define LP5994_I2C1_PORTSEL1        P5SEL1
#define LP5994_I2C1_SDA_BIT         BIT0
#define LP5994_I2C1_SCL_BIT         BIT1

// These map to eUSCI B2
#define LP5994_I2C2_PORTSEL0        P7SEL0
#define LP5994_I2C2_PORTSEL1        P7SEL1
#define LP5994_I2C2_SDA_BIT         BIT0
#define LP5994_I2C2_SCL_BIT         BIT1

// Generic assignments

// NOTE:  these LED outputs have been confirmed to only need
// PJDIR set correctly;  SEL0/1 default to 0, which
// is the correct setting for GPIO on these pins.
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

#if !defined(__BSP_Board_MSP430FR5994LaunchPad__)
// NOTE:  I2C1 (mapping to P5.0/5.1) is not accessible on LaunchPad
#define I2C1_PORTSEL0        LP5994_I2C1_PORTSEL0
#define I2C1_PORTSEL1        LP5994_I2C1_PORTSEL1
#define I2C1_SDA_BIT         LP5994_I2C1_SDA_BIT
#define I2C1_SCL_BIT         LP5994_I2C1_SCL_BIT
#endif

#define I2C2_PORTSEL0        LP5994_I2C2_PORTSEL0
#define I2C2_PORTSEL1        LP5994_I2C2_PORTSEL1
#define I2C2_SDA_BIT         LP5994_I2C2_SDA_BIT
#define I2C2_SCL_BIT         LP5994_I2C2_SCL_BIT

#elif defined(__BSP_Board_MSP430FR5994_CANMSPBlockv24__)

// NOTE:  these LED outputs have been confirmed to only need
// PJDIR set correctly;  SEL0/1 and CEPDx all default to 0, which
// is the correct setting for GPIO on these pins.
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

// These map to eUSCI B1
#define CANMSP_BLOCKV24_I2C1_PORTSEL0        P5SEL0
#define CANMSP_BLOCKV24_I2C1_PORTSEL1        P5SEL1
#define CANMSP_BLOCKV24_I2C1_SDA_BIT         BIT0
#define CANMSP_BLOCKV24_I2C1_SCL_BIT         BIT1

// These map to eUSCI B2
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


#endif /* BSP_BSP_H_ */
