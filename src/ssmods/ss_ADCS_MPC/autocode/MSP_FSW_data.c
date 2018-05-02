/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW_data.c
 *
 * Code generated for Simulink model 'MSP_FSW'.
 *
 * Model version                  : 1.369
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Tue May  1 19:12:57 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MSP_FSW.h"

/* Constant parameters (auto storage) */
const ConstP rtConstP = {
  /* Expression: fsw_params.estimation.ic.error_cov
   * Referenced by: '<S3>/Unit Delay2'
   */
  { 0.0027415567780803771, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0027415567780803771,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0027415567780803771, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 2.7415567780803768E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    2.7415567780803768E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.7415567780803768E-5 },

  /* Expression: fsw_params.estimation.meas_cov
   * Referenced by: '<S3>/Constant1'
   */
  { 1.4578147600000002E-12, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.4578147600000002E-12,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.4578147600000002E-12, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.010000000000000002, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.010000000000000002, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.010000000000000002 },

  /* Expression: fsw_params.actuators.reaction_wheel.inertia_matrix
   * Referenced by: '<S36>/MoI'
   */
  { 1.788E-6, 0.0, 0.0, 0.0, 1.788E-6, 0.0, 0.0, 0.0, 1.788E-6 },

  /* Expression: fsw_params.estimation.G
   * Referenced by: '<S21>/G'
   */
  { -1.0, -0.0, -0.0, 0.0, 0.0, 0.0, -0.0, -1.0, -0.0, 0.0, 0.0, 0.0, -0.0, -0.0,
    -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 },

  /* Expression: fsw_params.estimation.proc_cov
   * Referenced by: '<S21>/Constant3'
   */
  { 1.0033333333333333E-7, 0.0, 0.0, -5.0000000000000009E-9, -0.0, -0.0, 0.0,
    1.0033333333333333E-7, 0.0, -0.0, -5.0000000000000009E-9, -0.0, 0.0, 0.0,
    1.0033333333333333E-7, -0.0, -0.0, -5.0000000000000009E-9,
    -5.0000000000000009E-9, -0.0, -0.0, 1.0E-7, 0.0, 0.0, -0.0,
    -5.0000000000000009E-9, -0.0, 0.0, 1.0E-7, 0.0, -0.0, -0.0,
    -5.0000000000000009E-9, 0.0, 0.0, 1.0E-7 },

  /* Computed Parameter: r_SEA_Value
   * Referenced by: '<S11>/r_SEA'
   */
  { 0.409930646F, -0.538629532F, 0.736094475F }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
