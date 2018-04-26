/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW.h
 *
 * Code generated for Simulink model 'MSP_FSW'.
 *
 * Model version                  : 1.357
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Wed Apr 25 18:16:06 2018
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
  real32_T mag_vec_body_T[4];
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

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T u0[36];                       /* '<S3>/ 10' */
  real_T RateTransition[3];            /* '<S34>/Rate Transition' */
  real_T RateTransition2_d[3];         /* '<S34>/Rate Transition2' */
  real_T RateTransition1[3];           /* '<S34>/Rate Transition1' */
  real_T UnitDelay_DSTATE[4];          /* '<S3>/Unit Delay' */
  real_T UnitDelay1_DSTATE[3];         /* '<S3>/Unit Delay1' */
  real_T UnitDelay2_DSTATE[36];        /* '<S3>/Unit Delay2' */
  real_T UnitDelay_DSTATE_a[3];        /* '<S34>/Unit Delay' */
  real_T RateTransition2_Buffer0[4];   /* '<S3>/Rate Transition2' */
  real_T RateTransition7_2_Buffer0[3]; /* '<S1>/Rate Transition7' */
  real_T RateTransition_Buffer0[3];    /* '<S34>/Rate Transition' */
  real_T RateTransition4;              /* '<S34>/Rate Transition4' */
  real_T RateTransition5;              /* '<S34>/Rate Transition5' */
  real32_T RateTransition6[4];         /* '<S8>/Rate Transition6' */
  real32_T RateTransition6_Buffer0[4]; /* '<S8>/Rate Transition6' */
  real32_T RateTransition3;            /* '<S34>/Rate Transition3' */
  int32_T clockTickCounter;            /* '<S6>/Pulse Generator' */
  int32_T clockTickCounter_l;          /* '<S34>/MT_on' */
  struct {
    uint_T Relay_Mode:1;               /* '<S7>/Relay' */
  } bitsForTID1;

  int8_T RateTransition3_n[5];         /* '<S1>/Rate Transition3' */
  int8_T RateTransition4_2_Buffer0[3]; /* '<S1>/Rate Transition4' */
  int8_T RateTransition3_Buffer0[5];   /* '<S1>/Rate Transition3' */
  int8_T UnitDelay_DSTATE_ao;          /* '<S7>/Unit Delay' */
  int8_T RateTransition7_3_Buffer0;    /* '<S1>/Rate Transition7' */
  int8_T RateTransition4_1_Buffer0;    /* '<S1>/Rate Transition4' */
  boolean_T MT_valid[3];               /* '<S1>/Rate Transition2' */
  boolean_T RateTransition7_5_Buffer0; /* '<S1>/Rate Transition7' */
} DW;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T Product1[36];           /* '<S19>/Product1' */
} ConstB;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: fsw_params.estimation.meas_cov
   * Referenced by: '<S3>/Constant1'
   */
  real_T Constant1_Value[36];

  /* Expression: fsw_params.estimation.ic.error_cov
   * Referenced by: '<S3>/Unit Delay2'
   */
  real_T UnitDelay2_InitialCondition[36];

  /* Expression: fsw_params.actuators.reaction_wheel.inertia_matrix
   * Referenced by: '<S34>/MoI'
   */
  real_T MoI_Value[9];

  /* Computed Parameter: r_SEA_Value
   * Referenced by: '<S8>/r_SEA'
   */
  real32_T r_SEA_Value[3];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  boolean_T MT_valid[3];               /* '<Root>/MT_valid' */
  boolean_T MT_pwr[3];                 /* '<Root>/MT_pwr' */
  boolean_T sc_in_sun;                 /* '<Root>/sc_in_sun' */
  real_T sc2sun_unit[3];               /* '<Root>/sc2sun_unit' */
  real_T mag_eci_unit[3];              /* '<Root>/mag_eci_unit' */
  boolean_T sc_above_gs;               /* '<Root>/sc_above_gs' */
  real_T pos_eci_m[3];                 /* '<Root>/pos_eci_m' */
  real_T vel_eci_mps[3];               /* '<Root>/vel_eci_mps' */
  int8_T SGP4_flag;                    /* '<Root>/SGP4_flag' */
  real32_T mag_vec_body_T[4];          /* '<Root>/mag_vec_body_T' */
  real32_T gyro_omega_body_radps[4];   /* '<Root>/gyro_omega_body_radps' */
  real32_T sun_vec_body_sunsensor[4];  /* '<Root>/sun_vec_body_sunsensor' */
  int8_T CAN[5];                       /* '<Root>/CAN_IN' */
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

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID0_1;
      uint8_T TID0_2;
      uint8_T TID1_2;
    } RateInteraction;
  } Timing;
};

/* Block signals and states (auto storage) */
extern DW rtDW;

/* External inputs (root inport signals with auto storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY rtY;
extern const ConstB rtConstB;          /* constant block i/o */

/* Constant parameters (auto storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void MSP_FSW_initialize(void);
extern void MSP_FSW_step0(void);
extern void MSP_FSW_step1(void);
extern void MSP_FSW_step2(void);
extern void MSP_FSW_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S9>/Constant1' : Unused code path elimination
 * Block '<S9>/Constant2' : Unused code path elimination
 * Block '<S9>/Constant26' : Unused code path elimination
 * Block '<S9>/Constant4' : Unused code path elimination
 * Block '<S9>/Constant5' : Unused code path elimination
 * Block '<S15>/Product' : Unused code path elimination
 * Block '<S15>/Product1' : Unused code path elimination
 * Block '<S15>/Product2' : Unused code path elimination
 * Block '<S15>/Product3' : Unused code path elimination
 * Block '<S15>/Sum' : Unused code path elimination
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
 * Block '<S3>/Rate Transition10' : Unused code path elimination
 * Block '<S3>/Reshape2' : Unused code path elimination
 * Block '<S3>/Reshape3' : Unused code path elimination
 * Block '<S4>/Rate Transition3' : Unused code path elimination
 * Block '<S5>/Constant26' : Unused code path elimination
 * Block '<S5>/Constant6' : Unused code path elimination
 * Block '<S8>/Data Type Conversion1' : Unused code path elimination
 * Block '<S8>/Rate Transition2' : Unused code path elimination
 * Block '<S8>/Rate Transition8' : Unused code path elimination
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
 * Block '<S3>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S3>/Reshape1' : Reshape block reduction
 * Block '<S3>/Reshape4' : Reshape block reduction
 * Block '<S1>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition10' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition11' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition5' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition6' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition8' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition9' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S7>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S8>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S8>/Rate Transition1' : Eliminated since input and output rates are identical
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
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mode_selecction'
 * '<S8>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/target_generation_lib'
 * '<S9>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/bus_stub_CAN_OUT'
 * '<S10>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib'
 * '<S11>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Compare To Constant'
 * '<S12>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Conjugate'
 * '<S13>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication'
 * '<S14>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q0'
 * '<S15>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q1'
 * '<S16>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q2'
 * '<S17>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/CAN_out_lib/quat_degerr_check_lib/Quaternion Multiplication/q3'
 * '<S18>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /3_sig_bnd '
 * '<S19>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step '
 * '<S20>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step '
 * '<S21>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize'
 * '<S22>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /propagate_quat'
 * '<S23>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /state_transition'
 * '<S24>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus'
 * '<S25>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S26>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem'
 * '<S27>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem1'
 * '<S28>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body'
 * '<S29>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/covariance_update'
 * '<S30>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/kalman_gain'
 * '<S31>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/observation_matrix'
 * '<S32>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/Estimation_EKF /Update Step /If Action Subsystem/update_state '
 * '<S33>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Control Selection'
 * '<S34>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Momentum unloading'
 * '<S35>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/command_processing'
 * '<S36>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Control Selection/control_selection'
 * '<S37>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/Momentum unloading/Cross Product'
 * '<S38>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/command_generation/command_processing/dipole-2-digVal'
 * '<S39>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/mode_selecction/MATLAB Function'
 * '<S40>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_FSW/target_generation_lib/TARG_GEN'
 */
#endif                                 /* RTW_HEADER_MSP_FSW_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
