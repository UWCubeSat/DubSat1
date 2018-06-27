/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW_data.c
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

#include "MSP_FSW.h"

/* Constant parameters (auto storage) */
const ConstP rtConstP = {
  /* Expression: fsw_params.bus.solar_panel_unit
   * Referenced by: '<S38>/Constant'
   */
  { 0.0, 1.0, 0.0 },

  /* Expression: fsw_params.control.sun_point.prop_gain
   * Referenced by: '<S38>/prop_gain'
   */
  { -7.15925376030017E-16, -2.9518306409416E-5, -1.16471856520801E-5,
    2.95183064103284E-5, 6.91389624109808E-16, -6.39736541290793E-6,
    1.16471856488024E-5, 6.3973654111362E-6, 1.52865094782168E-17 },

  /* Expression: fsw_params.control.sun_point.drv_gain
   * Referenced by: '<S38>/drv_gain'
   */
  { -1.13493695706464E-9, -3.0013255295274E-5, -1.18422341375943E-5,
    3.00242063478115E-5, 7.31311204065323E-10, -6.50671819922309E-6,
    1.16904253272767E-5, 6.42213829448181E-6, 4.03625743741717E-10 },

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

  /* Expression: fsw_params.control.mag_pd_controller.p_gain
   * Referenced by: '<S37>/p-gain'
   */
  { 0.03, -0.0, -0.0, -0.0, 0.03, -0.0, -0.0, -0.0, 0.15 },

  /* Expression: fsw_params.control.mag_pd_controller.d_gain
   * Referenced by: '<S37>/d-gain'
   */
  { 21.0, -0.0, -0.0, -0.0, 21.0, -0.0, -0.0, -0.0, 35.0 },

  /* Expression: fsw_params.estimation.G
   * Referenced by: '<S20>/G'
   */
  { -1.0, -0.0, -0.0, 0.0, 0.0, 0.0, -0.0, -1.0, -0.0, 0.0, 0.0, 0.0, -0.0, -0.0,
    -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 },

  /* Expression: fsw_params.estimation.proc_cov
   * Referenced by: '<S20>/Constant3'
   */
  { 1.0033333333333333E-7, 0.0, 0.0, -5.0000000000000009E-9, -0.0, -0.0, 0.0,
    1.0033333333333333E-7, 0.0, -0.0, -5.0000000000000009E-9, -0.0, 0.0, 0.0,
    1.0033333333333333E-7, -0.0, -0.0, -5.0000000000000009E-9,
    -5.0000000000000009E-9, -0.0, -0.0, 1.0E-7, 0.0, 0.0, -0.0,
    -5.0000000000000009E-9, -0.0, 0.0, 1.0E-7, 0.0, -0.0, -0.0,
    -5.0000000000000009E-9, 0.0, 0.0, 1.0E-7 },

  /* Computed Parameter: r_SEA_Value
   * Referenced by: '<S10>/r_SEA'
   */
  { 0.409930646F, -0.538629532F, 0.736094475F }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
