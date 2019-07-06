/*
 * MS5611_barometer.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#ifndef DSBASE_SENSORS_MS5611_BAROMETER_H_
#define DSBASE_SENSORS_MS5611_BAROMETER_H_

// I2C 7 bit address - complement of CSB pin (tied LOW - 1, tied HIGH - 0)
#define MS5611_I2C_7BIT_ADDRESS             0x77

// Reset Sequence
#define MS5611_RESET                        0x1E

// PROM Register Start
#define MS5611_PROM_START_ADDR              0xA0

#define MS5611_CONV                         0x40

#define MS5611_D1_CONV                      0x00
#define MS5611_D2_CONV                      0x10

#define MS5611_CONV_OSR256                  0x00
#define MS5611_CONV_OSR512                  0x02
#define MS5611_CONV_OSR1024                 0x04
#define MS5611_CONV_OSR2048                 0x06
#define MS5611_CONV_OSR4096                 0x08

typedef enum {
    Vendor  = 0,
    C1      = 1,
    C2      = 2,
    C3      = 3,
    C4      = 4,
    C5      = 5,
    C6      = 6,
    CRC     = 7
} PROMData;

// ADC registry address
#define MS5611_ADC_ADDR                 0x00    // After conversion sequence has ended

#endif /* DSBASE_SENSORS_MS5611_BAROMETER_H_ */
