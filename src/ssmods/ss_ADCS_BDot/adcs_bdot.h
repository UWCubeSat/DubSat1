/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_BDOT_H_
#define ADCS_BDOT_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"


#define MY_TELEM_DISABLED 0
#define MY_TELEM_ENABLED 1

#define TLM_ID_MAGNETOMETER  127
#define TLM_ID_MTQ_INFO 126
#define TLM_ID_SIMULINK_INFO 124
#define TLM_ID_POLLING_TIMER 123


#define OPCODE_MY_CMD 1

#define MTQ_MEASUREMENT_PHASE 0
#define MTQ_ACTUATION_PHASE  1


CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float xMag; // nT
    float yMag; // nT
    float zMag; // nT
    float tempMag; // nT
} magnetometer_segment;

TLM_SEGMENT {
    BcTlmHeader header;
    int8_t xDipole;
    int8_t yDipole;
    int8_t zDipole;
} mtq_info_segment;

TLM_SEGMENT {
    BcTlmHeader header;
    int8_t sim_xDipole;
    int8_t sim_yDipole;
    int8_t sim_zDipole;
    uint8_t tumble;
} simulink_segment;

TLM_SEGMENT {
    BcTlmHeader header;
    uint16_t user_id;
    uint8_t timer_id;
    uint8_t inUse;
    uint16_t start_timer_counter;
    uint16_t start_TAR;
    uint16_t counter_dif;
    uint16_t tar_dif;
} polling_timer_info_segment;



typedef struct mtq_info {
    uint8_t tumble_status;
    int8_t xDipole;
    int8_t yDipole;
    int8_t zDipole;
} mtq_info;




// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    mag_valid,
    mag_invalid,
} magDataStatus;

// Additional, it can be helpful if states are grouped into higher level
// "modes" in a hierarchical way for certain kinds of decision making and
// reporting.  These are not mandatory, however.  State transitions will need
// to explicitly transition the mode as well
typedef enum _subsystem_mode {
    Mode_FirstMode,
    Mode_SecondMode,
    Mode_ThirdMode,
} SubsystemMode;

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;


void initial_setup();
void receive_packet();
void sendDipolePacket(int8_t x, int8_t y, int8_t z);
void getMagnetometerData();
void updateRCData();
void simulink_compute();
void sendHealthSegment();
void sendMagReadingSegment();
void send_all_polling_timers_segment();
void sendMtqInfoSegment();
void sendSimulinkSegment();
void sendMtqState();
void updateMtqInfo();
void sendTelemetry();
void rollCall();
int map(int val);
int mapGeneral(int x, int in_min, int in_max, int out_min, int out_max);

void rt_OneStep(void);

void handleRollCall();
uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
void handlePPTFiringNotification();
#endif /* ADCS_BDOT_H_ */
