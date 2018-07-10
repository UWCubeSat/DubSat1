/*
 * canwrap.h
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include <inttypes.h>
#include "../core/can.h"


#ifndef DSBASE_INTERFACES_CANWRAP_H_
#define DSBASE_INTERFACES_CANWRAP_H_

#define PARAM_ADCS_STATUS_VELOCITY_RPM 0x4201;

// the initial value assigned to the coils before commands are received
#define ELOISE_UNKNOWN -128 
// what phase the mtq is in (needed by fsw and bdot)
#define MEASUREMENT_PHASE CAN_ENUM_MTQ_PHASE_MEASUREMENT_PHASE 
#define ACTUATION_PHASE CAN_ENUM_MTQ_PHASE_ACTUATION_PHASE
#define PMS_PHASE CAN_ENUM_MTQ_PHASE_PMS_PHASE
#define FROM_FSW_IS_ZERO 0
#define FROM_BDOT_IS_ONE 1
#define FROM_FSW FROM_FSW_IS_ZERO
#define FROM_BDOT FROM_BDOT_IS_ONE

// BEGIN GENERATOR MACROS

#define CAN_ID_RC_EPS_DIST_19 303039196
#define CAN_ID_RC_EPS_DIST_18 303039195
#define CAN_ID_GCMD_BATT_SET_HEATER_CHECK 303039194
#define CAN_ID_DIST_AUTOSEQ_GET_MET_RSP 303039193
#define CAN_ID_DIST_AUTOSEQ_GET_IND_RSP 303039192
#define CAN_ID_GCMD_AUTOSEQ_GET_MET 302252759
#define CAN_ID_GCMD_AUTOSEQ_GET_INDICES 302252758
#define CAN_ID_GCMD_AUTOSEQ_REMOVE_CAN_ID 302252757
#define CAN_ID_GCMD_AUTOSEQ_RM_AT_INDEX 302252756
#define CAN_ID_GCMD_AUTOSEQ_ADD_2 302252755
#define CAN_ID_GCMD_AUTOSEQ_ADD_1 302252754
#define CAN_ID_RC_ADCS_BDOT_7 304677489
#define CAN_ID_RC_ADCS_BDOT_6 304677488
#define CAN_ID_GCMD_PPT_MULTIPLE_FIRE 302252753
#define CAN_ID_GCMD_MTQ_PWM_TIME 302252752
#define CAN_ID_GCMD_EPS_BATT_FULLDEF 302252751
#define CAN_ID_RC_ADCS_BDOT_5 304677468
#define CAN_ID_GCMD_RESET_MINMAX 302252750
#define CAN_ID_RC_EPS_BATT_H2 304677486
#define CAN_ID_RC_ADCS_BDOT_H2 304677485
#define CAN_ID_RC_EPS_DIST_H2 304677484
#define CAN_ID_RC_ADCS_MPC_H2 304677483
#define CAN_ID_RC_ADCS_ESTIM_H2 304677482
#define CAN_ID_RC_ADCS_SP_H2 304677481
#define CAN_ID_RC_EPS_GEN_H2 304677480
#define CAN_ID_RC_ADCS_MTQ_H2 304677479
#define CAN_ID_RC_PPT_H2 304677478
#define CAN_ID_RC_EPS_BATT_H1 304677477
#define CAN_ID_RC_ADCS_BDOT_H1 304677476
#define CAN_ID_RC_EPS_DIST_H1 304677475
#define CAN_ID_RC_ADCS_MPC_H1 304677474
#define CAN_ID_RC_ADCS_ESTIM_H1 304677473
#define CAN_ID_RC_ADCS_SP_H1 304677472
#define CAN_ID_RC_EPS_GEN_H1 304677471
#define CAN_ID_RC_ADCS_MTQ_H1 304677470
#define CAN_ID_RC_PPT_H1 304677469
#define CAN_ID_GCMD_MTQ_PMS 302252749
#define CAN_ID_GCMD_BDOT_MAX_TUMBLE 302252748
#define CAN_ID_GCMD_BDOT_SPAM 302252747
#define CAN_ID_GCMD_BDOT_CONTROL 302252746
#define CAN_ID_GCMD_BDOT_MAG_CONTROL 302252745
#define CAN_ID_GCMD_BDOT_POLE_OVERRIDE 302252744
#define CAN_ID_GCMD_GEN_SET_PT_STATE 302252742
#define CAN_ID_GCMD_SP_SET_THRESH 302252743
#define CAN_ID_GCMD_DIST_SET_PD_OVC_PPT 302252741
#define CAN_ID_GCMD_DIST_SET_PD_OVC_EPS 302252740
#define CAN_ID_GCMD_DIST_SET_PD_OVC_WHEELS 302252739
#define CAN_ID_GCMD_DIST_SET_PD_OVC_ESTIM 302252738
#define CAN_ID_GCMD_DIST_SET_PD_OVC_BDOT 302252737
#define CAN_ID_GCMD_DIST_SET_PD_OVC_RAHS 302252736
#define CAN_ID_GCMD_DIST_SET_PD_OVC_COM2 302252735
#define CAN_ID_GCMD_DIST_SET_PD_OVC_COM1 302252734
#define CAN_ID_GCMD_DIST_SET_PD_STATE 302252733
#define CAN_ID_GCMD_MTQ_POP 302449340
#define CAN_ID_RC_EPS_BATT_7 304677466
#define CAN_ID_SENSORPROC_MAG2 335872068
#define CAN_ID_RC_ADCS_ESTIM_14 304677465
#define CAN_ID_RC_ADCS_ESTIM_13 304677464
#define CAN_ID_RC_ADCS_ESTIM_12 304677463
#define CAN_ID_RC_ADCS_ESTIM_11 304677462
#define CAN_ID_RC_ADCS_ESTIM_10 304677461
#define CAN_ID_RC_ADCS_ESTIM_9 304677460
#define CAN_ID_RC_EPS_DIST_16 304677458
#define CAN_ID_RC_EPS_DIST_9 304677451
#define CAN_ID_RC_EPS_DIST_7 304677449
#define CAN_ID_RC_EPS_DIST_6 304677448
#define CAN_ID_RC_EPS_DIST_5 304677447
#define CAN_ID_RC_EPS_DIST_3 304677445
#define CAN_ID_RC_EPS_DIST_2 304677444
#define CAN_ID_RC_EPS_DIST_17 304677459
#define CAN_ID_RC_EPS_DIST_14 304677456
#define CAN_ID_RC_EPS_DIST_13 304677455
#define CAN_ID_RC_EPS_DIST_11 304677453
#define CAN_ID_RC_EPS_DIST_12 304677454
#define CAN_ID_RC_EPS_DIST_15 304677457
#define CAN_ID_RC_EPS_DIST_10 304677452
#define CAN_ID_RC_EPS_DIST_8 304677450
#define CAN_ID_RC_EPS_DIST_4 304677446
#define CAN_ID_RC_EPS_DIST_1 304677443
#define CAN_ID_RC_ADCS_MPC_11 304677438
#define CAN_ID_RC_ADCS_MPC_7 304677434
#define CAN_ID_RC_ADCS_MPC_8 304677435
#define CAN_ID_RC_ADCS_MPC_9 304677436
#define CAN_ID_RC_ADCS_MPC_5 304677432
#define CAN_ID_RC_ADCS_MPC_6 304677433
#define CAN_ID_RC_ADCS_MPC_4 304677431
#define CAN_ID_RC_ADCS_MPC_3 304677430
#define CAN_ID_RC_ADCS_MPC_15 304677442
#define CAN_ID_RC_ADCS_MPC_12 304677439
#define CAN_ID_RC_ADCS_MPC_13 304677440
#define CAN_ID_RC_ADCS_MPC_14 304677441
#define CAN_ID_RC_ADCS_MPC_10 304677437
#define CAN_ID_RC_ADCS_MPC_2 304677429
#define CAN_ID_RC_ADCS_MPC_1 304677428
#define CAN_ID_RC_ADCS_ESTIM_8 304677427
#define CAN_ID_RC_ADCS_ESTIM_7 304677426
#define CAN_ID_RC_ADCS_ESTIM_6 304677425
#define CAN_ID_RC_ADCS_ESTIM_5 304677424
#define CAN_ID_RC_ADCS_ESTIM_4 304677423
#define CAN_ID_RC_ADCS_ESTIM_2 304677421
#define CAN_ID_RC_ADCS_ESTIM_3 304677422
#define CAN_ID_RC_ADCS_ESTIM_1 304677420
#define CAN_ID_RC_ADCS_SP_12 304677414
#define CAN_ID_RC_ADCS_SP_11 304677413
#define CAN_ID_RC_ADCS_SP_7 304677409
#define CAN_ID_RC_ADCS_SP_6 304677408
#define CAN_ID_RC_ADCS_SP_17 304677419
#define CAN_ID_RC_ADCS_SP_16 304677418
#define CAN_ID_RC_ADCS_SP_10 304677412
#define CAN_ID_RC_ADCS_SP_8 304677410
#define CAN_ID_RC_ADCS_SP_9 304677411
#define CAN_ID_RC_ADCS_SP_13 304677415
#define CAN_ID_RC_ADCS_SP_15 304677417
#define CAN_ID_RC_ADCS_SP_14 304677416
#define CAN_ID_RC_ADCS_SP_5 304677407
#define CAN_ID_RC_ADCS_SP_4 304677406
#define CAN_ID_RC_ADCS_SP_3 304677405
#define CAN_ID_RC_ADCS_SP_2 304677404
#define CAN_ID_RC_ADCS_SP_1 304677403
#define CAN_ID_RC_ADCS_BDOT_4 304677402
#define CAN_ID_RC_ADCS_BDOT_3 304677401
#define CAN_ID_RC_ADCS_BDOT_2 304677383
#define CAN_ID_RC_ADCS_BDOT_1 304677382
#define CAN_ID_ESTIM_SUN_UNIT_Z 302449332
#define CAN_ID_ESTIM_SUN_UNIT_Y 302449331
#define CAN_ID_ESTIM_SUN_UNIT_X 302449330
#define CAN_ID_ESTIM_MAG_UNIT_Z 302449335
#define CAN_ID_ESTIM_MAG_UNIT_Y 302449334
#define CAN_ID_ESTIM_MAG_UNIT_X 302449333
#define CAN_ID_ESTIM_STATE 302449336
#define CAN_ID_RC_ADCS_MTQ_5 304677391
#define CAN_ID_RC_ADCS_MTQ_4 304677390
#define CAN_ID_RC_ADCS_MTQ_3 304677389
#define CAN_ID_RC_ADCS_MTQ_2 304677388
#define CAN_ID_RC_ADCS_MTQ_1 304677387
#define CAN_ID_RC_PPT_3 304677386
#define CAN_ID_RC_PPT_2 304677385
#define CAN_ID_RC_PPT_1 304677384
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
#define CAN_ID_MTQ_ACK 302448688
#define CAN_ID_GEN_PANEL_TEMP 304677075
#define CAN_ID_CMD_GEN_RST 303300880
#define CAN_ID_CMD_PPT_SINGLE_FIRE 303300864
#define CAN_ID_GEN_PANEL_PWR 304218322
#define CAN_ID_GEN_PANEL_CURRENT 304218321
#define CAN_ID_GEN_PANEL_VOLTAGE 303169744
#define CAN_ID_MPC_VP 305594402
#define CAN_ID_EPS_BATT_STATE 303169730
#define CAN_ID_EPS_BATT_CURRENT 303169729
#define CAN_ID_EPS_BATT_VOLTAGE 303169728
#define CAN_ID_SENSORPROC_SUN 335872065
#define CAN_ID_CMD_MTQ_BDOT 304545825
#define CAN_ID_BDOT_TUMBLE_STATUS 302448672
#define CAN_ID_SYNC_2 65554
#define CAN_ID_SYNC_1 65553
#define CAN_ID_MSP_TEMP 304676883
#define CAN_ID_GRND_EPOCH 302449337

#define CAN_ENUM_FIRE_RESULT_NOMAINDISCHARGE 2
#define CAN_ENUM_FIRE_RESULT_NOMAINCHARGE 1
#define CAN_ENUM_FIRE_RESULT_FIRESUCCESSFUL 0
#define CAN_ENUM_MTQ_PHASE_PMS_PHASE 2
#define CAN_ENUM_MTQ_PHASE_ACTUATION_PHASE 1
#define CAN_ENUM_MTQ_PHASE_MEASUREMENT_PHASE 0
#define CAN_ENUM_BDOT_MAG_MODE_SP2MODE 3
#define CAN_ENUM_BDOT_MAG_MODE_SP1MODE 2
#define CAN_ENUM_BDOT_MAG_MODE_BDOTMODE 1
#define CAN_ENUM_BDOT_MAG_MODE_AUTOMODE 0
#define CAN_ENUM_BDOT_CONTROL_MODE_SPAM 3
#define CAN_ENUM_BDOT_CONTROL_MODE_SPAMMAGSELFTEST 2
#define CAN_ENUM_BDOT_CONTROL_MODE_SLEEPMODE 1
#define CAN_ENUM_BDOT_CONTROL_MODE_NORMALMODE 0
#define CAN_ENUM_DOMAIN_STATE_OFF_BATT_UNDERVOLTAGE 3
#define CAN_ENUM_DOMAIN_STATE_OFF_OVERCURRENT 2
#define CAN_ENUM_DOMAIN_STATE_OFF_MANUAL 1
#define CAN_ENUM_DOMAIN_STATE_ON 0
#define CAN_ENUM_NBOOL_NULL 2
#define CAN_ENUM_NBOOL_TRUE 1
#define CAN_ENUM_NBOOL_FALSE 0
#define CAN_ENUM_FIRE_STATE_NODISCHARGE 3
#define CAN_ENUM_FIRE_STATE_NOCHARGE 2
#define CAN_ENUM_FIRE_STATE_FIREOVERRIDDEN 1
#define CAN_ENUM_FIRE_STATE_FIRESUCCESSFUL 0
#define CAN_ENUM_UV_STATE_UNDERVOLTAGE 1
#define CAN_ENUM_UV_STATE_NORMAL 0
#define CAN_ENUM_MODE_LOWPOWER 4
#define CAN_ENUM_MODE_POINTING 3
#define CAN_ENUM_MODE_BDOT 1
#define CAN_ENUM_MODE_FSWOFF 0
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

// Returns 0 if sending was successful
// Returns not zero if it couldn't right now.
uint8_t canSendPacket(CANPacket *packet);

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet));
typedef struct rc_eps_dist_19 {
    uint16_t rc_eps_dist_19_rahs_ocp_thresh; // ocpThresh
} rc_eps_dist_19;

typedef struct rc_eps_dist_18 {
    uint16_t rc_eps_dist_18_ppt_ocp_thresh; // ocpThresh
    uint16_t rc_eps_dist_18_estim_ocp_thresh; // ocpThresh
    uint16_t rc_eps_dist_18_eps_ocp_thresh; // ocpThresh
    uint16_t rc_eps_dist_18_com2_ocp_thresh; // ocpThresh
    uint16_t rc_eps_dist_18_com1_ocp_thresh; // ocpThresh
    uint16_t rc_eps_dist_18_bdot_ocp_thresh; // ocpThresh
} rc_eps_dist_18;

typedef struct gcmd_batt_set_heater_check {
    uint8_t gcmd_batt_set_heater_check_state; //  (No Units)
} gcmd_batt_set_heater_check;

typedef struct dist_autoseq_get_met_rsp {
    uint32_t dist_autoseq_get_met_rsp_met; // 2^-15s
} dist_autoseq_get_met_rsp;

typedef struct dist_autoseq_get_ind_rsp {
    uint64_t dist_autoseq_get_ind_rsp_indices; //  (No Units)
} dist_autoseq_get_ind_rsp;

typedef struct gcmd_autoseq_get_met {
    uint8_t gcmd_autoseq_get_met_index; //  (No Units)
} gcmd_autoseq_get_met;

typedef struct gcmd_autoseq_get_indices {
    uint32_t gcmd_autoseq_get_indices_id; //  (No Units)
} gcmd_autoseq_get_indices;

typedef struct gcmd_autoseq_remove_can_id {
    uint32_t gcmd_autoseq_remove_can_id_id; //  (No Units)
} gcmd_autoseq_remove_can_id;

typedef struct gcmd_autoseq_rm_at_index {
    uint8_t gcmd_autoseq_rm_at_index_index; //  (No Units)
} gcmd_autoseq_rm_at_index;

typedef struct gcmd_autoseq_add_2 {
    uint64_t gcmd_autoseq_add_2_data; //  (No Units)
} gcmd_autoseq_add_2;

typedef struct gcmd_autoseq_add_1 {
    uint8_t gcmd_autoseq_add_1_sendflg; //  (No Units)
    uint32_t gcmd_autoseq_add_1_met; // 2^-15s
    uint32_t gcmd_autoseq_add_1_can_id; //  (No Units)
} gcmd_autoseq_add_1;

typedef struct rc_adcs_bdot_7 {
    int16_t rc_adcs_bdot_7_spam_magnitude_z; //  (No Units)
    int16_t rc_adcs_bdot_7_spam_magnitude_y; //  (No Units)
    int16_t rc_adcs_bdot_7_spam_magnitude_x; //  (No Units)
} rc_adcs_bdot_7;

typedef struct rc_adcs_bdot_6 {
    uint16_t rc_adcs_bdot_6_spam_on_time; // minutes
    uint16_t rc_adcs_bdot_6_spam_off_time; // minutes
    uint8_t rc_adcs_bdot_6_spam_control; //  (No Units)
    uint8_t rc_adcs_bdot_6_pop_status_z; //  (No Units)
    uint8_t rc_adcs_bdot_6_pop_status_y; //  (No Units)
    uint8_t rc_adcs_bdot_6_pop_status_x; //  (No Units)
    uint16_t rc_adcs_bdot_6_max_tumble_time; // minutes
    uint8_t rc_adcs_bdot_6_mag_control; //  (No Units)
    uint8_t rc_adcs_bdot_6_gain_ovr_status_z; //  (No Units)
    uint8_t rc_adcs_bdot_6_gain_ovr_status_y; //  (No Units)
    uint8_t rc_adcs_bdot_6_gain_ovr_status_x; //  (No Units)
    uint8_t rc_adcs_bdot_6_current_state; //  (No Units)
} rc_adcs_bdot_6;

typedef struct gcmd_ppt_multiple_fire {
    uint8_t gcmd_ppt_multiple_fire_override; //  (No Units)
    uint8_t gcmd_ppt_multiple_fire_count; //  (No Units)
} gcmd_ppt_multiple_fire;

typedef struct gcmd_mtq_pwm_time {
    uint8_t gcmd_mtq_pwm_time_measurement; // seconds
    uint8_t gcmd_mtq_pwm_time_actuation; // seconds
} gcmd_mtq_pwm_time;

typedef struct gcmd_eps_batt_fulldef {
    float gcmd_eps_batt_fulldef_const_volt; // V
    float gcmd_eps_batt_fulldef_chg_curr; // Amps
} gcmd_eps_batt_fulldef;

typedef struct rc_adcs_bdot_5 {
    int16_t rc_adcs_bdot_5_last_spam_z_mtq_z; // 1/73 nT
    int16_t rc_adcs_bdot_5_last_spam_z_mtq_y; // 1/73 nT
    int16_t rc_adcs_bdot_5_last_spam_z_mtq_x; // 1/73 nT
} rc_adcs_bdot_5;

typedef struct gcmd_reset_minmax {
} gcmd_reset_minmax;

typedef struct rc_eps_batt_h2 {
    uint8_t rc_eps_batt_h2_canrxerror; //  (No Units)
} rc_eps_batt_h2;

typedef struct rc_adcs_bdot_h2 {
    uint8_t rc_adcs_bdot_h2_canrxerror; //  (No Units)
} rc_adcs_bdot_h2;

typedef struct rc_eps_dist_h2 {
    uint8_t rc_eps_dist_h2_canrxerror; //  (No Units)
} rc_eps_dist_h2;

typedef struct rc_adcs_mpc_h2 {
    uint8_t rc_adcs_mpc_h2_canrxerror; //  (No Units)
} rc_adcs_mpc_h2;

typedef struct rc_adcs_estim_h2 {
    uint8_t rc_adcs_estim_h2_canrxerror; //  (No Units)
} rc_adcs_estim_h2;

typedef struct rc_adcs_sp_h2 {
    uint8_t rc_adcs_sp_h2_canrxerror; //  (No Units)
} rc_adcs_sp_h2;

typedef struct rc_eps_gen_h2 {
    uint8_t rc_eps_gen_h2_canrxerror; //  (No Units)
} rc_eps_gen_h2;

typedef struct rc_adcs_mtq_h2 {
    uint8_t rc_adcs_mtq_h2_canrxerror; //  (No Units)
} rc_adcs_mtq_h2;

typedef struct rc_ppt_h2 {
    uint8_t rc_ppt_h2_last_fire_result; //  (No Units)
    uint8_t rc_ppt_h2_canrxerror; //  (No Units)
} rc_ppt_h2;

typedef struct rc_eps_batt_h1 {
    int16_t rc_eps_batt_h1_temp_min; // cC
    int16_t rc_eps_batt_h1_temp_max; // cC
    int16_t rc_eps_batt_h1_temp_avg; // cC
    uint8_t rc_eps_batt_h1_sysrstiv; //  (No Units)
    uint8_t rc_eps_batt_h1_reset_count; //  (No Units)
} rc_eps_batt_h1;

typedef struct rc_adcs_bdot_h1 {
    int16_t rc_adcs_bdot_h1_temp_min; // cC
    int16_t rc_adcs_bdot_h1_temp_max; // cC
    int16_t rc_adcs_bdot_h1_temp_avg; // cC
    uint8_t rc_adcs_bdot_h1_sysrstiv; //  (No Units)
    uint8_t rc_adcs_bdot_h1_reset_count; //  (No Units)
} rc_adcs_bdot_h1;

typedef struct rc_eps_dist_h1 {
    int16_t rc_eps_dist_h1_temp_min; // cC
    int16_t rc_eps_dist_h1_temp_max; // cC
    int16_t rc_eps_dist_h1_temp_avg; // cC
    uint8_t rc_eps_dist_h1_sysrstiv; //  (No Units)
    uint8_t rc_eps_dist_h1_reset_count; //  (No Units)
} rc_eps_dist_h1;

typedef struct rc_adcs_mpc_h1 {
    int16_t rc_adcs_mpc_h1_temp_min; // cC
    int16_t rc_adcs_mpc_h1_temp_max; // cC
    int16_t rc_adcs_mpc_h1_temp_avg; // cC
    uint8_t rc_adcs_mpc_h1_sysrstiv; //  (No Units)
    uint8_t rc_adcs_mpc_h1_reset_count; //  (No Units)
} rc_adcs_mpc_h1;

typedef struct rc_adcs_estim_h1 {
    int16_t rc_adcs_estim_h1_temp_min; // cC
    int16_t rc_adcs_estim_h1_temp_max; // cC
    int16_t rc_adcs_estim_h1_temp_avg; // cC
    uint8_t rc_adcs_estim_h1_sysrstiv; //  (No Units)
    uint8_t rc_adcs_estim_h1_reset_count; //  (No Units)
} rc_adcs_estim_h1;

typedef struct rc_adcs_sp_h1 {
    int16_t rc_adcs_sp_h1_temp_min; // cC
    int16_t rc_adcs_sp_h1_temp_max; // cC
    int16_t rc_adcs_sp_h1_temp_avg; // cC
    uint8_t rc_adcs_sp_h1_sysrstiv; //  (No Units)
    uint8_t rc_adcs_sp_h1_reset_count; //  (No Units)
} rc_adcs_sp_h1;

typedef struct rc_eps_gen_h1 {
    int16_t rc_eps_gen_h1_temp_min; // cC
    int16_t rc_eps_gen_h1_temp_max; // cC
    int16_t rc_eps_gen_h1_temp_avg; // cC
    uint8_t rc_eps_gen_h1_sysrstiv; //  (No Units)
    uint8_t rc_eps_gen_h1_reset_count; //  (No Units)
} rc_eps_gen_h1;

typedef struct rc_adcs_mtq_h1 {
    int16_t rc_adcs_mtq_h1_temp_min; // cC
    int16_t rc_adcs_mtq_h1_temp_max; // cC
    int16_t rc_adcs_mtq_h1_temp_avg; // cC
    uint8_t rc_adcs_mtq_h1_sysrstiv; //  (No Units)
    uint8_t rc_adcs_mtq_h1_reset_count; //  (No Units)
} rc_adcs_mtq_h1;

typedef struct rc_ppt_h1 {
    int16_t rc_ppt_h1_temp_min; // cC
    int16_t rc_ppt_h1_temp_max; // cC
    int16_t rc_ppt_h1_temp_avg; // cC
    uint8_t rc_ppt_h1_sysrstiv; //  (No Units)
    uint8_t rc_ppt_h1_reset_count; //  (No Units)
} rc_ppt_h1;

typedef struct gcmd_mtq_pms {
    int8_t gcmd_mtq_pms_z; //  (No Units)
    int8_t gcmd_mtq_pms_y; //  (No Units)
    int8_t gcmd_mtq_pms_x; //  (No Units)
    uint8_t gcmd_mtq_pms_enable; //  (No Units)
} gcmd_mtq_pms;

typedef struct gcmd_bdot_max_tumble {
    uint16_t gcmd_bdot_max_tumble_time; // minutes
} gcmd_bdot_max_tumble;

typedef struct gcmd_bdot_spam {
    int8_t gcmd_bdot_spam_magnitude_z; //  (No Units)
    int8_t gcmd_bdot_spam_magnitude_y; //  (No Units)
    int8_t gcmd_bdot_spam_magnitude_x; //  (No Units)
    uint16_t gcmd_bdot_spam_time_on; // minutes
    uint16_t gcmd_bdot_spam_time_off; // minutes
    uint8_t gcmd_bdot_spam_control; //  (No Units)
} gcmd_bdot_spam;

typedef struct gcmd_bdot_control {
    uint8_t gcmd_bdot_control_mode; //  (No Units)
} gcmd_bdot_control;

typedef struct gcmd_bdot_mag_control {
    uint8_t gcmd_bdot_mag_control_mode; //  (No Units)
} gcmd_bdot_mag_control;

typedef struct gcmd_bdot_pole_override {
    uint8_t gcmd_bdot_pole_override_gain_z; // pct
    uint8_t gcmd_bdot_pole_override_gain_y; // pct
    uint8_t gcmd_bdot_pole_override_gain_x; // pct
    uint8_t gcmd_bdot_pole_override_z; //  (No Units)
    uint8_t gcmd_bdot_pole_override_y; //  (No Units)
    uint8_t gcmd_bdot_pole_override_x; //  (No Units)
} gcmd_bdot_pole_override;

typedef struct gcmd_gen_set_pt_state {
    uint8_t gcmd_gen_set_pt_state_3; //  (No Units)
    uint8_t gcmd_gen_set_pt_state_2; //  (No Units)
    uint8_t gcmd_gen_set_pt_state_1; //  (No Units)
} gcmd_gen_set_pt_state;

typedef struct gcmd_sp_set_thresh {
    uint16_t gcmd_sp_set_thresh_thresh; // 1/73 nanoTeslas
} gcmd_sp_set_thresh;

typedef struct gcmd_dist_set_pd_ovc_ppt {
    float gcmd_dist_set_pd_ovc_ppt_ovc; // A
} gcmd_dist_set_pd_ovc_ppt;

typedef struct gcmd_dist_set_pd_ovc_eps {
    float gcmd_dist_set_pd_ovc_eps_ovc; // A
} gcmd_dist_set_pd_ovc_eps;

typedef struct gcmd_dist_set_pd_ovc_wheels {
    float gcmd_dist_set_pd_ovc_wheels_ovc; // A
} gcmd_dist_set_pd_ovc_wheels;

typedef struct gcmd_dist_set_pd_ovc_estim {
    float gcmd_dist_set_pd_ovc_estim_ovc; // A
} gcmd_dist_set_pd_ovc_estim;

typedef struct gcmd_dist_set_pd_ovc_bdot {
    float gcmd_dist_set_pd_ovc_bdot_ovc; // A
} gcmd_dist_set_pd_ovc_bdot;

typedef struct gcmd_dist_set_pd_ovc_rahs {
    float gcmd_dist_set_pd_ovc_rahs_ovc; // A
} gcmd_dist_set_pd_ovc_rahs;

typedef struct gcmd_dist_set_pd_ovc_com2 {
    float gcmd_dist_set_pd_ovc_com2_ovc; // A
} gcmd_dist_set_pd_ovc_com2;

typedef struct gcmd_dist_set_pd_ovc_com1 {
    float gcmd_dist_set_pd_ovc_com1_ovc; // A
} gcmd_dist_set_pd_ovc_com1;

typedef struct gcmd_dist_set_pd_state {
    uint8_t gcmd_dist_set_pd_state_wheels; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_rahs; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_ppt; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_estim; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_eps; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_com2; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_com1; //  (No Units)
    uint8_t gcmd_dist_set_pd_state_bdot; //  (No Units)
} gcmd_dist_set_pd_state;

typedef struct gcmd_mtq_pop {
    uint8_t gcmd_mtq_pop_z; //  (No Units)
    uint8_t gcmd_mtq_pop_y; //  (No Units)
    uint8_t gcmd_mtq_pop_x; //  (No Units)
} gcmd_mtq_pop;

typedef struct rc_eps_batt_7 {
    uint16_t rc_eps_batt_7_acc_charge_min; // 22.588 mAh
    uint16_t rc_eps_batt_7_acc_charge_max; // 22.588 mAh
    uint16_t rc_eps_batt_7_acc_charge_avg; // 22.588 mAh
} rc_eps_batt_7;

typedef struct sensorproc_mag2 {
    int16_t sensorproc_mag2_z; // 73 nanoTeslas
    int16_t sensorproc_mag2_y; // 73 nanoTeslas
    int16_t sensorproc_mag2_x; // 73 nanoTeslas
    uint8_t sensorproc_mag2_valid; // bool
} sensorproc_mag2;

typedef struct rc_adcs_estim_14 {
    double rc_adcs_estim_14_mag_z; // u
} rc_adcs_estim_14;

typedef struct rc_adcs_estim_13 {
    double rc_adcs_estim_13_mag_y; // u
} rc_adcs_estim_13;

typedef struct rc_adcs_estim_12 {
    double rc_adcs_estim_12_mag_x; // u
} rc_adcs_estim_12;

typedef struct rc_adcs_estim_11 {
    double rc_adcs_estim_11_sun_z; // u
} rc_adcs_estim_11;

typedef struct rc_adcs_estim_10 {
    double rc_adcs_estim_10_sun_y; // u
} rc_adcs_estim_10;

typedef struct rc_adcs_estim_9 {
    double rc_adcs_estim_9_sun_x; // u
} rc_adcs_estim_9;

typedef struct rc_eps_dist_16 {
    uint8_t rc_eps_dist_16_ppt_state; //  (No Units)
    int16_t rc_eps_dist_16_ppt_c_min; // raw node current dist
    int16_t rc_eps_dist_16_ppt_c_max; // raw node current dist
    int16_t rc_eps_dist_16_ppt_c_avg; // raw node current dist
} rc_eps_dist_16;

typedef struct rc_eps_dist_9 {
    uint16_t rc_eps_dist_9_rahs_v_min; // raw node voltage
    uint16_t rc_eps_dist_9_rahs_v_max; // raw node voltage
    uint16_t rc_eps_dist_9_rahs_v_avg; // raw node voltage
} rc_eps_dist_9;

typedef struct rc_eps_dist_7 {
    uint16_t rc_eps_dist_7_com2_v_min; // raw node voltage
    uint16_t rc_eps_dist_7_com2_v_max; // raw node voltage
    uint16_t rc_eps_dist_7_com2_v_avg; // raw node voltage
} rc_eps_dist_7;

typedef struct rc_eps_dist_6 {
    uint8_t rc_eps_dist_6_com2_state; //  (No Units)
    int16_t rc_eps_dist_6_com2_c_min; // raw node current dist
    int16_t rc_eps_dist_6_com2_c_max; // raw node current dist
    int16_t rc_eps_dist_6_com2_c_avg; // raw node current dist
} rc_eps_dist_6;

typedef struct rc_eps_dist_5 {
    uint16_t rc_eps_dist_5_com1_v_min; // raw node voltage
    uint16_t rc_eps_dist_5_com1_v_max; // raw node voltage
    uint16_t rc_eps_dist_5_com1_v_avg; // raw node voltage
} rc_eps_dist_5;

typedef struct rc_eps_dist_3 {
    uint16_t rc_eps_dist_3_batt_v_avg; // raw dist battery voltage
    uint16_t rc_eps_dist_3_batt_v_max; // raw dist battery voltage
    uint16_t rc_eps_dist_3_batt_v_min; // raw dist battery voltage
} rc_eps_dist_3;

typedef struct rc_eps_dist_2 {
    uint8_t rc_eps_dist_2_uv_state; //  (No Units)
    uint8_t rc_eps_dist_2_met_overflow; //  (No Units)
    uint64_t rc_eps_dist_2_met; // 2^-8 s
} rc_eps_dist_2;

typedef struct rc_eps_dist_17 {
    uint16_t rc_eps_dist_17_ppt_v_min; // raw node voltage
    uint16_t rc_eps_dist_17_ppt_v_max; // raw node voltage
    uint16_t rc_eps_dist_17_ppt_v_avg; // raw node voltage
} rc_eps_dist_17;

typedef struct rc_eps_dist_14 {
    uint8_t rc_eps_dist_14_eps_state; //  (No Units)
    int16_t rc_eps_dist_14_eps_c_min; // raw node current dist
    int16_t rc_eps_dist_14_eps_c_max; // raw node current dist
    int16_t rc_eps_dist_14_eps_c_avg; // raw node current dist
} rc_eps_dist_14;

typedef struct rc_eps_dist_13 {
    uint16_t rc_eps_dist_13_estim_v_min; // raw node voltage
    uint16_t rc_eps_dist_13_estim_v_max; // raw node voltage
    uint16_t rc_eps_dist_13_estim_v_avg; // raw node voltage
} rc_eps_dist_13;

typedef struct rc_eps_dist_11 {
    uint16_t rc_eps_dist_11_bdot_v_min; // raw node voltage
    uint16_t rc_eps_dist_11_bdot_v_max; // raw node voltage
    uint16_t rc_eps_dist_11_bdot_v_avg; // raw node voltage
} rc_eps_dist_11;

typedef struct rc_eps_dist_12 {
    uint8_t rc_eps_dist_12_estim_state; //  (No Units)
    int16_t rc_eps_dist_12_estim_c_min; // raw node current dist
    int16_t rc_eps_dist_12_estim_c_max; // raw node current dist
    int16_t rc_eps_dist_12_estim_c_avg; // raw node current dist
} rc_eps_dist_12;

typedef struct rc_eps_dist_15 {
    uint16_t rc_eps_dist_15_eps_v_min; // raw node voltage
    uint16_t rc_eps_dist_15_eps_v_max; // raw node voltage
    uint16_t rc_eps_dist_15_eps_v_avg; // raw node voltage
} rc_eps_dist_15;

typedef struct rc_eps_dist_10 {
    uint8_t rc_eps_dist_10_bdot_state; //  (No Units)
    int16_t rc_eps_dist_10_bdot_c_min; // raw node current dist
    int16_t rc_eps_dist_10_bdot_c_max; // raw node current dist
    int16_t rc_eps_dist_10_bdot_c_avg; // raw node current dist
} rc_eps_dist_10;

typedef struct rc_eps_dist_8 {
    uint8_t rc_eps_dist_8_rahs_state; //  (No Units)
    int16_t rc_eps_dist_8_rahs_c_min; // raw node current dist
    int16_t rc_eps_dist_8_rahs_c_max; // raw node current dist
    int16_t rc_eps_dist_8_rahs_c_avg; // raw node current dist
} rc_eps_dist_8;

typedef struct rc_eps_dist_4 {
    uint8_t rc_eps_dist_4_com1_state; //  (No Units)
    int16_t rc_eps_dist_4_com1_c_min; // raw node current dist
    int16_t rc_eps_dist_4_com1_c_max; // raw node current dist
    int16_t rc_eps_dist_4_com1_c_avg; // raw node current dist
} rc_eps_dist_4;

typedef struct rc_eps_dist_1 {
} rc_eps_dist_1;

typedef struct rc_adcs_mpc_11 {
    double rc_adcs_mpc_11_omega_z; // r/s
} rc_adcs_mpc_11;

typedef struct rc_adcs_mpc_7 {
    double rc_adcs_mpc_7_omega_max; // r/s
} rc_adcs_mpc_7;

typedef struct rc_adcs_mpc_8 {
    double rc_adcs_mpc_8_omega_avg; // r/s
} rc_adcs_mpc_8;

typedef struct rc_adcs_mpc_9 {
    double rc_adcs_mpc_9_omega_x; // r/s
} rc_adcs_mpc_9;

typedef struct rc_adcs_mpc_5 {
    double rc_adcs_mpc_5_sc_quat_4; // u
} rc_adcs_mpc_5;

typedef struct rc_adcs_mpc_6 {
    double rc_adcs_mpc_6_omega_min; // r/s
} rc_adcs_mpc_6;

typedef struct rc_adcs_mpc_4 {
    double rc_adcs_mpc_4_sc_quat_3; // u
} rc_adcs_mpc_4;

typedef struct rc_adcs_mpc_3 {
    double rc_adcs_mpc_3_sc_quat_2; // u
} rc_adcs_mpc_3;

typedef struct rc_adcs_mpc_15 {
    uint8_t rc_adcs_mpc_15_sc_mode; //  (No Units)
    uint8_t rc_adcs_mpc_15_point_true; //  (No Units)
} rc_adcs_mpc_15;

typedef struct rc_adcs_mpc_12 {
} rc_adcs_mpc_12;

typedef struct rc_adcs_mpc_13 {
} rc_adcs_mpc_13;

typedef struct rc_adcs_mpc_14 {
} rc_adcs_mpc_14;

typedef struct rc_adcs_mpc_10 {
    double rc_adcs_mpc_10_omega_y; // r/s
} rc_adcs_mpc_10;

typedef struct rc_adcs_mpc_2 {
    double rc_adcs_mpc_2_sc_quat_1; // u
} rc_adcs_mpc_2;

typedef struct rc_adcs_mpc_1 {
} rc_adcs_mpc_1;

typedef struct rc_adcs_estim_8 {
    uint8_t rc_adcs_estim_8_tle_id; //  (No Units)
    int8_t rc_adcs_estim_8_sgp4_flag; //  (No Units)
    uint8_t rc_adcs_estim_8_sc_in_sun; //  (No Units)
    uint8_t rc_adcs_estim_8_sc_above_gs; //  (No Units)
    uint8_t rc_adcs_estim_8_epoch_overflow; //  (No Units)
    uint32_t rc_adcs_estim_8_epoch; // 2^-8 s
} rc_adcs_estim_8;

typedef struct rc_adcs_estim_7 {
    double rc_adcs_estim_7_vel_z; // m/s
} rc_adcs_estim_7;

typedef struct rc_adcs_estim_6 {
    double rc_adcs_estim_6_vel_y; // m/s
} rc_adcs_estim_6;

typedef struct rc_adcs_estim_5 {
    double rc_adcs_estim_5_vel_x; // m/s
} rc_adcs_estim_5;

typedef struct rc_adcs_estim_4 {
    double rc_adcs_estim_4_pos_z; // m
} rc_adcs_estim_4;

typedef struct rc_adcs_estim_2 {
    double rc_adcs_estim_2_pos_x; // m
} rc_adcs_estim_2;

typedef struct rc_adcs_estim_3 {
    double rc_adcs_estim_3_pos_y; // m
} rc_adcs_estim_3;

typedef struct rc_adcs_estim_1 {
} rc_adcs_estim_1;

typedef struct rc_adcs_sp_12 {
    int16_t rc_adcs_sp_12_mag2_z_min; // 73 nanoTeslas
    int16_t rc_adcs_sp_12_mag2_z_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_12_mag2_y_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_12_mag2_y_avg; // 73 nanoTeslas
} rc_adcs_sp_12;

typedef struct rc_adcs_sp_11 {
    int16_t rc_adcs_sp_11_mag2_x_avg; // 73 nanoTeslas
    int16_t rc_adcs_sp_11_mag2_x_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_11_mag2_x_min; // 73 nanoTeslas
    int16_t rc_adcs_sp_11_mag2_y_min; // 73 nanoTeslas
} rc_adcs_sp_11;

typedef struct rc_adcs_sp_7 {
} rc_adcs_sp_7;

typedef struct rc_adcs_sp_6 {
    uint8_t rc_adcs_sp_6_mag2_valid; //  (No Units)
    int16_t rc_adcs_sp_6_mag1_x_min; // 73 nanoTeslas
    uint8_t rc_adcs_sp_6_mag1_valid; //  (No Units)
    int16_t rc_adcs_sp_6_sun_z_max; // 1/32768 units
    int16_t rc_adcs_sp_6_sun_z_avg; // 1/32768 units
} rc_adcs_sp_6;

typedef struct rc_adcs_sp_17 {
    int16_t rc_adcs_sp_17_imu_z_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_17_imu_z_avg; // 0.004375 deg/s
} rc_adcs_sp_17;

typedef struct rc_adcs_sp_16 {
    int16_t rc_adcs_sp_16_imu_z_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_16_imu_y_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_16_imu_y_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_16_imu_y_avg; // 0.004375 deg/s
} rc_adcs_sp_16;

typedef struct rc_adcs_sp_10 {
    int16_t rc_adcs_sp_10_mag1_z_min; // 73 nanoTeslas
    int16_t rc_adcs_sp_10_mag1_z_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_10_mag1_z_avg; // 73 nanoTeslas
    int16_t rc_adcs_sp_10_mag1_y_avg; // 73 nanoTeslas
} rc_adcs_sp_10;

typedef struct rc_adcs_sp_8 {
} rc_adcs_sp_8;

typedef struct rc_adcs_sp_9 {
    int16_t rc_adcs_sp_9_mag1_y_min; // 73 nanoTeslas
    int16_t rc_adcs_sp_9_mag1_y_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_9_mag1_x_max; // 73 nanoTeslas
    int16_t rc_adcs_sp_9_mag1_x_avg; // 73 nanoTeslas
} rc_adcs_sp_9;

typedef struct rc_adcs_sp_13 {
    int16_t rc_adcs_sp_13_suna_min; // 60/32767 degrees
    int16_t rc_adcs_sp_13_suna_max; // 60/32767 degrees
    int16_t rc_adcs_sp_13_suna_avg; // 60/32767 degrees
    int16_t rc_adcs_sp_13_mag2_z_avg; // 73 nanoTeslas
} rc_adcs_sp_13;

typedef struct rc_adcs_sp_15 {
    int16_t rc_adcs_sp_15_imu_x_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_15_imu_x_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_15_imu_x_avg; // 0.004375 deg/s
    uint8_t rc_adcs_sp_15_imu_valid; //  (No Units)
} rc_adcs_sp_15;

typedef struct rc_adcs_sp_14 {
    int16_t rc_adcs_sp_14_sunb_min; // 60/32767 degrees
    int16_t rc_adcs_sp_14_sunb_max; // 60/32767 degrees
    int16_t rc_adcs_sp_14_sunb_avg; // 60/32767 degrees
    uint8_t rc_adcs_sp_14_sun_valid; //  (No Units)
} rc_adcs_sp_14;

typedef struct rc_adcs_sp_5 {
    int16_t rc_adcs_sp_5_sun_z_min; // 1/32768 units
    int16_t rc_adcs_sp_5_sun_y_min; // 1/32768 units
    int16_t rc_adcs_sp_5_sun_y_max; // 1/32768 units
    int16_t rc_adcs_sp_5_sun_y_avg; // 1/32768 units
} rc_adcs_sp_5;

typedef struct rc_adcs_sp_4 {
    int16_t rc_adcs_sp_4_sun_x_min; // 1/32768 units
    int16_t rc_adcs_sp_4_sun_x_max; // 1/32768 units
    int16_t rc_adcs_sp_4_sun_x_avg; // 1/32768 units
    int16_t rc_adcs_sp_4_imup_z_avg; // 0.004375 deg/s
} rc_adcs_sp_4;

typedef struct rc_adcs_sp_3 {
    int16_t rc_adcs_sp_3_imup_z_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_3_imup_z_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_3_imup_y_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_3_imup_y_avg; // 0.004375 deg/s
} rc_adcs_sp_3;

typedef struct rc_adcs_sp_2 {
    int16_t rc_adcs_sp_2_imup_y_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_2_imup_x_min; // 0.004375 deg/s
    int16_t rc_adcs_sp_2_imup_x_max; // 0.004375 deg/s
    int16_t rc_adcs_sp_2_imup_x_avg; // 0.004375 deg/s
} rc_adcs_sp_2;

typedef struct rc_adcs_sp_1 {
} rc_adcs_sp_1;

typedef struct rc_adcs_bdot_4 {
    int16_t rc_adcs_bdot_4_last_spam_y_mtq_z; // 1/73 nT
    int16_t rc_adcs_bdot_4_last_spam_y_mtq_y; // 1/73 nT
    int16_t rc_adcs_bdot_4_mag_z_avg; // 1/73 nT
    uint8_t rc_adcs_bdot_4_tumble; //  (No Units)
} rc_adcs_bdot_4;

typedef struct rc_adcs_bdot_3 {
    int16_t rc_adcs_bdot_3_mag_z_min; // 1/73 nT
    int16_t rc_adcs_bdot_3_mag_z_max; // 1/73 nT
    int16_t rc_adcs_bdot_3_mag_y_max; // 1/73 nT
    int16_t rc_adcs_bdot_3_mag_y_avg; // 1/73 nT
} rc_adcs_bdot_3;

typedef struct rc_adcs_bdot_2 {
    int16_t rc_adcs_bdot_2_mag_y_min; // 1/73 nT
    int16_t rc_adcs_bdot_2_mag_x_avg; // 1/73 nT
    int16_t rc_adcs_bdot_2_mag_x_min; // 1/73 nT
    int16_t rc_adcs_bdot_2_mag_x_max; // 1/73 nT
} rc_adcs_bdot_2;

typedef struct rc_adcs_bdot_1 {
    int16_t rc_adcs_bdot_1_last_spam_y_mtq_x; // 1/73 nT
    int16_t rc_adcs_bdot_1_last_spam_x_mtq_z; // 1/73 nT
    int16_t rc_adcs_bdot_1_last_spam_x_mtq_y; // 1/73 nT
    int16_t rc_adcs_bdot_1_last_spam_x_mtq_x; // 1/73 nT
} rc_adcs_bdot_1;

typedef struct estim_sun_unit_z {
    double estim_sun_unit_z_val; // u
} estim_sun_unit_z;

typedef struct estim_sun_unit_y {
    double estim_sun_unit_y_val; // u
} estim_sun_unit_y;

typedef struct estim_sun_unit_x {
    double estim_sun_unit_x_val; // u
} estim_sun_unit_x;

typedef struct estim_mag_unit_z {
    double estim_mag_unit_z_val; // u
} estim_mag_unit_z;

typedef struct estim_mag_unit_y {
    double estim_mag_unit_y_val; // u
} estim_mag_unit_y;

typedef struct estim_mag_unit_x {
    double estim_mag_unit_x_val; // u
} estim_mag_unit_x;

typedef struct estim_state {
    uint8_t estim_state_in_sun; //  (No Units)
    uint8_t estim_state_above_gs; //  (No Units)
} estim_state;

typedef struct rc_adcs_mtq_5 {
    uint8_t rc_adcs_mtq_5_reset_counts; //  (No Units)
    uint8_t rc_adcs_mtq_5_fsw_ignore; //  (No Units)
} rc_adcs_mtq_5;

typedef struct rc_adcs_mtq_4 {
    uint8_t rc_adcs_mtq_4_fsw_z_min; //  (No Units)
    uint8_t rc_adcs_mtq_4_fsw_y_max; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_z2_avg; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_z1_avg; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_y2_avg; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_y1_avg; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_x2_avg; //  (No Units)
    uint8_t rc_adcs_mtq_4_duty_x1_avg; //  (No Units)
} rc_adcs_mtq_4;

typedef struct rc_adcs_mtq_3 {
    uint8_t rc_adcs_mtq_3_fsw_z_avg; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_y_min; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_y_max; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_y_avg; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_x_min; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_x_max; //  (No Units)
    uint8_t rc_adcs_mtq_3_fsw_x_avg; //  (No Units)
    uint8_t rc_adcs_mtq_3_bdot_z_min; //  (No Units)
} rc_adcs_mtq_3;

typedef struct rc_adcs_mtq_2 {
    int8_t rc_adcs_mtq_2_bdot_z_max; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_z_avg; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_y_min; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_y_max; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_y_avg; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_x_min; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_x_max; //  (No Units)
    int8_t rc_adcs_mtq_2_bdot_x_avg; //  (No Units)
} rc_adcs_mtq_2;

typedef struct rc_adcs_mtq_1 {
} rc_adcs_mtq_1;

typedef struct rc_ppt_3 {
    int16_t rc_ppt_3_ign_chg_min; // 2^-15 seconds
    int16_t rc_ppt_3_ign_chg_max; // 2^-15 seconds
    int16_t rc_ppt_3_ign_chg_avg; // 2^-15 seconds
} rc_ppt_3;

typedef struct rc_ppt_2 {
    int16_t rc_ppt_2_total_fire_count; //  (No Units)
    int16_t rc_ppt_2_main_chg_min; // 2^-15 seconds
    int16_t rc_ppt_2_main_chg_max; // 2^-15 seconds
    int16_t rc_ppt_2_main_chg_avg; // 2^-15 seconds
} rc_ppt_2;

typedef struct rc_ppt_1 {
    uint16_t rc_ppt_1_fault_count; // 2^-15s
    uint16_t rc_ppt_1_fire_count; //  (No Units)
} rc_ppt_1;

typedef struct rc_eps_gen_9 {
    int8_t rc_eps_gen_9_pnl_3_temp_min; // raw tmp36
    int8_t rc_eps_gen_9_pnl_3_temp_max; // raw tmp36
    int8_t rc_eps_gen_9_pnl_3_temp_avg; // raw tmp36
    int8_t rc_eps_gen_9_pnl_2_temp_min; // raw tmp36
    int8_t rc_eps_gen_9_pnl_2_temp_max; // raw tmp36
    int8_t rc_eps_gen_9_pnl_2_temp_avg; // raw tmp36
    int8_t rc_eps_gen_9_pnl_1_temp_max; // raw tmp36
    int8_t rc_eps_gen_9_pnl_1_temp_avg; // raw tmp36
} rc_eps_gen_9;

typedef struct rc_eps_gen_8 {
    int16_t rc_eps_gen_8_pnl_3_power_min; // raw power gen
    int16_t rc_eps_gen_8_pnl_3_power_max; // raw power gen
    int16_t rc_eps_gen_8_pnl_3_power_avg; // raw power gen
    int8_t rc_eps_gen_8_pnl_1_temp_min; // raw tmp36
} rc_eps_gen_8;

typedef struct rc_eps_gen_7 {
    int16_t rc_eps_gen_7_pnl_2_power_min; // raw power gen
    int16_t rc_eps_gen_7_pnl_2_power_max; // raw power gen
    int16_t rc_eps_gen_7_pnl_2_power_avg; // raw power gen
    int16_t rc_eps_gen_7_pnl_1_power_avg; // raw power gen
} rc_eps_gen_7;

typedef struct rc_eps_gen_6 {
    int16_t rc_eps_gen_6_pnl_3_current_max; // raw node current gen
    int16_t rc_eps_gen_6_pnl_3_current_avg; // raw node current gen
    int16_t rc_eps_gen_6_pnl_1_power_min; // raw power gen
    int16_t rc_eps_gen_6_pnl_1_power_max; // raw power gen
} rc_eps_gen_6;

typedef struct rc_eps_gen_5 {
    int16_t rc_eps_gen_5_pnl_3_current_min; // raw node current gen
    int16_t rc_eps_gen_5_pnl_2_current_min; // raw node current gen
    int16_t rc_eps_gen_5_pnl_2_current_max; // raw node current gen
    int16_t rc_eps_gen_5_pnl_2_current_avg; // raw node current gen
} rc_eps_gen_5;

typedef struct rc_eps_gen_4 {
    uint16_t rc_eps_gen_4_pnl_3_voltage_avg; // raw node voltage
    int16_t rc_eps_gen_4_pnl_1_current_min; // raw node current gen
    int16_t rc_eps_gen_4_pnl_1_current_max; // raw node current gen
    int16_t rc_eps_gen_4_pnl_1_current_avg; // raw node current gen
} rc_eps_gen_4;

typedef struct rc_eps_gen_3 {
    uint16_t rc_eps_gen_3_pnl_3_voltage_min; // raw node voltage
    uint16_t rc_eps_gen_3_pnl_3_voltage_max; // raw node voltage
    uint16_t rc_eps_gen_3_pnl_2_voltage_max; // raw node voltage
    uint16_t rc_eps_gen_3_pnl_2_voltage_avg; // raw node voltage
} rc_eps_gen_3;

typedef struct rc_eps_gen_2 {
    uint16_t rc_eps_gen_2_pnl_2_voltage_min; // raw node voltage
    uint16_t rc_eps_gen_2_pnl_1_voltage_min; // raw node voltage
    uint16_t rc_eps_gen_2_pnl_1_voltage_max; // raw node voltage
    uint16_t rc_eps_gen_2_pnl_1_voltage_avg; // raw node voltage
} rc_eps_gen_2;

typedef struct rc_eps_gen_1 {
    uint8_t rc_eps_gen_1_pnl_3_charging; //  (No Units)
    uint8_t rc_eps_gen_1_pnl_2_charging; //  (No Units)
    uint8_t rc_eps_gen_1_pnl_1_charging; //  (No Units)
    uint8_t rc_eps_gen_1_pnl_3_enabled; //  (No Units)
    uint8_t rc_eps_gen_1_pnl_2_enabled; //  (No Units)
    uint8_t rc_eps_gen_1_pnl_1_enabled; //  (No Units)
} rc_eps_gen_1;

typedef struct rc_eps_batt_6 {
    uint8_t rc_eps_batt_6_status; //  (No Units)
    uint8_t rc_eps_batt_6_ctrl; //  (No Units)
    uint64_t rc_eps_batt_6_last_charge; // 2^-8 seconds
} rc_eps_batt_6;

typedef struct rc_eps_batt_5 {
    int16_t rc_eps_batt_5_node_c_min; // raw node current batt
    int16_t rc_eps_batt_5_node_c_max; // raw node current batt
    int16_t rc_eps_batt_5_node_c_avg; // raw node current batt
    int8_t rc_eps_batt_5_batt_temp_min; // raw tmp36
    int8_t rc_eps_batt_5_batt_temp_max; // raw tmp36
} rc_eps_batt_5;

typedef struct rc_eps_batt_4 {
    uint16_t rc_eps_batt_4_voltage_min; // raw voltage
    uint16_t rc_eps_batt_4_voltage_max; // raw voltage
    uint16_t rc_eps_batt_4_voltage_avg; // raw voltage
    uint8_t rc_eps_batt_4_heater_state; //  (No Units)
    uint8_t rc_eps_batt_4_balancer_state; //  (No Units)
} rc_eps_batt_4;

typedef struct rc_eps_batt_3 {
    uint16_t rc_eps_batt_3_current_min; // raw current batt
    uint16_t rc_eps_batt_3_current_max; // raw current batt
    uint16_t rc_eps_batt_3_current_avg; // raw current batt
    int8_t rc_eps_batt_3_batt_temp_avg; // raw tmp36
} rc_eps_batt_3;

typedef struct rc_eps_batt_2 {
    uint16_t rc_eps_batt_2_node_v_min; // raw node voltage
    uint16_t rc_eps_batt_2_node_v_max; // raw node voltage
    uint16_t rc_eps_batt_2_node_v_avg; // raw node voltage
} rc_eps_batt_2;

typedef struct rc_eps_batt_1 {
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

typedef struct tle_5 {
    double tle_5_mnm; //  (No Units)
} tle_5;

typedef struct tle_4 {
    float tle_4_aop; //  (No Units)
    float tle_4_raan; //  (No Units)
} tle_4;

typedef struct tle_3 {
    float tle_3_ecc; //  (No Units)
    float tle_3_inc; //  (No Units)
} tle_3;

typedef struct tle_2 {
    double tle_2_day; //  (No Units)
} tle_2;

typedef struct tle_1 {
    float tle_1_mna; //  (No Units)
    float tle_1_bstar; //  (No Units)
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
    int16_t sensorproc_imu_z; // 0.004375 deg/s
    int16_t sensorproc_imu_y; // 0.004375 deg/s
    int16_t sensorproc_imu_x; // 0.004375 deg/s
    uint8_t sensorproc_imu_valid; // bool
} sensorproc_imu;

typedef struct sensorproc_mag {
    int16_t sensorproc_mag_z; // 73 nanoTeslas
    int16_t sensorproc_mag_y; // 73 nanoTeslas
    int16_t sensorproc_mag_x; // 73 nanoTeslas
    uint8_t sensorproc_mag_valid; // bool
} sensorproc_mag;

typedef struct cmd_rollcall {
    uint8_t cmd_rollcall_type; //  (No Units)
    uint8_t cmd_rollcall_met_overflow; //  (No Units)
    uint32_t cmd_rollcall_met; // 2^-8 s
    uint8_t cmd_rollcall_msp; //  (No Units)
} cmd_rollcall;

typedef struct mtq_ack {
    int8_t mtq_ack_source; //  (No Units)
    int8_t mtq_ack_phase; //  (No Units)
    int8_t mtq_ack_last_fsw_z; // pct
    int8_t mtq_ack_last_fsw_y; // pct
    int8_t mtq_ack_last_fsw_x; // pct
    int8_t mtq_ack_last_bdot_z; // pct
    int8_t mtq_ack_last_bdot_y; // pct
    int8_t mtq_ack_last_bdot_x; // pct
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
    int16_t msp_temp_temp; // cC
} msp_temp;

typedef struct grnd_epoch {
    uint8_t grnd_epoch_val_overflow; //  (No Units)
    uint32_t grnd_epoch_val; // 2^-8 s
} grnd_epoch;

void encoderc_eps_dist_19(rc_eps_dist_19 *input, CANPacket* output);
void decoderc_eps_dist_19(CANPacket *input, rc_eps_dist_19 *output);

void encoderc_eps_dist_18(rc_eps_dist_18 *input, CANPacket* output);
void decoderc_eps_dist_18(CANPacket *input, rc_eps_dist_18 *output);

void encodegcmd_batt_set_heater_check(gcmd_batt_set_heater_check *input, CANPacket* output);
void decodegcmd_batt_set_heater_check(CANPacket *input, gcmd_batt_set_heater_check *output);

void encodedist_autoseq_get_met_rsp(dist_autoseq_get_met_rsp *input, CANPacket* output);
void decodedist_autoseq_get_met_rsp(CANPacket *input, dist_autoseq_get_met_rsp *output);

void encodedist_autoseq_get_ind_rsp(dist_autoseq_get_ind_rsp *input, CANPacket* output);
void decodedist_autoseq_get_ind_rsp(CANPacket *input, dist_autoseq_get_ind_rsp *output);

void encodegcmd_autoseq_get_met(gcmd_autoseq_get_met *input, CANPacket* output);
void decodegcmd_autoseq_get_met(CANPacket *input, gcmd_autoseq_get_met *output);

void encodegcmd_autoseq_get_indices(gcmd_autoseq_get_indices *input, CANPacket* output);
void decodegcmd_autoseq_get_indices(CANPacket *input, gcmd_autoseq_get_indices *output);

void encodegcmd_autoseq_remove_can_id(gcmd_autoseq_remove_can_id *input, CANPacket* output);
void decodegcmd_autoseq_remove_can_id(CANPacket *input, gcmd_autoseq_remove_can_id *output);

void encodegcmd_autoseq_rm_at_index(gcmd_autoseq_rm_at_index *input, CANPacket* output);
void decodegcmd_autoseq_rm_at_index(CANPacket *input, gcmd_autoseq_rm_at_index *output);

void encodegcmd_autoseq_add_2(gcmd_autoseq_add_2 *input, CANPacket* output);
void decodegcmd_autoseq_add_2(CANPacket *input, gcmd_autoseq_add_2 *output);

void encodegcmd_autoseq_add_1(gcmd_autoseq_add_1 *input, CANPacket* output);
void decodegcmd_autoseq_add_1(CANPacket *input, gcmd_autoseq_add_1 *output);

void encoderc_adcs_bdot_7(rc_adcs_bdot_7 *input, CANPacket* output);
void decoderc_adcs_bdot_7(CANPacket *input, rc_adcs_bdot_7 *output);

void encoderc_adcs_bdot_6(rc_adcs_bdot_6 *input, CANPacket* output);
void decoderc_adcs_bdot_6(CANPacket *input, rc_adcs_bdot_6 *output);

void encodegcmd_ppt_multiple_fire(gcmd_ppt_multiple_fire *input, CANPacket* output);
void decodegcmd_ppt_multiple_fire(CANPacket *input, gcmd_ppt_multiple_fire *output);

void encodegcmd_mtq_pwm_time(gcmd_mtq_pwm_time *input, CANPacket* output);
void decodegcmd_mtq_pwm_time(CANPacket *input, gcmd_mtq_pwm_time *output);

void encodegcmd_eps_batt_fulldef(gcmd_eps_batt_fulldef *input, CANPacket* output);
void decodegcmd_eps_batt_fulldef(CANPacket *input, gcmd_eps_batt_fulldef *output);

void encoderc_adcs_bdot_5(rc_adcs_bdot_5 *input, CANPacket* output);
void decoderc_adcs_bdot_5(CANPacket *input, rc_adcs_bdot_5 *output);

void encodegcmd_reset_minmax(gcmd_reset_minmax *input, CANPacket* output);
void decodegcmd_reset_minmax(CANPacket *input, gcmd_reset_minmax *output);

void encoderc_eps_batt_h2(rc_eps_batt_h2 *input, CANPacket* output);
void decoderc_eps_batt_h2(CANPacket *input, rc_eps_batt_h2 *output);

void encoderc_adcs_bdot_h2(rc_adcs_bdot_h2 *input, CANPacket* output);
void decoderc_adcs_bdot_h2(CANPacket *input, rc_adcs_bdot_h2 *output);

void encoderc_eps_dist_h2(rc_eps_dist_h2 *input, CANPacket* output);
void decoderc_eps_dist_h2(CANPacket *input, rc_eps_dist_h2 *output);

void encoderc_adcs_mpc_h2(rc_adcs_mpc_h2 *input, CANPacket* output);
void decoderc_adcs_mpc_h2(CANPacket *input, rc_adcs_mpc_h2 *output);

void encoderc_adcs_estim_h2(rc_adcs_estim_h2 *input, CANPacket* output);
void decoderc_adcs_estim_h2(CANPacket *input, rc_adcs_estim_h2 *output);

void encoderc_adcs_sp_h2(rc_adcs_sp_h2 *input, CANPacket* output);
void decoderc_adcs_sp_h2(CANPacket *input, rc_adcs_sp_h2 *output);

void encoderc_eps_gen_h2(rc_eps_gen_h2 *input, CANPacket* output);
void decoderc_eps_gen_h2(CANPacket *input, rc_eps_gen_h2 *output);

void encoderc_adcs_mtq_h2(rc_adcs_mtq_h2 *input, CANPacket* output);
void decoderc_adcs_mtq_h2(CANPacket *input, rc_adcs_mtq_h2 *output);

void encoderc_ppt_h2(rc_ppt_h2 *input, CANPacket* output);
void decoderc_ppt_h2(CANPacket *input, rc_ppt_h2 *output);

void encoderc_eps_batt_h1(rc_eps_batt_h1 *input, CANPacket* output);
void decoderc_eps_batt_h1(CANPacket *input, rc_eps_batt_h1 *output);

void encoderc_adcs_bdot_h1(rc_adcs_bdot_h1 *input, CANPacket* output);
void decoderc_adcs_bdot_h1(CANPacket *input, rc_adcs_bdot_h1 *output);

void encoderc_eps_dist_h1(rc_eps_dist_h1 *input, CANPacket* output);
void decoderc_eps_dist_h1(CANPacket *input, rc_eps_dist_h1 *output);

void encoderc_adcs_mpc_h1(rc_adcs_mpc_h1 *input, CANPacket* output);
void decoderc_adcs_mpc_h1(CANPacket *input, rc_adcs_mpc_h1 *output);

void encoderc_adcs_estim_h1(rc_adcs_estim_h1 *input, CANPacket* output);
void decoderc_adcs_estim_h1(CANPacket *input, rc_adcs_estim_h1 *output);

void encoderc_adcs_sp_h1(rc_adcs_sp_h1 *input, CANPacket* output);
void decoderc_adcs_sp_h1(CANPacket *input, rc_adcs_sp_h1 *output);

void encoderc_eps_gen_h1(rc_eps_gen_h1 *input, CANPacket* output);
void decoderc_eps_gen_h1(CANPacket *input, rc_eps_gen_h1 *output);

void encoderc_adcs_mtq_h1(rc_adcs_mtq_h1 *input, CANPacket* output);
void decoderc_adcs_mtq_h1(CANPacket *input, rc_adcs_mtq_h1 *output);

void encoderc_ppt_h1(rc_ppt_h1 *input, CANPacket* output);
void decoderc_ppt_h1(CANPacket *input, rc_ppt_h1 *output);

void encodegcmd_mtq_pms(gcmd_mtq_pms *input, CANPacket* output);
void decodegcmd_mtq_pms(CANPacket *input, gcmd_mtq_pms *output);

void encodegcmd_bdot_max_tumble(gcmd_bdot_max_tumble *input, CANPacket* output);
void decodegcmd_bdot_max_tumble(CANPacket *input, gcmd_bdot_max_tumble *output);

void encodegcmd_bdot_spam(gcmd_bdot_spam *input, CANPacket* output);
void decodegcmd_bdot_spam(CANPacket *input, gcmd_bdot_spam *output);

void encodegcmd_bdot_control(gcmd_bdot_control *input, CANPacket* output);
void decodegcmd_bdot_control(CANPacket *input, gcmd_bdot_control *output);

void encodegcmd_bdot_mag_control(gcmd_bdot_mag_control *input, CANPacket* output);
void decodegcmd_bdot_mag_control(CANPacket *input, gcmd_bdot_mag_control *output);

void encodegcmd_bdot_pole_override(gcmd_bdot_pole_override *input, CANPacket* output);
void decodegcmd_bdot_pole_override(CANPacket *input, gcmd_bdot_pole_override *output);

void encodegcmd_gen_set_pt_state(gcmd_gen_set_pt_state *input, CANPacket* output);
void decodegcmd_gen_set_pt_state(CANPacket *input, gcmd_gen_set_pt_state *output);

void encodegcmd_sp_set_thresh(gcmd_sp_set_thresh *input, CANPacket* output);
void decodegcmd_sp_set_thresh(CANPacket *input, gcmd_sp_set_thresh *output);

void encodegcmd_dist_set_pd_ovc_ppt(gcmd_dist_set_pd_ovc_ppt *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_ppt(CANPacket *input, gcmd_dist_set_pd_ovc_ppt *output);

void encodegcmd_dist_set_pd_ovc_eps(gcmd_dist_set_pd_ovc_eps *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_eps(CANPacket *input, gcmd_dist_set_pd_ovc_eps *output);

void encodegcmd_dist_set_pd_ovc_wheels(gcmd_dist_set_pd_ovc_wheels *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_wheels(CANPacket *input, gcmd_dist_set_pd_ovc_wheels *output);

void encodegcmd_dist_set_pd_ovc_estim(gcmd_dist_set_pd_ovc_estim *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_estim(CANPacket *input, gcmd_dist_set_pd_ovc_estim *output);

void encodegcmd_dist_set_pd_ovc_bdot(gcmd_dist_set_pd_ovc_bdot *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_bdot(CANPacket *input, gcmd_dist_set_pd_ovc_bdot *output);

void encodegcmd_dist_set_pd_ovc_rahs(gcmd_dist_set_pd_ovc_rahs *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_rahs(CANPacket *input, gcmd_dist_set_pd_ovc_rahs *output);

void encodegcmd_dist_set_pd_ovc_com2(gcmd_dist_set_pd_ovc_com2 *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_com2(CANPacket *input, gcmd_dist_set_pd_ovc_com2 *output);

void encodegcmd_dist_set_pd_ovc_com1(gcmd_dist_set_pd_ovc_com1 *input, CANPacket* output);
void decodegcmd_dist_set_pd_ovc_com1(CANPacket *input, gcmd_dist_set_pd_ovc_com1 *output);

void encodegcmd_dist_set_pd_state(gcmd_dist_set_pd_state *input, CANPacket* output);
void decodegcmd_dist_set_pd_state(CANPacket *input, gcmd_dist_set_pd_state *output);

void encodegcmd_mtq_pop(gcmd_mtq_pop *input, CANPacket* output);
void decodegcmd_mtq_pop(CANPacket *input, gcmd_mtq_pop *output);

void encoderc_eps_batt_7(rc_eps_batt_7 *input, CANPacket* output);
void decoderc_eps_batt_7(CANPacket *input, rc_eps_batt_7 *output);

void encodesensorproc_mag2(sensorproc_mag2 *input, CANPacket* output);
void decodesensorproc_mag2(CANPacket *input, sensorproc_mag2 *output);

void encoderc_adcs_estim_14(rc_adcs_estim_14 *input, CANPacket* output);
void decoderc_adcs_estim_14(CANPacket *input, rc_adcs_estim_14 *output);

void encoderc_adcs_estim_13(rc_adcs_estim_13 *input, CANPacket* output);
void decoderc_adcs_estim_13(CANPacket *input, rc_adcs_estim_13 *output);

void encoderc_adcs_estim_12(rc_adcs_estim_12 *input, CANPacket* output);
void decoderc_adcs_estim_12(CANPacket *input, rc_adcs_estim_12 *output);

void encoderc_adcs_estim_11(rc_adcs_estim_11 *input, CANPacket* output);
void decoderc_adcs_estim_11(CANPacket *input, rc_adcs_estim_11 *output);

void encoderc_adcs_estim_10(rc_adcs_estim_10 *input, CANPacket* output);
void decoderc_adcs_estim_10(CANPacket *input, rc_adcs_estim_10 *output);

void encoderc_adcs_estim_9(rc_adcs_estim_9 *input, CANPacket* output);
void decoderc_adcs_estim_9(CANPacket *input, rc_adcs_estim_9 *output);

void encoderc_eps_dist_16(rc_eps_dist_16 *input, CANPacket* output);
void decoderc_eps_dist_16(CANPacket *input, rc_eps_dist_16 *output);

void encoderc_eps_dist_9(rc_eps_dist_9 *input, CANPacket* output);
void decoderc_eps_dist_9(CANPacket *input, rc_eps_dist_9 *output);

void encoderc_eps_dist_7(rc_eps_dist_7 *input, CANPacket* output);
void decoderc_eps_dist_7(CANPacket *input, rc_eps_dist_7 *output);

void encoderc_eps_dist_6(rc_eps_dist_6 *input, CANPacket* output);
void decoderc_eps_dist_6(CANPacket *input, rc_eps_dist_6 *output);

void encoderc_eps_dist_5(rc_eps_dist_5 *input, CANPacket* output);
void decoderc_eps_dist_5(CANPacket *input, rc_eps_dist_5 *output);

void encoderc_eps_dist_3(rc_eps_dist_3 *input, CANPacket* output);
void decoderc_eps_dist_3(CANPacket *input, rc_eps_dist_3 *output);

void encoderc_eps_dist_2(rc_eps_dist_2 *input, CANPacket* output);
void decoderc_eps_dist_2(CANPacket *input, rc_eps_dist_2 *output);

void encoderc_eps_dist_17(rc_eps_dist_17 *input, CANPacket* output);
void decoderc_eps_dist_17(CANPacket *input, rc_eps_dist_17 *output);

void encoderc_eps_dist_14(rc_eps_dist_14 *input, CANPacket* output);
void decoderc_eps_dist_14(CANPacket *input, rc_eps_dist_14 *output);

void encoderc_eps_dist_13(rc_eps_dist_13 *input, CANPacket* output);
void decoderc_eps_dist_13(CANPacket *input, rc_eps_dist_13 *output);

void encoderc_eps_dist_11(rc_eps_dist_11 *input, CANPacket* output);
void decoderc_eps_dist_11(CANPacket *input, rc_eps_dist_11 *output);

void encoderc_eps_dist_12(rc_eps_dist_12 *input, CANPacket* output);
void decoderc_eps_dist_12(CANPacket *input, rc_eps_dist_12 *output);

void encoderc_eps_dist_15(rc_eps_dist_15 *input, CANPacket* output);
void decoderc_eps_dist_15(CANPacket *input, rc_eps_dist_15 *output);

void encoderc_eps_dist_10(rc_eps_dist_10 *input, CANPacket* output);
void decoderc_eps_dist_10(CANPacket *input, rc_eps_dist_10 *output);

void encoderc_eps_dist_8(rc_eps_dist_8 *input, CANPacket* output);
void decoderc_eps_dist_8(CANPacket *input, rc_eps_dist_8 *output);

void encoderc_eps_dist_4(rc_eps_dist_4 *input, CANPacket* output);
void decoderc_eps_dist_4(CANPacket *input, rc_eps_dist_4 *output);

void encoderc_eps_dist_1(rc_eps_dist_1 *input, CANPacket* output);
void decoderc_eps_dist_1(CANPacket *input, rc_eps_dist_1 *output);

void encoderc_adcs_mpc_11(rc_adcs_mpc_11 *input, CANPacket* output);
void decoderc_adcs_mpc_11(CANPacket *input, rc_adcs_mpc_11 *output);

void encoderc_adcs_mpc_7(rc_adcs_mpc_7 *input, CANPacket* output);
void decoderc_adcs_mpc_7(CANPacket *input, rc_adcs_mpc_7 *output);

void encoderc_adcs_mpc_8(rc_adcs_mpc_8 *input, CANPacket* output);
void decoderc_adcs_mpc_8(CANPacket *input, rc_adcs_mpc_8 *output);

void encoderc_adcs_mpc_9(rc_adcs_mpc_9 *input, CANPacket* output);
void decoderc_adcs_mpc_9(CANPacket *input, rc_adcs_mpc_9 *output);

void encoderc_adcs_mpc_5(rc_adcs_mpc_5 *input, CANPacket* output);
void decoderc_adcs_mpc_5(CANPacket *input, rc_adcs_mpc_5 *output);

void encoderc_adcs_mpc_6(rc_adcs_mpc_6 *input, CANPacket* output);
void decoderc_adcs_mpc_6(CANPacket *input, rc_adcs_mpc_6 *output);

void encoderc_adcs_mpc_4(rc_adcs_mpc_4 *input, CANPacket* output);
void decoderc_adcs_mpc_4(CANPacket *input, rc_adcs_mpc_4 *output);

void encoderc_adcs_mpc_3(rc_adcs_mpc_3 *input, CANPacket* output);
void decoderc_adcs_mpc_3(CANPacket *input, rc_adcs_mpc_3 *output);

void encoderc_adcs_mpc_15(rc_adcs_mpc_15 *input, CANPacket* output);
void decoderc_adcs_mpc_15(CANPacket *input, rc_adcs_mpc_15 *output);

void encoderc_adcs_mpc_12(rc_adcs_mpc_12 *input, CANPacket* output);
void decoderc_adcs_mpc_12(CANPacket *input, rc_adcs_mpc_12 *output);

void encoderc_adcs_mpc_13(rc_adcs_mpc_13 *input, CANPacket* output);
void decoderc_adcs_mpc_13(CANPacket *input, rc_adcs_mpc_13 *output);

void encoderc_adcs_mpc_14(rc_adcs_mpc_14 *input, CANPacket* output);
void decoderc_adcs_mpc_14(CANPacket *input, rc_adcs_mpc_14 *output);

void encoderc_adcs_mpc_10(rc_adcs_mpc_10 *input, CANPacket* output);
void decoderc_adcs_mpc_10(CANPacket *input, rc_adcs_mpc_10 *output);

void encoderc_adcs_mpc_2(rc_adcs_mpc_2 *input, CANPacket* output);
void decoderc_adcs_mpc_2(CANPacket *input, rc_adcs_mpc_2 *output);

void encoderc_adcs_mpc_1(rc_adcs_mpc_1 *input, CANPacket* output);
void decoderc_adcs_mpc_1(CANPacket *input, rc_adcs_mpc_1 *output);

void encoderc_adcs_estim_8(rc_adcs_estim_8 *input, CANPacket* output);
void decoderc_adcs_estim_8(CANPacket *input, rc_adcs_estim_8 *output);

void encoderc_adcs_estim_7(rc_adcs_estim_7 *input, CANPacket* output);
void decoderc_adcs_estim_7(CANPacket *input, rc_adcs_estim_7 *output);

void encoderc_adcs_estim_6(rc_adcs_estim_6 *input, CANPacket* output);
void decoderc_adcs_estim_6(CANPacket *input, rc_adcs_estim_6 *output);

void encoderc_adcs_estim_5(rc_adcs_estim_5 *input, CANPacket* output);
void decoderc_adcs_estim_5(CANPacket *input, rc_adcs_estim_5 *output);

void encoderc_adcs_estim_4(rc_adcs_estim_4 *input, CANPacket* output);
void decoderc_adcs_estim_4(CANPacket *input, rc_adcs_estim_4 *output);

void encoderc_adcs_estim_2(rc_adcs_estim_2 *input, CANPacket* output);
void decoderc_adcs_estim_2(CANPacket *input, rc_adcs_estim_2 *output);

void encoderc_adcs_estim_3(rc_adcs_estim_3 *input, CANPacket* output);
void decoderc_adcs_estim_3(CANPacket *input, rc_adcs_estim_3 *output);

void encoderc_adcs_estim_1(rc_adcs_estim_1 *input, CANPacket* output);
void decoderc_adcs_estim_1(CANPacket *input, rc_adcs_estim_1 *output);

void encoderc_adcs_sp_12(rc_adcs_sp_12 *input, CANPacket* output);
void decoderc_adcs_sp_12(CANPacket *input, rc_adcs_sp_12 *output);

void encoderc_adcs_sp_11(rc_adcs_sp_11 *input, CANPacket* output);
void decoderc_adcs_sp_11(CANPacket *input, rc_adcs_sp_11 *output);

void encoderc_adcs_sp_7(rc_adcs_sp_7 *input, CANPacket* output);
void decoderc_adcs_sp_7(CANPacket *input, rc_adcs_sp_7 *output);

void encoderc_adcs_sp_6(rc_adcs_sp_6 *input, CANPacket* output);
void decoderc_adcs_sp_6(CANPacket *input, rc_adcs_sp_6 *output);

void encoderc_adcs_sp_17(rc_adcs_sp_17 *input, CANPacket* output);
void decoderc_adcs_sp_17(CANPacket *input, rc_adcs_sp_17 *output);

void encoderc_adcs_sp_16(rc_adcs_sp_16 *input, CANPacket* output);
void decoderc_adcs_sp_16(CANPacket *input, rc_adcs_sp_16 *output);

void encoderc_adcs_sp_10(rc_adcs_sp_10 *input, CANPacket* output);
void decoderc_adcs_sp_10(CANPacket *input, rc_adcs_sp_10 *output);

void encoderc_adcs_sp_8(rc_adcs_sp_8 *input, CANPacket* output);
void decoderc_adcs_sp_8(CANPacket *input, rc_adcs_sp_8 *output);

void encoderc_adcs_sp_9(rc_adcs_sp_9 *input, CANPacket* output);
void decoderc_adcs_sp_9(CANPacket *input, rc_adcs_sp_9 *output);

void encoderc_adcs_sp_13(rc_adcs_sp_13 *input, CANPacket* output);
void decoderc_adcs_sp_13(CANPacket *input, rc_adcs_sp_13 *output);

void encoderc_adcs_sp_15(rc_adcs_sp_15 *input, CANPacket* output);
void decoderc_adcs_sp_15(CANPacket *input, rc_adcs_sp_15 *output);

void encoderc_adcs_sp_14(rc_adcs_sp_14 *input, CANPacket* output);
void decoderc_adcs_sp_14(CANPacket *input, rc_adcs_sp_14 *output);

void encoderc_adcs_sp_5(rc_adcs_sp_5 *input, CANPacket* output);
void decoderc_adcs_sp_5(CANPacket *input, rc_adcs_sp_5 *output);

void encoderc_adcs_sp_4(rc_adcs_sp_4 *input, CANPacket* output);
void decoderc_adcs_sp_4(CANPacket *input, rc_adcs_sp_4 *output);

void encoderc_adcs_sp_3(rc_adcs_sp_3 *input, CANPacket* output);
void decoderc_adcs_sp_3(CANPacket *input, rc_adcs_sp_3 *output);

void encoderc_adcs_sp_2(rc_adcs_sp_2 *input, CANPacket* output);
void decoderc_adcs_sp_2(CANPacket *input, rc_adcs_sp_2 *output);

void encoderc_adcs_sp_1(rc_adcs_sp_1 *input, CANPacket* output);
void decoderc_adcs_sp_1(CANPacket *input, rc_adcs_sp_1 *output);

void encoderc_adcs_bdot_4(rc_adcs_bdot_4 *input, CANPacket* output);
void decoderc_adcs_bdot_4(CANPacket *input, rc_adcs_bdot_4 *output);

void encoderc_adcs_bdot_3(rc_adcs_bdot_3 *input, CANPacket* output);
void decoderc_adcs_bdot_3(CANPacket *input, rc_adcs_bdot_3 *output);

void encoderc_adcs_bdot_2(rc_adcs_bdot_2 *input, CANPacket* output);
void decoderc_adcs_bdot_2(CANPacket *input, rc_adcs_bdot_2 *output);

void encoderc_adcs_bdot_1(rc_adcs_bdot_1 *input, CANPacket* output);
void decoderc_adcs_bdot_1(CANPacket *input, rc_adcs_bdot_1 *output);

void encodeestim_sun_unit_z(estim_sun_unit_z *input, CANPacket* output);
void decodeestim_sun_unit_z(CANPacket *input, estim_sun_unit_z *output);

void encodeestim_sun_unit_y(estim_sun_unit_y *input, CANPacket* output);
void decodeestim_sun_unit_y(CANPacket *input, estim_sun_unit_y *output);

void encodeestim_sun_unit_x(estim_sun_unit_x *input, CANPacket* output);
void decodeestim_sun_unit_x(CANPacket *input, estim_sun_unit_x *output);

void encodeestim_mag_unit_z(estim_mag_unit_z *input, CANPacket* output);
void decodeestim_mag_unit_z(CANPacket *input, estim_mag_unit_z *output);

void encodeestim_mag_unit_y(estim_mag_unit_y *input, CANPacket* output);
void decodeestim_mag_unit_y(CANPacket *input, estim_mag_unit_y *output);

void encodeestim_mag_unit_x(estim_mag_unit_x *input, CANPacket* output);
void decodeestim_mag_unit_x(CANPacket *input, estim_mag_unit_x *output);

void encodeestim_state(estim_state *input, CANPacket* output);
void decodeestim_state(CANPacket *input, estim_state *output);

void encoderc_adcs_mtq_5(rc_adcs_mtq_5 *input, CANPacket* output);
void decoderc_adcs_mtq_5(CANPacket *input, rc_adcs_mtq_5 *output);

void encoderc_adcs_mtq_4(rc_adcs_mtq_4 *input, CANPacket* output);
void decoderc_adcs_mtq_4(CANPacket *input, rc_adcs_mtq_4 *output);

void encoderc_adcs_mtq_3(rc_adcs_mtq_3 *input, CANPacket* output);
void decoderc_adcs_mtq_3(CANPacket *input, rc_adcs_mtq_3 *output);

void encoderc_adcs_mtq_2(rc_adcs_mtq_2 *input, CANPacket* output);
void decoderc_adcs_mtq_2(CANPacket *input, rc_adcs_mtq_2 *output);

void encoderc_adcs_mtq_1(rc_adcs_mtq_1 *input, CANPacket* output);
void decoderc_adcs_mtq_1(CANPacket *input, rc_adcs_mtq_1 *output);

void encoderc_ppt_3(rc_ppt_3 *input, CANPacket* output);
void decoderc_ppt_3(CANPacket *input, rc_ppt_3 *output);

void encoderc_ppt_2(rc_ppt_2 *input, CANPacket* output);
void decoderc_ppt_2(CANPacket *input, rc_ppt_2 *output);

void encoderc_ppt_1(rc_ppt_1 *input, CANPacket* output);
void decoderc_ppt_1(CANPacket *input, rc_ppt_1 *output);

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

void encodegrnd_epoch(grnd_epoch *input, CANPacket* output);
void decodegrnd_epoch(CANPacket *input, grnd_epoch *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
