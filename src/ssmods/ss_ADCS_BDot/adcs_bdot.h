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

#define CMD_SELECT_AUTO             (CAN_ENUM_BDOT_MAG_MODE_AUTOMODE)
#define CMD_SELECT_BDOT_MAG         (CAN_ENUM_BDOT_MAG_MODE_BDOTMODE)
#define CMD_SELECT_SP_MAG1          (CAN_ENUM_BDOT_MAG_MODE_SP1MODE)
#define CMD_SELECT_SP_MAG2          (CAN_ENUM_BDOT_MAG_MODE_SP2MODE)

#define CMD_MODE_OP_NORMAL 1
#define CMD_SELF_TEST_OP 0

#define TLM_ID_BDOT_MAGNETOMETER  127
#define TLM_ID_MTQ_INFO 126
#define TLM_ID_SIMULINK_INFO 124
#define TLM_ID_POLLING_TIMER 123
#define TLM_ID_SP_MAG1 122
#define TLM_ID_SP_MAG2 121
#define TLM_ID_CONTINUOUS_MAG 120
#define TLM_ID_BDOT_STATE_STATUS 119

TLM_SEGMENT {
    BcTlmHeader header;
    uint8_t calibration_switch_status;
    float calibration_factor_x;
    float calibration_factor_y;
    float calibration_factor_z;
} bdot_calibration_status;

TLM_SEGMENT {
    BcTlmHeader header;
    uint8_t state_status;
    uint8_t mag_selection_mode;
    uint8_t current_listening_mag;
} bdot_state_status;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float xMag; // nT
    float yMag; // nT
    float zMag; // nT
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

/************************* Initial Setup***********************************/
FILE_STATIC void initial_setup();
FILE_STATIC void can_rx_callback(CANPacket *packet);
FILE_STATIC void initialize_aggregate();
FILE_STATIC void reset_spam_avg_agg();
/**************************************************************************/


/*******************************Timers************************************/
FILE_STATIC void start_check_best_mag_timer();
FILE_STATIC uint8_t check_check_best_mag_timer();
FILE_STATIC void end_check_best_mag_timer();

FILE_STATIC void start_check_nap_status_timer();
FILE_STATIC void end_check_nap_status_timer();
FILE_STATIC uint8_t check_check_nap_status_timer();

FILE_STATIC void start_spam_timer(uint32_t spam_timer_ms);
FILE_STATIC uint8_t check_spam_timer();
FILE_STATIC void end_spam_timer();

FILE_STATIC void start_spam_avg_timer();
FILE_STATIC void end_spam_avg_timer();
FILE_STATIC uint8_t check_spam_avg_timer();

FILE_STATIC void simulink_compute();
/*************************************************************************/


/*************************** CAN/Rollcall ********************************/
FILE_STATIC void reset_aggregate();
FILE_STATIC void send_dipole_packet(int8_t x, int8_t y, int8_t z);
FILE_STATIC void updateRCData();
FILE_STATIC void rcPopulateH1(CANPacket *out);
FILE_STATIC void rcPopulateH2(CANPacket *out);
FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC void rcPopulate5(CANPacket *out);
FILE_STATIC void rcPopulate6(CANPacket *out);
FILE_STATIC void rcPopulate7(CANPacket *out);
FILE_STATIC void rcPopulate8(CANPacket *out);
FILE_STATIC void rcPopulate9(CANPacket *out);
FILE_STATIC void rcPopulate10(CANPacket *out);
/************************************************************************/


/**************************Cosmos Telemetry******************************/
FILE_STATIC void send_bdot_mag_reading_cosmos();
FILE_STATIC void send_continuous_mag_reading_cosmos();

FILE_STATIC void send_sp_mag1_reading_cosmos();
FILE_STATIC void send_sp_mag2_reading_cosmos();

FILE_STATIC void send_mtq_info_segment_cosmos();
FILE_STATIC void send_simulink_segment_cosmos();

FILE_STATIC void send_health_segment_cosmos();

FILE_STATIC void send_bdot_state_status_cosmos();

FILE_STATIC void send_cosmos_telem();
/************************************************************************/


/****************Ground Commands and Supporting Functions****************/
FILE_STATIC void mag_select_switch(uint8_t mag_selection);
FILE_STATIC void select_mode_operation(uint8_t reading_mode_selection);
FILE_STATIC void ground_cmd_bdot_nap_schedule(uint8_t nap_status);
FILE_STATIC void change_max_tumble_time(uint16_t max_tumble_time_min);
FILE_STATIC void spam_control_operation(uint16_t off_time_min, uint16_t on_time_min, uint8_t spam_switch,
                                        int8_t x_dipole, int8_t y_dipole, int8_t z_dipole);
FILE_STATIC void bdot_pop_operation(uint8_t pop_control_x, uint8_t pop_control_y, uint8_t pop_control_z);
FILE_STATIC void change_spam_avg_time(uint8_t mtq_pwm_measurement, uint8_t mtq_pwm_actuation);
/***********************************************************************/


/*******************Magnetometer Analysis*******************************/
FILE_STATIC void determine_best_fit_mag();
FILE_STATIC void calc_best_fit_mag();
FILE_STATIC void process_sp_mag();

FILE_STATIC void convert_mag_data_raw_to_teslas(MagnetometerData * mag);

FILE_STATIC void clear_update_bdot_state_flags();
FILE_STATIC void determine_bdot_state();
FILE_STATIC void determine_spam_axis();
FILE_STATIC void handle_spam_average();

FILE_STATIC void read_magnetometer_data();
FILE_STATIC void update_valid_mag_data();
FILE_STATIC void rt_OneStep(void);
FILE_STATIC void update_simulink_info();
FILE_STATIC void determine_mtq_commands();
FILE_STATIC int8_t find_max_dipole(int16_t dipole);
/**********************************************************************/


/*****************************Others***********************************/
void handlePPTFiringNotification();
/**********************************************************************/
#endif /* ADCS_BDOT_H_ */
