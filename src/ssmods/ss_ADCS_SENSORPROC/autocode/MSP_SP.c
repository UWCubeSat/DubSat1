/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.c
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.359
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Fri Apr 27 17:45:09 2018
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
extern real32_T rt_roundf_snf(real32_T u);
real32_T rt_roundf_snf(real32_T u)
{
  real32_T y;
  if ((real32_T)fabs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = (real32_T)floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = (real32_T)ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

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

      /* RateTransition: '<S5>/Rate Transition1' incorporates:
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
  boolean_T rtb_LogicalOperator_o;
  real32_T rtb_Switch2;
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
  rtb_Switch2 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[2L];

  /* Logic: '<S3>/Logical Operator' */
  rtb_LogicalOperator_o = ((rtDW.RateTransition2[3] != 0.0F) ||
    (rtDW.RateTransition[3] != 0.0F));

  /* RateTransition: '<S3>/Rate Transition1' */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    /* Product: '<S3>/Product' incorporates:
     *  Constant: '<S3>/process_matrix'
     *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
     *  Product: '<S3>/Product1'
     */
    for (i = 0; i < 3; i++) {
      tmp[i] = rtConstP.pooled1[i + 6] * rtb_Switch2 + (rtConstP.pooled1[i + 3] *
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
     */
    rtY.mag_body_processed_T[3] = rtb_LogicalOperator_o;
  }

  /* End of RateTransition: '<S3>/Rate Transition1' */

  /* Switch: '<S3>/Switch2' incorporates:
   *  Constant: '<S3>/1'
   *  Constant: '<S3>/2'
   */
  if (rtb_LogicalOperator_o) {
    rtb_Switch2 = 2.0F;
  } else {
    rtb_Switch2 = 1.0F;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[0];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[0];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[0L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) /
    rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[0L];

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[1];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[1];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[1L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) /
    rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[1L];

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_idx_0 = rtDW.RateTransition[2];
  } else {
    rtb_Switch_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_idx_1 = rtDW.RateTransition2[2];
  } else {
    rtb_Switch_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[2L] = (rtb_Switch_idx_0 + rtb_Switch_idx_1) /
    rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[2L];
}

/* Model step function for TID2 */
void MSP_SP_step2(void)                /* Sample time: [0.1s, 0.0s] */
{
  real32_T rtb_Abs;
  real32_T rtb_TrigonometricFunction;
  real32_T rtb_Abs1;
  real32_T rtb_TrigonometricFunction1;
  real32_T tmp[3];
  int16_T i;
  real32_T u0;

  /* Gain: '<S7>/deg2rad' */
  rtb_Abs = 0.0174532924F * rtDW.RateTransition1_m[0];

  /* Trigonometry: '<S7>/Trigonometric Function' */
  rtb_TrigonometricFunction = (real32_T)sin(rtb_Abs);

  /* Gain: '<S7>/deg2rad ' */
  rtb_Abs1 = 0.0174532924F * rtDW.RateTransition1_m[1];

  /* Trigonometry: '<S7>/Trigonometric Function1' */
  rtb_TrigonometricFunction1 = (real32_T)sin(rtb_Abs1);

  /* Abs: '<S7>/Abs' */
  rtb_Abs = (real32_T)fabs(rtb_Abs);

  /* Abs: '<S7>/Abs1' */
  rtb_Abs1 = (real32_T)fabs(rtb_Abs1);

  /* Sqrt: '<S7>/Sqrt' incorporates:
   *  Constant: '<S7>/Constant'
   *  Math: '<S7>/Math Function'
   *  Math: '<S7>/Math Function1'
   *  Saturate: '<S7>/Saturation'
   *  Sum: '<S7>/Sum'
   */
  /* MATLAB Function 'sunsensor_processing_lib/MATLAB Function': '<S6>:1' */
  /* '<S6>:1:4' flag_bool = logical(round(flag_float)); */
  u0 = (1.0F - rtb_TrigonometricFunction * rtb_TrigonometricFunction) -
    rtb_TrigonometricFunction1 * rtb_TrigonometricFunction1;
  if (u0 > 1.0F) {
    u0 = 1.0F;
  } else {
    if (u0 < 0.0F) {
      u0 = 0.0F;
    }
  }

  /* Sum: '<S5>/Sum' incorporates:
   *  Sqrt: '<S7>/Sqrt'
   */
  u0 = (real32_T)sqrt(u0);

  /* Product: '<S5>/Product' incorporates:
   *  Constant: '<S5>/process_matrix'
   *  Product: '<S5>/Product1'
   *  Sum: '<S5>/Sum'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = rtConstP.pooled1[i + 6] * u0 + (rtConstP.pooled1[i + 3] *
      rtb_TrigonometricFunction1 + rtConstP.pooled1[i] *
      rtb_TrigonometricFunction);
  }

  /* End of Product: '<S5>/Product' */
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sun_vec_body' incorporates:
     *  Constant: '<S5>/sensor2body'
     *  Product: '<S5>/Product1'
     */
    rtY.sun_vec_body[i] = rtConstP.pooled1[i + 6] * tmp[2] + (rtConstP.pooled1[i
      + 3] * tmp[1] + rtConstP.pooled1[i] * tmp[0]);
  }

  /* Outport: '<Root>/sun_vec_body' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion'
   *  Logic: '<S5>/Logical Operator'
   *  Logic: '<S7>/Logical Operator'
   *  MATLAB Function: '<S5>/MATLAB Function'
   *  Switch: '<S7>/Switch'
   *  Switch: '<S7>/Switch1'
   */
  rtY.sun_vec_body[3] = ((!(rtb_Abs > 1.04719758F)) && (!(rtb_Abs1 > 1.04719758F))
    && (rt_roundf_snf(rtDW.RateTransition1_m[2]) != 0.0F));
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
