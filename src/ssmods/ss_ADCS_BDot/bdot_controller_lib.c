/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bdot_controller_lib.c
 *
 * Code generated for Simulink model 'bdot_controller_lib'.
 *
 * Model version                  : 1.28
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Aug 28 19:59:51 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "bdot_controller_lib.h"

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
void bdot_controller_lib_step0(void)   /* Sample time: [0.025s, 0.0s] */
{
  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.025s, 0.0s] to Sample time: [0.1s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_1)++;
  if ((rtM->Timing.RateInteraction.TID0_1) > 3) {
    rtM->Timing.RateInteraction.TID0_1 = 0;
  }

  /* RateTransition: '<S1>/Rate Transition3' incorporates:
   *  Inport: '<Root>/MT_on'
   */
  if (!(rtDW.RateTransition3_semaphoreTaken != 0)) {
    rtDW.RateTransition3_Buffer0 = rtU.MT_on;
  }

  /* End of RateTransition: '<S1>/Rate Transition3' */

  /* RateTransition: '<S1>/Rate Transition2' incorporates:
   *  Inport: '<Root>/B_meas_valid'
   */
  if (!(rtDW.RateTransition2_semaphoreTaken != 0)) {
    rtDW.RateTransition2_Buffer0 = rtU.B_meas_valid;
  }

  /* End of RateTransition: '<S1>/Rate Transition2' */

  /* RateTransition: '<S1>/Rate Transition1' */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    /* Outport: '<Root>/Dig_val' */
    rtY.Dig_val[0] = rtDW.RateTransition1_Buffer0[0];
    rtY.Dig_val[1] = rtDW.RateTransition1_Buffer0[1];
    rtY.Dig_val[2] = rtDW.RateTransition1_Buffer0[2];
  }

  /* End of RateTransition: '<S1>/Rate Transition1' */

  /* RateTransition: '<S1>/Rate Transition4' incorporates:
   *  Inport: '<Root>/B_body_in_T'
   */
  if (!(rtDW.RateTransition4_semaphoreTaken != 0)) {
    rtDW.RateTransition4_Buffer0[0] = rtU.B_body_in_T[0];
    rtDW.RateTransition4_Buffer0[1] = rtU.B_body_in_T[1];
    rtDW.RateTransition4_Buffer0[2] = rtU.B_body_in_T[2];
  }

  /* End of RateTransition: '<S1>/Rate Transition4' */
}

/* Model step function for TID1 */
void bdot_controller_lib_step1(void)   /* Sample time: [0.1s, 0.0s] */
{
  int16_T k;
  real_T rtb_RateTransition3;
  boolean_T rtb_LogicalOperator1;
  real_T rtb_RateTransition2;
  real_T rtb_Sum[3];
  real_T rtb_MultiportSwitch1[3];
  real_T rtb_MultiportSwitch2[3];
  real_T rtb_RateTransition4[3];
  real_T Product[3];
  real_T u0;

  /* RateTransition: '<S1>/Rate Transition3' */
  rtDW.RateTransition3_semaphoreTaken = 1;
  rtb_RateTransition3 = rtDW.RateTransition3_Buffer0;
  rtDW.RateTransition3_semaphoreTaken = 0;

  /* Logic: '<S1>/Logical Operator1' */
  rtb_LogicalOperator1 = !(rtb_RateTransition3 != 0.0);

  /* RateTransition: '<S1>/Rate Transition2' */
  rtDW.RateTransition2_semaphoreTaken = 1;
  rtb_RateTransition2 = rtDW.RateTransition2_Buffer0;
  rtDW.RateTransition2_semaphoreTaken = 0;

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_Sum[0] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[0L] * 10.0;
  rtb_Sum[1] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[1L] * 10.0;
  rtb_Sum[2] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[2L] * 10.0;

  /* MultiPortSwitch: '<S1>/Multiport Switch1' incorporates:
   *  Logic: '<S1>/Logical Operator'
   *  Sum: '<S2>/Diff'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S2>/UD'
   *
   * Block description for '<S2>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S2>/UD':
   *
   *  Store in Global RAM
   */
  if (!(rtb_LogicalOperator1 && (rtb_RateTransition2 != 0.0))) {
    rtb_MultiportSwitch1[0] = rtDW.UnitDelay_DSTATE[0];
    rtb_MultiportSwitch1[1] = rtDW.UnitDelay_DSTATE[1];
    rtb_MultiportSwitch1[2] = rtDW.UnitDelay_DSTATE[2];
  } else {
    rtb_MultiportSwitch1[0] = rtb_Sum[0] - rtDW.UD_DSTATE[0];
    rtb_MultiportSwitch1[1] = rtb_Sum[1] - rtDW.UD_DSTATE[1];
    rtb_MultiportSwitch1[2] = rtb_Sum[2] - rtDW.UD_DSTATE[2];
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch1' */

  /* MultiPortSwitch: '<S1>/Multiport Switch2' incorporates:
   *  Gain: '<S1>/To DigVal1'
   */
  if (!rtb_LogicalOperator1) {
    rtb_MultiportSwitch2[0] = 0.0;
    rtb_MultiportSwitch2[1] = 0.0;
    rtb_MultiportSwitch2[2] = 0.0;
  } else {
    /* Gain: '<S1>/Control Gain' */
    u0 = -75000.0 * rtb_MultiportSwitch1[0];

    /* Saturate: '<S1>/Saturation' */
    if (u0 > 0.15) {
      u0 = 0.15;
    } else {
      if (u0 < -0.15) {
        u0 = -0.15;
      }
    }

    rtb_MultiportSwitch2[0] = 1700.0000000000002 * u0;

    /* Gain: '<S1>/Control Gain' incorporates:
     *  Gain: '<S1>/To DigVal1'
     */
    u0 = -75000.0 * rtb_MultiportSwitch1[1];

    /* Saturate: '<S1>/Saturation' */
    if (u0 > 0.15) {
      u0 = 0.15;
    } else {
      if (u0 < -0.15) {
        u0 = -0.15;
      }
    }

    rtb_MultiportSwitch2[1] = 1700.0000000000002 * u0;

    /* Gain: '<S1>/Control Gain' incorporates:
     *  Gain: '<S1>/To DigVal1'
     */
    u0 = -75000.0 * rtb_MultiportSwitch1[2];

    /* Saturate: '<S1>/Saturation' */
    if (u0 > 0.15) {
      u0 = 0.15;
    } else {
      if (u0 < -0.15) {
        u0 = -0.15;
      }
    }

    rtb_MultiportSwitch2[2] = 1700.0000000000002 * u0;
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch2' */

  /* RateTransition: '<S1>/Rate Transition4' */
  rtDW.RateTransition4_semaphoreTaken = 1;
  rtb_RateTransition4[0] = rtDW.RateTransition4_Buffer0[0];
  rtb_RateTransition4[1] = rtDW.RateTransition4_Buffer0[1];
  rtb_RateTransition4[2] = rtDW.RateTransition4_Buffer0[2];
  rtDW.RateTransition4_semaphoreTaken = 0;
  for (k = 0; k < 3; k++) {
    /* Product: '<S3>/Product' incorporates:
     *  Constant: '<S3>/Constant1'
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Sum: '<S3>/Sum'
     */
    Product[k] = 0.0;
    Product[k] += rtConstP.Constant1_Value[k] * rtb_RateTransition4[0];
    Product[k] += rtConstP.Constant1_Value[k + 3] * rtb_RateTransition4[1];
    Product[k] += rtConstP.Constant1_Value[k + 6] * rtb_RateTransition4[2];

    /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     */
    rtDW.UnitDelay_DSTATE[k] = rtb_MultiportSwitch1[k];

    /* Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' */
    rtDW.DiscreteTransferFcn_states[(int32_T)k] = Product[(int32_T)k] -
      -0.93910136742429262 * rtDW.DiscreteTransferFcn_states[(int32_T)k];

    /* Update for UnitDelay: '<S2>/UD' incorporates:
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *
     * Block description for '<S2>/UD':
     *
     *  Store in Global RAM
     */
    rtDW.UD_DSTATE[k] = rtb_Sum[k];

    /* Update for RateTransition: '<S1>/Rate Transition1' incorporates:
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     */
    rtDW.RateTransition1_Buffer0[k] = rtb_MultiportSwitch2[k];
  }
}

/* Model initialize function */
void bdot_controller_lib_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void bdot_controller_lib_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
