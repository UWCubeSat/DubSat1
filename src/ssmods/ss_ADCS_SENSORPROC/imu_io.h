/*
 * imu_io.h
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#ifndef IMU_IO_H_
#define IMU_IO_H_

// TODO confirm this bus after descope board redesign
#define IMU_I2CBUS I2CBus1

#include <stdint.h>

#include "core/debugtools.h"
#include "interfaces/canwrap.h"

// telemetry segment for raw imu gryo accel. data
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    int16_t x;
    int16_t y;
    int16_t z;
} imu_segment;

void imuioInit();
void imuioUpdate();
void imuioSendBackchannel();
void imuioSendCAN();

// rollcall
void imuioRcPopulate2(rc_adcs_sp_2 *rc);
void imuioRcPopulate3(rc_adcs_sp_3 *rc);
void imuioRcPopulate4(rc_adcs_sp_4 *rc);
void imuioRcPopulate15(rc_adcs_sp_15 *rc);
void imuioRcPopulate16(rc_adcs_sp_16 *rc);
void imuioRcPopulate17(rc_adcs_sp_17 *rc);

#endif /* IMU_IO_H_ */
