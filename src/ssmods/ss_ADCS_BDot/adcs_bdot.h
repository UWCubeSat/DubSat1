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
#include "interfaces/canwrap.h"
#include "sensors/magnetometer.h"


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

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;


void initial_setup();

void can_rx_callback(CANPacket *packet);
void send_dipole_packet(int8_t x, int8_t y, int8_t z);

void read_magnetometer_data();
void simulink_compute();

void send_bdot_mag_reading_cosmos();
void send_all_polling_timers_segment();
void send_health_segment_cosmos();
void send_mtq_info_segment_cosmos();
void send_simulink_segment_cosmos();

void convert_mag_data_raw_to_teslas(MagnetometerData * mag);
void determine_best_fit_mag();
void start_check_best_mag_timer();

void determine_mtq_commands();
void send_cosmos_telem();

void handleRollCall();
void updateRCData();
void rcPopulate1(CANPacket *out);
void rcPopulate2(CANPacket *out);
void rcPopulate3(CANPacket *out);
void rcPopulate4(CANPacket *out);

int map(int val);
int map_general(int x, int in_min, int in_max, int out_min, int out_max);

void rt_OneStep(void);
void update_simulink_info();
uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);
void handlePPTFiringNotification();
#endif /* ADCS_BDOT_H_ */
