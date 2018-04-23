#include <msp430.h> 

/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "core/debugtools.h"
#include "bsp/bsp.h"
#include "sensors/imu.h"

char buff[64] = "Hello: %d";
char spew[] = "#: %d\r\n";

#define I2C_PORTSEL0        P7SEL0
#define I2C_PORTSEL1        P7SEL1
#define I2C_SDA_BIT         BIT0
#define I2C_SCL_BIT         BIT1

IMUData *latestIMUData;

int main(void)
{
    bspInit(Module_ADCS_BDot);

    I2C_PORTSEL1 &= ~(I2C_SDA_BIT | I2C_SCL_BIT);
    I2C_PORTSEL0 |= I2C_SDA_BIT | I2C_SCL_BIT;
    imuInit();

    __bis_SR_register(GIE);     // Enter LPM3, interrupts enabled

    int i = 0;
    for(;;)
    {
        latestIMUData = imuReadGyroAccelData();
        debugPrintF("%d,%d,%d\r\n", latestIMUData->rawGyroX, latestIMUData->rawGyroY, latestIMUData->rawGyroZ);

        _delay_cycles(500000);
    }

    __bis_SR_register(GIE); // Enter LPM0 w/ interrupts


}
