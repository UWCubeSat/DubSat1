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

// COSMOS telemetry IDs
#define TLM_ID_SUNSENSOR  121
#define TLM_ID_PHOTODIODE 118
#define TLM_ID_GPSHEALTH  120
#define TLM_ID_GPSPOWER   122
#define TLM_ID_RXSTATUS   123
#define TLM_ID_BESTXYZ    124
#define TLM_ID_TIME       125
#define TLM_ID_HWMONITOR  126
#define TLM_ID_SATVIS2    127
#define TLM_ID_RANGE      119

// COSMOS command opcodes
#define OPCODE_SENDASCII 0x01
#define OPCODE_ENABLE    0x02

#include <stdint.h>

#include "core/utils.h"

// --- COSMOS telem and cmd packets ---

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float alpha;
    float beta;
    uint8_t error;
} sunsensor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t center;
    uint32_t right;
    uint32_t left;
} photodiode_segment;

CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

#endif /* ADCS_SENSORPROC_IDS_H_ */
