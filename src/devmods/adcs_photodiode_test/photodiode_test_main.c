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
const uint8_t LTC2481_I2C_ADDR1 = 0x26;
const uint8_t LTC2481_I2C_ADDR2 = 0x24;

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

    const uint8_t handle0 = photodiodeInit(LTC2481_I2C_ADDR1);
    const uint8_t handle1 = photodiodeInit(LTC2481_I2C_ADDR2);

    for (;;)
    {
        volatile double adcVoltage0, adcVoltage1;
        volatile unsigned int i;

        adcVoltage0 = photodiodeVoltage(handle0);

        i = 50000;
        while (i > 0)
        {
            i--;
        }

        adcVoltage1 = photodiodeVoltage(handle1);
        i = 50000;
        while (i > 0)
        {
                i--;
        }


        P1OUT ^= BIT0;

    }
}
