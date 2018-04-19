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
#define TLM_ID_MY_TELEM 125
#define TLM_ID_SIMULINK_INFO 124
#define TLM_ID_MTQ_STATE 123


#define OPCODE_MY_CMD 1

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float alpha;
    float beta;
    uint8_t status;
} my_telem;

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
    uint8_t tumble_status;
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
    uint8_t mtq_state;
} mtq_state_segment;


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
    mtq_not_on,
    mtq_turn_on,
    mtq_on_wait,
    mtq_turn_off,
    mtq_turn_off_ack,
    mtq_off_wait
} SubsystemState;

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

void receive_packet();
void handlePPTFiringNotification();
void handleRollCall();
void sendDipolePacket(int8_t x, int8_t y, int8_t z);
void sendTumblePacket(uint8_t status);
void simulink_compute();
void changeX();
void changeY();
void changeZ();
void sendHealthSegment();
void sendMagReadingSegment();
void sendMtqInfoSegment();
void sendSimulinkSegment();
void sendMtqState();
void updateMtqInfo();
void start_mtq_on_timer();
void start_mtq_off_timer();
void start_telem_timer();
void start_packet_timer();
void sendTelemetry();
void initial_setup();
int map(int val);
int mapGeneral(int x, int in_min, int in_max, int out_min, int out_max);

void rt_OneStep(void);

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* ADCS_BDOT_H_ */
