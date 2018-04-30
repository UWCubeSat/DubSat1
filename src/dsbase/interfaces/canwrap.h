/*
 * canwrap.h
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include <inttypes.h>

#ifndef DSBASE_INTERFACES_CANWRAP_H_
#define DSBASE_INTERFACES_CANWRAP_H_

#define PARAM_ADCS_STATUS_VELOCITY_RPM 0x4201;


// BEGIN GENERATOR MACROS

#define CAN_ID_RC_EPS_GEN_11 304677402
#define CAN_ID_RC_EPS_GEN_10 304677401
#define CAN_ID_RC_EPS_GEN_19 304677410
#define CAN_ID_RC_EPS_GEN_18 304677409
#define CAN_ID_RC_EPS_GEN_17 304677408
#define CAN_ID_RC_EPS_GEN_15 304677406
#define CAN_ID_RC_EPS_GEN_16 304677407
#define CAN_ID_RC_EPS_GEN_13 304677404
#define CAN_ID_RC_EPS_GEN_14 304677405
#define CAN_ID_RC_EPS_GEN_12 304677403
#define CAN_ID_RC_EPS_GEN_9 304677400
#define CAN_ID_RC_EPS_GEN_8 304677399
#define CAN_ID_RC_EPS_GEN_7 304677398
#define CAN_ID_RC_EPS_GEN_6 304677397
#define CAN_ID_RC_EPS_GEN_5 304677396
#define CAN_ID_RC_EPS_GEN_4 304677395
#define CAN_ID_RC_EPS_GEN_3 304677394
#define CAN_ID_RC_EPS_GEN_2 304677393
#define CAN_ID_RC_EPS_GEN_1 304677392
#define CAN_ID_RC_EPS_BATT_6 304677381
#define CAN_ID_RC_EPS_BATT_5 304677380
#define CAN_ID_RC_EPS_BATT_4 304677379
#define CAN_ID_RC_EPS_BATT_3 304677378
#define CAN_ID_RC_EPS_BATT_2 304677377
#define CAN_ID_RC_EPS_BATT_1 304677376
#define CAN_ID_CMD_PPT_SET_COUNT 302252294
#define CAN_ID_CMD_PPT_TIME_UPD 302252293
#define CAN_ID_CMD_PPT_HALT 285475076
#define CAN_ID_PPT_FIRING_RESULT 304677104
#define CAN_ID_TLE_6 309788745
#define CAN_ID_TLE_5 309788744
#define CAN_ID_TLE_4 309788743
#define CAN_ID_TLE_3 309788742
#define CAN_ID_TLE_2 309788741
#define CAN_ID_TLE_1 302448708
#define CAN_ID_EPS_PD_UNDERVOLTAGE 304218338
#define CAN_ID_EPS_PD_BATT_VOLTAGE 304218337
#define CAN_ID_CMD_PD_RST 304218385
#define CAN_ID_CMD_COM2_RUN 302514673
#define CAN_ID_COM2_STATE 304611824
#define CAN_ID_CMD_PD_ENABLE 303300865
#define CAN_ID_CMD_BATT_RST 304349442
#define CAN_ID_CMD_BATT_BAL_ENABLE 304349443
#define CAN_ID_CMD_REBOOT_REQUEST 1310728
#define CAN_ID_CMD_IGNORE_FSW 304349220
#define CAN_ID_CMD_MTQ_FSW 304349219
#define CAN_ID_EPS_DOMAIN_OUTPUT 304218336
#define CAN_ID_SENSORPROC_IMU 335872066
#define CAN_ID_SENSORPROC_MAG 335872067
#define CAN_ID_CMD_ROLLCALL 1114132
#define CAN_ID_MTQ_ACK 34013232
#define CAN_ID_GEN_PANEL_TEMP 36241619
#define CAN_ID_CMD_GEN_RST 34865424
#define CAN_ID_CMD_PPT_SINGLE_FIRE 34865408
#define CAN_ID_GEN_PANEL_PWR 35782866
#define CAN_ID_GEN_PANEL_CURRENT 35782865
#define CAN_ID_GEN_PANEL_VOLTAGE 34734288
#define CAN_ID_MPC_VP 37158946
#define CAN_ID_EPS_BATT_STATE 35782850
#define CAN_ID_EPS_BATT_CURRENT 35782849
#define CAN_ID_EPS_BATT_VOLTAGE 35782848
#define CAN_ID_SENSORPROC_SUN 335872065
#define CAN_ID_CMD_MTQ_BDOT 304545825
#define CAN_ID_BDOT_TUMBLE_STATUS 34013216
#define CAN_ID_SYNC_2 65554
#define CAN_ID_SYNC_1 65553
#define CAN_ID_MSP_TEMP 36241427

#define CAN_ENUM_NBOOL_NULL 2
#define CAN_ENUM_NBOOL_TRUE 1
#define CAN_ENUM_NBOOL_FALSE 0
#define CAN_ENUM_FIRE_STATE_NODISCHARGE 3
#define CAN_ENUM_FIRE_STATE_NOCHARGE 2
#define CAN_ENUM_FIRE_STATE_FIREOVERRIDDEN 1
#define CAN_ENUM_FIRE_STATE_FIRESUCCESSFUL 0
#define CAN_ENUM_UV_STATE_UNDERVOLTAGE 1
#define CAN_ENUM_UV_STATE_NORMAL 0
#define CAN_ENUM_MODE_NOTTUMBLINGCHARLIE 3
#define CAN_ENUM_MODE_NOTTUMBLINGBOB 2
#define CAN_ENUM_MODE_NOTTUMBLINGALICE 1
#define CAN_ENUM_MODE_TUMBLING 0
#define CAN_ENUM_PWR_DOMAIN_UNK 8
#define CAN_ENUM_PWR_DOMAIN_COM1 7
#define CAN_ENUM_PWR_DOMAIN_COM2 6
#define CAN_ENUM_PWR_DOMAIN_RAHS 5
#define CAN_ENUM_PWR_DOMAIN_BDOT 4
#define CAN_ENUM_PWR_DOMAIN_ESTIM 3
#define CAN_ENUM_PWR_DOMAIN_WHEEL 2
#define CAN_ENUM_PWR_DOMAIN_EPS 1
#define CAN_ENUM_PWR_DOMAIN_PPT 0
#define CAN_ENUM_SAT_STATE_GOOD 1
#define CAN_ENUM_SAT_STATE_BAD 0
#define CAN_ENUM_NODE_RAHS 15
#define CAN_ENUM_NODE_COM_2 14
#define CAN_ENUM_NODE_COM_1 13
#define CAN_ENUM_NODE_ADCS_MTQ 12
#define CAN_ENUM_NODE_ADCS_SENSORPROC 11
#define CAN_ENUM_NODE_ADCS_MPC 10
#define CAN_ENUM_NODE_ADCS_ESTIM 9
#define CAN_ENUM_NODE_EPS_BATT 8
#define CAN_ENUM_NODE_EPS_GEN 7
#define CAN_ENUM_NODE_EPS_DIST 6
#define CAN_ENUM_NODE_PPT 5
#define CAN_ENUM_NODE_ADCS_RWZ 4
#define CAN_ENUM_NODE_ADCS_RWY 3
#define CAN_ENUM_NODE_ADCS_RWX 2
#define CAN_ENUM_NODE_ADCS_BDOT 1
#define CAN_ENUM_NODE_TEST 0
#define CAN_ENUM_AGG_SUM 5
#define CAN_ENUM_AGG_MAX 4
#define CAN_ENUM_AGG_MIN 3
#define CAN_ENUM_AGG_COUNT 2
#define CAN_ENUM_AGG_AVG 1
#define CAN_ENUM_AGG_NONE 0
#define CAN_ENUM_BOOL_TRUE 1
#define CAN_ENUM_BOOL_FALSE 0

typedef struct CANPacket {
   uint32_t id; // Actual physical ID of the packet
   uint8_t data[8]; // Data
   uint8_t bufferNum; // Only applicable for Rx, which buffer it landed in
   uint8_t length; // Only applies to sending packets. We don't know how long incoming packets are.
} CANPacket;

void canWrapInit();
void canWrapInitWithFilter();

// Global function pointer to point to the function
// when a packet is received through CAN
void (*CANPacketReceived)(CANPacket *);

void canSendPacket(CANPacket *packet);

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet));
typedef struct rc_eps_gen_11 {
    float rc_eps_gen_11_pnl_1_power_min; // V
    float rc_eps_gen_11_pnl_1_power_max; // V
} rc_eps_gen_11;

typedef struct rc_eps_gen_10 {
    float rc_eps_gen_10_pnl_3_current_max; // V
    float rc_eps_gen_10_pnl_3_current_avg; // V
} rc_eps_gen_10;

typedef struct rc_eps_gen_19 {
    float rc_eps_gen_19_pnl_3_temp_max; // V
    float rc_eps_gen_19_pnl_3_temp_avg; // V
} rc_eps_gen_19;

typedef struct rc_eps_gen_18 {
    float rc_eps_gen_18_pnl_3_temp_min; // V
    float rc_eps_gen_18_pnl_2_temp_avg; // V
} rc_eps_gen_18;

typedef struct rc_eps_gen_17 {
    float rc_eps_gen_17_pnl_2_temp_max; // V
    float rc_eps_gen_17_pnl_2_temp_min; // V
} rc_eps_gen_17;

typedef struct rc_eps_gen_15 {
    float rc_eps_gen_15_pnl_3_power_avg; // V
    float rc_eps_gen_15_pnl_1_temp_min; // V
} rc_eps_gen_15;

typedef struct rc_eps_gen_16 {
    float rc_eps_gen_16_pnl_1_temp_max; // V
    float rc_eps_gen_16_pnl_1_temp_avg; // V
} rc_eps_gen_16;

typedef struct rc_eps_gen_13 {
    float rc_eps_gen_13_pnl_2_power_max; // V
    float rc_eps_gen_13_pnl_2_power_avg; // V
} rc_eps_gen_13;

typedef struct rc_eps_gen_14 {
    float rc_eps_gen_14_pnl_3_power_min; // V
    float rc_eps_gen_14_pnl_3_power_max; // V
} rc_eps_gen_14;

typedef struct rc_eps_gen_12 {
    float rc_eps_gen_12_pnl_2_power_min; // V
    float rc_eps_gen_12_pnl_1_power_avg; // V
} rc_eps_gen_12;

typedef struct rc_eps_gen_9 {
    float rc_eps_gen_9_pnl_3_current_min; // V
    float rc_eps_gen_9_pnl_2_current_avg; // V
} rc_eps_gen_9;

typedef struct rc_eps_gen_8 {
    float rc_eps_gen_8_pnl_2_current_min; // V
    float rc_eps_gen_8_pnl_2_current_max; // V
} rc_eps_gen_8;

typedef struct rc_eps_gen_7 {
    float rc_eps_gen_7_pnl_1_current_max; // V
    float rc_eps_gen_7_pnl_1_current_avg; // V
} rc_eps_gen_7;

typedef struct rc_eps_gen_6 {
    float rc_eps_gen_6_pnl_3_voltage_avg; // V
    float rc_eps_gen_6_pnl_1_current_min; // V
} rc_eps_gen_6;

typedef struct rc_eps_gen_5 {
    float rc_eps_gen_5_pnl_3_voltage_min; // V
    float rc_eps_gen_5_pnl_3_voltage_max; // V
} rc_eps_gen_5;

typedef struct rc_eps_gen_4 {
    float rc_eps_gen_4_pnl_2_voltage_avg; // V
    float rc_eps_gen_4_pnl_2_voltage_max; // V
} rc_eps_gen_4;

typedef struct rc_eps_gen_3 {
    float rc_eps_gen_3_pnl_1_voltage_avg; // V
    float rc_eps_gen_3_pnl_2_voltage_min; // V
} rc_eps_gen_3;

typedef struct rc_eps_gen_2 {
    float rc_eps_gen_2_pnl_1_voltage_min; // V
    float rc_eps_gen_2_pnl_1_voltage_max; // V
} rc_eps_gen_2;

typedef struct rc_eps_gen_1 {
    uint16_t rc_eps_gen_1_temp_min; // dK
    uint16_t rc_eps_gen_1_temp_max; // dK
    uint16_t rc_eps_gen_1_temp_avg; // dK
    uint16_t rc_eps_gen_1_sysrstiv; //  (No Units)
} rc_eps_gen_1;

typedef struct rc_eps_batt_6 {
    uint8_t rc_eps_batt_6_soc_min; // pct
    uint8_t rc_eps_batt_6_soc_max; // pct
    uint8_t rc_eps_batt_6_soc_avg; // pct
    uint64_t rc_eps_batt_6_last_charge; // 2^-8 seconds
} rc_eps_batt_6;

typedef struct rc_eps_batt_5 {
    int16_t rc_eps_batt_5_node_c_min; // dmA
    int16_t rc_eps_batt_5_node_c_max; // dmA
    int16_t rc_eps_batt_5_node_c_avg; // dmA
    int8_t rc_eps_batt_5_batt_temp_min; // deg C
    int8_t rc_eps_batt_5_batt_temp_max; // deg C
} rc_eps_batt_5;

typedef struct rc_eps_batt_4 {
    uint16_t rc_eps_batt_4_voltage_min; // mV
    uint16_t rc_eps_batt_4_voltage_max; // mV
    uint16_t rc_eps_batt_4_voltage_avg; // mV
    uint8_t rc_eps_batt_4_heater_state; //  (No Units)
    uint8_t rc_eps_batt_4_balancer_state; //  (No Units)
} rc_eps_batt_4;

typedef struct rc_eps_batt_3 {
    int16_t rc_eps_batt_3_current_min; // cA
    int16_t rc_eps_batt_3_current_max; // cA
    int16_t rc_eps_batt_3_current_avg; // cA
    int8_t rc_eps_batt_3_batt_temp_avg; // deg C
} rc_eps_batt_3;

typedef struct rc_eps_batt_2 {
    uint16_t rc_eps_batt_2_node_v_min; // mV
    uint16_t rc_eps_batt_2_node_v_max; // mV
    uint16_t rc_eps_batt_2_node_v_avg; // mV
} rc_eps_batt_2;

typedef struct rc_eps_batt_1 {
    uint16_t rc_eps_batt_1_temp_min; // dK
    uint16_t rc_eps_batt_1_temp_max; // dK
    uint16_t rc_eps_batt_1_temp_avg; // dK
    uint16_t rc_eps_batt_1_sysrstiv; //  (No Units)
} rc_eps_batt_1;

typedef struct cmd_ppt_set_count {
    uint8_t cmd_ppt_set_count_count; //  (No Units)
} cmd_ppt_set_count;

typedef struct cmd_ppt_time_upd {
    uint16_t cmd_ppt_time_upd_ign_charge; // 2^-15s
    uint16_t cmd_ppt_time_upd_cooldown; // 2^-15s
    uint16_t cmd_ppt_time_upd_ign_delay; // 2^-15s
    uint16_t cmd_ppt_time_upd_charge; // 2^-15s
} cmd_ppt_time_upd;

typedef struct cmd_ppt_halt {
    uint8_t cmd_ppt_halt_confirm; //  (No Units)
} cmd_ppt_halt;

typedef struct ppt_firing_result {
    uint8_t ppt_firing_result_panel_good; //  (No Units)
    uint8_t ppt_firing_result_batt_good; //  (No Units)
    uint8_t ppt_firing_result_coulmb_good; //  (No Units)
    uint16_t ppt_firing_result_ign_time; // 2^-15s
    uint16_t ppt_firing_result_main_time; // 2^-15s
    uint8_t ppt_firing_result_code; //  (No Units)
} ppt_firing_result;

typedef struct tle_6 {
    double tle_6_mnm; //  (No Units)
} tle_6;

typedef struct tle_5 {
    float tle_5_mna; //  (No Units)
} tle_5;

typedef struct tle_4 {
    float tle_4_aop; //  (No Units)
    float tle_4_ecc; //  (No Units)
} tle_4;

typedef struct tle_3 {
    float tle_3_raan; //  (No Units)
    float tle_3_inc; //  (No Units)
} tle_3;

typedef struct tle_2 {
    double tle_2_day; //  (No Units)
} tle_2;

typedef struct tle_1 {
    float tle_1_bstar; //  (No Units)
    uint8_t tle_1_year; //  (No Units)
} tle_1;

typedef struct eps_pd_undervoltage {
    uint8_t eps_pd_undervoltage_state; //  (No Units)
} eps_pd_undervoltage;

typedef struct eps_pd_batt_voltage {
    uint8_t eps_pd_batt_voltage_agg; //  (No Units)
    uint16_t eps_pd_batt_voltage_val; // mV
} eps_pd_batt_voltage;

typedef struct cmd_pd_rst {
    uint8_t cmd_pd_rst_confirm; //  (No Units)
} cmd_pd_rst;

typedef struct cmd_com2_run {
    uint8_t cmd_com2_run_clear; //  (No Units)
    uint8_t cmd_com2_run_jump; //  (No Units)
    uint8_t cmd_com2_run_fileno; //  (No Units)
} cmd_com2_run;

typedef struct com2_state {
    uint8_t com2_state_health_state; //  (No Units)
    uint8_t com2_state_curr_file; //  (No Units)
    uint16_t com2_state_qfilesize; // kB
    uint16_t com2_state_qlen; // files
    uint16_t com2_state_uptime; // s
} com2_state;

typedef struct cmd_pd_enable {
    uint8_t cmd_pd_enable_enable; //  (No Units)
    uint8_t cmd_pd_enable_domain; //  (No Units)
} cmd_pd_enable;

typedef struct cmd_batt_rst {
    uint8_t cmd_batt_rst_confirm; //  (No Units)
} cmd_batt_rst;

typedef struct cmd_batt_bal_enable {
    uint8_t cmd_batt_bal_enable_enable; //  (No Units)
} cmd_batt_bal_enable;

typedef struct cmd_reboot_request {
    uint8_t cmd_reboot_request_domain; //  (No Units)
} cmd_reboot_request;

typedef struct cmd_ignore_fsw {
    uint8_t cmd_ignore_fsw_ignore; //  (No Units)
} cmd_ignore_fsw;

typedef struct cmd_mtq_fsw {
    uint8_t cmd_mtq_fsw_sc_mode; //  (No Units)
    int8_t cmd_mtq_fsw_z; // pct doodie
    int8_t cmd_mtq_fsw_y; // pct doodie
    int8_t cmd_mtq_fsw_x; // pct doodie
} cmd_mtq_fsw;

typedef struct eps_domain_output {
    uint8_t eps_domain_output_state; //  (No Units)
    uint8_t eps_domain_output_oc_trip; //  (No Units)
    uint16_t eps_domain_output_v_c_max; // mA
    uint16_t eps_domain_output_v_c; // mA
    uint16_t eps_domain_output_v; // mV
    uint8_t eps_domain_output_num; //  (No Units)
} eps_domain_output;

typedef struct sensorproc_imu {
    uint16_t sensorproc_imu_z; //  (No Units)
    uint16_t sensorproc_imu_y; //  (No Units)
    uint16_t sensorproc_imu_x; //  (No Units)
    uint8_t sensorproc_imu_valid; // bool
} sensorproc_imu;

typedef struct sensorproc_mag {
    uint16_t sensorproc_mag_z; //  (No Units)
    uint16_t sensorproc_mag_y; //  (No Units)
    uint16_t sensorproc_mag_x; //  (No Units)
    uint8_t sensorproc_mag_valid; // bool
} sensorproc_mag;

typedef struct cmd_rollcall {
    uint8_t cmd_rollcall_type; //  (No Units)
    uint8_t cmd_rollcall_met_overflow; //  (No Units)
    uint32_t cmd_rollcall_met; //  (No Units)
    uint8_t cmd_rollcall_msp; //  (No Units)
} cmd_rollcall;

typedef struct mtq_ack {
    uint8_t mtq_ack_coils_state; //  (No Units)
    uint8_t mtq_ack_node; //  (No Units)
} mtq_ack;

typedef struct gen_panel_temp {
    uint8_t gen_panel_temp_agg; //  (No Units)
    int8_t gen_panel_temp_x_pos; //  (No Units)
    int8_t gen_panel_temp_y_neg; //  (No Units)
    int8_t gen_panel_temp_y_pos; //  (No Units)
} gen_panel_temp;

typedef struct cmd_gen_rst {
    uint8_t cmd_gen_rst_confirm; // bool
} cmd_gen_rst;

typedef struct cmd_ppt_single_fire {
    uint8_t cmd_ppt_single_fire_override; //  (No Units)
    uint8_t cmd_ppt_single_fire_with_pulse; //  (No Units)
} cmd_ppt_single_fire;

typedef struct gen_panel_pwr {
    uint8_t gen_panel_pwr_agg; //  (No Units)
    uint16_t gen_panel_pwr_x_pos; // mW
    uint16_t gen_panel_pwr_y_neg; // mW
    uint16_t gen_panel_pwr_y_pos; // mW
} gen_panel_pwr;

typedef struct gen_panel_current {
    uint16_t gen_panel_current_x_pos; // mA
    uint16_t gen_panel_current_y_neg; // mA
    uint16_t gen_panel_current_y_pos; // mA
} gen_panel_current;

typedef struct gen_panel_voltage {
    uint16_t gen_panel_voltage_x_pos; // mV
    uint16_t gen_panel_voltage_y_neg; // mV
    uint16_t gen_panel_voltage_y_pos; // mV
} gen_panel_voltage;

typedef struct mpc_vp {
    uint8_t mpc_vp_status; //  (No Units)
} mpc_vp;

typedef struct eps_batt_state {
    uint8_t eps_batt_state_bal; //  (No Units)
    uint8_t eps_batt_state_heat; //  (No Units)
    uint8_t eps_batt_state_soc; // pct
    int8_t eps_batt_state_temp; // Deg C
} eps_batt_state;

typedef struct eps_batt_current {
    int16_t eps_batt_current_node_c; // dmA (0.1mA)
    int16_t eps_batt_current_min_c; // cA
    int16_t eps_batt_current_max_c; // cA
    int16_t eps_batt_current_c; // cA
} eps_batt_current;

typedef struct eps_batt_voltage {
    uint16_t eps_batt_voltage_v_max_delta; // mV
    uint16_t eps_batt_voltage_node_v; // mV
    uint16_t eps_batt_voltage_min_v; // mV
    uint16_t eps_batt_voltage_v; // mV
} eps_batt_voltage;

typedef struct sensorproc_sun {
    uint8_t sensorproc_sun_valid; // bool
    int16_t sensorproc_sun_z; //  (No Units)
    int16_t sensorproc_sun_y; //  (No Units)
    int16_t sensorproc_sun_x; //  (No Units)
} sensorproc_sun;

typedef struct cmd_mtq_bdot {
    int8_t cmd_mtq_bdot_z; // pct doodie
    int8_t cmd_mtq_bdot_y; // pct doodie
    int8_t cmd_mtq_bdot_x; // pct doodie
} cmd_mtq_bdot;

typedef struct bdot_tumble_status {
    uint8_t bdot_tumble_status_status; // bool
} bdot_tumble_status;

typedef struct sync_2 {
} sync_2;

typedef struct sync_1 {
} sync_1;

typedef struct msp_temp {
    uint16_t msp_temp_temp; // dK
} msp_temp;

void encoderc_eps_gen_11(rc_eps_gen_11 *input, CANPacket* output);
void decoderc_eps_gen_11(CANPacket *input, rc_eps_gen_11 *output);

void encoderc_eps_gen_10(rc_eps_gen_10 *input, CANPacket* output);
void decoderc_eps_gen_10(CANPacket *input, rc_eps_gen_10 *output);

void encoderc_eps_gen_19(rc_eps_gen_19 *input, CANPacket* output);
void decoderc_eps_gen_19(CANPacket *input, rc_eps_gen_19 *output);

void encoderc_eps_gen_18(rc_eps_gen_18 *input, CANPacket* output);
void decoderc_eps_gen_18(CANPacket *input, rc_eps_gen_18 *output);

void encoderc_eps_gen_17(rc_eps_gen_17 *input, CANPacket* output);
void decoderc_eps_gen_17(CANPacket *input, rc_eps_gen_17 *output);

void encoderc_eps_gen_15(rc_eps_gen_15 *input, CANPacket* output);
void decoderc_eps_gen_15(CANPacket *input, rc_eps_gen_15 *output);

void encoderc_eps_gen_16(rc_eps_gen_16 *input, CANPacket* output);
void decoderc_eps_gen_16(CANPacket *input, rc_eps_gen_16 *output);

void encoderc_eps_gen_13(rc_eps_gen_13 *input, CANPacket* output);
void decoderc_eps_gen_13(CANPacket *input, rc_eps_gen_13 *output);

void encoderc_eps_gen_14(rc_eps_gen_14 *input, CANPacket* output);
void decoderc_eps_gen_14(CANPacket *input, rc_eps_gen_14 *output);

void encoderc_eps_gen_12(rc_eps_gen_12 *input, CANPacket* output);
void decoderc_eps_gen_12(CANPacket *input, rc_eps_gen_12 *output);

void encoderc_eps_gen_9(rc_eps_gen_9 *input, CANPacket* output);
void decoderc_eps_gen_9(CANPacket *input, rc_eps_gen_9 *output);

void encoderc_eps_gen_8(rc_eps_gen_8 *input, CANPacket* output);
void decoderc_eps_gen_8(CANPacket *input, rc_eps_gen_8 *output);

void encoderc_eps_gen_7(rc_eps_gen_7 *input, CANPacket* output);
void decoderc_eps_gen_7(CANPacket *input, rc_eps_gen_7 *output);

void encoderc_eps_gen_6(rc_eps_gen_6 *input, CANPacket* output);
void decoderc_eps_gen_6(CANPacket *input, rc_eps_gen_6 *output);

void encoderc_eps_gen_5(rc_eps_gen_5 *input, CANPacket* output);
void decoderc_eps_gen_5(CANPacket *input, rc_eps_gen_5 *output);

void encoderc_eps_gen_4(rc_eps_gen_4 *input, CANPacket* output);
void decoderc_eps_gen_4(CANPacket *input, rc_eps_gen_4 *output);

void encoderc_eps_gen_3(rc_eps_gen_3 *input, CANPacket* output);
void decoderc_eps_gen_3(CANPacket *input, rc_eps_gen_3 *output);

void encoderc_eps_gen_2(rc_eps_gen_2 *input, CANPacket* output);
void decoderc_eps_gen_2(CANPacket *input, rc_eps_gen_2 *output);

void encoderc_eps_gen_1(rc_eps_gen_1 *input, CANPacket* output);
void decoderc_eps_gen_1(CANPacket *input, rc_eps_gen_1 *output);

void encoderc_eps_batt_6(rc_eps_batt_6 *input, CANPacket* output);
void decoderc_eps_batt_6(CANPacket *input, rc_eps_batt_6 *output);

void encoderc_eps_batt_5(rc_eps_batt_5 *input, CANPacket* output);
void decoderc_eps_batt_5(CANPacket *input, rc_eps_batt_5 *output);

void encoderc_eps_batt_4(rc_eps_batt_4 *input, CANPacket* output);
void decoderc_eps_batt_4(CANPacket *input, rc_eps_batt_4 *output);

void encoderc_eps_batt_3(rc_eps_batt_3 *input, CANPacket* output);
void decoderc_eps_batt_3(CANPacket *input, rc_eps_batt_3 *output);

void encoderc_eps_batt_2(rc_eps_batt_2 *input, CANPacket* output);
void decoderc_eps_batt_2(CANPacket *input, rc_eps_batt_2 *output);

void encoderc_eps_batt_1(rc_eps_batt_1 *input, CANPacket* output);
void decoderc_eps_batt_1(CANPacket *input, rc_eps_batt_1 *output);

void encodecmd_ppt_set_count(cmd_ppt_set_count *input, CANPacket* output);
void decodecmd_ppt_set_count(CANPacket *input, cmd_ppt_set_count *output);

void encodecmd_ppt_time_upd(cmd_ppt_time_upd *input, CANPacket* output);
void decodecmd_ppt_time_upd(CANPacket *input, cmd_ppt_time_upd *output);

void encodecmd_ppt_halt(cmd_ppt_halt *input, CANPacket* output);
void decodecmd_ppt_halt(CANPacket *input, cmd_ppt_halt *output);

void encodeppt_firing_result(ppt_firing_result *input, CANPacket* output);
void decodeppt_firing_result(CANPacket *input, ppt_firing_result *output);

void encodetle_6(tle_6 *input, CANPacket* output);
void decodetle_6(CANPacket *input, tle_6 *output);

void encodetle_5(tle_5 *input, CANPacket* output);
void decodetle_5(CANPacket *input, tle_5 *output);

void encodetle_4(tle_4 *input, CANPacket* output);
void decodetle_4(CANPacket *input, tle_4 *output);

void encodetle_3(tle_3 *input, CANPacket* output);
void decodetle_3(CANPacket *input, tle_3 *output);

void encodetle_2(tle_2 *input, CANPacket* output);
void decodetle_2(CANPacket *input, tle_2 *output);

void encodetle_1(tle_1 *input, CANPacket* output);
void decodetle_1(CANPacket *input, tle_1 *output);

void encodeeps_pd_undervoltage(eps_pd_undervoltage *input, CANPacket* output);
void decodeeps_pd_undervoltage(CANPacket *input, eps_pd_undervoltage *output);

void encodeeps_pd_batt_voltage(eps_pd_batt_voltage *input, CANPacket* output);
void decodeeps_pd_batt_voltage(CANPacket *input, eps_pd_batt_voltage *output);

void encodecmd_pd_rst(cmd_pd_rst *input, CANPacket* output);
void decodecmd_pd_rst(CANPacket *input, cmd_pd_rst *output);

void encodecmd_com2_run(cmd_com2_run *input, CANPacket* output);
void decodecmd_com2_run(CANPacket *input, cmd_com2_run *output);

void encodecom2_state(com2_state *input, CANPacket* output);
void decodecom2_state(CANPacket *input, com2_state *output);

void encodecmd_pd_enable(cmd_pd_enable *input, CANPacket* output);
void decodecmd_pd_enable(CANPacket *input, cmd_pd_enable *output);

void encodecmd_batt_rst(cmd_batt_rst *input, CANPacket* output);
void decodecmd_batt_rst(CANPacket *input, cmd_batt_rst *output);

void encodecmd_batt_bal_enable(cmd_batt_bal_enable *input, CANPacket* output);
void decodecmd_batt_bal_enable(CANPacket *input, cmd_batt_bal_enable *output);

void encodecmd_reboot_request(cmd_reboot_request *input, CANPacket* output);
void decodecmd_reboot_request(CANPacket *input, cmd_reboot_request *output);

void encodecmd_ignore_fsw(cmd_ignore_fsw *input, CANPacket* output);
void decodecmd_ignore_fsw(CANPacket *input, cmd_ignore_fsw *output);

void encodecmd_mtq_fsw(cmd_mtq_fsw *input, CANPacket* output);
void decodecmd_mtq_fsw(CANPacket *input, cmd_mtq_fsw *output);

void encodeeps_domain_output(eps_domain_output *input, CANPacket* output);
void decodeeps_domain_output(CANPacket *input, eps_domain_output *output);

void encodesensorproc_imu(sensorproc_imu *input, CANPacket* output);
void decodesensorproc_imu(CANPacket *input, sensorproc_imu *output);

void encodesensorproc_mag(sensorproc_mag *input, CANPacket* output);
void decodesensorproc_mag(CANPacket *input, sensorproc_mag *output);

void encodecmd_rollcall(cmd_rollcall *input, CANPacket* output);
void decodecmd_rollcall(CANPacket *input, cmd_rollcall *output);

void encodemtq_ack(mtq_ack *input, CANPacket* output);
void decodemtq_ack(CANPacket *input, mtq_ack *output);

void encodegen_panel_temp(gen_panel_temp *input, CANPacket* output);
void decodegen_panel_temp(CANPacket *input, gen_panel_temp *output);

void encodecmd_gen_rst(cmd_gen_rst *input, CANPacket* output);
void decodecmd_gen_rst(CANPacket *input, cmd_gen_rst *output);

void encodecmd_ppt_single_fire(cmd_ppt_single_fire *input, CANPacket* output);
void decodecmd_ppt_single_fire(CANPacket *input, cmd_ppt_single_fire *output);

void encodegen_panel_pwr(gen_panel_pwr *input, CANPacket* output);
void decodegen_panel_pwr(CANPacket *input, gen_panel_pwr *output);

void encodegen_panel_current(gen_panel_current *input, CANPacket* output);
void decodegen_panel_current(CANPacket *input, gen_panel_current *output);

void encodegen_panel_voltage(gen_panel_voltage *input, CANPacket* output);
void decodegen_panel_voltage(CANPacket *input, gen_panel_voltage *output);

void encodempc_vp(mpc_vp *input, CANPacket* output);
void decodempc_vp(CANPacket *input, mpc_vp *output);

void encodeeps_batt_state(eps_batt_state *input, CANPacket* output);
void decodeeps_batt_state(CANPacket *input, eps_batt_state *output);

void encodeeps_batt_current(eps_batt_current *input, CANPacket* output);
void decodeeps_batt_current(CANPacket *input, eps_batt_current *output);

void encodeeps_batt_voltage(eps_batt_voltage *input, CANPacket* output);
void decodeeps_batt_voltage(CANPacket *input, eps_batt_voltage *output);

void encodesensorproc_sun(sensorproc_sun *input, CANPacket* output);
void decodesensorproc_sun(CANPacket *input, sensorproc_sun *output);

void encodecmd_mtq_bdot(cmd_mtq_bdot *input, CANPacket* output);
void decodecmd_mtq_bdot(CANPacket *input, cmd_mtq_bdot *output);

void encodebdot_tumble_status(bdot_tumble_status *input, CANPacket* output);
void decodebdot_tumble_status(CANPacket *input, bdot_tumble_status *output);

void encodesync_2(sync_2 *input, CANPacket* output);
void decodesync_2(CANPacket *input, sync_2 *output);

void encodesync_1(sync_1 *input, CANPacket* output);
void decodesync_1(CANPacket *input, sync_1 *output);

void encodemsp_temp(msp_temp *input, CANPacket* output);
void decodemsp_temp(CANPacket *input, msp_temp *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
