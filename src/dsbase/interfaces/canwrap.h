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

#define CAN_ID_CMD_IGNORE_FSW 304349220
#define CAN_ID_CMD_MTQ_FSW 304349219
#define CAN_ID_EPS_DOMAIN_OUTPUT 304218336
#define CAN_ID_SENSORPROC_IMU 335872066
#define CAN_ID_SENSORPROC_MAG 335872067
#define CAN_ID_CMD_ROLLCALL 1114132
#define CAN_ID_MTQ_ACK 34013232
#define CAN_ID_GEN_PANEL_TEMP 36241619
#define CAN_ID_CMD_GEN_RST 34865424
#define CAN_ID_CMD_PPT_FIRE 34865408
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

#define CAN_ENUM_CMD_IGNORE_FSW_IGNORE_TRUE 1
#define CAN_ENUM_CMD_IGNORE_FSW_IGNORE_FALSE 0
#define CAN_ENUM_CMD_MTQ_FSW_SC_MODE_NOTTUMBLINGCHARLIE 3
#define CAN_ENUM_CMD_MTQ_FSW_SC_MODE_NOTTUMBLINGBOB 2
#define CAN_ENUM_CMD_MTQ_FSW_SC_MODE_NOTTUMBLINGALICE 1
#define CAN_ENUM_CMD_MTQ_FSW_SC_MODE_TUMBLING 0
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_STATE_TRUE 1
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_STATE_FALSE 0
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_OC_TRIP_TRUE 1
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_OC_TRIP_FALSE 0
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_UNK 8
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_COM1 7
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_COM2 6
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_RAHS 5
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_BDOT 4
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_ESTIM 3
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_WHEEL 2
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_EPS 1
#define CAN_ENUM_EPS_DOMAIN_OUTPUT_NUM_PPT 0
#define CAN_ENUM_SENSORPROC_IMU_VALID_TRUE 1
#define CAN_ENUM_SENSORPROC_IMU_VALID_FALSE 0
#define CAN_ENUM_SENSORPROC_MAG_VALID_TRUE 1
#define CAN_ENUM_SENSORPROC_MAG_VALID_FALSE 0
#define CAN_ENUM_CMD_ROLLCALL_STATE_GOOD 1
#define CAN_ENUM_CMD_ROLLCALL_STATE_BAD 0
#define CAN_ENUM_MTQ_ACK_COILS_STATE_TRUE 1
#define CAN_ENUM_MTQ_ACK_COILS_STATE_FALSE 0
#define CAN_ENUM_MTQ_ACK_NODE_RAHS 15
#define CAN_ENUM_MTQ_ACK_NODE_COM_2 14
#define CAN_ENUM_MTQ_ACK_NODE_COM_1 13
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_MTQ 12
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_SENSORPROC 11
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_MPC 10
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_ESTIM 9
#define CAN_ENUM_MTQ_ACK_NODE_EPS_BATT 8
#define CAN_ENUM_MTQ_ACK_NODE_EPS_GEN 7
#define CAN_ENUM_MTQ_ACK_NODE_EPS_DIST 6
#define CAN_ENUM_MTQ_ACK_NODE_PPT 5
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_RWZ 4
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_RWY 3
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_RWX 2
#define CAN_ENUM_MTQ_ACK_NODE_ADCS_BDOT 1
#define CAN_ENUM_MTQ_ACK_NODE_TEST 0
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_SUM 5
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_MAX 4
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_MIN 3
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_COUNT 2
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_AVG 1
#define CAN_ENUM_GEN_PANEL_TEMP_AGG_NONE 0
#define CAN_ENUM_CMD_GEN_RST_CONFIRM_TRUE 1
#define CAN_ENUM_CMD_GEN_RST_CONFIRM_FALSE 0
#define CAN_ENUM_CMD_PPT_FIRE_FIRE_TRUE 1
#define CAN_ENUM_CMD_PPT_FIRE_FIRE_FALSE 0
#define CAN_ENUM_GEN_PANEL_PWR_AGG_SUM 5
#define CAN_ENUM_GEN_PANEL_PWR_AGG_MAX 4
#define CAN_ENUM_GEN_PANEL_PWR_AGG_MIN 3
#define CAN_ENUM_GEN_PANEL_PWR_AGG_COUNT 2
#define CAN_ENUM_GEN_PANEL_PWR_AGG_AVG 1
#define CAN_ENUM_GEN_PANEL_PWR_AGG_NONE 0
#define CAN_ENUM_MPC_VP_STATUS_TRUE 1
#define CAN_ENUM_MPC_VP_STATUS_FALSE 0
#define CAN_ENUM_EPS_BATT_STATE_BAL_TRUE 1
#define CAN_ENUM_EPS_BATT_STATE_BAL_FALSE 0
#define CAN_ENUM_EPS_BATT_STATE_HEAT_TRUE 1
#define CAN_ENUM_EPS_BATT_STATE_HEAT_FALSE 0
#define CAN_ENUM_SENSORPROC_SUN_VALID_TRUE 1
#define CAN_ENUM_SENSORPROC_SUN_VALID_FALSE 0
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_TRUE 1
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_FALSE 0

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
    int16_t sensorproc_imu_z; //  (No Units)
    int16_t sensorproc_imu_y; //  (No Units)
    int16_t sensorproc_imu_x; //  (No Units)
    uint8_t sensorproc_imu_valid; // bool
} sensorproc_imu;

typedef struct sensorproc_mag {
    int16_t sensorproc_mag_z; //  (No Units)
    int16_t sensorproc_mag_y; //  (No Units)
    int16_t sensorproc_mag_x; //  (No Units)
    uint8_t sensorproc_mag_valid; // bool
} sensorproc_mag;

typedef struct cmd_rollcall {
    uint8_t cmd_rollcall_type; //  (No Units)
    uint8_t cmd_rollcall_met_overflow; //  (No Units)
    uint32_t cmd_rollcall_met; //  (No Units)
    uint8_t cmd_rollcall_state; //  (No Units)
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

typedef struct cmd_ppt_fire {
    uint8_t cmd_ppt_fire_fire; // bool
} cmd_ppt_fire;

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

void encodecmd_ppt_fire(cmd_ppt_fire *input, CANPacket* output);
void decodecmd_ppt_fire(CANPacket *input, cmd_ppt_fire *output);

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
