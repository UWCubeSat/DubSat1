/*
 * config_bus_i2c.h
 *
 *  Created on: Oct 21, 2017
 *      Author: SatLab
 */

#ifndef DSBASE_CONFIG_CONFIG_BUS_I2C_H_
#define DSBASE_CONFIG_CONFIG_BUS_I2C_H_

#include "config.h"

// CONFIGM... configuration values are defined as MACROS
// Use these for values that will be needed in other initializers (best example:  buffer sizes)
#define CONFIGM_i2c_maxperipheralinstances   3     // KEEP at 3, for ... stupid reasons, please.
#define CONFIGM_i2c_maxdevices               8

#endif /* DSBASE_CONFIG_CONFIG_BUS_I2C_H_ */
