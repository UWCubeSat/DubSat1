/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.c
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.347
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Apr 16 23:50:11 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MSP_SP.h"

/* Block signals and states (auto storage) */
DW rtDW;

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function for TID0 */
void MSP_SP_step0(void)                /* Sample time: [0.025s, 0.0s] */
{
  real32_T denAccum;
  int16_T i;
  real32_T rtb_DiscreteTransferFcn1_idx_0;
  real32_T rtb_DiscreteTransferFcn1_idx_1;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.025s, 0.0s] to Sample time: [0.05s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_1)++;
  if ((rtM->Timing.RateInteraction.TID0_1) > 1) {
    rtM->Timing.RateInteraction.TID0_1 = 0;
  }

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.025s, 0.0s] to Sample time: [0.1s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_2)++;
  if ((rtM->Timing.RateInteraction.TID0_2) > 3) {
    rtM->Timing.RateInteraction.TID0_2 = 0;
  }

  /* RateTransition: '<S3>/Rate Transition2' incorporates:
   *  Inport: '<Root>/mag2_body_T'
   */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    rtDW.RateTransition2[0] = rtU.mag2_vec_body_T[0];

    /* RateTransition: '<S3>/Rate Transition' incorporates:
     *  Inport: '<Root>/mag1_body_T'
     *  Inport: '<Root>/mag2_body_T'
     */
    rtDW.RateTransition[0] = rtU.mag1_vec_body_T[0];
    rtDW.RateTransition2[1] = rtU.mag2_vec_body_T[1];

    /* RateTransition: '<S3>/Rate Transition' incorporates:
     *  Inport: '<Root>/mag1_body_T'
     *  Inport: '<Root>/mag2_body_T'
     */
    rtDW.RateTransition[1] = rtU.mag1_vec_body_T[1];
    rtDW.RateTransition2[2] = rtU.mag2_vec_body_T[2];

    /* RateTransition: '<S3>/Rate Transition' incorporates:
     *  Inport: '<Root>/mag1_body_T'
     *  Inport: '<Root>/mag2_body_T'
     */
    rtDW.RateTransition[2] = rtU.mag1_vec_body_T[2];
    rtDW.RateTransition2[3] = rtU.mag2_vec_body_T[3];

    /* RateTransition: '<S3>/Rate Transition' incorporates:
     *  Inport: '<Root>/mag1_body_T'
     *  Inport: '<Root>/mag2_body_T'
     */
    rtDW.RateTransition[3] = rtU.mag1_vec_body_T[3];
  }

  /* End of RateTransition: '<S3>/Rate Transition2' */

  /* DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn1' */
  rtb_DiscreteTransferFcn1_idx_0 = 0.145364F * rtDW.DiscreteTransferFcn1_states
    [0L];
  rtb_DiscreteTransferFcn1_idx_1 = 0.145364F * rtDW.DiscreteTransferFcn1_states
    [1L];
  denAccum = 0.145364F * rtDW.DiscreteTransferFcn1_states[2L];

  /* RateTransition: '<S2>/Rate Transition1' */
  if (rtM->Timing.RateInteraction.TID0_2 == 1) {
    /* Outport: '<Root>/omega_radps_processed' incorporates:
     *  Inport: '<Root>/omega_radps_gyro'
     */
    rtY.omega_radps_processed[3] = rtU.omega_body_radps_gyro[3];
    for (i = 0; i < 3; i++) {
      /* Outport: '<Root>/omega_radps_processed' incorporates:
       *  Constant: '<S2>/Constant1'
       *  DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn1'
       *  Product: '<S2>/Product'
       */
      rtY.omega_radps_processed[i] = rtConstP.pooled1[i + 6] * denAccum +
        (rtConstP.pooled1[i + 3] * rtb_DiscreteTransferFcn1_idx_1 +
         rtConstP.pooled1[i] * rtb_DiscreteTransferFcn1_idx_0);

      /* RateTransition: '<S4>/Rate Transition1' incorporates:
       *  Inport: '<Root>/sun_vec_body_angles'
       */
      rtDW.RateTransition1_m[i] = rtU.sun_vec_body_sunsensor[i];
    }
  }

  /* End of RateTransition: '<S2>/Rate Transition1' */

  /* Update for DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn1' incorporates:
   *  Update for Inport: '<Root>/omega_radps_gyro'
   */
  rtDW.DiscreteTransferFcn1_states[0L] = rtU.omega_body_radps_gyro[0L] -
    -0.854636F * rtDW.DiscreteTransferFcn1_states[0L];
  rtDW.DiscreteTransferFcn1_states[1L] = rtU.omega_body_radps_gyro[1L] -
    -0.854636F * rtDW.DiscreteTransferFcn1_states[1L];
  rtDW.DiscreteTransferFcn1_states[2L] = rtU.omega_body_radps_gyro[2L] -
    -0.854636F * rtDW.DiscreteTransferFcn1_states[2L];
}

/* Model step function for TID1 */
void MSP_SP_step1(void)                /* Sample time: [0.05s, 0.0s] */
{
  real32_T denAccum;
  real32_T tmp[3];
  int16_T i;
  real32_T rtb_Switch_idx_0;
  real32_T rtb_Switch_idx_1;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.05s, 0.0s] to Sample time: [0.1s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID1_2)++;
  if ((rtM->Timing.RateInteraction.TID1_2) > 1) {
    rtM->Timing.RateInteraction.TID1_2 = 0;
  }

  /* DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' */
  rtb_Switch_idx_0 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[0L];
  rtb_Switch_idx_1 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[1L];
  denAccum = 0.0608986318F * rtDW.DiscreteTransferFcn_states[2L];

  /* RateTransition: '<S3>/Rate Transition1' */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    /* Product: '<S3>/Product' incorporates:
     *  Constant: '<S3>/process_matrix'
     *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
     *  Product: '<S3>/Product1'
     */
    for (i = 0; i < 3; i++) {
      tmp[i] = rtConstP.pooled1[i + 6] * denAccum + (rtConstP.pooled1[i + 3] *
        rtb_Switch_idx_1 + rtConstP.pooled1[i] * rtb_Switch_idx_0);
    }

    /* End of Product: '<S3>/Product' */

    /* Product: '<S3>/Product1' */
    for (i = 0; i < 3; i++) {
      /* Outport: '<Root>/mag_body_processed_T' incorporates:
       *  Constant: '<S3>/sensor2body'
       */
      rtY.mag_body_processed_T[i] = rtConstP.pooled1[i + 6] * tmp[2] +
        (rtConstP.pooled1[i + 3] * tmp[1] + rtConstP.pooled1[i] * tmp[0]);
    }

    /* Outport: '<Root>/mag_body_processed_T' incorporates:
     *  DataTypeConversion: '<S3>/Data Type Conversion'
     *  Logic: '<S3>/Logical Operator'
     */
    rtY.mag_body_processed_T[3] = ((rtDW.RateTransition2[3] != 0.0F) ||
      (rtDW.RateTransition[3] != 0.0F));
  }

  /* End of RateTransition: '<S3>/Rate Transition1' */

  /* Switch: '<S3>/Switch1' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[0];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[0];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Gain: '<S3>/Gain'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[0L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) *
    0.5F - -0.939101338F * rtDW.DiscreteTransferFcn_states[0L];

  /* Switch: '<S3>/Switch1' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[1];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[1];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Gain: '<S3>/Gain'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[1L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) *
    0.5F - -0.939101338F * rtDW.DiscreteTransferFcn_states[1L];

  /* Switch: '<S3>/Switch1' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[2];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[2];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Gain: '<S3>/Gain'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[2L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) *
    0.5F - -0.939101338F * rtDW.DiscreteTransferFcn_states[2L];
}

/* Model step function for TID2 */
void MSP_SP_step2(void)                /* Sample time: [0.1s, 0.0s] */
{
  real32_T rtb_TrigonometricFunction;
  real32_T rtb_TrigonometricFunction1;
  real32_T tmp[3];
  int16_T i;
  real32_T u0;

  /* Trigonometry: '<S5>/Trigonometric Function' */
  rtb_TrigonometricFunction = sinf(rtDW.RateTransition1_m[0]);

  /* Trigonometry: '<S5>/Trigonometric Function1' */
  rtb_TrigonometricFunction1 = sinf(rtDW.RateTransition1_m[1]);

  /* Sqrt: '<S5>/Sqrt' incorporates:
   *  Constant: '<S5>/Constant'
   *  Math: '<S5>/Math Function'
   *  Math: '<S5>/Math Function1'
   *  Saturate: '<S5>/Saturation'
   *  Sum: '<S5>/Sum'
   */
  u0 = (1.0F - rtb_TrigonometricFunction * rtb_TrigonometricFunction) -
    rtb_TrigonometricFunction1 * rtb_TrigonometricFunction1;
  if (u0 > 1.0F) {
    u0 = 1.0F;
  } else {
    if (u0 < 0.0F) {
      u0 = 0.0F;
    }
  }

  /* Sum: '<S4>/Sum' incorporates:
   *  Sqrt: '<S5>/Sqrt'
   */
  u0 = sqrtf(u0);

  /* Product: '<S4>/Product' incorporates:
   *  Constant: '<S4>/process_matrix'
   *  Product: '<S4>/Product1'
   *  Sum: '<S4>/Sum'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = rtConstP.pooled1[i + 6] * u0 + (rtConstP.pooled1[i + 3] *
      rtb_TrigonometricFunction1 + rtConstP.pooled1[i] *
      rtb_TrigonometricFunction);
  }

  /* End of Product: '<S4>/Product' */
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sun_vec_body' incorporates:
     *  Constant: '<S4>/sensor2body'
     *  Product: '<S4>/Product1'
     */
    rtY.sun_vec_body[i] = rtConstP.pooled1[i + 6] * tmp[2] + (rtConstP.pooled1[i
      + 3] * tmp[1] + rtConstP.pooled1[i] * tmp[0]);
  }

  /* Outport: '<Root>/sun_vec_body' */
  rtY.sun_vec_body[3] = rtDW.RateTransition1_m[2];
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
