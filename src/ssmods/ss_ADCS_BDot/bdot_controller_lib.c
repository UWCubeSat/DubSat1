/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bdot_controller_lib.c
 *
 * Code generated for Simulink model 'bdot_controller_lib'.
 *
 * Model version                  : 1.330
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Wed Apr 11 19:37:04 2018
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

/* Model step function */
void bdot_controller_lib_step(void)
{
  int16_T k;
  real_T rtb_Diff[3];
  real_T rtb_TSamp[3];
  real_T rtb_MultiportSwitch1[3];
  real_T rtb_Product[3];
  real_T rtb_Sum_idx_0;
  real_T rtb_Sum_idx_1;
  real_T rtb_Sum_idx_2;
  real_T v;

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp[0] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[0L] *
    10.0;

  /* Sum: '<S2>/Diff' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
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
  rtb_Diff[0] = rtb_TSamp[0] - rtDW.UD_DSTATE[0];

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp[1] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[1L] *
    10.0;

  /* Sum: '<S2>/Diff' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
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
  rtb_Diff[1] = rtb_TSamp[1] - rtDW.UD_DSTATE[1];

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp[2] = 0.060898632575707344 * rtDW.DiscreteTransferFcn_states[2L] *
    10.0;

  /* Sum: '<S2>/Diff' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
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
  rtb_Diff[2] = rtb_TSamp[2] - rtDW.UD_DSTATE[2];

  /* MultiPortSwitch: '<S1>/Multiport Switch1' incorporates:
   *  Inport: '<Root>/B_meas_valid'
   *  Inport: '<Root>/MT_on'
   *  Logic: '<S1>/Logical Operator'
   *  Logic: '<S1>/Logical Operator1'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  if (!((!(rtU.MT_on != 0.0)) && (rtU.B_meas_valid != 0.0))) {
    rtb_MultiportSwitch1[0] = rtDW.UnitDelay_DSTATE[0];
    rtb_MultiportSwitch1[1] = rtDW.UnitDelay_DSTATE[1];
    rtb_MultiportSwitch1[2] = rtDW.UnitDelay_DSTATE[2];
  } else {
    rtb_MultiportSwitch1[0] = rtb_Diff[0];
    rtb_MultiportSwitch1[1] = rtb_Diff[1];
    rtb_MultiportSwitch1[2] = rtb_Diff[2];
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch1' */

  /* MultiPortSwitch: '<S1>/Multiport Switch2' incorporates:
   *  Gain: '<S1>/To DigVal1'
   *  Gain: '<S1>/To DigVal2'
   *  Gain: '<S1>/To DigVal3'
   *  Inport: '<Root>/MT_on'
   */
  if ((int16_T)rtU.MT_on != 0) {
    rtb_Sum_idx_0 = 0.0;
    rtb_Sum_idx_1 = 0.0;
    rtb_Sum_idx_2 = 0.0;
  } else {
    /* Product: '<S1>/Product' incorporates:
     *  Constant: '<S1>/gain matrix'
     */
    for (k = 0; k < 3; k++) {
      rtb_Product[k] = rtConstP.gainmatrix_Value[k + 6] * rtb_MultiportSwitch1[2]
        + (rtConstP.gainmatrix_Value[k + 3] * rtb_MultiportSwitch1[1] +
           rtConstP.gainmatrix_Value[k] * rtb_MultiportSwitch1[0]);
    }

    /* End of Product: '<S1>/Product' */

    /* Saturate: '<S1>/Saturation1' */
    if (rtb_Product[0] > 0.15) {
      rtb_Sum_idx_2 = 0.15;
    } else if (rtb_Product[0] < -0.15) {
      rtb_Sum_idx_2 = -0.15;
    } else {
      rtb_Sum_idx_2 = rtb_Product[0];
    }

    /* End of Saturate: '<S1>/Saturation1' */
    rtb_Sum_idx_0 = 846.66666666666663 * rtb_Sum_idx_2;

    /* Saturate: '<S1>/Saturation2' incorporates:
     *  Gain: '<S1>/To DigVal1'
     */
    if (rtb_Product[1] > 0.15) {
      rtb_Sum_idx_2 = 0.15;
    } else if (rtb_Product[1] < -0.15) {
      rtb_Sum_idx_2 = -0.15;
    } else {
      rtb_Sum_idx_2 = rtb_Product[1];
    }

    /* End of Saturate: '<S1>/Saturation2' */
    rtb_Sum_idx_1 = 846.66666666666663 * rtb_Sum_idx_2;

    /* Saturate: '<S1>/Saturation3' incorporates:
     *  Gain: '<S1>/To DigVal2'
     */
    if (rtb_Product[2] > 0.15) {
      rtb_Sum_idx_2 = 0.15;
    } else if (rtb_Product[2] < -0.15) {
      rtb_Sum_idx_2 = -0.15;
    } else {
      rtb_Sum_idx_2 = rtb_Product[2];
    }

    /* End of Saturate: '<S1>/Saturation3' */
    rtb_Sum_idx_2 *= 846.66666666666663;
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch2' */

  /* DataTypeConversion: '<S1>/Data Type Conversion' */
  v = fabs(rtb_Sum_idx_0);
  if (v < 4.503599627370496E+15) {
    if (v >= 0.5) {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[0] = (int8_T)floor(rtb_Sum_idx_0 + 0.5);
    } else {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[0] = (int8_T)(rtb_Sum_idx_0 * 0.0);
    }
  } else {
    /* Outport: '<Root>/Dig_val' */
    rtY.Dig_val[0] = (int8_T)rtb_Sum_idx_0;
  }

  v = fabs(rtb_Sum_idx_1);
  if (v < 4.503599627370496E+15) {
    if (v >= 0.5) {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[1] = (int8_T)floor(rtb_Sum_idx_1 + 0.5);
    } else {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[1] = (int8_T)(rtb_Sum_idx_1 * 0.0);
    }
  } else {
    /* Outport: '<Root>/Dig_val' */
    rtY.Dig_val[1] = (int8_T)rtb_Sum_idx_1;
  }

  v = fabs(rtb_Sum_idx_2);
  if (v < 4.503599627370496E+15) {
    if (v >= 0.5) {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[2] = (int8_T)floor(rtb_Sum_idx_2 + 0.5);
    } else {
      /* Outport: '<Root>/Dig_val' */
      rtY.Dig_val[2] = (int8_T)(rtb_Sum_idx_2 * 0.0);
    }
  } else {
    /* Outport: '<Root>/Dig_val' */
    rtY.Dig_val[2] = (int8_T)rtb_Sum_idx_2;
  }

  /* End of DataTypeConversion: '<S1>/Data Type Conversion' */

  /* Sqrt: '<S1>/Sqrt' incorporates:
   *  DotProduct: '<S1>/Dot Product'
   */
  rtb_Sum_idx_0 = sqrt((rtb_Diff[0] * rtb_Diff[0] + rtb_Diff[1] * rtb_Diff[1]) +
                       rtb_Diff[2] * rtb_Diff[2]);

  /* Relay: '<S1>/Relay' */
  if (rtb_Sum_idx_0 >= 2.9698484809834993E-6) {
    rtDW.Relay_Mode = true;
  } else {
    if (rtb_Sum_idx_0 <= 1.0790449480906716E-7) {
      rtDW.Relay_Mode = false;
    }
  }

  /* Outport: '<Root>/tumble' incorporates:
   *  Relay: '<S1>/Relay'
   */
  rtY.tumble = rtDW.Relay_Mode;
  for (k = 0; k < 3; k++) {
    /* Product: '<S3>/Product' incorporates:
     *  Constant: '<S3>/Constant1'
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Inport: '<Root>/B_body_in_T'
     *  Sum: '<S3>/Sum'
     */
    rtb_Diff[k] = 0.0;
    rtb_Diff[k] += rtConstP.Constant1_Value[k] * rtU.B_body_in_T[0];
    rtb_Diff[k] += rtConstP.Constant1_Value[k + 3] * rtU.B_body_in_T[1];
    rtb_Diff[k] += rtConstP.Constant1_Value[k + 6] * rtU.B_body_in_T[2];

    /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     */
    rtDW.UnitDelay_DSTATE[k] = rtb_MultiportSwitch1[k];

    /* Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' */
    rtDW.DiscreteTransferFcn_states[(int32_T)k] = rtb_Diff[(int32_T)k] -
      -0.93910136742429262 * rtDW.DiscreteTransferFcn_states[(int32_T)k];

    /* Update for UnitDelay: '<S2>/UD' incorporates:
     *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *  Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
     *
     * Block description for '<S2>/UD':
     *
     *  Store in Global RAM
     */
    rtDW.UD_DSTATE[k] = rtb_TSamp[k];
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
