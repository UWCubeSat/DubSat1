#ifndef _INA219_CURRENT_SENSOR_H
#define _INA219_CURRENT_SENSOR_H

// TODO FIND ACTUAL ADDRESS
#define INA219_CURRENT_SENSOR_I2C_ADDRESS 0x80 // A1: GND, A0: GND

// register addresses
#define CUR_INA219_REG_ADDR_CONFIG  0x00
#define CUR_INA219_REG_ADDR_SHUNT_V 0x01
#define CUR_INA219_REG_ADDR_BUS_V   0x02
#define CUR_INA219_REG_ADDR_POWER   0x03
#define CUR_INA219_REG_ADDR_CURRENT 0x04
#define CUR_INA219_REG_ADDR_CALIB   0x05

// configuration register
#define CUR_INA219_REG_CONFIG_RESET_LSB 0x00
#define CUR_INA219_REG_CONFIG_RESET_MSB 0x80

#define CUR_INA219_REG_CONFIG_SADC_LSB 0x78
#define CUR_INA219_REG_CONFIG_SADC_MSB 0x00

#define CUR_INA219_REG_CONFIG_BADC_LSB 0x80
#define CUR_INA219_REG_CONFIG_BADC_MSB 0x07

#endif
