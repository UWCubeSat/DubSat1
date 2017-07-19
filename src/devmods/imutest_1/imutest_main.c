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

    // Configure I2C pins - Goal:  '01' for both 7.0 (SDA) and 7.1 (SCL)
    // NOTE:  P7DIR not necessary, direction set by eUSCI module
    I2C_PORTSEL1 &= ~(I2C_SDA_BIT | I2C_SCL_BIT);
    I2C_PORTSEL0 |= I2C_SDA_BIT | I2C_SCL_BIT;
    imuInit();

    __bis_SR_register(GIE);     // Enter LPM3, interrupts enabled

    int i = 0;
    for(;;)
    {
        //debugTraceF(1, prompt, i);
        latestIMUData = imuReadGyroAccelData();
        debugTraceF(1, "%d,%d,%d,%d,%d,%d\r\n", latestIMUData->rawGyroX, latestIMUData->rawGyroY, latestIMUData->rawGyroZ,
                                                                latestIMUData->rawAccelX, latestIMUData->rawAccelY, latestIMUData->rawAccelZ);
        i++;

        _delay_cycles(500000);
        //debugTraceF(3, "This is trace level 3.\r\n");
    }

    //__bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts

    __no_operation();
}


