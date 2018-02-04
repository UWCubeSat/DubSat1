#include <msp430.h>
#include <stdint.h>
#include "sensors/photodiode.h"
#include "bsp/bsp.h"

#define I2C_PORTSEL0        P7SEL0
#define I2C_PORTSEL1        P7SEL1
#define I2C_SDA_BIT         BIT0
#define I2C_SCL_BIT         BIT1
#define LED_PORT            P1OUT
//#define LED_PORT_DIR        P1DIR
#define LED_PIN             BIT0

//both addr lines wired to vcc
const uint8_t LH_addr = 0x14;
const uint8_t LF_addr = 0x15;
const uint8_t FH_addr = 0x17;
const uint8_t FF_addr = 0x24;
const uint8_t HH_addr = 0x26;
const uint8_t HF_addr = 0x27;

int main(void)
{
    bspInit(Module_Test);
    WDTCTL = WDTPW | WDTHOLD;

    // Configure GPIO for status LED
    LED_PORT &= ~LED_PIN;                               // Clear P1.0 output latch
    LED_PORT_DIR |= LED_PIN;                            // For LED

    // Configure I2C pins - Goal:  '01' for both 7.0 (SDA) and 7.1 (SCL)
    // NOTE:  P7DIR not necessary, direction set by eUSCI module
    I2C_PORTSEL1 &= ~(I2C_SDA_BIT | I2C_SCL_BIT);
    I2C_PORTSEL0 |= I2C_SDA_BIT | I2C_SCL_BIT;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    const uint8_t handleCenter = photodiodeInit(HH_addr, I2CBus2);
    const uint8_t handleRight = photodiodeInit(FF_addr, I2CBus2);
    const uint8_t handleLeft = photodiodeInit(HF_addr, I2CBus2);
    for (;;)
    {
        volatile uint32_t adcVoltageCenter, adcVoltageRight, adcVoltageLeft;
        //need to have a debug breakpoint here
        adcVoltageCenter = photodiodeVoltage(handleCenter, GAIN1);
        adcVoltageRight = photodiodeVoltage(handleRight, GAIN1);
        adcVoltageLeft = photodiodeVoltage(handleLeft, GAIN1);
        P1OUT ^= BIT0;

    }
}
