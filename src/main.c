#include <msp430.h> 
#include <stdint.h>

// TODO:  Support '#define USCI_MODULE  B2' or similar
#include "i2c_support.h"
#include "magnetometer.h"

#define MAX_BUFF_SIZE   0x10

volatile uint8_t indexBuff;
uint8_t szBuff;
uint8_t readBuff[MAX_BUFF_SIZE];

int16_t magDataX, magDataY, magDataZ;

#define I2C_PORTSEL0        P7SEL0
#define I2C_PORTSEL1        P7SEL1
#define I2C_SDA_BIT         BIT0
#define I2C_SCL_BIT         BIT1


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // Configure GPIO for status LED
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch
    P1DIR |= BIT0;                          // For LED

    // Configure I2C pins - Goal:  '01' for both 7.0 (SDA) and 7.1 (SCL)
    // NOTE:  P7DIR not necessary, direction set by eUSCI module
    I2C_PORTSEL1 &= ~(I2C_SDA_BIT | I2C_SCL_BIT);
    I2C_PORTSEL0 |= I2C_SDA_BIT | I2C_SCL_BIT;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    i2cInit(MAG_I2C_7BIT_ADDRESS);

    // NOTE:  Order important, MUST clear software reset, THEN enable interrupts
    i2cReleaseReset();
    //UCB2IE |= I2C_MASTER_TRANSMIT_INTERRUPT_MASK;

    magInit();

    // Main loop - repeatedly write the buffer
    for (;;)
    {
        P1OUT ^= BIT0;
        __delay_cycles(5000);

        magReadBytesFromRegisters(MAG_DATA_OUTPUT_ADDRESS_START, readBuff, 6);

        // NOTE:  Order of X/Z/Y is, unfortunately, intentional ...
        magDataX = (int16_t)(readBuff[1] | ((int16_t)readBuff[0] << 8));
        magDataZ = (int16_t)(readBuff[3] | ((int16_t)readBuff[2] << 8));
        magDataY = (int16_t)(readBuff[5] | ((int16_t)readBuff[4] << 8));

        //__bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts

    }
}

