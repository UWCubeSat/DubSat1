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
#define CAN_ID_SENSORPROC_SUN 33882177
#define CAN_ID_BDOT_COMMAND_DIPOLE 33882145
#define CAN_ID_BDOT_TUMBLE_STATUS 34013216
#define CAN_ID_SYNC_2 65554
#define CAN_ID_SYNC_1 65553
#define CAN_ID_MSP_TEMP 36241427

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
#define CAN_ENUM_SENSORPROC_SUN_STATUS_OVER_ILLUMINATION 12
#define CAN_ENUM_SENSORPROC_SUN_STATUS_UNDER_ILLUMINATION 11
#define CAN_ENUM_SENSORPROC_SUN_STATUS_LIGHT_FROM_OUTSIDE_FOV_5 5
#define CAN_ENUM_SENSORPROC_SUN_STATUS_LIGHT_FROM_OUTSIDE_FOV_4 4
#define CAN_ENUM_SENSORPROC_SUN_STATUS_NO_LIGHT 3
#define CAN_ENUM_SENSORPROC_SUN_STATUS_GOOD_DATA 0
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_TRUE 1
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_FALSE 0

#define CAN_ENUM_AGG_SUM 5
#define CAN_ENUM_AGG_MAX 4
#define CAN_ENUM_AGG_MIN 3
#define CAN_ENUM_AGG_COUNT 2
#define CAN_ENUM_AGG_AVG 1
#define CAN_ENUM_AGG_NONE 0
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_OVER_ILLUMINATION 12
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_UNDER_ILLUMINATION 11
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_LIGHT_FROM_OUTSIDE_FOV_5 5
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_LIGHT_FROM_OUTSIDE_FOV_4 4
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_NO_LIGHT 3
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_GOOD_DATA 0
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
    uint8_t sensorproc_sun_status; //  (No Units)
    int16_t sensorproc_sun_reading_beta; // arcminutes
    int16_t sensorproc_sun_reading_alpha; // arcminutes
} sensorproc_sun;

typedef struct bdot_command_dipole {
    int8_t bdot_command_dipole_z; // mA*m^2
    int8_t bdot_command_dipole_y; // mA*m^2
    int8_t bdot_command_dipole_x; // mA*m^2
} bdot_command_dipole;

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

void encodebdot_command_dipole(bdot_command_dipole *input, CANPacket* output);
void decodebdot_command_dipole(CANPacket *input, bdot_command_dipole *output);

void encodebdot_tumble_status(bdot_tumble_status *input, CANPacket* output);
void decodebdot_tumble_status(CANPacket *input, bdot_tumble_status *output);

void encodesync_2(sync_2 *input, CANPacket* output);
void decodesync_2(CANPacket *input, sync_2 *output);

void encodesync_1(sync_1 *input, CANPacket* output);
void decodesync_1(CANPacket *input, sync_1 *output);

void encodemsp_temp(msp_temp *input, CANPacket* output);
void decodemsp_temp(CANPacket *input, msp_temp *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
