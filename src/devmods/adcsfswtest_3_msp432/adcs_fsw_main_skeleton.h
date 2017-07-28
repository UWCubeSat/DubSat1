/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: adcs_fsw_main_skeleton.h
 *
 * Code generated for Simulink model 'adcs_fsw_main_skeleton'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Tue Jul 25 18:49:09 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_adcs_fsw_main_skeleton_h_
#define RTW_HEADER_adcs_fsw_main_skeleton_h_
#include "rtwtypes.h"
#ifndef adcs_fsw_main_skeleton_COMMON_INCLUDES_
# define adcs_fsw_main_skeleton_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* adcs_fsw_main_skeleton_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef DEFINED_TYPEDEF_FOR_act_meas_
#define DEFINED_TYPEDEF_FOR_act_meas_

typedef struct {
  boolean_T MT_power_OK;
  boolean_T MT_on;
  real_T RW_pwrstate[3];
  real_T magnetorquer_pwrstate[3];
  real_T current_RW_RPM[3];
} act_meas;

#endif

#ifndef DEFINED_TYPEDEF_FOR_sens_meas_
#define DEFINED_TYPEDEF_FOR_sens_meas_

typedef struct {
  real_T measurements_deg_err[6];
  real_T measurements_valid[3];
  real_T sun_eci_unit_photodiode[3];
  real_T sun_eci_unit_sunsensor[3];
  real_T mag_eci_T[3];
  real_T GPS[8];
  real_T omega_radps_gyro[3];
} sens_meas;

#endif

#ifndef DEFINED_TYPEDEF_FOR_commands_
#define DEFINED_TYPEDEF_FOR_commands_

typedef struct {
  real_T RW_RPM[3];
  real_T magnetorquer_volt[3];
} commands;

#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
  act_meas act_meas_i;                 /* '<Root>/act_meas' */
  sens_meas sens_meas_m;               /* '<Root>/sens_meas' */
  real_T ADCS_INCANDATA;               /* '<Root>/ADCS_IN CAN-DATA' */
  real_T MPC_IN;                       /* '<Root>/MPC_IN' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T commads[6];                   /* '<Root>/commads' */
  real_T ADCS_OUTCANBUS[40];           /* '<Root>/ADCS_OUT CAN-BUS' */
  real_T MPC_OUT[13];                  /* '<Root>/MPC_OUT' */
} ExtY;

/* External inputs (root inport signals with auto storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void adcs_fsw_main_skeleton_initialize(void);
extern void adcs_fsw_main_skeleton_step(void);
extern void adcs_fsw_main_skeleton_terminate(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Constant' : Unused code path elimination
 * Block '<S2>/Constant' : Unused code path elimination
 * Block '<S2>/Constant1' : Unused code path elimination
 * Block '<S2>/Constant2' : Unused code path elimination
 * Block '<S2>/Constant3' : Unused code path elimination
 * Block '<S2>/Constant4' : Unused code path elimination
 * Block '<S2>/Constant5' : Unused code path elimination
 * Block '<S2>/Constant6' : Unused code path elimination
 * Block '<S3>/Constant' : Unused code path elimination
 * Block '<S3>/Constant1' : Unused code path elimination
 * Block '<S3>/Constant2' : Unused code path elimination
 * Block '<S3>/Constant4' : Unused code path elimination
 * Block '<S6>/Constant2' : Unused code path elimination
 * Block '<Root>/Switch' : Eliminated due to constant selection input
 * Block '<S4>/Constant' : Unused code path elimination
 * Block '<S6>/Constant' : Unused code path elimination
 * Block '<S6>/Constant1' : Unused code path elimination
 * Block '<S7>/Constant' : Unused code path elimination
 * Block '<S8>/Constant' : Unused code path elimination
 * Block '<Root>/Product' : Unused code path elimination
 * Block '<Root>/Product1' : Unused code path elimination
 * Block '<Root>/Product2' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_fsw_main_skeleton'
 * '<S1>'   : 'adcs_fsw_main_skeleton/ADCS Out'
 * '<S2>'   : 'adcs_fsw_main_skeleton/Environment'
 * '<S3>'   : 'adcs_fsw_main_skeleton/Estimation'
 * '<S4>'   : 'adcs_fsw_main_skeleton/MPC Mode Selection'
 * '<S5>'   : 'adcs_fsw_main_skeleton/MPC Out'
 * '<S6>'   : 'adcs_fsw_main_skeleton/Mode Selection and Reference'
 * '<S7>'   : 'adcs_fsw_main_skeleton/Momentum Unloading'
 * '<S8>'   : 'adcs_fsw_main_skeleton/Pointing Control'
 */
#endif                                 /* RTW_HEADER_adcs_fsw_main_skeleton_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
