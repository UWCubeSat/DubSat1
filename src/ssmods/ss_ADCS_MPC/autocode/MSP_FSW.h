/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW.h
 *
 * Code generated for Simulink model 'MSP_FSW'.
 *
 * Model version                  : 1.384
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Jun 25 21:15:08 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_MSP_FSW_h_
#define RTW_HEADER_MSP_FSW_h_
#include <stddef.h>
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#ifndef MSP_FSW_COMMON_INCLUDES_
# define MSP_FSW_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* MSP_FSW_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_sp2fsw_
#define DEFINED_TYPEDEF_FOR_sp2fsw_

typedef struct {
  real32_T mag1_vec_body_T[4];
  real32_T mag2_vec_body_T[4];
  real32_T gyro_omega_body_radps[4];
  real32_T sun_vec_body_sunsensor[4];
} sp2fsw;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CAN_OUT_
#define DEFINED_TYPEDEF_FOR_CAN_OUT_

typedef struct {
  real_T sc_quat[4];
  real_T body_rates[3];
  int8_T sc_mode;

  /* indication that we meet the velocity pointing requirement */
  boolean_T point_true;

  /* indication that the s/c is above the ground station and COM2 can commence */
  boolean_T sc_above_gs;
} CAN_OUT;

#endif

#ifndef DEFINED_TYPEDEF_FOR_fsw2mt_
#define DEFINED_TYPEDEF_FOR_fsw2mt_

typedef struct {
  /* ADCS mode */
  int8_T sc_mode;
  int8_T cmd_MT_fsw_dv[3];
} fsw2mt;

#endif

/* Block signals and states (auto storage) for system '<S6>/sun_point_lib' */
typedef struct {
  real_T UnitDelay_DSTATE[3];          /* '<S38>/Unit Delay' */
  real_T UnitDelay_DSTATE_b[3];        /* '<S53>/Unit Delay' */
} DW_sun_point_lib;

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  DW_sun_point_lib sun_point_lib_j;    /* '<S6>/sun_point_lib' */
  real_T UnitDelay_DSTATE[4];          /* '<S3>/Unit Delay' */
  real_T UnitDelay1_DSTATE[3];         /* '<S3>/Unit Delay1' */
  real_T UnitDelay2_DSTATE[36];        /* '<S3>/Unit Delay2' */
  int8_T UnitDelay_DSTATE_a;           /* '<S8>/Unit Delay' */
  boolean_T Relay_Mode;                /* '<S8>/Relay' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: fsw_params.bus.solar_panel_unit
   * Referenced by: '<S38>/Constant'
   */
  real_T Constant_Value[3];

  /* Expression: fsw_params.control.sun_point.prop_gain
   * Referenced by: '<S38>/prop_gain'
   */
  real_T prop_gain_Gain[9];

  /* Expression: fsw_params.control.sun_point.drv_gain
   * Referenced by: '<S38>/drv_gain'
   */
  real_T drv_gain_Gain[9];

  /* Expression: fsw_params.estimation.ic.error_cov
   * Referenced by: '<S3>/Unit Delay2'
   */
  real_T UnitDelay2_InitialCondition[36];

  /* Expression: fsw_params.estimation.meas_cov
   * Referenced by: '<S3>/Constant1'
   */
  real_T Constant1_Value[36];

  /* Expression: fsw_params.control.mag_pd_controller.p_gain
   * Referenced by: '<S37>/p-gain'
   */
  real_T pgain_Value[9];

  /* Expression: fsw_params.control.mag_pd_controller.d_gain
   * Referenced by: '<S37>/d-gain'
   */
  real_T dgain_Value[9];

  /* Expression: fsw_params.estimation.G
   * Referenced by: '<S20>/G'
   */
  real_T G_Value[36];

  /* Expression: fsw_params.estimation.proc_cov
   * Referenced by: '<S20>/Constant3'
   */
  real_T Constant3_Value[36];

  /* Computed Parameter: r_SEA_Value
   * Referenced by: '<S10>/r_SEA'
   */
  real32_T r_SEA_Value[3];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  boolean_T MT_valid[3];               /* '<Root>/MT_valid' */
  boolean_T MT_pwr[3];                 /* '<Root>/MT_pwr' */
  boolean_T sc_in_sun;                 /* '<Root>/sc_in_sun' */
  boolean_T sc_above_gs;               /* '<Root>/sc_above_gs' */
  real_T sc2sun_unit[3];               /* '<Root>/sc2sun_unit' */
  real_T mag_eci_unit[3];              /* '<Root>/mag_eci_unit' */
  real32_T mag1_vec_body_T[4];         /* '<Root>/mag1_vec_body_T' */
  real32_T mag2_vec_body_T[4];         /* '<Root>/mag2_vec_body_T' */
  real32_T gyro_omega_body_radps[4];   /* '<Root>/gyro_omega_body_radps' */
  real32_T sun_vec_body_sunsensor[4];  /* '<Root>/sun_vec_body_sunsensor' */
  int8_T CAN_IN[5];                    /* '<Root>/CAN_IN' */
  real32_T solar_panel_power_W[3];     /* '<Root>/solar_panel_power_W' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T sc_quat[4];                   /* '<Root>/sc_quat' */
  real_T body_rates[3];                /* '<Root>/body_rates' */
  int8_T sc_mode;                      /* '<Root>/sc_mode' */
  boolean_T point_true;                /* '<Root>/point_true' */
  boolean_T sc_above_gsb;              /* '<Root>/sc_above_gsb' */
  int8_T sc_modeb;                     /* '<Root>/sc_modeb' */
  int8_T cmd_MT_fsw_dv[3];             /* '<Root>/cmd_MT_fsw_dv' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (auto storage) */
extern DW rtDW;

/* External inputs (root inport signals with auto storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY rtY;

/* Constant parameters (auto storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void MSP_FSW_initialize(void);
extern void MSP_FSW_step(void);
extern void MSP_FSW_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S11>/Constant1' : Unused code path elimination
 * Block '<S11>/Constant2' : Unused code path elimination
 * Block '<S11>/Constant26' : Unused code path elimination
 * Block '<S11>/Constant4' : Unused code path elimination
 * Block '<S11>/Constant5' : Unused code path elimination
 * Block '<S16>/Product' : Unused code path elimination
 * Block '<S16>/Product1' : Unused code path elimination
 * Block '<S16>/Product2' : Unused code path elimination
 * Block '<S16>/Product3' : Unused code path elimination
 * Block '<S16>/Sum' : Unused code path elimination
 * Block '<S17>/Product' : Unused code path elimination
 * Block '<S17>/Product1' : Unused code path elimination
 * Block '<S17>/Product2' : Unused code path elimination
 * Block '<S17>/Product3' : Unused code path elimination
 * Block '<S17>/Sum' : Unused code path elimination
 * Block '<S18>/Product' : Unused code path elimination
 * Block '<S18>/Product1' : Unused code path elimination
 * Block '<S18>/Product2' : Unused code path elimination
 * Block '<S18>/Product3' : Unused code path elimination
 * Block '<S18>/Sum' : Unused code path elimination
 * Block '<S3>/Rate Transition10' : Unused code path elimination
 * Block '<S3>/Reshape2' : Unused code path elimination
 * Block '<S3>/Reshape3' : Unused code path elimination
 * Block '<S4>/Rate Transition3' : Unused code path elimination
 * Block '<S5>/Constant26' : Unused code path elimination
 * Block '<S5>/Constant6' : Unused code path elimination
 * Block '<S10>/Rate Transition8' : Unused code path elimination
 * Block '<S3>/ ' : Eliminated since input and output rates are identical
 * Block '<S3>/ 1' : Eliminated since input and output rates are identical
 * Block '<S3>/ 11' : Eliminated since input and output rates are identical
 * Block '<S3>/ 2' : Eliminated since input and output rates are identical
 * Block '<S3>/ 3' : Eliminated since input and output rates are identical
 * Block '<S3>/ 4' : Eliminated since input and output rates are identical
 * Block '<S3>/ 5' : Eliminated since input and output rates are identical
 * Block '<S3>/ 6' : Eliminated since input and output rates are identical
 * Block '<S3>/ 7' : Eliminated since input and output rates are identical
 * Block '<S3>/ 8' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S3>/Reshape4' : Reshape block reduction
 * Block '<S4>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition5' : Eliminated since input and output rates are identical
 * Block '<S38>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S38>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S38>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S8>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S10>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S10>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S10>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S10>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S10>/Rate Transition6' : Eliminated since input and output rates are identical
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW')    - opens subsystem adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW
 * hilite_system('adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/adcs_main'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib'
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF '
 * '<S4>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/actuator_processing'
 * '<S5>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/bus_stub_fsw2mt'
 * '<S6>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation'
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mag_logic_lib'
 * '<S8>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mode_selecction'
 * '<S9>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/single_2_bool'
 * '<S10>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/target_generation_lib'
 * '<S11>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/bus_stub_CAN_OUT'
 * '<S12>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib'
 * '<S13>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Conjugate'
 * '<S14>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication'
 * '<S15>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q0'
 * '<S16>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q1'
 * '<S17>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q2'
 * '<S18>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q3'
 * '<S19>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /3_sig_bnd '
 * '<S20>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step '
 * '<S21>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step '
 * '<S22>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /single_2_bool2'
 * '<S23>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize'
 * '<S24>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /propagate_quat'
 * '<S25>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /state_transition'
 * '<S26>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus'
 * '<S27>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S28>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem'
 * '<S29>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem1'
 * '<S30>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body'
 * '<S31>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/covariance_update'
 * '<S32>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/kalman_gain'
 * '<S33>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/observation_matrix'
 * '<S34>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/update_state '
 * '<S35>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Control Selection'
 * '<S36>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/command_processing'
 * '<S37>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib '
 * '<S38>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib'
 * '<S39>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Control Selection/control_selection'
 * '<S40>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/command_processing/dipole-2-digVal'
 * '<S41>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Cross Product'
 * '<S42>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Normalize Vector'
 * '<S43>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Inverse'
 * '<S44>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Multiplication'
 * '<S45>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Inverse/Quaternion Conjugate'
 * '<S46>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Inverse/Quaternion Norm'
 * '<S47>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Multiplication/q0'
 * '<S48>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Multiplication/q1'
 * '<S49>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Multiplication/q2'
 * '<S50>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/mag_PD_control_lib /Quaternion Multiplication/q3'
 * '<S51>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/Cross Product'
 * '<S52>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/MATLAB Function'
 * '<S53>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/compute_error'
 * '<S54>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/compute_error/3x3 Cross Product'
 * '<S55>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/compute_error/3x3 Cross Product/Subsystem'
 * '<S56>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/sun_point_lib/compute_error/3x3 Cross Product/Subsystem1'
 * '<S57>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mag_logic_lib/single_2_bool1'
 * '<S58>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mode_selecction/MATLAB Function'
 * '<S59>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/target_generation_lib/TARG_GEN'
 */
#endif                                 /* RTW_HEADER_MSP_FSW_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
