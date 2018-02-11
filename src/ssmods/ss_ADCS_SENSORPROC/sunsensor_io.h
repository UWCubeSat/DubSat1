/*
 * sunsensor_io.h
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#ifndef SUNSENSOR_IO_H_
#define SUNSENSOR_IO_H_

// sun sensor i2c bus
#define I2C_BUS_SUNSENSOR I2CBus1

void sunsensorioInit();
void sunsensorioUpdate();
void sunsensorioSendData();

#endif /* SUNSENSOR_IO_H_ */
