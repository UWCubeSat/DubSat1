/*
 * MAG3110_Magnetometer.h
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#ifndef MAG3110_MAGNETOMETER_H_
#define MAG3110_MAGNETOMETER_H_

// Core definitions for chip
#define MAG_MAG3110_I2C_7BIT_ADDRESS     0x0E    // Fixed in hardware for this chip

// Register addresses (NOTE:  listed in decimal in datasheet)
#define MAG_MAG3110_REG_ADDR_DR_STATUS          0x00
#define MAG_MAG3110_REG_ADDR_OUT_X_MSB          0x01
#define MAG_MAG3110_REG_ADDR_OUT_X_LSB          0x02
#define MAG_MAG3110_REG_ADDR_OUT_Y_MSB          0x03
#define MAG_MAG3110_REG_ADDR_OUT_Y_LSB          0x04
#define MAG_MAG3110_REG_ADDR_OUT_Z_MSB          0x05
#define MAG_MAG3110_REG_ADDR_OUT_Z_LSB          0x06
#define MAG_MAG3110_REG_ADDR_WHO_AM_I           0x07
#define MAG_MAG3110_REG_ADDR_SYSMOD             0x08
#define MAG_MAG3110_REG_ADDR_OFF_X_MSB          0x09
#define MAG_MAG3110_REG_ADDR_OFF_X_LSB          0x0A
#define MAG_MAG3110_REG_ADDR_OFF_Y_MSB          0x0B
#define MAG_MAG3110_REG_ADDR_OFF_Y_LSB          0x0C
#define MAG_MAG3110_REG_ADDR_OFF_Z_MSB          0x0D
#define MAG_MAG3110_REG_ADDR_OFF_Z_LSB          0x0E
#define MAG_MAG3110_REG_ADDR_DIE_TEMP           0x0F
#define MAG_MAG3110_REG_ADDR_CTRL_REG1          0x10
#define MAG_MAG3110_REG_ADDR_CTRL_REG2          0x11

#define MAG_MAG3110_XYZ_OUTPUT_REG_ADDR_START   MAG_MAG3110_REG_ADDR_OUT_X_MSB

// DR_STATUS register flags
// Note:  XDR and YDR flags are mis-labeled in latest datasheet
#define MAG_MAG3110_DR_STATUS_XDR               0x01    // Indicates axis has new data
#define MAG_MAG3110_DR_STATUS_YDR               0x02    // Indicates axis has new data
#define MAG_MAG3110_DR_STATUS_ZDR               0x04    // Indicates axis has new data
#define MAG_MAG3110_DR_STATUS_ZYXDR             0x08    // One of the axes has new data
#define MAG_MAG3110_DR_STATUS_XOW               0x10    // Data was overwritten on axis
#define MAG_MAG3110_DR_STATUS_YOW               0x20    // Data was overwritten on axis
#define MAG_MAG3110_DR_STATUS_ZOW               0x40    // Data was overwritten on axis
#define MAG_MAG3110_DR_STATUS_ZYXOW             0x80    // Data was overwritten an axis

#define MAG_MAG3110_DR_STATUS_DATA_READY        0x01  // Both are 1 if true
#define MAG_MAG3110_DR_STATUS_DATA_OVERWRITTEN  0x01  // Both are 1 if true

// SYSMOD register - INDICATES the current mode. Use CTRL_REGx to CHANGE modes
#define MAG_MAG3110_SYSMOD_STANDY_MODE              0x00
#define MAG_MAG3110_SYSMOD_ACTIVE_RAW_MODE          0x01  // User-corrections NOT applied
#define MAG_MAG3110_SYSMOD_ACTIVE_CORRECTED_MODE    0x02  // User-corrections ARE applied

// Control register:  CTRL_REG1
#define MAG_MAG3110_CTRL_REG1_MODE_SELECT_ACTIVE    0x01
#define MAG_MAG3110_CTRL_REG1_MODE_SELECT_STANDBY   0x00  // Default
#define MAG_MAG3110_CTRL_REG1_TRIGGER_MODE          0x02  // "Trigger" means SINGLE SAMPLE
#define MAG_MAG3110_CTRL_REG1_AUTO_MODE             0x00  // Default - CONTINUOUSLY SAMPLE
#define MAG_MAG3110_CTRL_REG1_FAST_READ_ON          0x04  // Fast read = XYZ MSBs only
#define MAG_MAG3110_CTRL_REG1_FAST_READ_OFF         0x00  // Default - Read all 6 XYZ bytes

#define MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_16   0x00
#define MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_32   0x08
#define MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_64   0x10
#define MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_128  0x18

// This sets all three DR flags to 0, yielding fastest four output rates
#define MAG_MAG3110_CTRL_REG1_DATA_RATE_MAX             0x00
#define MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_80HZ     (MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_16 | MAG_MAG3110_CTRL_REG1_DATA_RATE_MAX)
#define MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_40HZ     (MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_32 | MAG_MAG3110_CTRL_REG1_DATA_RATE_MAX)
#define MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_20HZ     (MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_64 | MAG_MAG3110_CTRL_REG1_DATA_RATE_MAX)
#define MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_10HZ     (MAG_MAG3110_CTRL_REG1_OVERSAMPLE_RATIO_128 | MAG_MAG3110_CTRL_REG1_DATA_RATE_MAX)


// Control register:  CTRL_REG2
#define MAG_MAG3110_CTRL_REG2_AUTO_MAG_SENSOR_RESET     0x80  // For continuous
#define MAG_MAG3110_CTRL_REG2_RAW_MODE                  0x20  // Raw = no user offsets applied
#define MAG_MAG3110_CTRL_REG2_SINGLE_MAG_SENSOR_RESET   0x10  // For triggered measurements


// Device-specific conversion factors
// NOTE:  On the MAG3110, the "full-scale range" is +/- 1000 uT = 10,000 LSBs
// BUT ... total output can be up to +/- 3000 uT = 30,000 LSBs due to factory calibration settings
// and user-stored corrections in the *_OFF_* offset registers
#define MAG3110_CONVERSION_FACTOR_RAW_TO_NANOTESLAS     100.0
#define MAG3110_CONVERSION_FACTOR_RAW_TO_TESLAS         0.0000001


#endif /* MAG3110_MAGNETOMETER_H_ */
