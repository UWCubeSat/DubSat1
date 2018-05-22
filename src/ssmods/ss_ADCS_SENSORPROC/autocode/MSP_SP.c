/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.c
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.380
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon May 21 16:28:33 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MSP_SP.h"
#define NumBitsPerChar                 8U

/* Block signals and states (auto storage) */
DW rtDW;

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern real_T rtGetInf(void);
extern real32_T rtGetInfF(void);
extern real_T rtGetMinusInf(void);
extern real32_T rtGetMinusInfF(void);
extern real_T rtGetNaN(void);
extern real32_T rtGetNaNF(void);
extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
extern void rt_InitInfAndNaN(size_t realSize);
extern boolean_T rtIsInf(real_T value);
extern boolean_T rtIsInfF(real32_T value);
extern boolean_T rtIsNaN(real_T value);
extern boolean_T rtIsNaNF(real32_T value);
typedef struct {
  struct {
    uint32_T wordH;
    uint32_T wordL;
  } words;
} BigEndianIEEEDouble;

typedef struct {
  struct {
    uint32_T wordL;
    uint32_T wordH;
  } words;
} LittleEndianIEEEDouble;

typedef struct {
  union {
    real32_T wordLreal;
    uint32_T wordLuint;
  } wordL;
} IEEESingle;

real_T rtInf;
real_T rtMinusInf;
real_T rtNaN;
real32_T rtInfF;
real32_T rtMinusInfF;
real32_T rtNaNF;

/*
 * Initialize rtInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T inf = 0.0;
  if (bitsPerReal == 32U) {
    inf = rtGetInfF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0x7FF00000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    inf = tmpVal.fltVal;
  }

  return inf;
}

/*
 * Initialize rtInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetInfF(void)
{
  IEEESingle infF;
  infF.wordL.wordLuint = 0x7F800000U;
  return infF.wordL.wordLreal;
}

/*
 * Initialize rtMinusInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetMinusInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T minf = 0.0;
  if (bitsPerReal == 32U) {
    minf = rtGetMinusInfF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0xFFF00000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    minf = tmpVal.fltVal;
  }

  return minf;
}

/*
 * Initialize rtMinusInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetMinusInfF(void)
{
  IEEESingle minfF;
  minfF.wordL.wordLuint = 0xFF800000U;
  return minfF.wordL.wordLreal;
}

/*
 * Initialize rtNaN needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetNaN(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T nan = 0.0;
  if (bitsPerReal == 32U) {
    nan = rtGetNaNF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0xFFF80000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    nan = tmpVal.fltVal;
  }

  return nan;
}

/*
 * Initialize rtNaNF needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetNaNF(void)
{
  IEEESingle nanF = { { 0 } };

  nanF.wordL.wordLuint = 0xFFC00000U;
  return nanF.wordL.wordLreal;
}

/*
 * Initialize the rtInf, rtMinusInf, and rtNaN needed by the
 * generated code. NaN is initialized as non-signaling. Assumes IEEE.
 */
void rt_InitInfAndNaN(size_t realSize)
{
  (void) (realSize);
  rtNaN = rtGetNaN();
  rtNaNF = rtGetNaNF();
  rtInf = rtGetInf();
  rtInfF = rtGetInfF();
  rtMinusInf = rtGetMinusInf();
  rtMinusInfF = rtGetMinusInfF();
}

/* Test if value is infinite */
boolean_T rtIsInf(real_T value)
{
  return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
}

/* Test if single-precision value is infinite */
boolean_T rtIsInfF(real32_T value)
{
  return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
}

/* Test if value is not a number */
boolean_T rtIsNaN(real_T value)
{
  return (boolean_T)((value!=value) ? 1U : 0U);
}

/* Test if single-precision value is not a number */
boolean_T rtIsNaNF(real32_T value)
{
  return (boolean_T)(((value!=value) ? 1U : 0U));
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
      rtY.omega_radps_processed[i] = rtConstP.pooled4[i + 6] * denAccum +
        (rtConstP.pooled4[i + 3] * rtb_DiscreteTransferFcn1_idx_1 +
         rtConstP.pooled4[i] * rtb_DiscreteTransferFcn1_idx_0);

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
  real32_T rtb_Switch2;
  real32_T tmp[3];
  int16_T i;
  real32_T rtb_Switch_b_idx_0;
  real32_T rtb_Switch_b_idx_1;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.05s, 0.0s] to Sample time: [0.1s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID1_2)++;
  if ((rtM->Timing.RateInteraction.TID1_2) > 1) {
    rtM->Timing.RateInteraction.TID1_2 = 0;
  }

  /* DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' */
  rtb_Switch_b_idx_0 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[0L];
  rtb_Switch_b_idx_1 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[1L];
  rtb_Switch2 = 0.0608986318F * rtDW.DiscreteTransferFcn_states[2L];

  /* RateTransition: '<S3>/Rate Transition1' */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    /* Product: '<S3>/Product' incorporates:
     *  Constant: '<S3>/process_matrix'
     *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
     *  Product: '<S3>/Product1'
     */
    for (i = 0; i < 3; i++) {
      tmp[i] = rtConstP.pooled4[i + 6] * rtb_Switch2 + (rtConstP.pooled4[i + 3] *
        rtb_Switch_b_idx_1 + rtConstP.pooled4[i] * rtb_Switch_b_idx_0);
    }

    /* End of Product: '<S3>/Product' */

    /* Product: '<S3>/Product1' */
    for (i = 0; i < 3; i++) {
      /* Outport: '<Root>/mag_body_processed_T' incorporates:
       *  Constant: '<S3>/sensor2body'
       */
      rtY.mag_body_processed_T[i] = rtConstP.pooled4[i + 6] * tmp[2] +
        (rtConstP.pooled4[i + 3] * tmp[1] + rtConstP.pooled4[i] * tmp[0]);
    }

    /* Outport: '<Root>/mag_body_processed_T' incorporates:
     *  DataTypeConversion: '<S3>/Data Type Conversion'
     *  Logic: '<S3>/Logical Operator'
     */
    rtY.mag_body_processed_T[3] = ((rtDW.RateTransition2[3] != 0.0F) ||
      (rtDW.RateTransition[3] != 0.0F));
  }

  /* End of RateTransition: '<S3>/Rate Transition1' */

  /* Switch: '<S3>/Switch2' incorporates:
   *  Constant: '<S3>/1'
   *  Constant: '<S3>/2'
   *  Logic: '<S3>/Logical Operator1'
   */
  if ((rtDW.RateTransition2[3] != 0.0F) && (rtDW.RateTransition[3] != 0.0F)) {
    rtb_Switch2 = 2.0F;
  } else {
    rtb_Switch2 = 1.0F;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_b_idx_0 = rtDW.RateTransition[0];
  } else {
    rtb_Switch_b_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_b_idx_1 = rtDW.RateTransition2[0];
  } else {
    rtb_Switch_b_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[0L] = (rtb_Switch_b_idx_0 + rtb_Switch_b_idx_1)
    / rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[0L];

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_b_idx_0 = rtDW.RateTransition[1];
  } else {
    rtb_Switch_b_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_b_idx_1 = rtDW.RateTransition2[1];
  } else {
    rtb_Switch_b_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[1L] = (rtb_Switch_b_idx_0 + rtb_Switch_b_idx_1)
    / rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[1L];

  /* Switch: '<S3>/Switch1' */
  if (rtDW.RateTransition[3] != 0.0F) {
    rtb_Switch_b_idx_0 = rtDW.RateTransition[2];
  } else {
    rtb_Switch_b_idx_0 = 0.0F;
  }

  /* Switch: '<S3>/Switch' */
  if (rtDW.RateTransition2[3] != 0.0F) {
    rtb_Switch_b_idx_1 = rtDW.RateTransition2[2];
  } else {
    rtb_Switch_b_idx_1 = 0.0F;
  }

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtDW.DiscreteTransferFcn_states[2L] = (rtb_Switch_b_idx_0 + rtb_Switch_b_idx_1)
    / rtb_Switch2 - -0.939101338F * rtDW.DiscreteTransferFcn_states[2L];
}

/* Model step function for TID2 */
void MSP_SP_step2(void)                /* Sample time: [0.1s, 0.0s] */
{
  real32_T rtb_DataTypeConversion2;
  real32_T rtb_DataTypeConversion1;
  real_T rtb_ReciprocalSqrt;
  int8_T rtPrevAction;
  int8_T rtAction;
  real32_T rtb_Switch;
  real32_T tmp[3];
  int16_T i;

  /* Trigonometry: '<S7>/Trigonometric Function' incorporates:
   *  Gain: '<S7>/deg2rad'
   */
  rtb_DataTypeConversion2 = (real32_T)tan(0.0174532924F *
    rtDW.RateTransition1_m[0]);

  /* Trigonometry: '<S7>/Trigonometric Function1' incorporates:
   *  Gain: '<S7>/deg2rad '
   */
  rtb_DataTypeConversion1 = (real32_T)tan(0.0174532924F *
    rtDW.RateTransition1_m[1]);

  /* Sum: '<S7>/Sum' incorporates:
   *  Constant: '<S7>/Constant'
   *  Math: '<S7>/Math Function'
   *  Math: '<S7>/Math Function1'
   */
  rtb_ReciprocalSqrt = ((real_T)(rtb_DataTypeConversion2 *
    rtb_DataTypeConversion2) + rtb_DataTypeConversion1 * rtb_DataTypeConversion1)
    + 1.0;

  /* Sqrt: '<S7>/Reciprocal Sqrt' */
  if (rtb_ReciprocalSqrt > 0.0) {
    if (rtIsInf(rtb_ReciprocalSqrt)) {
      rtb_ReciprocalSqrt = 0.0;
    } else {
      rtb_ReciprocalSqrt = 1.0 / sqrt(rtb_ReciprocalSqrt);
    }
  } else if (rtb_ReciprocalSqrt == 0.0) {
    rtb_ReciprocalSqrt = (rtInf);
  } else {
    rtb_ReciprocalSqrt = (rtNaN);
  }

  /* End of Sqrt: '<S7>/Reciprocal Sqrt' */

  /* DataTypeConversion: '<S7>/Data Type Conversion2' incorporates:
   *  Product: '<S7>/Product1'
   */
  rtb_DataTypeConversion2 = (real32_T)(rtb_DataTypeConversion2 *
    rtb_ReciprocalSqrt);

  /* DataTypeConversion: '<S7>/Data Type Conversion1' incorporates:
   *  Product: '<S7>/Product'
   */
  rtb_DataTypeConversion1 = (real32_T)(rtb_DataTypeConversion1 *
    rtb_ReciprocalSqrt);

  /* If: '<S4>/If' incorporates:
   *  Inport: '<S6>/ss_flag'
   */
  rtPrevAction = rtDW.If_ActiveSubsystem;
  rtAction = (int8_T)!(rtDW.RateTransition1_m[2] > 0.0F);
  rtDW.If_ActiveSubsystem = rtAction;
  switch (rtAction) {
   case 0:
    if (rtAction != rtPrevAction) {
      /* InitializeConditions for IfAction SubSystem: '<S4>/If Action Subsystem' incorporates:
       *  InitializeConditions for ActionPort: '<S5>/Action Port'
       */
      /* InitializeConditions for If: '<S4>/If' incorporates:
       *  InitializeConditions for UnitDelay: '<S5>/Unit Delay'
       */
      rtDW.UnitDelay_DSTATE = 2.0F;

      /* End of InitializeConditions for SubSystem: '<S4>/If Action Subsystem' */
    }

    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem' incorporates:
     *  ActionPort: '<S5>/Action Port'
     */
    /* Switch: '<S5>/Switch' incorporates:
     *  Constant: '<S8>/Lower Limit'
     *  Constant: '<S8>/Upper Limit'
     *  Constant: '<S9>/Lower Limit'
     *  Constant: '<S9>/Upper Limit'
     *  Logic: '<S5>/Logical Operator'
     *  Logic: '<S8>/AND'
     *  Logic: '<S9>/AND'
     *  RelationalOperator: '<S8>/Lower Test'
     *  RelationalOperator: '<S8>/Upper Test'
     *  RelationalOperator: '<S9>/Lower Test'
     *  RelationalOperator: '<S9>/Upper Test'
     *  UnitDelay: '<S5>/Unit Delay'
     */
    if ((-20.0F <= rtDW.RateTransition1_m[0]) && (rtDW.RateTransition1_m[0] <=
         20.0F) && ((-20.0F <= rtDW.RateTransition1_m[1]) &&
                    (rtDW.RateTransition1_m[1] <= 20.0F))) {
      rtb_Switch = rtDW.RateTransition1_m[2];
    } else {
      rtb_Switch = rtDW.UnitDelay_DSTATE;
    }

    /* End of Switch: '<S5>/Switch' */

    /* Update for UnitDelay: '<S5>/Unit Delay' */
    rtDW.UnitDelay_DSTATE = rtb_Switch;

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S4>/Switch Case Action Subsystem' incorporates:
     *  ActionPort: '<S6>/Action Port'
     */
    rtb_Switch = rtDW.RateTransition1_m[2];

    /* End of Outputs for SubSystem: '<S4>/Switch Case Action Subsystem' */
    break;
  }

  /* End of If: '<S4>/If' */

  /* Product: '<S4>/Product' incorporates:
   *  Constant: '<S4>/process_matrix'
   *  DataTypeConversion: '<S7>/Data Type Conversion'
   *  Product: '<S4>/Product1'
   *  Sum: '<S4>/Sum'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = rtConstP.pooled4[i + 6] * (real32_T)rtb_ReciprocalSqrt +
      (rtConstP.pooled4[i + 3] * rtb_DataTypeConversion1 + rtConstP.pooled4[i] *
       rtb_DataTypeConversion2);
  }

  /* End of Product: '<S4>/Product' */
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sun_vec_body' incorporates:
     *  Constant: '<S4>/sensor2body'
     *  Product: '<S4>/Product1'
     */
    rtY.sun_vec_body[i] = rtConstP.sensor2body_Value[i + 6] * tmp[2] +
      (rtConstP.sensor2body_Value[i + 3] * tmp[1] + rtConstP.sensor2body_Value[i]
       * tmp[0]);
  }

  /* Outport: '<Root>/sun_vec_body' */
  rtY.sun_vec_body[3] = rtb_Switch;
}

/* Model initialize function */
void MSP_SP_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* Start for If: '<S4>/If' */
  rtDW.If_ActiveSubsystem = -1;

  /* SystemInitialize for IfAction SubSystem: '<S4>/If Action Subsystem' */
  /* InitializeConditions for UnitDelay: '<S5>/Unit Delay' */
  rtDW.UnitDelay_DSTATE = 2.0F;

  /* End of SystemInitialize for SubSystem: '<S4>/If Action Subsystem' */
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
