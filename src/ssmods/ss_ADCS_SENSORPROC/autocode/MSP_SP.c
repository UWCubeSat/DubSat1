/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.c
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.330
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Sat Apr 14 10:28:14 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MSP_SP.h"

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function */
void MSP_SP_step(void)
{
  real_T tmp[3];
  int16_T i;

  /* Product: '<S2>/Product' incorporates:
   *  Constant: '<S2>/process_matrix'
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   *  Product: '<S2>/Product1'
   *  Sum: '<S2>/Sum'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = rtConstP.pooled1[i + 6] * rtU.sun_vec_body_sunsensor[2] +
      (rtConstP.pooled1[i + 3] * rtU.sun_vec_body_sunsensor[1] +
       rtConstP.pooled1[i] * rtU.sun_vec_body_sunsensor[0]);
  }

  /* End of Product: '<S2>/Product' */
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sun_vec_body' incorporates:
     *  Constant: '<S2>/sensor2body'
     *  Product: '<S2>/Product1'
     */
    rtY.sun_vec_body[i] = rtConstP.pooled1[i + 6] * tmp[2] + (rtConstP.pooled1[i
      + 3] * tmp[1] + rtConstP.pooled1[i] * tmp[0]);
  }

  /* Outport: '<Root>/sun_vec_body' incorporates:
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   */
  rtY.sun_vec_body[3] = rtU.sun_vec_body_sunsensor[3];
}

/* Model initialize function */
void MSP_SP_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void MSP_SP_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
