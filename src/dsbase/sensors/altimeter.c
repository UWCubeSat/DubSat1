/*
 * altimeter.c
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#include "core/timer.h"
#include <math.h>
#include <sensors/altimeter.h>

#define MAX_BUFF_SIZE 3
#define PROM_WORD_MAX_VALUE 65535
#define MAX_DIGITAL_VALUES 16777216

FILE_STATIC AltimeterData altData;
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hBarom;
FILE_STATIC int32_t tDiff;
FILE_STATIC int32_t tActual;
FILE_STATIC uint16_t prom[8];
FILE_STATIC int32_t lastAltitude;

/*
 * Register the sensor with the MSP430 on the i2c bus 'bus'.
 * bus_instance_i2c bus: MSP430 bus to initialize on
 */
void baromInit(bus_instance_i2c bus)
{
    i2cEnable(bus);
    hBarom = i2cInit(bus, MS5611_I2C_7BIT_ADDRESS);
    initializeTimer();
    reset();                                        // send reset sequence
    lastAltitude = 0;

    // read calibration coefficients from PROM (do something with CRC/vendor data?)
    int8_t reg;
    for (reg = 0; reg < 8; reg++) {
        prom[reg] = readPROMWord(reg);
    }
}

/*
 * Reads and returns a single 16-bit calibration coefficient from the sensor's PROM.
 * int reg: offset from start address
 */
uint16_t readPROMWord(int8_t reg)
{
    uint16_t word = 0;
    reg = reg * 2;
    uint8_t addr = MS5611_PROM_START_ADDR + reg;
    i2cMasterRegisterRead(hBarom, addr, i2cBuff, 2);
    word = (uint16_t)(i2cBuff[1]) | ((uint16_t)(i2cBuff[0]) << 8);   // MSB first
    return word;
}

/*
 * Send sensor reset command.
 */
void reset()
{
    i2cBuff[0] = MS5611_RESET;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    wait(3);
}

/*
 * Initiates a conversion sequence, either D1 or D2, with command 'cmd'.
 * uint8_t cmd: conversion sequence to send
 */
static void send_conversion(uint8_t cmd)
{
    i2cBuff[0] = MS5611_CONV + cmd;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    switch (cmd & 0x0F)                     // wait necessary amount of time
    {
        case MS5611_CONV_OSR256:
            wait(1);
            break;
        case MS5611_CONV_OSR512:
            wait(3);
            break;
        case MS5611_CONV_OSR1024:
            wait(4);
            break;
        case MS5611_CONV_OSR2048:
            wait(6);
            break;
        case MS5611_CONV_OSR4096:
            wait(10);
            break;
    }
    return;
}

/*
 * Sends a conversion sequence 'conv_cmd' with sensitivity 'osr'. The ADC registry
 * is then read and its value returned.
 * uint8_t conv_cmd: conversion sequence
 * uint8_t osr: sensitivity of conversion
 */
static float read_adc(uint8_t conv_cmd, uint8_t osr)
{
    send_conversion(conv_cmd + osr);
    i2cMasterRegisterRead(hBarom, MS5611_ADC_ADDR, i2cBuff, 3);
    uint32_t rec = ((uint32_t)(i2cBuff[2]) | ((uint32_t)(i2cBuff[1]) << 8)) | ((uint32_t)(i2cBuff[0]) << 16);

    int32_t ret;
    switch (conv_cmd)
    {
        case MS5611_D1_CONV:
        {
            int32_t tDiff2 = 0;
            int64_t off2 = 0;
            int64_t sens2 = 0;

            if (tActual < 2000)
            {
                tDiff2 = pow(tDiff, 2) / pow(2, 31);
                off2 = 5 * pow((tActual - 2000), 2) / 2;
                sens2 = 5 * pow((tActual - 2000), 2) / 4;
                if (tActual < -1500)
                {
                    off2 = off2 + 7 * pow((tActual + 1500), 2);
                    sens2 = sens2 + (11 * pow((tActual + 1500), 2) / 2);
                }
            }

            int64_t pOff = prom[C2] * pow(2, 17) + prom[C4] * tDiff / pow(2, 6);
            int64_t pSens = prom[C1] * pow(2, 16) + prom[C3] * tDiff / pow(2, 7);
            ret = (rec * pSens / pow(2, 21) - pOff) / pow(2, 16);
            break;
        }
        case MS5611_D2_CONV:
            tDiff = rec - prom[C5] * pow(2, 8);
            ret = 2000 + (tDiff * prom[C6]) / pow(2, 23);
            tActual = ret;
            break;
    }
    return (float)ret / 100;
}

float calculateAltitude()
{
    float staticPressure;
    float standardTemp;
    float lapseRate;
    float altitude;
    float layerHeight;

    if (lastAltitude < H_4 && lastAltitude >= H_3)
    {
        staticPressure = 868.02f;
        standardTemp = 228.65f;
        lapseRate = 0.0028f;
        layerHeight = H_3;
    }
    else if (lastAltitude < H_3 && lastAltitude >= H_2)
    {
        staticPressure = 5474.89f;
        standardTemp = 216.65f;
        lapseRate = 0.001f;
        layerHeight = H_2;
    }
    else if (lastAltitude < H_2 && lastAltitude >= H_1)
    {
        staticPressure = 22632.10f;
        standardTemp = 216.65f;
        lapseRate = 0.0f;
        layerHeight = H_1;
    }
    else if (lastAltitude < H_1 && lastAltitude >= 0)
    {
        staticPressure = 101325.00f;
        standardTemp = 288.15f;
        lapseRate = 0.0065f;
        layerHeight = 0;
    }

    float exponent =  (GAS_CONSTANT * lapseRate) / (G_0 * AIR_MOLAR_MASS);
    altitude = (standardTemp / lapseRate) * (1 - pow((altData.pressure / staticPressure), exponent)) + layerHeight;
    lastAltitude = altitude;
    return altitude;
}

AltimeterData *readAltitudeData(int8_t bits)
{
    AltimeterData *aData = &altData;
    aData->temperature = read_adc(MS5611_D2_CONV, MS5611_CONV_OSR4096);
    aData->pressure = read_adc(MS5611_D1_CONV, MS5611_CONV_OSR4096);
    aData->altitude = calculateAltitude();
    return aData;
}

/*
 * Waits 'ms' milliseconds using our timer library.
 * int ms: time to wait
 */
void wait(int ms)
{
	TIMER_HANDLE tHandle = timerPollInitializer(ms);
    while(!checkTimer(tHandle)) {}
    endPollingTimer(tHandle);
    return;
}
