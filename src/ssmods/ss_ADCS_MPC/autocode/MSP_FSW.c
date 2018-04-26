/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW.c
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

#include "MSP_FSW.h"
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
extern real_T rt_powd_snf(real_T u0, real_T u1);

/* Forward declaration for local functions */
static void mrdivide(real_T A[36], const real_T B_0[36]);
static real_T norm(const real_T x[3]);
static real32_T norm_i(const real32_T x[3]);
static void myDCM2quat(const real32_T DCM[9], real_T q[4]);
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

/* Function for MATLAB Function: '<S26>/kalman_gain' */
static void mrdivide(real_T A[36], const real_T B_0[36])
{
  int16_T jp;
  real_T temp;
  real_T b_A[36];
  int8_T ipiv[6];
  int16_T j;
  int16_T ix;
  real_T s;
  int16_T c_ix;
  int16_T d;
  int16_T ijA;
  int16_T b_jAcol;
  int16_T b_kBcol;
  memcpy(&b_A[0], &B_0[0], 36U * sizeof(real_T));
  for (j = 0; j < 6; j++) {
    ipiv[j] = (int8_T)(1 + j);
  }

  for (j = 0; j < 5; j++) {
    jp = j * 7;
    b_jAcol = 0;
    ix = jp;
    temp = fabs(b_A[jp]);
    for (b_kBcol = 2; b_kBcol <= 6 - j; b_kBcol++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        b_jAcol = b_kBcol - 1;
        temp = s;
      }
    }

    if (b_A[jp + b_jAcol] != 0.0) {
      if (b_jAcol != 0) {
        ipiv[j] = (int8_T)((j + b_jAcol) + 1);
        ix = j;
        b_jAcol += j;
        for (b_kBcol = 0; b_kBcol < 6; b_kBcol++) {
          temp = b_A[ix];
          b_A[ix] = b_A[b_jAcol];
          b_A[b_jAcol] = temp;
          ix += 6;
          b_jAcol += 6;
        }
      }

      b_jAcol = (jp - j) + 6;
      for (ix = jp + 1; ix + 1 <= b_jAcol; ix++) {
        b_A[ix] /= b_A[jp];
      }
    }

    b_jAcol = jp;
    ix = jp + 6;
    for (b_kBcol = 1; b_kBcol <= 5 - j; b_kBcol++) {
      temp = b_A[ix];
      if (b_A[ix] != 0.0) {
        c_ix = jp + 1;
        d = (b_jAcol - j) + 12;
        for (ijA = 7 + b_jAcol; ijA + 1 <= d; ijA++) {
          b_A[ijA] += b_A[c_ix] * -temp;
          c_ix++;
        }
      }

      ix += 6;
      b_jAcol += 6;
    }
  }

  for (j = 0; j < 6; j++) {
    jp = 6 * j;
    b_jAcol = 6 * j;
    for (ix = 1; ix <= j; ix++) {
      b_kBcol = (ix - 1) * 6;
      if (b_A[(ix + b_jAcol) - 1] != 0.0) {
        for (c_ix = 0; c_ix < 6; c_ix++) {
          A[c_ix + jp] -= b_A[(ix + b_jAcol) - 1] * A[c_ix + b_kBcol];
        }
      }
    }

    temp = 1.0 / b_A[j + b_jAcol];
    for (b_jAcol = 0; b_jAcol < 6; b_jAcol++) {
      A[b_jAcol + jp] *= temp;
    }
  }

  for (j = 5; j >= 0; j += -1) {
    jp = 6 * j;
    b_jAcol = 6 * j - 1;
    for (ix = j + 2; ix < 7; ix++) {
      b_kBcol = (ix - 1) * 6;
      if (b_A[ix + b_jAcol] != 0.0) {
        for (c_ix = 0; c_ix < 6; c_ix++) {
          A[c_ix + jp] -= b_A[ix + b_jAcol] * A[c_ix + b_kBcol];
        }
      }
    }
  }

  for (j = 4; j >= 0; j += -1) {
    if (j + 1 != ipiv[j]) {
      jp = ipiv[j] - 1;
      for (b_jAcol = 0; b_jAcol < 6; b_jAcol++) {
        temp = A[6 * j + b_jAcol];
        A[b_jAcol + 6 * j] = A[6 * jp + b_jAcol];
        A[b_jAcol + 6 * jp] = temp;
      }
    }
  }
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = (rtNaN);
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Function for MATLAB Function: '<S8>/TARG_GEN' */
static real_T norm(const real_T x[3])
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;
  scale = 2.2250738585072014E-308;
  absxk = fabs(x[0]);
  if (absxk > 2.2250738585072014E-308) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrt(y);
}

/* Function for MATLAB Function: '<S8>/TARG_GEN' */
static real32_T norm_i(const real32_T x[3])
{
  real32_T y;
  real32_T scale;
  real32_T absxk;
  real32_T t;
  scale = 1.17549435E-38F;
  absxk = (real32_T)fabs(x[0]);
  if (absxk > 1.17549435E-38F) {
    y = 1.0F;
    scale = absxk;
  } else {
    t = absxk / 1.17549435E-38F;
    y = t * t;
  }

  absxk = (real32_T)fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0F;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = (real32_T)fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0F;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * (real32_T)sqrt(y);
}

/*
 * Function for MATLAB Function: '<S8>/TARG_GEN'
 * function q = myDCM2quat( DCM )
 */
static void myDCM2quat(const real32_T DCM[9], real_T q[4])
{
  real_T eta;
  real32_T x;
  real32_T y;

  /* '<S40>:1:77' q   = zeros(4,1); */
  /* '<S40>:1:79' tr  = trace(DCM); */
  /* '<S40>:1:81' q(1)    = 0.5*sqrt(tr + 1); */
  x = (real32_T)sqrt(((DCM[0] + DCM[4]) + DCM[8]) + 1.0F);
  y = 0.5F * x;
  q[0] = 0.5F * x;

  /* '<S40>:1:83' if( q(1) ~= 0 ) */
  if (y != 0.0F) {
    /* '<S40>:1:84' eta     = 1/(4*q(1)); */
    eta = 1.0 / (4.0 * y);

    /* '<S40>:1:85' q(2)    = eta*(DCM(2,3) - DCM(3,2)); */
    q[1] = (DCM[7] - DCM[5]) * (real32_T)eta;

    /* '<S40>:1:86' q(3)    = eta*(DCM(3,1) - DCM(1,3)); */
    q[2] = (DCM[2] - DCM[6]) * (real32_T)eta;

    /* '<S40>:1:87' q(4)    = eta*(DCM(1,2) - DCM(2,1)); */
    q[3] = (DCM[3] - DCM[1]) * (real32_T)eta;
  } else {
    /* '<S40>:1:88' else */
    /* '<S40>:1:89' q(2)  = sqrt(0.5*(DCM(1,1) + 1)); */
    q[1] = (real32_T)sqrt((DCM[0] + 1.0F) * 0.5F);

    /* '<S40>:1:90' q(3)  = sign(DCM(1,2))*sqrt(0.5*(DCM(2,2) + 1)); */
    if (DCM[3] < 0.0F) {
      x = -1.0F;
    } else if (DCM[3] > 0.0F) {
      x = 1.0F;
    } else if (DCM[3] == 0.0F) {
      x = 0.0F;
    } else {
      x = DCM[3];
    }

    q[2] = (real32_T)sqrt((DCM[4] + 1.0F) * 0.5F) * x;

    /* '<S40>:1:91' q(4)  = sign(DCM(1,3))*sqrt(0.5*(DCM(3,3) + 1)); */
    if (DCM[6] < 0.0F) {
      x = -1.0F;
    } else if (DCM[6] > 0.0F) {
      x = 1.0F;
    } else if (DCM[6] == 0.0F) {
      x = 0.0F;
    } else {
      x = DCM[6];
    }

    q[3] = (real32_T)sqrt((DCM[8] + 1.0F) * 0.5F) * x;
  }
}

/* Model step function for TID0 */
void MSP_FSW_step0(void)               /* Sample time: [0.004s, 0.0s] */
{
  real_T rtb_PulseGenerator;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.004s, 0.0s] to Sample time: [0.1s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_1)++;
  if ((rtM->Timing.RateInteraction.TID0_1) > 24) {
    rtM->Timing.RateInteraction.TID0_1 = 0;
  }

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.004s, 0.0s] to Sample time: [0.2s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_2)++;
  if ((rtM->Timing.RateInteraction.TID0_2) > 49) {
    rtM->Timing.RateInteraction.TID0_2 = 0;
  }

  /* RateTransition: '<S3>/Rate Transition2' incorporates:
   *  RateTransition: '<S1>/Rate Transition7'
   *  RateTransition: '<S8>/Rate Transition6'
   */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    /* Outport: '<Root>/sc_quat' */
    rtY.sc_quat[0] = rtDW.RateTransition2_Buffer0[0];
    rtY.sc_quat[1] = rtDW.RateTransition2_Buffer0[1];
    rtY.sc_quat[2] = rtDW.RateTransition2_Buffer0[2];
    rtY.sc_quat[3] = rtDW.RateTransition2_Buffer0[3];

    /* Outport: '<Root>/body_rates' */
    rtY.body_rates[0] = rtDW.RateTransition7_2_Buffer0[0];
    rtY.body_rates[1] = rtDW.RateTransition7_2_Buffer0[1];
    rtY.body_rates[2] = rtDW.RateTransition7_2_Buffer0[2];

    /* Outport: '<Root>/sc_mode' */
    rtY.sc_mode = rtDW.RateTransition7_3_Buffer0;
    rtDW.RateTransition6[0] = rtDW.RateTransition6_Buffer0[0];
    rtDW.RateTransition6[1] = rtDW.RateTransition6_Buffer0[1];
    rtDW.RateTransition6[2] = rtDW.RateTransition6_Buffer0[2];
    rtDW.RateTransition6[3] = rtDW.RateTransition6_Buffer0[3];
  }

  /* End of RateTransition: '<S3>/Rate Transition2' */

  /* Product: '<S14>/Product3' incorporates:
   *  UnaryMinus: '<S12>/Unary Minus2'
   */
  rtb_PulseGenerator = -rtY.sc_quat[3] * rtDW.RateTransition6[3];

  /* Sum: '<S14>/Sum' incorporates:
   *  Product: '<S14>/Product'
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product2'
   *  UnaryMinus: '<S12>/Unary Minus'
   *  UnaryMinus: '<S12>/Unary Minus1'
   */
  rtb_PulseGenerator = ((rtY.sc_quat[0] * rtDW.RateTransition6[0] -
    -rtY.sc_quat[1] * rtDW.RateTransition6[1]) - -rtY.sc_quat[2] *
                        rtDW.RateTransition6[2]) - rtb_PulseGenerator;

  /* Trigonometry: '<S10>/Trigonometric Function' */
  if (rtb_PulseGenerator > 1.0) {
    rtb_PulseGenerator = 1.0;
  } else {
    if (rtb_PulseGenerator < -1.0) {
      rtb_PulseGenerator = -1.0;
    }
  }

  /* Outport: '<Root>/point_true' incorporates:
   *  Constant: '<S11>/Constant'
   *  Gain: '<S10>/Gain'
   *  Gain: '<S10>/rad2deg'
   *  RelationalOperator: '<S11>/Compare'
   *  Trigonometry: '<S10>/Trigonometric Function'
   */
  rtY.point_true = (2.0 * acos(rtb_PulseGenerator) * 57.295779513082323 <= 12.0);

  /* RateTransition: '<S1>/Rate Transition7' incorporates:
   *  Constant: '<S3>/Constant1'
   *  RateTransition: '<S1>/Rate Transition4'
   *  RateTransition: '<S3>/ 10'
   */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    /* Outport: '<Root>/sc_above_gsb' */
    rtY.sc_above_gsb = rtDW.RateTransition7_5_Buffer0;

    /* Outport: '<Root>/sc_modeb' */
    rtY.sc_modeb = rtDW.RateTransition4_1_Buffer0;

    /* Outport: '<Root>/cmd_MT_fsw_dv' */
    rtY.cmd_MT_fsw_dv[0] = rtDW.RateTransition4_2_Buffer0[0];
    rtY.cmd_MT_fsw_dv[1] = rtDW.RateTransition4_2_Buffer0[1];
    rtY.cmd_MT_fsw_dv[2] = rtDW.RateTransition4_2_Buffer0[2];
    memcpy(&rtDW.u0[0], &rtConstP.Constant1_Value[0], 36U * sizeof(real_T));

    /* RateTransition: '<S1>/Rate Transition2' incorporates:
     *  Constant: '<S3>/Constant1'
     *  Inport: '<Root>/MT_valid'
     */
    rtDW.MT_valid[0] = rtU.MT_valid[0];
    rtDW.MT_valid[1] = rtU.MT_valid[1];
    rtDW.MT_valid[2] = rtU.MT_valid[2];
  }

  /* End of RateTransition: '<S1>/Rate Transition7' */

  /* DiscretePulseGenerator: '<S6>/Pulse Generator' */
  rtb_PulseGenerator = ((rtDW.clockTickCounter < 3L) && (rtDW.clockTickCounter >=
    0L));
  if (rtDW.clockTickCounter >= 499L) {
    rtDW.clockTickCounter = 0L;
  } else {
    rtDW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<S6>/Pulse Generator' */

  /* RateTransition: '<S34>/Rate Transition4' */
  if (rtM->Timing.RateInteraction.TID0_2 == 1) {
    rtDW.RateTransition4 = rtb_PulseGenerator;

    /* RateTransition: '<S34>/Rate Transition1' */
    rtDW.RateTransition1[0] = 0.0;
    rtDW.RateTransition1[1] = 0.0;
    rtDW.RateTransition1[2] = 0.0;
  }

  /* End of RateTransition: '<S34>/Rate Transition4' */
}

/* Model step function for TID1 */
void MSP_FSW_step1(void)               /* Sample time: [0.1s, 0.0s] */
{
  real_T w_n;
  int8_T Phi_22[9];
  int8_T I[9];
  int16_T c_k;
  static const int8_T a[9] = { -1, 0, 0, 0, -1, 0, 0, 0, -1 };

  static const int8_T b_a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real_T Psi[3];
  static const int8_T b[16] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };

  static const int8_T a_0[16] = { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0
  };

  static const int8_T b_b[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real32_T C_cmd[9];
  real32_T S1[3];
  real32_T Yti[3];
  real32_T y;
  static const real32_T b_0[9] = { 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,
    0.0F, 1.0F };

  static const int8_T d[9] = { 0, -1, 0, 0, 0, -1, 1, 0, 0 };

  real_T rotation_matrix[9];
  real_T scale;
  real_T absxk;
  real_T t;
  real_T KHI[36];
  int8_T I_0[36];
  real_T rtb_DataTypeConversion1[3];
  real_T rtb_q_min_k1[4];
  real_T rtb_Product1_h;
  real_T rtb_Product2_d;
  real_T rtb_Product3_f;
  real_T rtb_body_est[6];
  real_T rtb_MatrixMultiply[6];
  int8_T rtb_sc_mode;
  real_T rtb_Merge2[36];
  real_T rtb_K[36];
  int16_T i;
  real32_T S1_0[3];
  real32_T S1_1[3];
  real_T rtb_Merge2_0[36];
  real_T rtb_Merge2_1[36];
  real_T rtb_DataTypeConversion1_0[6];
  real_T rtb_DataTypeConversion1_1[6];
  real_T KHI_0[36];
  real_T rtb_K_0[36];
  real_T rotation_matrix_0[9];
  real_T rotation_matrix_1[9];
  real_T tmp[9];
  real_T x[16];
  real_T a_1[16];
  real_T a_2[16];
  real32_T S1_2[3];
  real32_T S1_3[9];
  int16_T i_0;
  real_T r_idx_1;
  boolean_T guard1 = false;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.1s, 0.0s] to Sample time: [0.2s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID1_2)++;
  if ((rtM->Timing.RateInteraction.TID1_2) > 1) {
    rtM->Timing.RateInteraction.TID1_2 = 0;
  }

  /* DataTypeConversion: '<S1>/Data Type Conversion1' incorporates:
   *  Inport: '<Root>/mag_vec_body_T'
   */
  rtb_DataTypeConversion1[0] = rtU.mag_vec_body_T[0];
  rtb_DataTypeConversion1[1] = rtU.mag_vec_body_T[1];
  rtb_DataTypeConversion1[2] = rtU.mag_vec_body_T[2];

  /* If: '<S20>/If' incorporates:
   *  Inport: '<Root>/mag_vec_body_T'
   *  Inport: '<Root>/sc_in_sun'
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   *  Inport: '<S27>/cov_min'
   *  Logic: '<S3>/Logical Operator'
   *  Logic: '<S3>/Logical Operator1'
   *  Logic: '<S3>/Logical Operator2'
   *  Logic: '<S4>/Logical Operator'
   *  UnitDelay: '<S3>/Unit Delay2'
   */
  if ((rtU.mag_vec_body_T[3] != 0.0F) && (rtDW.MT_valid[0] && rtDW.MT_valid[1] &&
       rtDW.MT_valid[2]) && ((rtU.sun_vec_body_sunsensor[3] != 0.0F) &&
       rtU.sc_in_sun)) {
    /* Outputs for IfAction SubSystem: '<S20>/If Action Subsystem' incorporates:
     *  ActionPort: '<S26>/Action Port'
     */
    /* MATLAB Function: '<S26>/convert_inertial_body' incorporates:
     *  SignalConversion: '<S28>/TmpSignal ConversionAt SFunction Inport2'
     *  UnitDelay: '<S3>/Unit Delay'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body': '<S28>:1' */
    /* '<S28>:1:3' rotation_matrix = att(q_min); */
    /* '<S28>:1:9' dcm = [q(1)^2 + q(2)^2 - q(3)^2 - q(4)^2,   2*(q(2)*q(3)+q(1)*q(4)),    2*(q(2)*q(4) - q(1)*q(3)); */
    /* '<S28>:1:10'         2*(q(2)*q(3) - q(1)*q(4)),          q(1)^2-q(2)^2+q(3)^2-q(4)^2,    2*(q(3)*q(4) + q(1)*q(2)); */
    /* '<S28>:1:11'         2*(q(2)*q(4)+q(1)*q(3)),            2*(q(3)*q(4)-q(1)*q(2)),        q(1)^2-q(2)^2-q(3)^2+q(4)^2]; */
    rotation_matrix[0] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] +
      rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) -
                          rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) -
      rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];
    rotation_matrix[3] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[2] +
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[3]) *
      2.0;
    rotation_matrix[6] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[3] -
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[2]) *
      2.0;
    rotation_matrix[1] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[2] -
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[3]) *
      2.0;
    rotation_matrix[4] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] -
      rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) +
                          rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) -
      rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];
    rotation_matrix[7] = (rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[3] +
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[1]) *
      2.0;
    rotation_matrix[2] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[3] +
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[2]) *
      2.0;
    rotation_matrix[5] = (rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[3] -
                          rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[1]) *
      2.0;
    rotation_matrix[8] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] -
      rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) -
                          rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) +
      rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];

    /* '<S28>:1:4' body_est = blkdiag(rotation_matrix,rotation_matrix)*meas_eci; */
    memset(&KHI[0], 0, 36U * sizeof(real_T));
    for (i = 0; i < 3; i++) {
      KHI[6 * i] = rotation_matrix[3 * i];
      KHI[3 + 6 * (3 + i)] = rotation_matrix[3 * i];
      KHI[1 + 6 * i] = rotation_matrix[3 * i + 1];
      KHI[4 + 6 * (3 + i)] = rotation_matrix[3 * i + 1];
      KHI[2 + 6 * i] = rotation_matrix[3 * i + 2];
      KHI[5 + 6 * (3 + i)] = rotation_matrix[3 * i + 2];

      /* SignalConversion: '<S28>/TmpSignal ConversionAt SFunction Inport2' incorporates:
       *  Inport: '<Root>/mag_eci_unit'
       *  Inport: '<Root>/sc2sun_unit'
       */
      rtb_DataTypeConversion1_1[i] = rtU.mag_eci_unit[i];
      rtb_DataTypeConversion1_1[i + 3] = rtU.sc2sun_unit[i];
    }

    for (i = 0; i < 6; i++) {
      rtb_body_est[i] = 0.0;
      for (c_k = 0; c_k < 6; c_k++) {
        rtb_body_est[i] += KHI[6 * c_k + i] * rtb_DataTypeConversion1_1[c_k];
      }
    }

    /* End of MATLAB Function: '<S26>/convert_inertial_body' */

    /* MATLAB Function: '<S26>/observation_matrix' */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/observation_matrix': '<S31>:1' */
    /* '<S31>:1:3' H = [  skew(body_est(1:3))     zeros(3); */
    /* '<S31>:1:4'        skew(body_est(4:6))     zeros(3)]; */
    /* '<S31>:1:9' S = [0  -v(3)   v(2); */
    /* '<S31>:1:10'     v(3)    0   -v(1); */
    /* '<S31>:1:11'     -v(2)   v(1)    0]; */
    /* '<S31>:1:9' S = [0  -v(3)   v(2); */
    /* '<S31>:1:10'     v(3)    0   -v(1); */
    /* '<S31>:1:11'     -v(2)   v(1)    0]; */
    rtb_Merge2[0] = 0.0;
    rtb_Merge2[6] = -rtb_body_est[2];
    rtb_Merge2[12] = rtb_body_est[1];
    rtb_Merge2[1] = rtb_body_est[2];
    rtb_Merge2[7] = 0.0;
    rtb_Merge2[13] = -rtb_body_est[0];
    rtb_Merge2[2] = -rtb_body_est[1];
    rtb_Merge2[8] = rtb_body_est[0];
    rtb_Merge2[14] = 0.0;
    rtb_Merge2[3] = 0.0;
    rtb_Merge2[9] = -rtb_body_est[5];
    rtb_Merge2[15] = rtb_body_est[4];
    rtb_Merge2[4] = rtb_body_est[5];
    rtb_Merge2[10] = 0.0;
    rtb_Merge2[16] = -rtb_body_est[3];
    rtb_Merge2[5] = -rtb_body_est[4];
    rtb_Merge2[11] = rtb_body_est[3];
    rtb_Merge2[17] = 0.0;
    for (i = 0; i < 3; i++) {
      rtb_Merge2[6 * (i + 3)] = 0.0;
      rtb_Merge2[3 + 6 * (i + 3)] = 0.0;
      rtb_Merge2[1 + 6 * (i + 3)] = 0.0;
      rtb_Merge2[4 + 6 * (i + 3)] = 0.0;
      rtb_Merge2[2 + 6 * (i + 3)] = 0.0;
      rtb_Merge2[5 + 6 * (i + 3)] = 0.0;
    }

    /* End of MATLAB Function: '<S26>/observation_matrix' */

    /* MATLAB Function: '<S26>/kalman_gain' incorporates:
     *  Inport: '<Root>/gyro_omega_body_radps'
     *  UnitDelay: '<S3>/Unit Delay2'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/kalman_gain': '<S30>:1' */
    /* '<S30>:1:3' K = cov_min*H'/(H*cov_min*H' + meas_cov); */
    for (i = 0; i < 6; i++) {
      for (c_k = 0; c_k < 6; c_k++) {
        rtb_K[i + 6 * c_k] = 0.0;
        rtb_Merge2_1[i + 6 * c_k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          rtb_K[i + 6 * c_k] += rtDW.UnitDelay2_DSTATE[6 * i_0 + i] *
            rtb_Merge2[6 * i_0 + c_k];
          rtb_Merge2_1[i + 6 * c_k] += rtb_Merge2[6 * i_0 + i] *
            rtDW.UnitDelay2_DSTATE[6 * c_k + i_0];
        }
      }

      for (c_k = 0; c_k < 6; c_k++) {
        scale = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          scale += rtb_Merge2_1[6 * i_0 + i] * rtb_Merge2[6 * i_0 + c_k];
        }

        rtb_Merge2_0[i + 6 * c_k] = rtDW.u0[6 * c_k + i] + scale;
      }
    }

    mrdivide(rtb_K, rtb_Merge2_0);

    /*  zero off part of gain if gyro reading is invalid */
    /* '<S30>:1:7' if valid_gyro == 0 */
    if (rtU.gyro_omega_body_radps[3] == 0.0F) {
      /* '<S30>:1:8' K(4:6,:) = zeros(3,6); */
      for (i = 0; i < 6; i++) {
        rtb_K[3 + 6 * i] = 0.0;
        rtb_K[4 + 6 * i] = 0.0;
        rtb_K[5 + 6 * i] = 0.0;
      }
    }

    /* End of MATLAB Function: '<S26>/kalman_gain' */

    /* Sum: '<S26>/Sum' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion2'
     *  Product: '<S26>/Matrix Multiply'
     */
    rtb_DataTypeConversion1_0[0] = rtb_DataTypeConversion1[0];
    rtb_DataTypeConversion1_0[3] = rtU.sun_vec_body_sunsensor[0];
    rtb_DataTypeConversion1_0[1] = rtb_DataTypeConversion1[1];
    rtb_DataTypeConversion1_0[4] = rtU.sun_vec_body_sunsensor[1];
    rtb_DataTypeConversion1_0[2] = rtb_DataTypeConversion1[2];
    rtb_DataTypeConversion1_0[5] = rtU.sun_vec_body_sunsensor[2];
    for (i = 0; i < 6; i++) {
      rtb_DataTypeConversion1_1[i] = rtb_DataTypeConversion1_0[i] -
        rtb_body_est[i];
    }

    /* End of Sum: '<S26>/Sum' */

    /* Product: '<S26>/Matrix Multiply' */
    for (i = 0; i < 6; i++) {
      rtb_MatrixMultiply[i] = 0.0;
      for (c_k = 0; c_k < 6; c_k++) {
        rtb_MatrixMultiply[i] += rtb_K[6 * c_k + i] *
          rtb_DataTypeConversion1_1[c_k];
      }
    }

    /* MATLAB Function: '<S26>/update_state ' incorporates:
     *  UnitDelay: '<S3>/Unit Delay'
     */
    /*  select out bias */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/update_state ': '<S32>:1' */
    /* '<S32>:1:5' bias_plu = bias_min + del_x_plu(4:6); */
    /* '<S32>:1:7' q_plu = quat_update(del_x_plu(1:3),q_min); */
    /*  this has a sqrt error if del_apk is too large, i.e. small angle */
    /*  approximation doesn't hold */
    /* q_hat_pk = quat_multiply([sqrt(1-norm(del_apk/2)); del_apk/2], q_hat_mk); */
    /*  the following two equations are equivalent; the first is probably a bit */
    /*  more computationally cheaper */
    /* '<S32>:1:22' q_hat_pk = quat_multiply([1; del_apk/2], q_hat_mk); */
    /* '<S32>:1:41' q2 = quatmultiply(q1',dq')'; */
    r_idx_1 = rtb_MatrixMultiply[0] / 2.0;
    rtb_Product3_f = rtb_MatrixMultiply[1] / 2.0;
    scale = rtb_MatrixMultiply[2] / 2.0;
    w_n = ((rtDW.UnitDelay_DSTATE[0] - rtDW.UnitDelay_DSTATE[1] * r_idx_1) -
           rtDW.UnitDelay_DSTATE[2] * rtb_Product3_f) - rtDW.UnitDelay_DSTATE[3]
      * scale;
    rtb_Product1_h = (rtDW.UnitDelay_DSTATE[0] * r_idx_1 +
                      rtDW.UnitDelay_DSTATE[1]) + (rtDW.UnitDelay_DSTATE[2] *
      scale - rtDW.UnitDelay_DSTATE[3] * rtb_Product3_f);
    rtb_Product2_d = (rtDW.UnitDelay_DSTATE[0] * rtb_Product3_f +
                      rtDW.UnitDelay_DSTATE[2]) + (rtDW.UnitDelay_DSTATE[3] *
      r_idx_1 - rtDW.UnitDelay_DSTATE[1] * scale);
    rtb_Product3_f = (rtDW.UnitDelay_DSTATE[0] * scale + rtDW.UnitDelay_DSTATE[3])
      + (rtDW.UnitDelay_DSTATE[1] * rtb_Product3_f - rtDW.UnitDelay_DSTATE[2] *
         r_idx_1);

    /* q_hat_pk = q_hat_mk + 0.5*xi_matrix(q_hat_mk)*del_apk; */
    /* '<S32>:1:8' q_plu = q_plu/norm(q_plu); */
    scale = 2.2250738585072014E-308;
    absxk = fabs(w_n);
    if (absxk > 2.2250738585072014E-308) {
      r_idx_1 = 1.0;
      scale = absxk;
    } else {
      t = absxk / 2.2250738585072014E-308;
      r_idx_1 = t * t;
    }

    absxk = fabs(rtb_Product1_h);
    if (absxk > scale) {
      t = scale / absxk;
      r_idx_1 = r_idx_1 * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      r_idx_1 += t * t;
    }

    absxk = fabs(rtb_Product2_d);
    if (absxk > scale) {
      t = scale / absxk;
      r_idx_1 = r_idx_1 * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      r_idx_1 += t * t;
    }

    absxk = fabs(rtb_Product3_f);
    if (absxk > scale) {
      t = scale / absxk;
      r_idx_1 = r_idx_1 * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      r_idx_1 += t * t;
    }

    r_idx_1 = scale * sqrt(r_idx_1);

    /* SignalConversion: '<S26>/OutportBufferForbias_plus' incorporates:
     *  MATLAB Function: '<S26>/update_state '
     *  UnitDelay: '<S3>/Unit Delay1'
     */
    rtDW.UnitDelay1_DSTATE[0] += rtb_MatrixMultiply[3];
    rtDW.UnitDelay1_DSTATE[1] += rtb_MatrixMultiply[4];
    rtDW.UnitDelay1_DSTATE[2] += rtb_MatrixMultiply[5];

    /* MATLAB Function: '<S26>/covariance_update' incorporates:
     *  UnitDelay: '<S3>/Unit Delay2'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/covariance_update': '<S29>:1' */
    /* '<S29>:1:4' KHI = (K*H - eye(6)); */
    for (i = 0; i < 36; i++) {
      I_0[i] = 0;
    }

    for (i = 0; i < 6; i++) {
      I_0[i + 6 * i] = 1;
    }

    /* '<S29>:1:5' cov_plus = KHI*cov_min*KHI' + K*meas_cov*K'; */
    for (i = 0; i < 6; i++) {
      for (c_k = 0; c_k < 6; c_k++) {
        scale = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          scale += rtb_K[6 * i_0 + i] * rtb_Merge2[6 * c_k + i_0];
        }

        KHI[i + 6 * c_k] = scale - (real_T)I_0[6 * c_k + i];
      }

      for (c_k = 0; c_k < 6; c_k++) {
        rtb_Merge2_0[i + 6 * c_k] = 0.0;
        rtb_Merge2_1[i + 6 * c_k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          rtb_Merge2_0[i + 6 * c_k] += KHI[6 * i_0 + i] *
            rtDW.UnitDelay2_DSTATE[6 * c_k + i_0];
          rtb_Merge2_1[i + 6 * c_k] += rtb_K[6 * i_0 + i] * rtDW.u0[6 * c_k +
            i_0];
        }
      }
    }

    for (i = 0; i < 6; i++) {
      for (c_k = 0; c_k < 6; c_k++) {
        KHI_0[i + 6 * c_k] = 0.0;
        rtb_K_0[i + 6 * c_k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          KHI_0[i + 6 * c_k] += rtb_Merge2_0[6 * i_0 + i] * KHI[6 * i_0 + c_k];
          rtb_K_0[i + 6 * c_k] += rtb_Merge2_1[6 * i_0 + i] * rtb_K[6 * i_0 +
            c_k];
        }
      }
    }

    /* SignalConversion: '<S26>/OutportBufferForcov_plus' incorporates:
     *  MATLAB Function: '<S26>/covariance_update'
     */
    for (i = 0; i < 6; i++) {
      for (c_k = 0; c_k < 6; c_k++) {
        rtb_Merge2[c_k + 6 * i] = KHI_0[6 * i + c_k] + rtb_K_0[6 * i + c_k];
      }
    }

    /* End of SignalConversion: '<S26>/OutportBufferForcov_plus' */

    /* SignalConversion: '<S26>/OutportBufferForq_plus' incorporates:
     *  MATLAB Function: '<S26>/update_state '
     */
    rtDW.UnitDelay_DSTATE[0] = w_n / r_idx_1;
    rtDW.UnitDelay_DSTATE[1] = rtb_Product1_h / r_idx_1;
    rtDW.UnitDelay_DSTATE[2] = rtb_Product2_d / r_idx_1;
    rtDW.UnitDelay_DSTATE[3] = rtb_Product3_f / r_idx_1;

    /* End of Outputs for SubSystem: '<S20>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S20>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S27>/Action Port'
     */
    memcpy(&rtb_Merge2[0], &rtDW.UnitDelay2_DSTATE[0], 36U * sizeof(real_T));

    /* End of Outputs for SubSystem: '<S20>/If Action Subsystem1' */
  }

  /* End of If: '<S20>/If' */

  /* Switch: '<S19>/Switch' incorporates:
   *  Inport: '<Root>/gyro_omega_body_radps'
   *  Sum: '<S19>/Sum'
   */
  if (rtU.gyro_omega_body_radps[3] >= 0.0F) {
    rtDW.RateTransition7_2_Buffer0[0] = rtU.gyro_omega_body_radps[0] -
      rtDW.UnitDelay1_DSTATE[0];
    rtDW.RateTransition7_2_Buffer0[1] = rtU.gyro_omega_body_radps[1] -
      rtDW.UnitDelay1_DSTATE[1];
    rtDW.RateTransition7_2_Buffer0[2] = rtU.gyro_omega_body_radps[2] -
      rtDW.UnitDelay1_DSTATE[2];
  } else {
    rtDW.RateTransition7_2_Buffer0[0] = 0.0;
    rtDW.RateTransition7_2_Buffer0[1] = 0.0;
    rtDW.RateTransition7_2_Buffer0[2] = 0.0;
  }

  /* End of Switch: '<S19>/Switch' */

  /* MATLAB Function: '<S19>/state_transition' incorporates:
   *  Constant: '<S19>/Constant'
   */
  /* MATLAB Function 'Estimation_EKF /Propagate Step /state_transition': '<S23>:1' */
  /* '<S23>:1:3' Phi = state_transition_matrix(w_plu,dt); */
  /* '<S23>:1:10' w_n = norm(w); */
  scale = 2.2250738585072014E-308;
  absxk = fabs(rtDW.RateTransition7_2_Buffer0[0]);
  if (absxk > 2.2250738585072014E-308) {
    w_n = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    w_n = t * t;
  }

  absxk = fabs(rtDW.RateTransition7_2_Buffer0[1]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  absxk = fabs(rtDW.RateTransition7_2_Buffer0[2]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  w_n = scale * sqrt(w_n);

  /* '<S23>:1:11' w_skew = skew(w); */
  /* '<S23>:1:31' S = [0  -v(3)   v(2); */
  /* '<S23>:1:32'     v(3)    0   -v(1); */
  /* '<S23>:1:33'     -v(2)   v(1)    0]; */
  rotation_matrix[0] = 0.0;
  rotation_matrix[3] = -rtDW.RateTransition7_2_Buffer0[2];
  rotation_matrix[6] = rtDW.RateTransition7_2_Buffer0[1];
  rotation_matrix[1] = rtDW.RateTransition7_2_Buffer0[2];
  rotation_matrix[4] = 0.0;
  rotation_matrix[7] = -rtDW.RateTransition7_2_Buffer0[0];
  rotation_matrix[2] = -rtDW.RateTransition7_2_Buffer0[1];
  rotation_matrix[5] = rtDW.RateTransition7_2_Buffer0[0];
  rotation_matrix[8] = 0.0;

  /* '<S23>:1:13' if w_n < 1e-14 */
  if (w_n < 1.0E-14) {
    /* '<S23>:1:14' phi = [ eye(3) -eye(3)*dt; */
    /* '<S23>:1:15'             zeros(3,3) eye(3)]; */
    memset(&rotation_matrix[0], 0, 9U * sizeof(real_T));
    for (i = 0; i < 9; i++) {
      I[i] = 0;
    }

    I[0] = 1;
    I[4] = 1;
    I[8] = 1;
    for (i = 0; i < 3; i++) {
      rotation_matrix[i + 3 * i] = 1.0;
      rtb_K[6 * i] = I[3 * i];
      rtb_K[6 * (i + 3)] = (real_T)a[3 * i] * 0.1;
      rtb_K[3 + 6 * i] = 0.0;
      rtb_K[1 + 6 * i] = I[3 * i + 1];
      rtb_K[1 + 6 * (i + 3)] = (real_T)a[3 * i + 1] * 0.1;
      rtb_K[4 + 6 * i] = 0.0;
      rtb_K[2 + 6 * i] = I[3 * i + 2];
      rtb_K[2 + 6 * (i + 3)] = (real_T)a[3 * i + 2] * 0.1;
      rtb_K[5 + 6 * i] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      rtb_K[3 + 6 * (i + 3)] = rotation_matrix[3 * i];
      rtb_K[4 + 6 * (i + 3)] = rotation_matrix[3 * i + 1];
      rtb_K[5 + 6 * (i + 3)] = rotation_matrix[3 * i + 2];
    }
  } else {
    /* '<S23>:1:16' else */
    /* '<S23>:1:17' Phi_11 = eye(3) - w_skew*sin(w_n*dt)/w_n + w_skew^2*(1 - cos(w_n*dt))/w_n^2; */
    for (i = 0; i < 9; i++) {
      I[i] = 0;
      Phi_22[i] = 0;
    }

    I[0] = 1;
    I[4] = 1;
    I[8] = 1;
    r_idx_1 = sin(w_n * 0.1);
    rtb_Product1_h = cos(w_n * 0.1);
    rtb_Product2_d = w_n * w_n;

    /* '<S23>:1:19' Phi_12 = w_skew*(1 - cos(w_n*dt))/w_n^2 - eye(3)*dt - w_skew^2*(w_n*dt - sin(w_n*dt))/w_n^3; */
    rtb_Product3_f = cos(w_n * 0.1);
    scale = w_n * w_n;
    absxk = w_n * 0.1 - sin(w_n * 0.1);
    t = rt_powd_snf(w_n, 3.0);

    /* '<S23>:1:21' Phi_21 = zeros(3,3); */
    /* '<S23>:1:23' Phi_22 = eye(3); */
    /* '<S23>:1:25' phi = [ Phi_11 Phi_12; */
    /* '<S23>:1:26'             Phi_21 Phi_22]; */
    for (c_k = 0; c_k < 3; c_k++) {
      Phi_22[c_k + 3 * c_k] = 1;
      for (i = 0; i < 3; i++) {
        rotation_matrix_0[c_k + 3 * i] = 0.0;
        rotation_matrix_1[c_k + 3 * i] = 0.0;
        rotation_matrix_0[c_k + 3 * i] += rotation_matrix[3 * i] *
          rotation_matrix[c_k];
        rotation_matrix_1[c_k + 3 * i] += rotation_matrix[3 * i] *
          rotation_matrix[c_k];
        rotation_matrix_0[c_k + 3 * i] += rotation_matrix[3 * i + 1] *
          rotation_matrix[c_k + 3];
        rotation_matrix_1[c_k + 3 * i] += rotation_matrix[3 * i + 1] *
          rotation_matrix[c_k + 3];
        rotation_matrix_0[c_k + 3 * i] += rotation_matrix[3 * i + 2] *
          rotation_matrix[c_k + 6];
        rotation_matrix_1[c_k + 3 * i] += rotation_matrix[3 * i + 2] *
          rotation_matrix[c_k + 6];
      }
    }

    for (i = 0; i < 3; i++) {
      rtb_K[6 * i] = ((real_T)I[3 * i] - rotation_matrix[3 * i] * r_idx_1 / w_n)
        + rotation_matrix_0[3 * i] * (1.0 - rtb_Product1_h) / rtb_Product2_d;
      rtb_K[6 * (i + 3)] = (rotation_matrix[3 * i] * (1.0 - rtb_Product3_f) /
                            scale - (real_T)b_a[3 * i] * 0.1) -
        rotation_matrix_1[3 * i] * absxk / t;
      rtb_K[3 + 6 * i] = 0.0;
      rtb_K[3 + 6 * (i + 3)] = Phi_22[3 * i];
      rtb_K[1 + 6 * i] = ((real_T)I[3 * i + 1] - rotation_matrix[3 * i + 1] *
                          r_idx_1 / w_n) + rotation_matrix_0[3 * i + 1] * (1.0 -
        rtb_Product1_h) / rtb_Product2_d;
      rtb_K[1 + 6 * (i + 3)] = (rotation_matrix[3 * i + 1] * (1.0 -
        rtb_Product3_f) / scale - (real_T)b_a[3 * i + 1] * 0.1) -
        rotation_matrix_1[3 * i + 1] * absxk / t;
      rtb_K[4 + 6 * i] = 0.0;
      rtb_K[4 + 6 * (i + 3)] = Phi_22[3 * i + 1];
      rtb_K[2 + 6 * i] = ((real_T)I[3 * i + 2] - rotation_matrix[3 * i + 2] *
                          r_idx_1 / w_n) + rotation_matrix_0[3 * i + 2] * (1.0 -
        rtb_Product1_h) / rtb_Product2_d;
      rtb_K[2 + 6 * (i + 3)] = (rotation_matrix[3 * i + 2] * (1.0 -
        rtb_Product3_f) / scale - (real_T)b_a[3 * i + 2] * 0.1) -
        rotation_matrix_1[3 * i + 2] * absxk / t;
      rtb_K[5 + 6 * i] = 0.0;
      rtb_K[5 + 6 * (i + 3)] = Phi_22[3 * i + 2];
    }
  }

  /* End of MATLAB Function: '<S19>/state_transition' */

  /* MATLAB Function: '<S19>/propagate_quat' incorporates:
   *  Constant: '<S19>/Constant1'
   */
  /* MATLAB Function 'Estimation_EKF /3_sig_bnd ': '<S18>:1' */
  /* '<S18>:1:3' bnd = cov; */
  /* MATLAB Function 'Estimation_EKF /Propagate Step /propagate_quat': '<S22>:1' */
  /* '<S22>:1:4' q_min_k1 = omega_matrix(w_plu,dt)*q_plu; */
  /* '<S22>:1:10' w_n = norm(w); */
  scale = 2.2250738585072014E-308;
  absxk = fabs(rtDW.RateTransition7_2_Buffer0[0]);
  if (absxk > 2.2250738585072014E-308) {
    w_n = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    w_n = t * t;
  }

  absxk = fabs(rtDW.RateTransition7_2_Buffer0[1]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  absxk = fabs(rtDW.RateTransition7_2_Buffer0[2]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  w_n = scale * sqrt(w_n);

  /* '<S22>:1:12' if w_n < 1e-14 */
  if (w_n < 1.0E-14) {
    /* '<S22>:1:13' Psi = 0*w; */
    Psi[0] = 0.0 * rtDW.RateTransition7_2_Buffer0[0];
    Psi[1] = 0.0 * rtDW.RateTransition7_2_Buffer0[1];
    Psi[2] = 0.0 * rtDW.RateTransition7_2_Buffer0[2];
  } else {
    /* '<S22>:1:14' else */
    /* '<S22>:1:15' Psi = sin(1/2*w_n*dt)/w_n*w; */
    r_idx_1 = sin(0.5 * w_n * 0.1) / w_n;
    Psi[0] = r_idx_1 * rtDW.RateTransition7_2_Buffer0[0];
    Psi[1] = r_idx_1 * rtDW.RateTransition7_2_Buffer0[1];
    Psi[2] = r_idx_1 * rtDW.RateTransition7_2_Buffer0[2];
  }

  /* '<S22>:1:18' Psi_skew = skew(Psi); */
  /* '<S22>:1:31' S = [0  -v(3)   v(2); */
  /* '<S22>:1:32'     v(3)    0   -v(1); */
  /* '<S22>:1:33'     -v(2)   v(1)    0]; */
  /* '<S22>:1:20' omega = [cos(1/2*w_n*dt)*eye(3) - Psi_skew      Psi; */
  /* '<S22>:1:21'         -Psi'                                   cos(1/2*w_n*dt)]; */
  r_idx_1 = cos(0.5 * w_n * 0.1);

  /* '<S22>:1:23' T = [zeros(3,1) eye(3); */
  /* '<S22>:1:24'     1       zeros(1,3)]; */
  /* '<S22>:1:26' omega = T'*omega*T; */
  tmp[0] = 0.0;
  tmp[3] = -Psi[2];
  tmp[6] = Psi[1];
  tmp[1] = Psi[2];
  tmp[4] = 0.0;
  tmp[7] = -Psi[0];
  tmp[2] = -Psi[1];
  tmp[5] = Psi[0];
  tmp[8] = 0.0;
  for (i = 0; i < 3; i++) {
    x[i << 2] = (real_T)b_b[3 * i] * r_idx_1 - tmp[3 * i];
    x[1 + (i << 2)] = (real_T)b_b[3 * i + 1] * r_idx_1 - tmp[3 * i + 1];
    x[2 + (i << 2)] = (real_T)b_b[3 * i + 2] * r_idx_1 - tmp[3 * i + 2];
    x[12 + i] = Psi[i];
    x[3 + (i << 2)] = -Psi[i];
  }

  x[15] = cos(0.5 * w_n * 0.1);
  for (i = 0; i < 4; i++) {
    for (c_k = 0; c_k < 4; c_k++) {
      a_1[i + (c_k << 2)] = 0.0;
      a_1[i + (c_k << 2)] += x[c_k << 2] * (real_T)a_0[i];
      a_1[i + (c_k << 2)] += x[(c_k << 2) + 1] * (real_T)a_0[i + 4];
      a_1[i + (c_k << 2)] += x[(c_k << 2) + 2] * (real_T)a_0[i + 8];
      a_1[i + (c_k << 2)] += x[(c_k << 2) + 3] * (real_T)a_0[i + 12];
    }

    rtb_q_min_k1[i] = 0.0;
    for (c_k = 0; c_k < 4; c_k++) {
      a_2[i + (c_k << 2)] = 0.0;
      a_2[i + (c_k << 2)] += (real_T)b[c_k << 2] * a_1[i];
      a_2[i + (c_k << 2)] += (real_T)b[(c_k << 2) + 1] * a_1[i + 4];
      a_2[i + (c_k << 2)] += (real_T)b[(c_k << 2) + 2] * a_1[i + 8];
      a_2[i + (c_k << 2)] += (real_T)b[(c_k << 2) + 3] * a_1[i + 12];
      rtb_q_min_k1[i] += a_2[(c_k << 2) + i] * rtDW.UnitDelay_DSTATE[c_k];
    }
  }

  /* End of MATLAB Function: '<S19>/propagate_quat' */

  /* Sum: '<S25>/Sum' incorporates:
   *  Product: '<S25>/Product'
   *  Product: '<S25>/Product1'
   *  Product: '<S25>/Product2'
   *  Product: '<S25>/Product3'
   */
  absxk = ((rtb_q_min_k1[0] * rtb_q_min_k1[0] + rtb_q_min_k1[1] * rtb_q_min_k1[1])
           + rtb_q_min_k1[2] * rtb_q_min_k1[2]) + rtb_q_min_k1[3] *
    rtb_q_min_k1[3];

  /* Sqrt: '<S24>/sqrt' */
  absxk = sqrt(absxk);

  /* Product: '<S21>/Product' */
  w_n = rtb_q_min_k1[0] / absxk;

  /* Product: '<S21>/Product1' */
  rtb_Product1_h = rtb_q_min_k1[1] / absxk;

  /* Product: '<S21>/Product2' */
  rtb_Product2_d = rtb_q_min_k1[2] / absxk;

  /* Product: '<S21>/Product3' */
  rtb_Product3_f = rtb_q_min_k1[3] / absxk;

  /* RateTransition: '<S1>/Rate Transition3' incorporates:
   *  RateTransition: '<S34>/Rate Transition'
   */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    for (i = 0; i < 5; i++) {
      rtDW.RateTransition3_n[i] = rtDW.RateTransition3_Buffer0[i];
    }

    rtDW.RateTransition[0] = rtDW.RateTransition_Buffer0[0];
    rtDW.RateTransition[1] = rtDW.RateTransition_Buffer0[1];
    rtDW.RateTransition[2] = rtDW.RateTransition_Buffer0[2];
  }

  /* End of RateTransition: '<S1>/Rate Transition3' */

  /* MATLAB Function: '<S33>/control_selection' */
  /*  Control Selection Logic */
  /*    HuskySat-1, ADCS Team */
  /*    Last Update: T. Reynolds, 11.21.17 */
  /* MATLAB Function 'command_generation/Control Selection/control_selection': '<S36>:1' */
  /*  Selects which command gets passed to the actuators based on the mode */
  /*  provided by upstream mode selection. Current modes are: */
  /*    mode_logic.bdot             = 1; */
  /*    mode_logic.desat_eclipse    = 2; */
  /*    mode_logic.point            = 3; */
  /*        point.velocity          = 31; */
  /*        point.ground            = 32; */
  /*        point.MPC               = 33; */
  /*    mode_logic.low_power        = 4; */
  /*  */
  /*  FLAG output can be used to make sure the control output matches up with */
  /*  what is being commanded, before commands are passed to actuators. FLAG */
  /*  should match mode, unless something is wrong. */
  /* '<S36>:1:20' FLAG    = 1; */
  /* '<S36>:1:22' if mode == 1 */
  switch (rtDW.UnitDelay_DSTATE_ao) {
   case 1:
    /* '<S36>:1:23' cmd_MT_out      = zeros(3,1); */
    Psi[0] = 0.0;
    Psi[1] = 0.0;
    Psi[2] = 0.0;
    break;

   case 2:
    /* '<S36>:1:24' elseif mode == 2 */
    /* '<S36>:1:25' cmd_MT_out      = cmd_MT; */
    Psi[0] = rtDW.RateTransition[0];
    Psi[1] = rtDW.RateTransition[1];
    Psi[2] = rtDW.RateTransition[2];
    break;

   default:
    if ((rtDW.UnitDelay_DSTATE_ao == 31) || (rtDW.UnitDelay_DSTATE_ao == 32) ||
        (rtDW.UnitDelay_DSTATE_ao == 33)) {
      /* '<S36>:1:26' elseif( mode == 31 || mode == 32 || mode == 33 ) */
      /* '<S36>:1:27' cmd_MT_out      = cmd_MT; */
      Psi[0] = rtDW.RateTransition[0];
      Psi[1] = rtDW.RateTransition[1];
      Psi[2] = rtDW.RateTransition[2];
    } else if (rtDW.UnitDelay_DSTATE_ao == 4) {
      /* '<S36>:1:28' elseif mode == 4 */
      /* '<S36>:1:29' cmd_MT_out      = zeros(3,1); */
      Psi[0] = 0.0;
      Psi[1] = 0.0;
      Psi[2] = 0.0;
    } else {
      /* '<S36>:1:30' else */
      /* '<S36>:1:31' FLAG = -1; */
      /* '<S36>:1:32' cmd_MT_out      = zeros(3,1); */
      Psi[0] = 0.0;
      Psi[1] = 0.0;
      Psi[2] = 0.0;
    }
    break;
  }

  /* End of MATLAB Function: '<S33>/control_selection' */

  /* RateTransition: '<S34>/Rate Transition3' incorporates:
   *  Inport: '<Root>/mag_vec_body_T'
   */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    rtDW.RateTransition3 = rtU.mag_vec_body_T[3];

    /* RateTransition: '<S34>/Rate Transition2' incorporates:
     *  Inport: '<Root>/mag_vec_body_T'
     */
    rtDW.RateTransition2_d[0] = rtb_DataTypeConversion1[0];
    rtDW.RateTransition2_d[1] = rtb_DataTypeConversion1[1];
    rtDW.RateTransition2_d[2] = rtb_DataTypeConversion1[2];
  }

  /* End of RateTransition: '<S34>/Rate Transition3' */

  /* DiscretePulseGenerator: '<S34>/MT_on' */
  absxk = ((rtDW.clockTickCounter_l < 6.9999999999999991) &&
           (rtDW.clockTickCounter_l >= 0L));
  if (rtDW.clockTickCounter_l >= 9L) {
    rtDW.clockTickCounter_l = 0L;
  } else {
    rtDW.clockTickCounter_l++;
  }

  /* End of DiscretePulseGenerator: '<S34>/MT_on' */

  /* RateTransition: '<S34>/Rate Transition5' */
  if (rtM->Timing.RateInteraction.TID1_2 == 1) {
    rtDW.RateTransition5 = absxk;
  }

  /* End of RateTransition: '<S34>/Rate Transition5' */

  /* Gain: '<S38>/To DigVal1' */
  absxk = 846.66666666666663 * Psi[0];

  /* DataTypeConversion: '<S38>/Data Type Conversion' */
  t = fabs(absxk);
  if (t < 4.503599627370496E+15) {
    if (t >= 0.5) {
      r_idx_1 = floor(absxk + 0.5);
    } else {
      r_idx_1 = absxk * 0.0;
    }
  } else {
    r_idx_1 = absxk;
  }

  /* Gain: '<S38>/To DigVal2' */
  absxk = 846.66666666666663 * Psi[1];

  /* DataTypeConversion: '<S38>/Data Type Conversion1' */
  t = fabs(absxk);
  if (t < 4.503599627370496E+15) {
    if (t >= 0.5) {
      scale = floor(absxk + 0.5);
    } else {
      scale = absxk * 0.0;
    }
  } else {
    scale = absxk;
  }

  /* Gain: '<S38>/To DigVal3' */
  absxk = 846.66666666666663 * Psi[2];

  /* DataTypeConversion: '<S38>/Data Type Conversion2' */
  t = fabs(absxk);
  if (t < 4.503599627370496E+15) {
    if (t >= 0.5) {
      t = floor(absxk + 0.5);
    } else {
      t = absxk * 0.0;
    }
  } else {
    t = absxk;
  }

  /* Abs: '<S7>/Abs' */
  rtb_DataTypeConversion1[0] = fabs(rtDW.RateTransition7_2_Buffer0[0]);
  rtb_DataTypeConversion1[1] = fabs(rtDW.RateTransition7_2_Buffer0[1]);
  rtb_DataTypeConversion1[2] = fabs(rtDW.RateTransition7_2_Buffer0[2]);

  /* MinMax: '<S7>/MinMax' */
  if ((rtb_DataTypeConversion1[0] >= rtb_DataTypeConversion1[1L]) || rtIsNaN
      (rtb_DataTypeConversion1[1L])) {
    absxk = rtb_DataTypeConversion1[0];
  } else {
    absxk = rtb_DataTypeConversion1[1L];
  }

  if (!((absxk >= rtb_DataTypeConversion1[2L]) || rtIsNaN
        (rtb_DataTypeConversion1[2L]))) {
    absxk = rtb_DataTypeConversion1[2L];
  }

  /* End of MinMax: '<S7>/MinMax' */

  /* Relay: '<S7>/Relay' */
  if (absxk >= 0.12) {
    rtDW.bitsForTID1.Relay_Mode = true;
  } else {
    if (absxk <= 0.00436) {
      rtDW.bitsForTID1.Relay_Mode = false;
    }
  }

  /* MATLAB Function: '<S7>/MATLAB Function' incorporates:
   *  Inport: '<Root>/sc_above_gs'
   *  Inport: '<Root>/sc_in_sun'
   *  Relay: '<S7>/Relay'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  Main FSW Mode Selector */
  /*    UW HuskySat-1, ADCS Team */
  /*    Last Updated: T. Reynolds 2.15.18 */
  /* MATLAB Function 'mode_selecction/MATLAB Function': '<S39>:1' */
  /*  Chooses control mode based on sensor and actuator data. */
  /*    mode_logic.bdot             = 1; */
  /*    mode_logic.desat            = 2; deprecated */
  /*    mode_logic.point            = 3; */
  /*        point.velocity          = 31; */
  /*        point.ground            = 32; */
  /*        point.MPC               = 33; */
  /*    mode_logic.low_power        = 4; */
  /*  */
  /*  ----- INPUTS ----- */
  /*  */
  /*  CAN - Data coming over the CAN bus. Type: 6x1 double. Parsed according to: */
  /*  */
  /*    CAN(1) == Override the autonomous mode selection and command the mode */
  /*                corresponding to this number. Set to 0 if the autonomous  */
  /*                selection is to go ahead. */
  /*    CAN(2) == Reboot just occured OR we just exited a detumble. Value: BOOL.  */
  /*                we will sit in this mode until either pointing takes over */
  /*                or body rates rise again. */
  /*    CAN(3) == Go ahead for the pointing mode. Value: BOOL. One means the s/c */
  /*                is go for pointing of any kind. */
  /*    CAN(4) == Go ahead for COM2 and GS pointing. Value: BOOL. One means that */
  /*                the COM2 system will be booted on the next flyover. */
  /*    CAN(5) == PPT On. Value: BOOL. Zero implies PPT is off and will not fire */
  /*                on the next sync cycle. */
  /*  */
  /*  sc_in_sun: - Tells us if the s/c is in eclipse. Type: 1x1 boolean. */
  /*  */
  /*  GS_appraoch: - Tells us if the ground station is approaching based on */
  /*                    reorientation criteria. Type: 1x1 boolean. */
  /*  */
  /*  bdot_flag: - Output from the relay block. Goes high when thresh_max is */
  /*  hit, and does not go low until thresh_min is hit (hysteresis). */
  /*  */
  /*  desat_flag: - Same as bdot_flag but with reaction wheel RPM thresholds. */
  /*  */
  /*  sc_mode_old - Previous spacecraft state, used to test exit conditions for */
  /*                    each mode. */
  /*  */
  /*  ----- OUTPUTS ----- */
  /*  */
  /*  sc_mode - Spacecraft state determined. Type: double. */
  /*  */
  /*  #codegen */
  /*  ----------------------------------------------------------------------- % */
  /*  Default to last known condition and no experimental control */
  /* '<S39>:1:55' sc_mode     = sc_mode_last; */
  rtb_sc_mode = rtDW.UnitDelay_DSTATE_ao;

  /* '<S39>:1:57' if( CAN(1) == 0 ) */
  if (rtDW.RateTransition3_n[0] == 0) {
    /*  Check for bdot condition */
    /* '<S39>:1:60' if( bdot_flag == 1 ) */
    if (rtDW.bitsForTID1.Relay_Mode) {
      /* '<S39>:1:61' sc_mode     = int8(1.0); */
      rtb_sc_mode = 1;

      /*  If Bdot is triggered, return immediately since control has high  */
      /*  priority and control will be handed off. */
    } else {
      /*  Bdot flag will go low once omega_radps drops below the lower */
      /*  threshold. Accounted for in the relay block. */
      /*  Check for low power condition or recent boot up */
      /* '<S39>:1:71' if( CAN(2) == 1 || (sc_mode == 1 && bdot_flag == 0) ) */
      if ((rtDW.RateTransition3_n[1] == 1) || (rtDW.UnitDelay_DSTATE_ao == 1)) {
        /*  If reboot/bdot just occured and we are not tumbling, enter low */
        /*  power/estimate mode. */
        /* '<S39>:1:74' sc_mode     = int8(4.0); */
        rtb_sc_mode = 4;
      }

      /*      % Check for desat conditions. This is for dedicated desat */
      /*      % event. The PPT should be off before we initiate a desat.  */
      /*      if( (CAN(5) == 0) || (CAN(5) == 1 && sc_mode_last == 2) ) */
      /*          sc_mode     = 2; */
      /*          return; */
      /*      end */
      /*  If all cases upstream have been passed, then enter pointing mode if */
      /*  allowed to do so. Check for pointing condition from CAN */
      /* '<S39>:1:86' if( CAN(3) == 1 ) */
      if (rtDW.RateTransition3_n[2] == 1) {
        /*  Default to velocity pointing mode */
        /* '<S39>:1:88' sc_mode     = int8(31.0); */
        rtb_sc_mode = 31;

        /*  Check to see if entering GS range, not in eclipse and */
        /*  COM2 system will be activated. */
        /* '<S39>:1:92' if( GS_approach && sc_in_sun && (CAN(4)==1) ) */
        if (rtU.sc_above_gs && rtU.sc_in_sun && (rtDW.RateTransition3_n[3] == 1))
        {
          /* '<S39>:1:93' sc_mode  = int8(32.0); */
          rtb_sc_mode = 32;
        }
      }

      /*  If signal received to override the autonomous mode selection, set mode  */
      /*  based on CAN(1) value. */
    }
  } else {
    if (rtDW.RateTransition3_n[0] != 0) {
      /* '<S39>:1:100' elseif( CAN(1) ~= 0 ) */
      /* '<S39>:1:102' sc_mode = CAN(1); */
      rtb_sc_mode = rtDW.RateTransition3_n[0];
    }
  }

  /* End of MATLAB Function: '<S7>/MATLAB Function' */

  /* MATLAB Function: '<S8>/TARG_GEN' incorporates:
   *  Constant: '<S8>/r_SEA'
   *  Inport: '<Root>/sc2sun_unit'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  FSW Target Generation Library */
  /*    UW HuskySat-1, ADCS Team */
  /*    T. Reynolds 4.23.18 */
  /*  */
  /*  Computes desired quaternion and angular velocity depending on the sc_mode */
  /*    mode_logic.bdot             = 1; */
  /*    mode_logic.desat_eclipse    = 2; */
  /*    mode_logic.point            = 3; */
  /*        point.velocity          = 31; */
  /*        point.ground            = 32; */
  /*    mode_logic.low_power        = 4; */
  /* MATLAB Function 'target_generation_lib/TARG_GEN': '<S40>:1' */
  /*  ----------------------------------------------------------------------- % */
  /* '<S40>:1:17' COM2    = [0; 0; 1]; */
  /* '<S40>:1:18' SP      = [0; 1; 0]; */
  /* '<S40>:1:19' flag    = int8(0); */
  /* '<S40>:1:20' tol     = 1e-3; */
  /*  PPT     = [0;1;2];    % these are for testing only */
  /*  SP      = [1;3;0]; */
  /*  Check for zero vectors */
  /* '<S40>:1:26' if( norm(sc2sun_eci) < tol ) */
  if (norm(rtU.sc2sun_unit) < 0.001) {
    /* '<S40>:1:27' flag    = int8(-1); */
    /* '<S40>:1:28' C_cmd   = single(eye(3)); */
    /* '<S40>:1:29' q_cmd   = myDCM2quat(C_cmd); */
    myDCM2quat(b_0, rtb_q_min_k1);

    /* '<S40>:1:30' w_cmd   = single(zeros(3,1)); */
  } else {
    /* '<S40>:1:34' if( (sc_mode == 31 || sc_mode == 32) && flag == 0 ) */
    guard1 = false;
    if ((rtDW.UnitDelay_DSTATE_ao == 31) || (rtDW.UnitDelay_DSTATE_ao == 32)) {
      /*  Body */
      /* '<S40>:1:37' B1  = COM2./norm(COM2); */
      /* '<S40>:1:38' B2  = SP./norm(SP); */
      /* '<S40>:1:39' Xtb     = B1; */
      /* '<S40>:1:40' Ytb     = cross(B1,B2)/norm(cross(B1,B2)); */
      /* '<S40>:1:41' Ztb     = cross(Xtb,Ytb)/norm(cross(Xtb,Ytb)); */
      /*  Inertial */
      /* '<S40>:1:44' S1  = -r_eci_SEA./norm(r_eci_SEA); */
      y = norm_i(rtConstP.r_SEA_Value);

      /* Pointing direction is towards SEA */
      /* '<S40>:1:45' S2  = sc2sun_eci./norm(sc2sun_eci); */
      absxk = norm(rtU.sc2sun_unit);
      S1[0] = -0.409930646F / y;
      rtb_DataTypeConversion1[0] = rtU.sc2sun_unit[0] / absxk;
      S1[1] = 0.538629532F / y;
      rtb_DataTypeConversion1[1] = rtU.sc2sun_unit[1] / absxk;
      S1[2] = -0.736094475F / y;
      rtb_DataTypeConversion1[2] = rtU.sc2sun_unit[2] / absxk;

      /* '<S40>:1:46' Xti     = S1; */
      /* '<S40>:1:47' Yti     = cross(S1,S2)/norm(cross(S1,S2)); */
      S1_1[0] = S1[1] * (real32_T)rtb_DataTypeConversion1[2] - S1[2] * (real32_T)
        rtb_DataTypeConversion1[1];
      S1_1[1] = S1[2] * (real32_T)rtb_DataTypeConversion1[0] - S1[0] * (real32_T)
        rtb_DataTypeConversion1[2];
      S1_1[2] = S1[0] * (real32_T)rtb_DataTypeConversion1[1] - S1[1] * (real32_T)
        rtb_DataTypeConversion1[0];
      y = norm_i(S1_1);
      Yti[0] = (S1[1] * (real32_T)rtb_DataTypeConversion1[2] - S1[2] * (real32_T)
                rtb_DataTypeConversion1[1]) / y;
      Yti[1] = (S1[2] * (real32_T)rtb_DataTypeConversion1[0] - S1[0] * (real32_T)
                rtb_DataTypeConversion1[2]) / y;
      Yti[2] = (S1[0] * (real32_T)rtb_DataTypeConversion1[1] - S1[1] * (real32_T)
                rtb_DataTypeConversion1[0]) / y;

      /* '<S40>:1:48' Zti     = cross(Xti,Yti)/norm(cross(Xti,Yti)); */
      S1_0[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
      S1_0[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
      S1_0[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
      y = norm_i(S1_0);

      /*  Checks */
      /* '<S40>:1:51' if ( abs(dot(B1,S2)) > 1-tol ) */
      if (fabs((0.0 * rtb_DataTypeConversion1[0] + 0.0 *
                rtb_DataTypeConversion1[1]) + rtb_DataTypeConversion1[2]) >
          0.999) {
        /* '<S40>:1:52' flag    = int8(-1); */
        /* '<S40>:1:53' C_cmd   = single(eye(3)); */
        /* '<S40>:1:54' q_cmd   = myDCM2quat(C_cmd); */
        myDCM2quat(b_0, rtb_q_min_k1);

        /* '<S40>:1:55' w_cmd   = single(zeros(3,1)); */
      } else {
        /*  Stack vectors */
        /* '<S40>:1:60' Cb  = [Xtb, Ytb, Ztb]; */
        /* '<S40>:1:61' Ci  = [Xti, Yti, Zti]; */
        /*  Commanded DCM ( body to inertial ) */
        /* '<S40>:1:64' C_cmd   = single(Ci*Cb'); */
        S1_2[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
        S1_2[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
        S1_2[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
        for (i = 0; i < 3; i++) {
          S1_3[i] = S1[i];
          S1_3[3 + i] = Yti[i];
          S1_3[6 + i] = S1_2[i] / y;
          for (c_k = 0; c_k < 3; c_k++) {
            C_cmd[i + 3 * c_k] = 0.0F;
            C_cmd[i + 3 * c_k] += (real32_T)d[3 * c_k] * S1_3[i];
            C_cmd[i + 3 * c_k] += (real32_T)d[3 * c_k + 1] * S1_3[i + 3];
            C_cmd[i + 3 * c_k] += (real32_T)d[3 * c_k + 2] * S1_3[i + 6];
          }
        }

        guard1 = true;
      }
    } else {
      /* '<S40>:1:66' else */
      /* '<S40>:1:67' C_cmd   = single(eye(3)); */
      for (i = 0; i < 9; i++) {
        I[i] = 0;
      }

      I[0] = 1;
      I[4] = 1;
      I[8] = 1;
      for (i = 0; i < 9; i++) {
        C_cmd[i] = I[i];
      }

      guard1 = true;
    }

    if (guard1) {
      /*  Compute commanded quaternion from DCM */
      /* '<S40>:1:71' q_cmd   = myDCM2quat( C_cmd ); */
      myDCM2quat(C_cmd, rtb_q_min_k1);

      /* '<S40>:1:72' w_cmd   = single(zeros(3,1)); */
    }
  }

  /* End of MATLAB Function: '<S8>/TARG_GEN' */

  /* DataTypeConversion: '<S8>/Data Type Conversion' */
  rtDW.RateTransition6_Buffer0[0] = (real32_T)rtb_q_min_k1[0];
  rtDW.RateTransition6_Buffer0[1] = (real32_T)rtb_q_min_k1[1];
  rtDW.RateTransition6_Buffer0[2] = (real32_T)rtb_q_min_k1[2];
  rtDW.RateTransition6_Buffer0[3] = (real32_T)rtb_q_min_k1[3];

  /* Update for RateTransition: '<S3>/Rate Transition2' */
  rtDW.RateTransition2_Buffer0[0] = w_n;
  rtDW.RateTransition2_Buffer0[1] = rtb_Product1_h;
  rtDW.RateTransition2_Buffer0[2] = rtb_Product2_d;
  rtDW.RateTransition2_Buffer0[3] = rtb_Product3_f;

  /* Update for RateTransition: '<S1>/Rate Transition7' */
  rtDW.RateTransition7_3_Buffer0 = rtDW.UnitDelay_DSTATE_ao;

  /* Update for RateTransition: '<S1>/Rate Transition7' incorporates:
   *  Update for Inport: '<Root>/sc_above_gs'
   */
  rtDW.RateTransition7_5_Buffer0 = rtU.sc_above_gs;

  /* Update for RateTransition: '<S1>/Rate Transition4' */
  rtDW.RateTransition4_1_Buffer0 = rtDW.UnitDelay_DSTATE_ao;

  /* Update for RateTransition: '<S1>/Rate Transition4' incorporates:
   *  DataTypeConversion: '<S38>/Data Type Conversion'
   *  DataTypeConversion: '<S38>/Data Type Conversion1'
   *  DataTypeConversion: '<S38>/Data Type Conversion2'
   */
  rtDW.RateTransition4_2_Buffer0[0] = (int8_T)r_idx_1;
  rtDW.RateTransition4_2_Buffer0[1] = (int8_T)scale;
  rtDW.RateTransition4_2_Buffer0[2] = (int8_T)t;

  /* Update for UnitDelay: '<S3>/Unit Delay' */
  rtDW.UnitDelay_DSTATE[0] = w_n;
  rtDW.UnitDelay_DSTATE[1] = rtb_Product1_h;
  rtDW.UnitDelay_DSTATE[2] = rtb_Product2_d;
  rtDW.UnitDelay_DSTATE[3] = rtb_Product3_f;

  /* Product: '<S19>/Product' incorporates:
   *  Math: '<S19>/Math Function'
   */
  for (i = 0; i < 6; i++) {
    for (c_k = 0; c_k < 6; c_k++) {
      rtb_Merge2_0[i + 6 * c_k] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        rtb_Merge2_0[i + 6 * c_k] += rtb_Merge2[6 * i_0 + i] * rtb_K[6 * i_0 +
          c_k];
      }
    }
  }

  /* Update for UnitDelay: '<S3>/Unit Delay2' incorporates:
   *  Product: '<S19>/Product'
   *  Sum: '<S19>/Add'
   */
  for (i = 0; i < 6; i++) {
    for (c_k = 0; c_k < 6; c_k++) {
      scale = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        scale += rtb_K[6 * i_0 + c_k] * rtb_Merge2_0[6 * i + i_0];
      }

      rtDW.UnitDelay2_DSTATE[c_k + 6 * i] = rtConstB.Product1[6 * i + c_k] +
        scale;
    }
  }

  /* End of Update for UnitDelay: '<S3>/Unit Delay2' */

  /* Update for UnitDelay: '<S7>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_ao = rtb_sc_mode;
}

/* Model step function for TID2 */
void MSP_FSW_step2(void)               /* Sample time: [0.2s, 0.0s] */
{
  int32_T idxStart;
  int16_T i;
  int32_T idx1;
  int32_T idx2;
  real_T acc;
  real_T rtb_Normalization[3];
  real_T rtb_Product_p[3];

  /* MultiPortSwitch: '<S34>/Multiport Switch' incorporates:
   *  Logic: '<S34>/Logical Operator'
   *  Logic: '<S34>/Logical Operator1'
   */
  switch ((!(rtDW.RateTransition4 != 0.0)) && (rtDW.RateTransition3 != 0.0F)) {
   case 0:
    break;

   default:
    rtDW.UnitDelay_DSTATE_a[0] = rtDW.RateTransition2_d[0];
    rtDW.UnitDelay_DSTATE_a[1] = rtDW.RateTransition2_d[1];
    rtDW.UnitDelay_DSTATE_a[2] = rtDW.RateTransition2_d[2];
    break;
  }

  /* End of MultiPortSwitch: '<S34>/Multiport Switch' */

  /* S-Function (sdsp2norm2): '<S34>/Normalization' */
  idxStart = 0L;
  i = 0;
  while (i < 1) {
    idx1 = idxStart;
    idx2 = idxStart;
    i = 0;
    while (i < 1) {
      acc = 0.0;
      for (i = 0; i < 3; i++) {
        acc += rtDW.UnitDelay_DSTATE_a[idx1] * rtDW.UnitDelay_DSTATE_a[idx1];
        idx1++;
      }

      acc = 1.0 / (acc + 1.0E-10);
      for (i = 0; i < 3; i++) {
        rtb_Normalization[idx2] = rtDW.UnitDelay_DSTATE_a[idx2] * acc;
        idx2++;
      }

      i = 1;
    }

    idxStart++;
    i = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S34>/Normalization' */

  /* Sum: '<S34>/Add' incorporates:
   *  Constant: '<S34>/MoI'
   *  Gain: '<S34>/rpm2rad'
   *  Product: '<S34>/Matrix Multiply'
   */
  for (i = 0; i < 3; i++) {
    rtb_Product_p[i] = (0.10471975511965977 * rtDW.RateTransition1[0] *
                        rtConstP.MoI_Value[i] + rtConstP.MoI_Value[i + 3] *
                        (0.10471975511965977 * rtDW.RateTransition1[1])) +
      rtConstP.MoI_Value[i + 6] * (0.10471975511965977 * rtDW.RateTransition1[2]);
  }

  /* End of Sum: '<S34>/Add' */

  /* Update for RateTransition: '<S1>/Rate Transition3' incorporates:
   *  Update for Inport: '<Root>/CAN_IN'
   */
  for (i = 0; i < 5; i++) {
    rtDW.RateTransition3_Buffer0[i] = rtU.CAN[i];
  }

  /* End of Update for RateTransition: '<S1>/Rate Transition3' */

  /* Saturate: '<S34>/Sat2' incorporates:
   *  Gain: '<S34>/-gain'
   *  Product: '<S34>/Product'
   *  Product: '<S37>/Element product'
   *  Sum: '<S37>/Add3'
   */
  acc = (rtb_Normalization[1] * rtb_Product_p[2] - rtb_Normalization[2] *
         rtb_Product_p[1]) * 10.0 * rtDW.RateTransition5;
  if (acc > 0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[0] = 0.15;
  } else if (acc < -0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[0] = -0.15;
  } else {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[0] = acc;
  }

  /* End of Saturate: '<S34>/Sat2' */

  /* Saturate: '<S34>/Sat' incorporates:
   *  Gain: '<S34>/-gain'
   *  Product: '<S34>/Product'
   *  Product: '<S37>/Element product'
   *  Sum: '<S37>/Add3'
   */
  acc = (rtb_Normalization[2] * rtb_Product_p[0] - rtb_Normalization[0] *
         rtb_Product_p[2]) * 10.0 * rtDW.RateTransition5;
  if (acc > 0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[1] = 0.15;
  } else if (acc < -0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[1] = -0.15;
  } else {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[1] = acc;
  }

  /* End of Saturate: '<S34>/Sat' */

  /* Saturate: '<S34>/Sat1' incorporates:
   *  Gain: '<S34>/-gain'
   *  Product: '<S34>/Product'
   *  Product: '<S37>/Element product'
   *  Sum: '<S37>/Add3'
   */
  acc = (rtb_Normalization[0] * rtb_Product_p[1] - rtb_Normalization[1] *
         rtb_Product_p[0]) * 10.0 * rtDW.RateTransition5;
  if (acc > 0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[2] = 0.15;
  } else if (acc < -0.15) {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[2] = -0.15;
  } else {
    /* Update for RateTransition: '<S34>/Rate Transition' */
    rtDW.RateTransition_Buffer0[2] = acc;
  }

  /* End of Saturate: '<S34>/Sat1' */
}

/* Model initialize function */
void MSP_FSW_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* Start for Outport: '<Root>/sc_quat' incorporates:
   *  Start for RateTransition: '<S3>/Rate Transition2'
   */
  rtY.sc_quat[0] = 1.0;

  /* Start for RateTransition: '<S8>/Rate Transition6' */
  rtDW.RateTransition6[0] = 1.0F;

  /* Start for Outport: '<Root>/sc_quat' incorporates:
   *  Start for RateTransition: '<S3>/Rate Transition2'
   */
  rtY.sc_quat[1] = 0.0;

  /* Start for RateTransition: '<S8>/Rate Transition6' */
  rtDW.RateTransition6[1] = 0.0F;

  /* Start for Outport: '<Root>/sc_quat' incorporates:
   *  Start for RateTransition: '<S3>/Rate Transition2'
   */
  rtY.sc_quat[2] = 0.0;

  /* Start for RateTransition: '<S8>/Rate Transition6' */
  rtDW.RateTransition6[2] = 0.0F;

  /* Start for Outport: '<Root>/sc_quat' incorporates:
   *  Start for RateTransition: '<S3>/Rate Transition2'
   */
  rtY.sc_quat[3] = 0.0;

  /* Start for RateTransition: '<S8>/Rate Transition6' */
  rtDW.RateTransition6[3] = 0.0F;

  /* InitializeConditions for RateTransition: '<S3>/Rate Transition2' */
  rtDW.RateTransition2_Buffer0[0] = 1.0;
  rtDW.RateTransition2_Buffer0[1] = 0.0;
  rtDW.RateTransition2_Buffer0[2] = 0.0;
  rtDW.RateTransition2_Buffer0[3] = 0.0;

  /* InitializeConditions for RateTransition: '<S8>/Rate Transition6' */
  rtDW.RateTransition6_Buffer0[0] = 1.0F;
  rtDW.RateTransition6_Buffer0[1] = 0.0F;
  rtDW.RateTransition6_Buffer0[2] = 0.0F;
  rtDW.RateTransition6_Buffer0[3] = 0.0F;

  /* InitializeConditions for UnitDelay: '<S3>/Unit Delay' */
  rtDW.UnitDelay_DSTATE[0] = 1.0;
  rtDW.UnitDelay_DSTATE[1] = 0.0;
  rtDW.UnitDelay_DSTATE[2] = 0.0;
  rtDW.UnitDelay_DSTATE[3] = 0.0;

  /* InitializeConditions for UnitDelay: '<S3>/Unit Delay2' */
  memcpy(&rtDW.UnitDelay2_DSTATE[0], &rtConstP.UnitDelay2_InitialCondition[0],
         36U * sizeof(real_T));

  /* InitializeConditions for UnitDelay: '<S7>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_ao = 1;
}

/* Model terminate function */
void MSP_FSW_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
