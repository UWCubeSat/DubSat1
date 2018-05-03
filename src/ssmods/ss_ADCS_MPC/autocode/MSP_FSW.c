/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW.c
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
static real_T norm(const real_T x[3]);
static real32_T norm_i(const real32_T x[3]);
static void myDCM2quat(const real32_T DCM[9], real_T q[4]);
static void mrdivide(real_T A[36], const real_T B_0[36]);
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

/* Function for MATLAB Function: '<S11>/TARG_GEN' */
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

/* Function for MATLAB Function: '<S11>/TARG_GEN' */
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
 * Function for MATLAB Function: '<S11>/TARG_GEN'
 * function q = myDCM2quat( DCM )
 */
static void myDCM2quat(const real32_T DCM[9], real_T q[4])
{
  real32_T tr;
  real_T eta;
  real32_T y;

  /* '<S42>:1:77' q   = zeros(4,1); */
  /* '<S42>:1:79' tr  = trace(DCM); */
  /* '<S42>:1:81' q(1)    = 0.5*sqrt(tr + 1); */
  tr = (real32_T)sqrt(((DCM[0] + DCM[4]) + DCM[8]) + 1.0F);
  y = 0.5F * tr;
  q[0] = 0.5F * tr;

  /* '<S42>:1:83' if( q(1) ~= 0 ) */
  if (y != 0.0F) {
    /* '<S42>:1:84' eta     = 1/(4*q(1)); */
    eta = 1.0 / (4.0 * y);

    /* '<S42>:1:85' q(2)    = eta*(DCM(2,3) - DCM(3,2)); */
    q[1] = (DCM[7] - DCM[5]) * (real32_T)eta;

    /* '<S42>:1:86' q(3)    = eta*(DCM(3,1) - DCM(1,3)); */
    q[2] = (DCM[2] - DCM[6]) * (real32_T)eta;

    /* '<S42>:1:87' q(4)    = eta*(DCM(1,2) - DCM(2,1)); */
    q[3] = (DCM[3] - DCM[1]) * (real32_T)eta;
  } else {
    /* '<S42>:1:88' else */
    /* '<S42>:1:89' q(2)  = sqrt(0.5*(DCM(1,1) + 1)); */
    q[1] = (real32_T)sqrt((DCM[0] + 1.0F) * 0.5F);

    /* '<S42>:1:90' q(3)  = sign(DCM(1,2))*sqrt(0.5*(DCM(2,2) + 1)); */
    if (DCM[3] < 0.0F) {
      tr = -1.0F;
    } else if (DCM[3] > 0.0F) {
      tr = 1.0F;
    } else if (DCM[3] == 0.0F) {
      tr = 0.0F;
    } else {
      tr = DCM[3];
    }

    q[2] = (real32_T)sqrt((DCM[4] + 1.0F) * 0.5F) * tr;

    /* '<S42>:1:91' q(4)  = sign(DCM(1,3))*sqrt(0.5*(DCM(3,3) + 1)); */
    if (DCM[6] < 0.0F) {
      tr = -1.0F;
    } else if (DCM[6] > 0.0F) {
      tr = 1.0F;
    } else if (DCM[6] == 0.0F) {
      tr = 0.0F;
    } else {
      tr = DCM[6];
    }

    q[3] = (real32_T)sqrt((DCM[8] + 1.0F) * 0.5F) * tr;
  }
}

/* Function for MATLAB Function: '<S28>/kalman_gain' */
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

/* Model step function */
void MSP_FSW_step(void)
{
  real32_T C_cmd[9];
  real32_T S1[3];
  real_T S2[3];
  real32_T Yti[3];
  int8_T I[9];
  int16_T k;
  static const real32_T b[9] = { 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F,
    1.0F };

  static const int8_T d[9] = { 0, -1, 0, 0, 0, -1, 1, 0, 0 };

  real_T w_n;
  real_T Psi[3];
  static const int8_T b_0[16] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0
  };

  static const int8_T a[16] = { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 };

  static const int8_T b_b[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  int8_T Phi_22[9];
  real_T absxk;
  real_T t;
  real_T c_x;
  real_T b_c;
  real_T b_1;
  real_T c_c;
  static const int8_T a_0[9] = { -1, 0, 0, 0, -1, 0, 0, 0, -1 };

  static const int8_T b_a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real_T rotation_matrix[9];
  real_T scale;
  real_T KHI[36];
  int8_T I_0[36];
  real_T rtb_q_cmd[4];
  int32_T idxStart;
  int16_T i;
  int32_T idx1;
  int32_T idx2;
  real_T rtb_Normalization[3];
  real_T rtb_q_min_k1[4];
  real_T rtb_Sum_j;
  real_T rtb_Product_am;
  real_T rtb_Product1_h;
  real_T rtb_Product2_d;
  real_T rtb_Switch[3];
  boolean_T rtb_Switch_ms;
  real_T rtb_body_est[6];
  real_T rtb_MatrixMultiply[6];
  int8_T rtb_sc_mode;
  real_T rtb_Merge2[36];
  real_T rtb_K[36];
  real_T rtb_Merge2_0[36];
  real32_T S1_0[3];
  real32_T S1_1[3];
  real32_T S1_2[3];
  real32_T S1_3[9];
  real_T rtb_Merge2_1[36];
  real_T rtb_DataTypeConversion1_0[6];
  real_T rtb_DataTypeConversion1_1[6];
  real_T KHI_0[36];
  real_T rtb_K_0[36];
  real_T absxk_0[16];
  real_T a_1[16];
  real_T a_2[16];
  real_T rotation_matrix_0[9];
  real_T rotation_matrix_1[9];
  int16_T i_0;
  real32_T rtb_DataTypeConversion_idx_0;
  real32_T rtb_DataTypeConversion_idx_1;
  real32_T rtb_DataTypeConversion_idx_2;
  real32_T rtb_DataTypeConversion_idx_3;
  real_T rtb_DataTypeConversion1_idx_0;
  real_T rtb_DataTypeConversion1_idx_1;
  real_T rtb_DataTypeConversion1_idx_2;
  boolean_T guard1 = false;

  /* Switch: '<S8>/Switch' incorporates:
   *  Inport: '<Root>/gyro_omega_body_radps'
   */
  rtb_Switch_ms = (rtU.gyro_omega_body_radps[3] > 0.5F);

  /* Outport: '<Root>/sc_mode' incorporates:
   *  UnitDelay: '<S7>/Unit Delay'
   */
  rtY.sc_mode = rtDW.UnitDelay_DSTATE_ao;

  /* MATLAB Function: '<S11>/TARG_GEN' incorporates:
   *  Constant: '<S11>/r_SEA'
   *  Inport: '<Root>/sc2sun_unit'
   *  UnitDelay: '<S7>/Unit Delay'
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
  /* MATLAB Function 'target_generation_lib/TARG_GEN': '<S42>:1' */
  /*  ----------------------------------------------------------------------- % */
  /* '<S42>:1:17' COM2    = [0; 0; 1]; */
  /* '<S42>:1:18' SP      = [0; 1; 0]; */
  /* '<S42>:1:19' flag    = int8(0); */
  /* '<S42>:1:20' tol     = 1e-3; */
  /*  PPT     = [0;1;2];    % these are for testing only */
  /*  SP      = [1;3;0]; */
  /*  Check for zero vectors */
  /* '<S42>:1:26' if( norm(sc2sun_eci) < tol ) */
  if (norm(rtU.sc2sun_unit) < 0.001) {
    /* '<S42>:1:27' flag    = int8(-1); */
    /* '<S42>:1:28' C_cmd   = single(eye(3)); */
    /* '<S42>:1:29' q_cmd   = myDCM2quat(C_cmd); */
    myDCM2quat(b, rtb_q_cmd);

    /* '<S42>:1:30' w_cmd   = single(zeros(3,1)); */
  } else {
    /* '<S42>:1:34' if( (sc_mode == 31 || sc_mode == 32) && flag == 0 ) */
    guard1 = false;
    if ((rtDW.UnitDelay_DSTATE_ao == 31) || (rtDW.UnitDelay_DSTATE_ao == 32)) {
      /*  Body */
      /* '<S42>:1:37' B1  = COM2./norm(COM2); */
      /* '<S42>:1:38' B2  = SP./norm(SP); */
      /* '<S42>:1:39' Xtb     = B1; */
      /* '<S42>:1:40' Ytb     = cross(B1,B2)/norm(cross(B1,B2)); */
      /* '<S42>:1:41' Ztb     = cross(Xtb,Ytb)/norm(cross(Xtb,Ytb)); */
      /*  Inertial */
      /* '<S42>:1:44' S1  = -r_eci_SEA./norm(r_eci_SEA); */
      rtb_DataTypeConversion_idx_0 = norm_i(rtConstP.r_SEA_Value);

      /* Pointing direction is towards SEA */
      /* '<S42>:1:45' S2  = sc2sun_eci./norm(sc2sun_eci); */
      rtb_Product_am = norm(rtU.sc2sun_unit);
      S1[0] = -0.409930646F / rtb_DataTypeConversion_idx_0;
      S2[0] = rtU.sc2sun_unit[0] / rtb_Product_am;
      S1[1] = 0.538629532F / rtb_DataTypeConversion_idx_0;
      S2[1] = rtU.sc2sun_unit[1] / rtb_Product_am;
      S1[2] = -0.736094475F / rtb_DataTypeConversion_idx_0;
      S2[2] = rtU.sc2sun_unit[2] / rtb_Product_am;

      /* '<S42>:1:46' Xti     = S1; */
      /* '<S42>:1:47' Yti     = cross(S1,S2)/norm(cross(S1,S2)); */
      S1_1[0] = S1[1] * (real32_T)S2[2] - S1[2] * (real32_T)S2[1];
      S1_1[1] = S1[2] * (real32_T)S2[0] - S1[0] * (real32_T)S2[2];
      S1_1[2] = S1[0] * (real32_T)S2[1] - S1[1] * (real32_T)S2[0];
      rtb_DataTypeConversion_idx_0 = norm_i(S1_1);
      Yti[0] = (S1[1] * (real32_T)S2[2] - S1[2] * (real32_T)S2[1]) /
        rtb_DataTypeConversion_idx_0;
      Yti[1] = (S1[2] * (real32_T)S2[0] - S1[0] * (real32_T)S2[2]) /
        rtb_DataTypeConversion_idx_0;
      Yti[2] = (S1[0] * (real32_T)S2[1] - S1[1] * (real32_T)S2[0]) /
        rtb_DataTypeConversion_idx_0;

      /* '<S42>:1:48' Zti     = cross(Xti,Yti)/norm(cross(Xti,Yti)); */
      S1_0[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
      S1_0[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
      S1_0[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
      rtb_DataTypeConversion_idx_0 = norm_i(S1_0);

      /*  Checks */
      /* '<S42>:1:51' if ( abs(dot(B1,S2)) > 1-tol ) */
      if (fabs((0.0 * S2[0] + 0.0 * S2[1]) + S2[2]) > 0.999) {
        /* '<S42>:1:52' flag    = int8(-1); */
        /* '<S42>:1:53' C_cmd   = single(eye(3)); */
        /* '<S42>:1:54' q_cmd   = myDCM2quat(C_cmd); */
        myDCM2quat(b, rtb_q_cmd);

        /* '<S42>:1:55' w_cmd   = single(zeros(3,1)); */
      } else {
        /*  Stack vectors */
        /* '<S42>:1:60' Cb  = [Xtb, Ytb, Ztb]; */
        /* '<S42>:1:61' Ci  = [Xti, Yti, Zti]; */
        /*  Commanded DCM ( body to inertial ) */
        /* '<S42>:1:64' C_cmd   = single(Ci*Cb'); */
        S1_2[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
        S1_2[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
        S1_2[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
        for (i = 0; i < 3; i++) {
          S1_3[i] = S1[i];
          S1_3[3 + i] = Yti[i];
          S1_3[6 + i] = S1_2[i] / rtb_DataTypeConversion_idx_0;
          for (k = 0; k < 3; k++) {
            C_cmd[i + 3 * k] = 0.0F;
            C_cmd[i + 3 * k] += (real32_T)d[3 * k] * S1_3[i];
            C_cmd[i + 3 * k] += (real32_T)d[3 * k + 1] * S1_3[i + 3];
            C_cmd[i + 3 * k] += (real32_T)d[3 * k + 2] * S1_3[i + 6];
          }
        }

        guard1 = true;
      }
    } else {
      /* '<S42>:1:66' else */
      /* '<S42>:1:67' C_cmd   = single(eye(3)); */
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
      /* '<S42>:1:71' q_cmd   = myDCM2quat( C_cmd ); */
      myDCM2quat(C_cmd, rtb_q_cmd);

      /* '<S42>:1:72' w_cmd   = single(zeros(3,1)); */
    }
  }

  /* End of MATLAB Function: '<S11>/TARG_GEN' */

  /* DataTypeConversion: '<S11>/Data Type Conversion' */
  rtb_DataTypeConversion_idx_0 = (real32_T)rtb_q_cmd[0];
  rtb_DataTypeConversion_idx_1 = (real32_T)rtb_q_cmd[1];
  rtb_DataTypeConversion_idx_2 = (real32_T)rtb_q_cmd[2];
  rtb_DataTypeConversion_idx_3 = (real32_T)rtb_q_cmd[3];

  /* Outport: '<Root>/sc_above_gsb' incorporates:
   *  Inport: '<Root>/sc_above_gs'
   */
  rtY.sc_above_gsb = rtU.sc_above_gs;

  /* Outport: '<Root>/sc_modeb' incorporates:
   *  UnitDelay: '<S7>/Unit Delay'
   */
  rtY.sc_modeb = rtDW.UnitDelay_DSTATE_ao;

  /* MultiPortSwitch: '<S36>/Multiport Switch' incorporates:
   *  UnitDelay: '<S36>/Unit Delay'
   */
  S2[0] = rtDW.UnitDelay_DSTATE_a[0];
  S2[1] = rtDW.UnitDelay_DSTATE_a[1];
  S2[2] = rtDW.UnitDelay_DSTATE_a[2];

  /* S-Function (sdsp2norm2): '<S36>/Normalization' */
  idxStart = 0L;
  i = 0;
  while (i < 1) {
    idx1 = idxStart;
    idx2 = idxStart;
    i = 0;
    while (i < 1) {
      rtb_Product_am = 0.0;
      for (k = 0; k < 3; k++) {
        rtb_Product_am += S2[idx1] * S2[idx1];
        idx1++;
      }

      rtb_Product_am = 1.0 / (rtb_Product_am + 1.0E-10);
      for (k = 0; k < 3; k++) {
        rtb_Normalization[idx2] = S2[idx2] * rtb_Product_am;
        idx2++;
      }

      i = 1;
    }

    idxStart++;
    i = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S36>/Normalization' */

  /* If: '<S22>/If' incorporates:
   *  Inport: '<Root>/MT_valid'
   *  Inport: '<Root>/mag_vec_body_T'
   *  Inport: '<Root>/sc_in_sun'
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   *  Inport: '<S29>/bias_min'
   *  Inport: '<S29>/cov_min'
   *  Inport: '<S29>/q_min'
   *  Logic: '<S3>/Logical Operator'
   *  Logic: '<S3>/Logical Operator1'
   *  Logic: '<S3>/Logical Operator2'
   *  Logic: '<S4>/Logical Operator'
   *  Switch: '<S10>/Switch'
   *  Switch: '<S9>/Switch'
   *  UnitDelay: '<S3>/Unit Delay'
   *  UnitDelay: '<S3>/Unit Delay1'
   *  UnitDelay: '<S3>/Unit Delay2'
   */
  if ((rtU.mag_vec_body_T[3] > 0.5F) && (rtU.MT_valid[0] && rtU.MT_valid[1] &&
       rtU.MT_valid[2]) && ((rtU.sun_vec_body_sunsensor[3] > 0.5F) &&
       rtU.sc_in_sun)) {
    /* Outputs for IfAction SubSystem: '<S22>/If Action Subsystem' incorporates:
     *  ActionPort: '<S28>/Action Port'
     */
    /* MATLAB Function: '<S28>/convert_inertial_body' incorporates:
     *  SignalConversion: '<S30>/TmpSignal ConversionAt SFunction Inport2'
     *  UnitDelay: '<S3>/Unit Delay'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body': '<S30>:1' */
    /* '<S30>:1:3' rotation_matrix = att(q_min); */
    /* '<S30>:1:9' dcm = [q(1)^2 + q(2)^2 - q(3)^2 - q(4)^2,   2*(q(2)*q(3)+q(1)*q(4)),    2*(q(2)*q(4) - q(1)*q(3)); */
    /* '<S30>:1:10'         2*(q(2)*q(3) - q(1)*q(4)),          q(1)^2-q(2)^2+q(3)^2-q(4)^2,    2*(q(3)*q(4) + q(1)*q(2)); */
    /* '<S30>:1:11'         2*(q(2)*q(4)+q(1)*q(3)),            2*(q(3)*q(4)-q(1)*q(2)),        q(1)^2-q(2)^2-q(3)^2+q(4)^2]; */
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

    /* '<S30>:1:4' body_est = blkdiag(rotation_matrix,rotation_matrix)*meas_eci; */
    memset(&KHI[0], 0, 36U * sizeof(real_T));
    for (i = 0; i < 3; i++) {
      KHI[6 * i] = rotation_matrix[3 * i];
      KHI[3 + 6 * (3 + i)] = rotation_matrix[3 * i];
      KHI[1 + 6 * i] = rotation_matrix[3 * i + 1];
      KHI[4 + 6 * (3 + i)] = rotation_matrix[3 * i + 1];
      KHI[2 + 6 * i] = rotation_matrix[3 * i + 2];
      KHI[5 + 6 * (3 + i)] = rotation_matrix[3 * i + 2];

      /* SignalConversion: '<S30>/TmpSignal ConversionAt SFunction Inport2' incorporates:
       *  Inport: '<Root>/mag_eci_unit'
       *  Inport: '<Root>/sc2sun_unit'
       */
      rtb_DataTypeConversion1_1[i] = rtU.mag_eci_unit[i];
      rtb_DataTypeConversion1_1[i + 3] = rtU.sc2sun_unit[i];
    }

    for (i = 0; i < 6; i++) {
      rtb_body_est[i] = 0.0;
      for (k = 0; k < 6; k++) {
        rtb_body_est[i] += KHI[6 * k + i] * rtb_DataTypeConversion1_1[k];
      }
    }

    /* End of MATLAB Function: '<S28>/convert_inertial_body' */

    /* MATLAB Function: '<S28>/observation_matrix' */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/observation_matrix': '<S33>:1' */
    /* '<S33>:1:3' H = [  skew(body_est(1:3))     zeros(3); */
    /* '<S33>:1:4'        skew(body_est(4:6))     zeros(3)]; */
    /* '<S33>:1:9' S = [0  -v(3)   v(2); */
    /* '<S33>:1:10'     v(3)    0   -v(1); */
    /* '<S33>:1:11'     -v(2)   v(1)    0]; */
    /* '<S33>:1:9' S = [0  -v(3)   v(2); */
    /* '<S33>:1:10'     v(3)    0   -v(1); */
    /* '<S33>:1:11'     -v(2)   v(1)    0]; */
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

    /* End of MATLAB Function: '<S28>/observation_matrix' */

    /* MATLAB Function: '<S28>/kalman_gain' incorporates:
     *  Constant: '<S3>/Constant1'
     *  UnitDelay: '<S3>/Unit Delay2'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/kalman_gain': '<S32>:1' */
    /* '<S32>:1:3' K = cov_min*H'/(H*cov_min*H' + meas_cov); */
    for (i = 0; i < 6; i++) {
      for (k = 0; k < 6; k++) {
        rtb_K[i + 6 * k] = 0.0;
        rtb_Merge2_1[i + 6 * k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          rtb_K[i + 6 * k] += rtDW.UnitDelay2_DSTATE[6 * i_0 + i] * rtb_Merge2[6
            * i_0 + k];
          rtb_Merge2_1[i + 6 * k] += rtb_Merge2[6 * i_0 + i] *
            rtDW.UnitDelay2_DSTATE[6 * k + i_0];
        }
      }

      for (k = 0; k < 6; k++) {
        scale = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          scale += rtb_Merge2_1[6 * i_0 + i] * rtb_Merge2[6 * i_0 + k];
        }

        rtb_Merge2_0[i + 6 * k] = rtConstP.Constant1_Value[6 * k + i] + scale;
      }
    }

    mrdivide(rtb_K, rtb_Merge2_0);

    /*  zero off part of gain if gyro reading is invalid */
    /* '<S32>:1:7' if valid_gyro == 0 */
    if (!rtb_Switch_ms) {
      /* '<S32>:1:8' K(4:6,:) = zeros(3,6); */
      for (i = 0; i < 6; i++) {
        rtb_K[3 + 6 * i] = 0.0;
        rtb_K[4 + 6 * i] = 0.0;
        rtb_K[5 + 6 * i] = 0.0;
      }
    }

    /* End of MATLAB Function: '<S28>/kalman_gain' */

    /* Sum: '<S28>/Sum' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion1'
     *  DataTypeConversion: '<S1>/Data Type Conversion2'
     *  Product: '<S28>/Matrix Multiply'
     */
    rtb_DataTypeConversion1_0[0] = rtU.mag_vec_body_T[0];
    rtb_DataTypeConversion1_0[3] = rtU.sun_vec_body_sunsensor[0];
    rtb_DataTypeConversion1_0[1] = rtU.mag_vec_body_T[1];
    rtb_DataTypeConversion1_0[4] = rtU.sun_vec_body_sunsensor[1];
    rtb_DataTypeConversion1_0[2] = rtU.mag_vec_body_T[2];
    rtb_DataTypeConversion1_0[5] = rtU.sun_vec_body_sunsensor[2];
    for (i = 0; i < 6; i++) {
      rtb_DataTypeConversion1_1[i] = rtb_DataTypeConversion1_0[i] -
        rtb_body_est[i];
    }

    /* End of Sum: '<S28>/Sum' */

    /* Product: '<S28>/Matrix Multiply' */
    for (i = 0; i < 6; i++) {
      rtb_MatrixMultiply[i] = 0.0;
      for (k = 0; k < 6; k++) {
        rtb_MatrixMultiply[i] += rtb_K[6 * k + i] * rtb_DataTypeConversion1_1[k];
      }
    }

    /* MATLAB Function: '<S28>/update_state ' incorporates:
     *  UnitDelay: '<S3>/Unit Delay'
     */
    /*  select out bias */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/update_state ': '<S34>:1' */
    /* '<S34>:1:5' bias_plu = bias_min + del_x_plu(4:6); */
    /* '<S34>:1:7' q_plu = quat_update(del_x_plu(1:3),q_min); */
    /*  this has a sqrt error if del_apk is too large, i.e. small angle */
    /*  approximation doesn't hold */
    /* q_hat_pk = quat_multiply([sqrt(1-norm(del_apk/2)); del_apk/2], q_hat_mk); */
    /*  the following two equations are equivalent; the first is probably a bit */
    /*  more computationally cheaper */
    /* '<S34>:1:22' q_hat_pk = quat_multiply([1; del_apk/2], q_hat_mk); */
    /* '<S34>:1:41' q2 = quatmultiply(q1',dq')'; */
    rtb_Product_am = rtb_MatrixMultiply[0] / 2.0;
    rtb_DataTypeConversion1_idx_0 = rtb_MatrixMultiply[1] / 2.0;
    rtb_DataTypeConversion1_idx_1 = rtb_MatrixMultiply[2] / 2.0;
    rtb_Sum_j = ((rtDW.UnitDelay_DSTATE[0] - rtDW.UnitDelay_DSTATE[1] *
                  rtb_Product_am) - rtDW.UnitDelay_DSTATE[2] *
                 rtb_DataTypeConversion1_idx_0) - rtDW.UnitDelay_DSTATE[3] *
      rtb_DataTypeConversion1_idx_1;
    rtb_Product1_h = (rtDW.UnitDelay_DSTATE[0] * rtb_Product_am +
                      rtDW.UnitDelay_DSTATE[1]) + (rtDW.UnitDelay_DSTATE[2] *
      rtb_DataTypeConversion1_idx_1 - rtDW.UnitDelay_DSTATE[3] *
      rtb_DataTypeConversion1_idx_0);
    rtb_Product2_d = (rtDW.UnitDelay_DSTATE[0] * rtb_DataTypeConversion1_idx_0 +
                      rtDW.UnitDelay_DSTATE[2]) + (rtDW.UnitDelay_DSTATE[3] *
      rtb_Product_am - rtDW.UnitDelay_DSTATE[1] * rtb_DataTypeConversion1_idx_1);
    w_n = (rtDW.UnitDelay_DSTATE[0] * rtb_DataTypeConversion1_idx_1 +
           rtDW.UnitDelay_DSTATE[3]) + (rtDW.UnitDelay_DSTATE[1] *
      rtb_DataTypeConversion1_idx_0 - rtDW.UnitDelay_DSTATE[2] * rtb_Product_am);

    /* q_hat_pk = q_hat_mk + 0.5*xi_matrix(q_hat_mk)*del_apk; */
    /* '<S34>:1:8' q_plu = q_plu/norm(q_plu); */
    scale = 2.2250738585072014E-308;
    absxk = fabs(rtb_Sum_j);
    if (absxk > 2.2250738585072014E-308) {
      rtb_Product_am = 1.0;
      scale = absxk;
    } else {
      t = absxk / 2.2250738585072014E-308;
      rtb_Product_am = t * t;
    }

    absxk = fabs(rtb_Product1_h);
    if (absxk > scale) {
      t = scale / absxk;
      rtb_Product_am = rtb_Product_am * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      rtb_Product_am += t * t;
    }

    absxk = fabs(rtb_Product2_d);
    if (absxk > scale) {
      t = scale / absxk;
      rtb_Product_am = rtb_Product_am * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      rtb_Product_am += t * t;
    }

    absxk = fabs(w_n);
    if (absxk > scale) {
      t = scale / absxk;
      rtb_Product_am = rtb_Product_am * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      rtb_Product_am += t * t;
    }

    rtb_Product_am = scale * sqrt(rtb_Product_am);

    /* SignalConversion: '<S28>/OutportBufferForbias_plus' incorporates:
     *  MATLAB Function: '<S28>/update_state '
     *  UnitDelay: '<S3>/Unit Delay1'
     */
    rtb_DataTypeConversion1_idx_0 = rtDW.UnitDelay1_DSTATE[0] +
      rtb_MatrixMultiply[3];
    rtb_DataTypeConversion1_idx_1 = rtDW.UnitDelay1_DSTATE[1] +
      rtb_MatrixMultiply[4];
    rtb_DataTypeConversion1_idx_2 = rtDW.UnitDelay1_DSTATE[2] +
      rtb_MatrixMultiply[5];

    /* MATLAB Function: '<S28>/covariance_update' incorporates:
     *  Constant: '<S3>/Constant1'
     *  UnitDelay: '<S3>/Unit Delay2'
     */
    /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/covariance_update': '<S31>:1' */
    /* '<S31>:1:4' KHI = (K*H - eye(6)); */
    for (i = 0; i < 36; i++) {
      I_0[i] = 0;
    }

    for (k = 0; k < 6; k++) {
      I_0[k + 6 * k] = 1;
    }

    /* '<S31>:1:5' cov_plus = KHI*cov_min*KHI' + K*meas_cov*K'; */
    for (i = 0; i < 6; i++) {
      for (k = 0; k < 6; k++) {
        scale = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          scale += rtb_K[6 * i_0 + i] * rtb_Merge2[6 * k + i_0];
        }

        KHI[i + 6 * k] = scale - (real_T)I_0[6 * k + i];
      }

      for (k = 0; k < 6; k++) {
        rtb_Merge2_0[i + 6 * k] = 0.0;
        rtb_Merge2_1[i + 6 * k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          rtb_Merge2_0[i + 6 * k] += KHI[6 * i_0 + i] * rtDW.UnitDelay2_DSTATE[6
            * k + i_0];
          rtb_Merge2_1[i + 6 * k] += rtb_K[6 * i_0 + i] *
            rtConstP.Constant1_Value[6 * k + i_0];
        }
      }
    }

    for (i = 0; i < 6; i++) {
      for (k = 0; k < 6; k++) {
        KHI_0[i + 6 * k] = 0.0;
        rtb_K_0[i + 6 * k] = 0.0;
        for (i_0 = 0; i_0 < 6; i_0++) {
          KHI_0[i + 6 * k] += rtb_Merge2_0[6 * i_0 + i] * KHI[6 * i_0 + k];
          rtb_K_0[i + 6 * k] += rtb_Merge2_1[6 * i_0 + i] * rtb_K[6 * i_0 + k];
        }
      }
    }

    /* SignalConversion: '<S28>/OutportBufferForcov_plus' incorporates:
     *  MATLAB Function: '<S28>/covariance_update'
     */
    for (i = 0; i < 6; i++) {
      for (k = 0; k < 6; k++) {
        rtb_Merge2[k + 6 * i] = KHI_0[6 * i + k] + rtb_K_0[6 * i + k];
      }
    }

    /* End of SignalConversion: '<S28>/OutportBufferForcov_plus' */

    /* SignalConversion: '<S28>/OutportBufferForq_plus' incorporates:
     *  MATLAB Function: '<S28>/update_state '
     */
    rtb_q_cmd[0] = rtb_Sum_j / rtb_Product_am;
    rtb_q_cmd[1] = rtb_Product1_h / rtb_Product_am;
    rtb_q_cmd[2] = rtb_Product2_d / rtb_Product_am;
    rtb_q_cmd[3] = w_n / rtb_Product_am;

    /* End of Outputs for SubSystem: '<S22>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S22>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S29>/Action Port'
     */
    rtb_q_cmd[0] = rtDW.UnitDelay_DSTATE[0];
    rtb_q_cmd[1] = rtDW.UnitDelay_DSTATE[1];
    rtb_q_cmd[2] = rtDW.UnitDelay_DSTATE[2];
    rtb_q_cmd[3] = rtDW.UnitDelay_DSTATE[3];
    rtb_DataTypeConversion1_idx_0 = rtDW.UnitDelay1_DSTATE[0];
    rtb_DataTypeConversion1_idx_1 = rtDW.UnitDelay1_DSTATE[1];
    rtb_DataTypeConversion1_idx_2 = rtDW.UnitDelay1_DSTATE[2];
    memcpy(&rtb_Merge2[0], &rtDW.UnitDelay2_DSTATE[0], 36U * sizeof(real_T));

    /* End of Outputs for SubSystem: '<S22>/If Action Subsystem1' */
  }

  /* End of If: '<S22>/If' */

  /* Switch: '<S21>/Switch' incorporates:
   *  Inport: '<Root>/gyro_omega_body_radps'
   *  Sum: '<S21>/Sum'
   */
  if (rtb_Switch_ms) {
    rtb_Switch[0] = rtU.gyro_omega_body_radps[0] - rtb_DataTypeConversion1_idx_0;
    rtb_Switch[1] = rtU.gyro_omega_body_radps[1] - rtb_DataTypeConversion1_idx_1;
    rtb_Switch[2] = rtU.gyro_omega_body_radps[2] - rtb_DataTypeConversion1_idx_2;
  } else {
    rtb_Switch[0] = 0.0;
    rtb_Switch[1] = 0.0;
    rtb_Switch[2] = 0.0;
  }

  /* End of Switch: '<S21>/Switch' */

  /* MATLAB Function: '<S21>/propagate_quat' incorporates:
   *  Constant: '<S21>/Constant1'
   */
  /* MATLAB Function 'Estimation_EKF /Propagate Step /propagate_quat': '<S24>:1' */
  /* '<S24>:1:4' q_min_k1 = omega_matrix(w_plu,dt)*q_plu; */
  /* '<S24>:1:10' w_n = norm(w); */
  scale = 2.2250738585072014E-308;
  absxk = fabs(rtb_Switch[0]);
  if (absxk > 2.2250738585072014E-308) {
    w_n = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    w_n = t * t;
  }

  absxk = fabs(rtb_Switch[1]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  absxk = fabs(rtb_Switch[2]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  w_n = scale * sqrt(w_n);

  /* '<S24>:1:12' if w_n < 1e-14 */
  if (w_n < 1.0E-14) {
    /* '<S24>:1:13' Psi = 0*w; */
    Psi[0] = 0.0 * rtb_Switch[0];
    Psi[1] = 0.0 * rtb_Switch[1];
    Psi[2] = 0.0 * rtb_Switch[2];
  } else {
    /* '<S24>:1:14' else */
    /* '<S24>:1:15' Psi = sin(1/2*w_n*dt)/w_n*w; */
    rtb_Product_am = sin(0.5 * w_n * 0.1) / w_n;
    Psi[0] = rtb_Product_am * rtb_Switch[0];
    Psi[1] = rtb_Product_am * rtb_Switch[1];
    Psi[2] = rtb_Product_am * rtb_Switch[2];
  }

  /* '<S24>:1:18' Psi_skew = skew(Psi); */
  /* '<S24>:1:31' S = [0  -v(3)   v(2); */
  /* '<S24>:1:32'     v(3)    0   -v(1); */
  /* '<S24>:1:33'     -v(2)   v(1)    0]; */
  /* '<S24>:1:20' omega = [cos(1/2*w_n*dt)*eye(3) - Psi_skew      Psi; */
  /* '<S24>:1:21'         -Psi'                                   cos(1/2*w_n*dt)]; */
  absxk = cos(0.5 * w_n * 0.1);

  /* '<S24>:1:23' T = [zeros(3,1) eye(3); */
  /* '<S24>:1:24'     1       zeros(1,3)]; */
  /* '<S24>:1:26' omega = T'*omega*T; */
  rotation_matrix_0[0] = 0.0;
  rotation_matrix_0[3] = -Psi[2];
  rotation_matrix_0[6] = Psi[1];
  rotation_matrix_0[1] = Psi[2];
  rotation_matrix_0[4] = 0.0;
  rotation_matrix_0[7] = -Psi[0];
  rotation_matrix_0[2] = -Psi[1];
  rotation_matrix_0[5] = Psi[0];
  rotation_matrix_0[8] = 0.0;
  for (i = 0; i < 3; i++) {
    absxk_0[i << 2] = (real_T)b_b[3 * i] * absxk - rotation_matrix_0[3 * i];
    absxk_0[1 + (i << 2)] = (real_T)b_b[3 * i + 1] * absxk - rotation_matrix_0[3
      * i + 1];
    absxk_0[2 + (i << 2)] = (real_T)b_b[3 * i + 2] * absxk - rotation_matrix_0[3
      * i + 2];
    absxk_0[12 + i] = Psi[i];
    absxk_0[3 + (i << 2)] = -Psi[i];
  }

  absxk_0[15] = cos(0.5 * w_n * 0.1);
  for (i = 0; i < 4; i++) {
    for (k = 0; k < 4; k++) {
      a_1[i + (k << 2)] = 0.0;
      a_1[i + (k << 2)] += absxk_0[k << 2] * (real_T)a[i];
      a_1[i + (k << 2)] += absxk_0[(k << 2) + 1] * (real_T)a[i + 4];
      a_1[i + (k << 2)] += absxk_0[(k << 2) + 2] * (real_T)a[i + 8];
      a_1[i + (k << 2)] += absxk_0[(k << 2) + 3] * (real_T)a[i + 12];
    }

    rtb_q_min_k1[i] = 0.0;
    for (k = 0; k < 4; k++) {
      a_2[i + (k << 2)] = 0.0;
      a_2[i + (k << 2)] += (real_T)b_0[k << 2] * a_1[i];
      a_2[i + (k << 2)] += (real_T)b_0[(k << 2) + 1] * a_1[i + 4];
      a_2[i + (k << 2)] += (real_T)b_0[(k << 2) + 2] * a_1[i + 8];
      a_2[i + (k << 2)] += (real_T)b_0[(k << 2) + 3] * a_1[i + 12];
      rtb_q_min_k1[i] += a_2[(k << 2) + i] * rtb_q_cmd[k];
    }
  }

  /* End of MATLAB Function: '<S21>/propagate_quat' */

  /* Sqrt: '<S26>/sqrt' incorporates:
   *  Product: '<S27>/Product'
   *  Product: '<S27>/Product1'
   *  Product: '<S27>/Product2'
   *  Product: '<S27>/Product3'
   *  Sum: '<S27>/Sum'
   */
  rtb_Sum_j = sqrt(((rtb_q_min_k1[0] * rtb_q_min_k1[0] + rtb_q_min_k1[1] *
                     rtb_q_min_k1[1]) + rtb_q_min_k1[2] * rtb_q_min_k1[2]) +
                   rtb_q_min_k1[3] * rtb_q_min_k1[3]);

  /* Product: '<S23>/Product' */
  rtb_Product_am = rtb_q_min_k1[0] / rtb_Sum_j;

  /* Product: '<S23>/Product1' */
  rtb_Product1_h = rtb_q_min_k1[1] / rtb_Sum_j;

  /* Product: '<S23>/Product2' */
  rtb_Product2_d = rtb_q_min_k1[2] / rtb_Sum_j;

  /* Product: '<S23>/Product3' */
  rtb_Sum_j = rtb_q_min_k1[3] / rtb_Sum_j;

  /* Outport: '<Root>/sc_quat' */
  rtY.sc_quat[0] = rtb_Product_am;
  rtY.sc_quat[1] = rtb_Product1_h;
  rtY.sc_quat[2] = rtb_Product2_d;
  rtY.sc_quat[3] = rtb_Sum_j;

  /* Sum: '<S16>/Sum' incorporates:
   *  Product: '<S16>/Product'
   *  Product: '<S16>/Product1'
   *  Product: '<S16>/Product2'
   *  Product: '<S16>/Product3'
   *  UnaryMinus: '<S14>/Unary Minus'
   *  UnaryMinus: '<S14>/Unary Minus1'
   *  UnaryMinus: '<S14>/Unary Minus2'
   */
  scale = ((rtb_Product_am * rtb_DataTypeConversion_idx_0 - -rtb_Product1_h *
            rtb_DataTypeConversion_idx_1) - -rtb_Product2_d *
           rtb_DataTypeConversion_idx_2) - -rtb_Sum_j *
    rtb_DataTypeConversion_idx_3;

  /* Saturate: '<S13>/Saturation' */
  if (scale > 1.0) {
    scale = 1.0;
  } else {
    if (scale < -1.0) {
      scale = -1.0;
    }
  }

  /* End of Saturate: '<S13>/Saturation' */

  /* Outport: '<Root>/point_true' incorporates:
   *  Gain: '<S13>/Gain'
   *  Gain: '<S13>/rad2deg'
   *  Switch: '<S13>/Switch'
   *  Trigonometry: '<S13>/Trigonometric Function'
   */
  rtY.point_true = !(2.0 * acos(scale) * 57.295779513082323 >= 20.0);
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/body_rates' */
    rtY.body_rates[i] = rtb_Switch[i];

    /* Sum: '<S36>/Add' incorporates:
     *  Constant: '<S36>/MoI'
     *  Gain: '<S36>/rpm2rad'
     *  Product: '<S36>/Matrix Multiply'
     */
    Psi[i] = (rtConstP.MoI_Value[i + 3] * 0.0 + rtConstP.MoI_Value[i] * 0.0) +
      rtConstP.MoI_Value[i + 6] * 0.0;
  }

  /* Product: '<S39>/Element product' */
  rtb_body_est[2] = rtb_Normalization[0] * Psi[1];
  rtb_body_est[4] = rtb_Normalization[0] * Psi[2];
  rtb_body_est[5] = rtb_Normalization[1] * Psi[0];

  /* DiscretePulseGenerator: '<S36>/MT_on' */
  w_n = ((rtDW.clockTickCounter < 6.9999999999999991) && (rtDW.clockTickCounter >=
          0L));
  if (rtDW.clockTickCounter >= 9L) {
    rtDW.clockTickCounter = 0L;
  } else {
    rtDW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<S36>/MT_on' */

  /* Saturate: '<S36>/Sat2' incorporates:
   *  Gain: '<S36>/-gain'
   *  Product: '<S36>/Product'
   *  Product: '<S39>/Element product'
   *  Sum: '<S39>/Add3'
   */
  scale = (rtb_Normalization[1] * Psi[2] - rtb_Normalization[2] * Psi[1]) * 10.0
    * w_n;
  if (scale > 0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[0] = 0.15;
  } else if (scale < -0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[0] = -0.15;
  } else {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[0] = scale;
  }

  /* End of Saturate: '<S36>/Sat2' */

  /* Saturate: '<S36>/Sat' incorporates:
   *  Gain: '<S36>/-gain'
   *  Product: '<S36>/Product'
   *  Product: '<S39>/Element product'
   *  Sum: '<S39>/Add3'
   */
  scale = (rtb_Normalization[2] * Psi[0] - rtb_body_est[4]) * 10.0 * w_n;
  if (scale > 0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[1] = 0.15;
  } else if (scale < -0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[1] = -0.15;
  } else {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[1] = scale;
  }

  /* End of Saturate: '<S36>/Sat' */

  /* Saturate: '<S36>/Sat1' incorporates:
   *  Gain: '<S36>/-gain'
   *  Product: '<S36>/Product'
   *  Sum: '<S39>/Add3'
   */
  scale = (rtb_body_est[2] - rtb_body_est[5]) * 10.0 * w_n;
  if (scale > 0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[2] = 0.15;
  } else if (scale < -0.15) {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[2] = -0.15;
  } else {
    /* SignalConversion: '<S38>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  MATLAB Function: '<S35>/control_selection'
     */
    rtb_Normalization[2] = scale;
  }

  /* End of Saturate: '<S36>/Sat1' */

  /* MATLAB Function: '<S35>/control_selection' incorporates:
   *  UnitDelay: '<S7>/Unit Delay'
   */
  /*  Control Selection Logic */
  /*    HuskySat-1, ADCS Team */
  /*    Last Update: T. Reynolds, 11.21.17 */
  /* MATLAB Function 'command_generation/Control Selection/control_selection': '<S38>:1' */
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
  /* '<S38>:1:20' FLAG    = 1; */
  /* '<S38>:1:22' if mode == 1 */
  switch (rtDW.UnitDelay_DSTATE_ao) {
   case 1:
    /* '<S38>:1:23' cmd_MT_out      = zeros(3,1); */
    rtb_Normalization[0] = 0.0;
    rtb_Normalization[1] = 0.0;
    rtb_Normalization[2] = 0.0;
    break;

   case 2:
    /* '<S38>:1:24' elseif mode == 2 */
    /* '<S38>:1:25' cmd_MT_out      = cmd_MT; */
    break;

   default:
    if (!((rtDW.UnitDelay_DSTATE_ao == 31) || (rtDW.UnitDelay_DSTATE_ao == 32) ||
          (rtDW.UnitDelay_DSTATE_ao == 33))) {
      if (rtDW.UnitDelay_DSTATE_ao == 4) {
        /* '<S38>:1:28' elseif mode == 4 */
        /* '<S38>:1:29' cmd_MT_out      = zeros(3,1); */
        rtb_Normalization[0] = 0.0;
        rtb_Normalization[1] = 0.0;
        rtb_Normalization[2] = 0.0;
      } else {
        /* '<S38>:1:30' else */
        /* '<S38>:1:31' FLAG = -1; */
        /* '<S38>:1:32' cmd_MT_out      = zeros(3,1); */
        rtb_Normalization[0] = 0.0;
        rtb_Normalization[1] = 0.0;
        rtb_Normalization[2] = 0.0;
      }
    } else {
      /* '<S38>:1:26' elseif( mode == 31 || mode == 32 || mode == 33 ) */
      /* '<S38>:1:27' cmd_MT_out      = cmd_MT; */
    }
    break;
  }

  /* Gain: '<S40>/To DigVal1' */
  scale = 846.66666666666663 * rtb_Normalization[0];

  /* DataTypeConversion: '<S40>/Data Type Conversion' */
  absxk = fabs(scale);
  if (absxk < 4.503599627370496E+15) {
    if (absxk >= 0.5) {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[0] = (int8_T)floor(scale + 0.5);
    } else {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[0] = (int8_T)(scale * 0.0);
    }
  } else {
    /* Outport: '<Root>/cmd_MT_fsw_dv' */
    rtY.cmd_MT_fsw_dv[0] = (int8_T)scale;
  }

  /* End of DataTypeConversion: '<S40>/Data Type Conversion' */

  /* Gain: '<S40>/To DigVal2' */
  scale = 846.66666666666663 * rtb_Normalization[1];

  /* DataTypeConversion: '<S40>/Data Type Conversion1' */
  absxk = fabs(scale);
  if (absxk < 4.503599627370496E+15) {
    if (absxk >= 0.5) {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[1] = (int8_T)floor(scale + 0.5);
    } else {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[1] = (int8_T)(scale * 0.0);
    }
  } else {
    /* Outport: '<Root>/cmd_MT_fsw_dv' */
    rtY.cmd_MT_fsw_dv[1] = (int8_T)scale;
  }

  /* End of DataTypeConversion: '<S40>/Data Type Conversion1' */

  /* Gain: '<S40>/To DigVal3' */
  scale = 846.66666666666663 * rtb_Normalization[2];

  /* DataTypeConversion: '<S40>/Data Type Conversion2' */
  absxk = fabs(scale);
  if (absxk < 4.503599627370496E+15) {
    if (absxk >= 0.5) {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[2] = (int8_T)floor(scale + 0.5);
    } else {
      /* Outport: '<Root>/cmd_MT_fsw_dv' */
      rtY.cmd_MT_fsw_dv[2] = (int8_T)(scale * 0.0);
    }
  } else {
    /* Outport: '<Root>/cmd_MT_fsw_dv' */
    rtY.cmd_MT_fsw_dv[2] = (int8_T)scale;
  }

  /* End of DataTypeConversion: '<S40>/Data Type Conversion2' */

  /* MATLAB Function: '<S21>/state_transition' incorporates:
   *  Constant: '<S21>/Constant'
   */
  /* MATLAB Function 'Estimation_EKF /Propagate Step /state_transition': '<S25>:1' */
  /* '<S25>:1:3' Phi = state_transition_matrix(w_plu,dt); */
  /* '<S25>:1:10' w_n = norm(w); */
  scale = 2.2250738585072014E-308;
  absxk = fabs(rtb_Switch[0]);
  if (absxk > 2.2250738585072014E-308) {
    w_n = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    w_n = t * t;
  }

  absxk = fabs(rtb_Switch[1]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  absxk = fabs(rtb_Switch[2]);
  if (absxk > scale) {
    t = scale / absxk;
    w_n = w_n * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    w_n += t * t;
  }

  w_n = scale * sqrt(w_n);

  /* '<S25>:1:11' w_skew = skew(w); */
  /* '<S25>:1:31' S = [0  -v(3)   v(2); */
  /* '<S25>:1:32'     v(3)    0   -v(1); */
  /* '<S25>:1:33'     -v(2)   v(1)    0]; */
  rotation_matrix[0] = 0.0;
  rotation_matrix[3] = -rtb_Switch[2];
  rotation_matrix[6] = rtb_Switch[1];
  rotation_matrix[1] = rtb_Switch[2];
  rotation_matrix[4] = 0.0;
  rotation_matrix[7] = -rtb_Switch[0];
  rotation_matrix[2] = -rtb_Switch[1];
  rotation_matrix[5] = rtb_Switch[0];
  rotation_matrix[8] = 0.0;

  /* '<S25>:1:13' if w_n < 1e-14 */
  if (w_n < 1.0E-14) {
    /* '<S25>:1:14' phi = [ eye(3) -eye(3)*dt; */
    /* '<S25>:1:15'             zeros(3,3) eye(3)]; */
    memset(&rotation_matrix[0], 0, 9U * sizeof(real_T));
    for (i = 0; i < 9; i++) {
      I[i] = 0;
    }

    I[0] = 1;
    I[4] = 1;
    I[8] = 1;
    for (k = 0; k < 3; k++) {
      rotation_matrix[k + 3 * k] = 1.0;
      rtb_K[6 * k] = I[3 * k];
      rtb_K[6 * (k + 3)] = (real_T)a_0[3 * k] * 0.1;
      rtb_K[3 + 6 * k] = 0.0;
      rtb_K[1 + 6 * k] = I[3 * k + 1];
      rtb_K[1 + 6 * (k + 3)] = (real_T)a_0[3 * k + 1] * 0.1;
      rtb_K[4 + 6 * k] = 0.0;
      rtb_K[2 + 6 * k] = I[3 * k + 2];
      rtb_K[2 + 6 * (k + 3)] = (real_T)a_0[3 * k + 2] * 0.1;
      rtb_K[5 + 6 * k] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      rtb_K[3 + 6 * (i + 3)] = rotation_matrix[3 * i];
      rtb_K[4 + 6 * (i + 3)] = rotation_matrix[3 * i + 1];
      rtb_K[5 + 6 * (i + 3)] = rotation_matrix[3 * i + 2];
    }
  } else {
    /* '<S25>:1:16' else */
    /* '<S25>:1:17' Phi_11 = eye(3) - w_skew*sin(w_n*dt)/w_n + w_skew^2*(1 - cos(w_n*dt))/w_n^2; */
    for (i = 0; i < 9; i++) {
      I[i] = 0;
      Phi_22[i] = 0;
    }

    I[0] = 1;
    I[4] = 1;
    I[8] = 1;
    absxk = sin(w_n * 0.1);
    t = cos(w_n * 0.1);
    scale = w_n * w_n;

    /* '<S25>:1:19' Phi_12 = w_skew*(1 - cos(w_n*dt))/w_n^2 - eye(3)*dt - w_skew^2*(w_n*dt - sin(w_n*dt))/w_n^3; */
    c_x = cos(w_n * 0.1);
    b_c = w_n * w_n;
    b_1 = w_n * 0.1 - sin(w_n * 0.1);
    c_c = rt_powd_snf(w_n, 3.0);

    /* '<S25>:1:21' Phi_21 = zeros(3,3); */
    /* '<S25>:1:23' Phi_22 = eye(3); */
    /* '<S25>:1:25' phi = [ Phi_11 Phi_12; */
    /* '<S25>:1:26'             Phi_21 Phi_22]; */
    for (k = 0; k < 3; k++) {
      Phi_22[k + 3 * k] = 1;
      for (i = 0; i < 3; i++) {
        rotation_matrix_0[k + 3 * i] = 0.0;
        rotation_matrix_1[k + 3 * i] = 0.0;
        rotation_matrix_0[k + 3 * i] += rotation_matrix[3 * i] *
          rotation_matrix[k];
        rotation_matrix_1[k + 3 * i] += rotation_matrix[3 * i] *
          rotation_matrix[k];
        rotation_matrix_0[k + 3 * i] += rotation_matrix[3 * i + 1] *
          rotation_matrix[k + 3];
        rotation_matrix_1[k + 3 * i] += rotation_matrix[3 * i + 1] *
          rotation_matrix[k + 3];
        rotation_matrix_0[k + 3 * i] += rotation_matrix[3 * i + 2] *
          rotation_matrix[k + 6];
        rotation_matrix_1[k + 3 * i] += rotation_matrix[3 * i + 2] *
          rotation_matrix[k + 6];
      }
    }

    for (i = 0; i < 3; i++) {
      rtb_K[6 * i] = ((real_T)I[3 * i] - rotation_matrix[3 * i] * absxk / w_n) +
        rotation_matrix_0[3 * i] * (1.0 - t) / scale;
      rtb_K[6 * (i + 3)] = (rotation_matrix[3 * i] * (1.0 - c_x) / b_c - (real_T)
                            b_a[3 * i] * 0.1) - rotation_matrix_1[3 * i] * b_1 /
        c_c;
      rtb_K[3 + 6 * i] = 0.0;
      rtb_K[3 + 6 * (i + 3)] = Phi_22[3 * i];
      rtb_K[1 + 6 * i] = ((real_T)I[3 * i + 1] - rotation_matrix[3 * i + 1] *
                          absxk / w_n) + rotation_matrix_0[3 * i + 1] * (1.0 - t)
        / scale;
      rtb_K[1 + 6 * (i + 3)] = (rotation_matrix[3 * i + 1] * (1.0 - c_x) / b_c -
        (real_T)b_a[3 * i + 1] * 0.1) - rotation_matrix_1[3 * i + 1] * b_1 / c_c;
      rtb_K[4 + 6 * i] = 0.0;
      rtb_K[4 + 6 * (i + 3)] = Phi_22[3 * i + 1];
      rtb_K[2 + 6 * i] = ((real_T)I[3 * i + 2] - rotation_matrix[3 * i + 2] *
                          absxk / w_n) + rotation_matrix_0[3 * i + 2] * (1.0 - t)
        / scale;
      rtb_K[2 + 6 * (i + 3)] = (rotation_matrix[3 * i + 2] * (1.0 - c_x) / b_c -
        (real_T)b_a[3 * i + 2] * 0.1) - rotation_matrix_1[3 * i + 2] * b_1 / c_c;
      rtb_K[5 + 6 * i] = 0.0;
      rtb_K[5 + 6 * (i + 3)] = Phi_22[3 * i + 2];
    }
  }

  /* End of MATLAB Function: '<S21>/state_transition' */

  /* Abs: '<S7>/Abs' */
  /* MATLAB Function 'Estimation_EKF /3_sig_bnd ': '<S20>:1' */
  /* '<S20>:1:3' bnd = cov; */
  Psi[0] = fabs(rtb_Switch[0]);
  Psi[1] = fabs(rtb_Switch[1]);
  Psi[2] = fabs(rtb_Switch[2]);

  /* MinMax: '<S7>/MinMax' */
  if ((Psi[0] >= Psi[1L]) || rtIsNaN(Psi[1L])) {
    w_n = Psi[0];
  } else {
    w_n = Psi[1L];
  }

  if (!((w_n >= Psi[2L]) || rtIsNaN(Psi[2L]))) {
    w_n = Psi[2L];
  }

  /* Relay: '<S7>/Relay' incorporates:
   *  MinMax: '<S7>/MinMax'
   */
  if (w_n >= 0.12) {
    rtDW.Relay_Mode = true;
  } else {
    if (w_n <= 0.00436) {
      rtDW.Relay_Mode = false;
    }
  }

  /* MATLAB Function: '<S7>/MATLAB Function' incorporates:
   *  Inport: '<Root>/CAN_IN'
   *  Inport: '<Root>/sc_above_gs'
   *  Inport: '<Root>/sc_in_sun'
   *  Relay: '<S7>/Relay'
   *  UnitDelay: '<S7>/Unit Delay'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  Main FSW Mode Selector */
  /*    UW HuskySat-1, ADCS Team */
  /*    Last Updated: T. Reynolds 2.15.18 */
  /* MATLAB Function 'mode_selecction/MATLAB Function': '<S41>:1' */
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
  /* '<S41>:1:55' sc_mode     = sc_mode_last; */
  rtb_sc_mode = rtDW.UnitDelay_DSTATE_ao;

  /* '<S41>:1:57' if( CAN(1) == 0 ) */
  if (rtU.CAN_IN[0] == 0) {
    /*  Check for bdot condition */
    /* '<S41>:1:60' if( bdot_flag == 1 ) */
    if (rtDW.Relay_Mode) {
      /* '<S41>:1:61' sc_mode     = int8(1.0); */
      rtb_sc_mode = 1;

      /*  If Bdot is triggered, return immediately since control has high  */
      /*  priority and control will be handed off. */
    } else {
      /*  Bdot flag will go low once omega_radps drops below the lower */
      /*  threshold. Accounted for in the relay block. */
      /*  Check for low power condition or recent boot up */
      /* '<S41>:1:71' if( CAN(2) == 1 || (sc_mode == 1 && bdot_flag == 0) ) */
      if ((rtU.CAN_IN[1] == 1) || (rtDW.UnitDelay_DSTATE_ao == 1)) {
        /*  If reboot/bdot just occured and we are not tumbling, enter low */
        /*  power/estimate mode. */
        /* '<S41>:1:74' sc_mode     = int8(4.0); */
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
      /* '<S41>:1:86' if( CAN(3) == 1 ) */
      if (rtU.CAN_IN[2] == 1) {
        /*  Default to velocity pointing mode */
        /* '<S41>:1:88' sc_mode     = int8(31.0); */
        rtb_sc_mode = 31;

        /*  Check to see if entering GS range, not in eclipse and */
        /*  COM2 system will be activated. */
        /* '<S41>:1:92' if( GS_approach && sc_in_sun && (CAN(4)==1) ) */
        if (rtU.sc_above_gs && rtU.sc_in_sun && (rtU.CAN_IN[3] == 1)) {
          /* '<S41>:1:93' sc_mode  = int8(32.0); */
          rtb_sc_mode = 32;
        }
      }

      /*  If signal received to override the autonomous mode selection, set mode  */
      /*  based on CAN(1) value. */
    }
  } else {
    if (rtU.CAN_IN[0] != 0) {
      /* '<S41>:1:100' elseif( CAN(1) ~= 0 ) */
      /* '<S41>:1:102' sc_mode = CAN(1); */
      rtb_sc_mode = rtU.CAN_IN[0];
    }
  }

  /* End of MATLAB Function: '<S7>/MATLAB Function' */

  /* Update for UnitDelay: '<S3>/Unit Delay' */
  rtDW.UnitDelay_DSTATE[0] = rtb_Product_am;
  rtDW.UnitDelay_DSTATE[1] = rtb_Product1_h;
  rtDW.UnitDelay_DSTATE[2] = rtb_Product2_d;
  rtDW.UnitDelay_DSTATE[3] = rtb_Sum_j;

  /* Update for UnitDelay: '<S3>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE[0] = rtb_DataTypeConversion1_idx_0;
  rtDW.UnitDelay1_DSTATE[1] = rtb_DataTypeConversion1_idx_1;
  rtDW.UnitDelay1_DSTATE[2] = rtb_DataTypeConversion1_idx_2;
  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      /* Product: '<S21>/Product' incorporates:
       *  Math: '<S21>/Math Function'
       */
      rtb_Merge2_0[i + 6 * k] = 0.0;

      /* Product: '<S21>/Product1' incorporates:
       *  Constant: '<S21>/Constant3'
       *  Constant: '<S21>/G'
       *  Math: '<S21>/Math Function1'
       */
      KHI[i + 6 * k] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        /* Product: '<S21>/Product' incorporates:
         *  Math: '<S21>/Math Function'
         */
        rtb_Merge2_0[i + 6 * k] += rtb_Merge2[6 * i_0 + i] * rtb_K[6 * i_0 + k];

        /* Product: '<S21>/Product1' incorporates:
         *  Constant: '<S21>/Constant3'
         *  Constant: '<S21>/G'
         *  Math: '<S21>/Math Function1'
         */
        KHI[i + 6 * k] += rtConstP.Constant3_Value[6 * i_0 + i] *
          rtConstP.G_Value[6 * i_0 + k];
      }
    }
  }

  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      /* Product: '<S21>/Product' incorporates:
       *  Sum: '<S21>/Add'
       */
      rtb_Merge2_1[k + 6 * i] = 0.0;

      /* Product: '<S21>/Product1' incorporates:
       *  Constant: '<S21>/G'
       *  Sum: '<S21>/Add'
       */
      rtb_Merge2[k + 6 * i] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        /* Product: '<S21>/Product' incorporates:
         *  Sum: '<S21>/Add'
         */
        rtb_Merge2_1[k + 6 * i] += rtb_K[6 * i_0 + k] * rtb_Merge2_0[6 * i + i_0];

        /* Product: '<S21>/Product1' incorporates:
         *  Constant: '<S21>/G'
         *  Sum: '<S21>/Add'
         */
        rtb_Merge2[k + 6 * i] += rtConstP.G_Value[6 * i_0 + k] * KHI[6 * i + i_0];
      }
    }
  }

  /* Update for UnitDelay: '<S3>/Unit Delay2' incorporates:
   *  Sum: '<S21>/Add'
   */
  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      rtDW.UnitDelay2_DSTATE[k + 6 * i] = rtb_Merge2_1[6 * i + k] + rtb_Merge2[6
        * i + k];
    }
  }

  /* End of Update for UnitDelay: '<S3>/Unit Delay2' */

  /* Update for UnitDelay: '<S7>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_ao = rtb_sc_mode;
}

/* Model initialize function */
void MSP_FSW_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

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
