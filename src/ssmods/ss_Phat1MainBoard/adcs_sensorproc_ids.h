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
#define TLM_ID_BESTPOS    117

// COSMOS command opcodes
#define OPCODE_SENDASCII       1
#define OPCODE_ENABLE          2
#define OPCODE_OVERRIDE_BUCK   3
#define OPCODE_TEST_RESET      4
#define OPCODE_TEST_BUCK       5
#define OPCODE_TEST_GPS_SWITCH 6

#endif /* ADCS_SENSORPROC_IDS_H_ */
