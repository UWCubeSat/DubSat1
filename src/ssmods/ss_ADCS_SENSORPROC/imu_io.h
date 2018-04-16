/*
 * imu_io.h
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#ifndef IMU_IO_H_
#define IMU_IO_H_

#define DELAY_IMU_UPDATE_MS 200

// TODO confirm this bus after descope board redesign
#define IMU_I2CBUS I2CBus1

#include <stdint.h>

#include "core/debugtools.h"

// telemetry segment for raw imu gryo accel. data
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint16_t x;
    uint16_t y;
    uint16_t z;
} imu_segment;

void imuioInit();
void imuioUpdate();
void imuioSendBackchannel();
void imuioSendCAN();

#endif /* IMU_IO_H_ */
