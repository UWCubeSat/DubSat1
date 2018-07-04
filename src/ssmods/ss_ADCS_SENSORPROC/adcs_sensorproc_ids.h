/*
 * adcs_sensorproc_ids.h
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 *
 * IDs to interface with COSMOS backchannel
 */

#ifndef ADCS_SENSORPROC_IDS_H_
#define ADCS_SENSORPROC_IDS_H_

#define ENABLE_SUNSENSOR   0
#define ENABLE_MAG1        0
#define ENABLE_MAG2        0
#define ENABLE_IMU         0

// COSMOS telemetry IDs
#define TLM_ID_SUNSENSOR_RAW    121
#define TLM_ID_SUNSENSOR_VECTOR 114
#define TLM_ID_MAG1_RAW         117
#define TLM_ID_MAG2_RAW         115
#define TLM_ID_MAG1_FILTERED    113
#define TLM_ID_MAG2_FILTERED    111
#define TLM_ID_IMU_RAW          116
#define TLM_ID_IMU_VECTOR       112

#include "core/utils.h"

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float x;
    float y;
    float z;
    uint8_t valid;
} sensor_vector_segment;

#endif /* ADCS_SENSORPROC_IDS_H_ */
