/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: env_estimation_lib.c
 *
 * Code generated for Simulink model 'env_estimation_lib'.
 *
 * Model version                  : 1.329
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Apr  9 09:46:58 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "env_estimation_lib.h"
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
extern real_T rt_roundd_snf(real_T u);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);
extern real_T rt_remd_snf(real_T u0, real_T u1);
void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo);
int32_T mul_s32_sat(int32_T a, int32_T b);

/* Forward declaration for local functions */
static real_T norm(const real_T x[3]);
static real_T norm_f(const real_T x[3]);
extern real_T rtGetInf(void);
extern real32_T rtGetInfF(void);
extern real_T rtGetMinusInf(void);
extern real32_T rtGetMinusInfF(void);
extern real_T rtGetNaN(void);
extern real32_T rtGetNaNF(void);

/*===========*
 * Constants *
 *===========*/
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

/*
 * UNUSED_PARAMETER(x)
 *   Used to specify that a function parameter (argument) is required but not
 *   accessed by the function body.
 */
#ifndef UNUSED_PARAMETER
# if defined(__LCC__)
#   define UNUSED_PARAMETER(x)                                   /* do nothing */
# else

/*
 * This is the semi-ANSI standard way of indicating that an
 * unused function parameter is required.
 */
#   define UNUSED_PARAMETER(x)         (void) (x)
# endif
#endif

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

/*=========*
 * Asserts *
 *=========*/
#ifndef utAssert
# if defined(DOASSERTS)
#  if !defined(PRINT_ASSERTS)
#    include <assert.h>
#    define utAssert(exp)              assert(exp)
#  else
#    include <stdio.h>

static void _assert(char *statement, char *file, int line)
{
  printf("%s in %s on line %d\n", statement, file, line);
}

#   define utAssert(_EX)               ((_EX) ? (void)0 : _assert(#_EX, __FILE__, __LINE__))
#  endif

# else
#  define utAssert(exp)                                          /* do nothing */
# endif
#endif

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

void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo)
{
  uint32_T absIn0;
  uint32_T absIn1;
  uint32_T in0Lo;
  uint32_T in0Hi;
  uint32_T in1Hi;
  uint32_T productHiLo;
  uint32_T productLoHi;
  absIn0 = in0 < 0L ? ~(uint32_T)in0 + 1UL : (uint32_T)in0;
  absIn1 = in1 < 0L ? ~(uint32_T)in1 + 1UL : (uint32_T)in1;
  in0Hi = absIn0 >> 16UL;
  in0Lo = absIn0 & 65535UL;
  in1Hi = absIn1 >> 16UL;
  absIn0 = absIn1 & 65535UL;
  productHiLo = in0Hi * absIn0;
  productLoHi = in0Lo * in1Hi;
  absIn0 *= in0Lo;
  absIn1 = 0UL;
  in0Lo = (productLoHi << /*MW:OvBitwiseOk*/ 16UL) + /*MW:OvCarryOk*/ absIn0;
  if (in0Lo < absIn0) {
    absIn1 = 1UL;
  }

  absIn0 = in0Lo;
  in0Lo += /*MW:OvCarryOk*/ productHiLo << /*MW:OvBitwiseOk*/ 16UL;
  if (in0Lo < absIn0) {
    absIn1++;
  }

  absIn0 = (((productLoHi >> 16UL) + (productHiLo >> 16UL)) + in0Hi * in1Hi) +
    absIn1;
  if (!((in0 == 0L) || ((in1 == 0L) || ((in0 > 0L) == (in1 > 0L))))) {
    absIn0 = ~absIn0;
    in0Lo = ~in0Lo;
    in0Lo++;
    if (in0Lo == 0UL) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = in0Lo;
}

int32_T mul_s32_sat(int32_T a, int32_T b)
{
  int32_T result;
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  if (((int32_T)u32_chi > 0L) || ((u32_chi == 0UL) && (u32_clo >= 2147483648UL)))
  {
    result = MAX_int32_T;
  } else if (((int32_T)u32_chi < -1L) || (((int32_T)u32_chi == -1L) && (u32_clo <
    2147483648UL))) {
    result = MIN_int32_T;
  } else {
    result = (int32_T)u32_clo;
  }

  return result;
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabsf(u0);
    tmp_0 = fabsf(u1);
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
      y = sqrtf(u0);
    } else if ((u0 < 0.0) && (u1 > floorf(u1))) {
      y = (rtNaN);
    } else {
      y = powf(u0, u1);
    }
  }

  return y;
}

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabsf(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floorf(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceilf(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int16_T u0_0;
  int16_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2f(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2f(u0, u1);
  }

  return y;
}

/* Function for MATLAB Function: '<S9>/MATLAB Function1' */
static real_T norm(const real_T x[3])
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;
  scale = 2.2250738585072014E-308;
  absxk = fabsf(x[0]);
  if (absxk > 2.2250738585072014E-308) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    y = t * t;
  }

  absxk = fabsf(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabsf(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrtf(y);
}

real_T rt_remd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T u1_0;
  if (!((!rtIsNaN(u0)) && (!rtIsInf(u0)) && ((!rtIsNaN(u1)) && (!rtIsInf(u1)))))
  {
    y = (rtNaN);
  } else {
    if (u1 < 0.0) {
      u1_0 = ceilf(u1);
    } else {
      u1_0 = floorf(u1);
    }

    if ((u1 != 0.0) && (u1 != u1_0)) {
      u1_0 = u0 / u1;
      if (fabsf(u1_0 - rt_roundd_snf(u1_0)) <= DBL_EPSILON * fabsf(u1_0)) {
        y = 0.0;
      } else {
        y = fmodf(u0, u1);
      }
    } else {
      y = fmodf(u0, u1);
    }
  }

  return y;
}

/* Function for MATLAB Function: '<S6>/MATLAB Function1' */
static real_T norm_f(const real_T x[3])
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;
  scale = 2.2250738585072014E-308;
  absxk = fabsf(x[0]);
  if (absxk > 2.2250738585072014E-308) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    y = t * t;
  }

  absxk = fabsf(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabsf(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrtf(y);
}

/* Model step function */
void env_estimation_lib_step(void)
{
  real_T JD_days_gps;
  real_T T1900;
  real_T days;
  real_T b_min;
  real_T deps_1980;
  real_T tod_to_mod[9];
  static const real_T nc[300] = { 1.0, 9.0, 31.0, 2.0, 10.0, 32.0, 11.0, 33.0,
    34.0, 12.0, 35.0, 13.0, 36.0, 38.0, 37.0, 40.0, 39.0, 41.0, 14.0, 3.0, 42.0,
    45.0, 43.0, 44.0, 46.0, 15.0, 47.0, 16.0, 18.0, 48.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, -1.0, 1.0, 0.0, -1.0, -1.0, 1.0, 2.0,
    -2.0, 0.0, 2.0, 2.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 2.0, 2.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 0.0, 0.0, 2.0, 0.0, 2.0, 0.0,
    2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0, 2.0, 0.0, 2.0, 0.0, 0.0, -2.0, 0.0, 0.0,
    0.0, 0.0, -2.0, 0.0, 0.0, -2.0, -2.0, -2.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0,
    -2.0, 0.0, 2.0, 0.0, 0.0, -2.0, 0.0, -2.0, 0.0, 0.0, -2.0, 2.0, 1.0, 2.0,
    2.0, 2.0, 0.0, 0.0, 2.0, 1.0, 2.0, 2.0, 0.0, 1.0, 2.0, 1.0, 0.0, 2.0, 1.0,
    1.0, 0.0, 1.0, 2.0, 2.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0, 2.0, 1.0, -171996.0,
    -13187.0, -2274.0, 2062.0, 1426.0, 712.0, -517.0, -386.0, -301.0, 217.0,
    -158.0, 129.0, 123.0, 63.0, 63.0, -59.0, -58.0, -51.0, 48.0, 46.0, -38.0,
    -31.0, 29.0, 29.0, 26.0, -22.0, 21.0, 17.0, -16.0, 16.0, -174.2, -1.6, -0.2,
    0.2, -3.4, 0.1, 1.2, -0.4, 0.0, -0.5, 0.0, 0.1, 0.0, 0.1, 0.0, 0.0, -0.1,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.1, 0.1, 0.0, 92025.0,
    5736.0, 977.0, -895.0, 54.0, -7.0, 224.0, 200.0, 129.0, -95.0, -1.0, -70.0,
    -53.0, -33.0, -2.0, 26.0, 32.0, 27.0, 1.0, -24.0, 16.0, 13.0, -1.0, -12.0,
    -1.0, 0.0, -10.0, 0.0, 7.0, -8.0, 8.9, -3.1, -0.5, 0.5, -0.1, 0.0, -0.6, 0.0,
    -0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  static const int8_T c[3] = { 1, 0, 0 };

  static const int8_T d[3] = { 0, 0, 1 };

  static const int8_T e[3] = { 0, 1, 0 };

  int16_T lowAlt;
  real_T xi;
  real_T b_o;
  real_T eta;
  real_T C_1;
  real_T C_4;
  real_T C_5;
  real_T D_2;
  real_T D_3;
  real_T D_4;
  real_T IL2;
  real_T IL3;
  real_T IL4;
  real_T IL5;
  real_T M_DF;
  real_T M[3];
  static const real_T a[9] = { 1.0, 0.0, 0.0, 0.0, 0.91748214306524178,
    0.397776969112606, 0.0, -0.397776969112606, 0.91748214306524178 };

  static const int8_T b[3] = { 1, 0, 0 };

  static const int8_T d_0[3] = { 0, 0, 1 };

  int16_T LOS;
  real_T rtb_Switch;
  real_T rtb_Abs1;
  boolean_T rtb_Compare_k;
  real_T rtb_Sum3;
  real_T rtb_z1;
  real_T rtb_Sum;
  real_T rtb_VectorConcatenate[9];
  real_T rtb_VectorConcatenate_n[9];
  int8_T s70_iter;
  real_T rtb_VectorConcatenate_o[9];
  boolean_T rtb_LogicalOperator;
  int32_T idx1;
  int32_T idx2;
  int32_T j;
  int32_T s136_iter;
  real_T rtb_T_TT_J2000;
  real_T rtb_pos_teme_km[3];
  real_T rtb_Sum_g[3];
  real_T rtb_Assignment[11];
  real_T rtb_Assignment1[11];
  real_T rtb_TmpSignalConversionAtppnInp[13];
  real_T rtb_teme_to_gcrf[9];
  real_T rtb_tc_old[169];
  real_T Assignment[169];
  real_T Assignment2[169];
  real_T rtb_Sum_j[3];
  real_T rtb_Sum_j_0[3];
  real_T c_0[9];
  real_T c_1[9];
  real_T rtb_VectorConcatenate_2[9];
  real_T rtb_Switch_0[9];
  real_T rtb_Sum3_0[3];
  int16_T i;
  real_T rtb_Sum1_i_idx_3;
  real_T rtb_Sum1_i_idx_2;
  real_T rtb_Sum1_i_idx_1;
  real_T rtb_Sum1_i_idx_0;
  real_T rtb_time_ut1_idx_5;
  real_T rtb_time_ut1_idx_4;
  real_T rtb_time_ut1_idx_3;
  real_T rtb_time_ut1_idx_2;
  real_T rtb_time_ut1_idx_0;
  int16_T qY;
  int16_T qY_0;
  int32_T j_0;

  /* UnaryMinus: '<S28>/Unary Minus' incorporates:
   *  Product: '<S28>/u(1)*u(4)'
   */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  rtb_VectorConcatenate[0] = 0.39377477022454854;

  /* UnaryMinus: '<S31>/Unary Minus' */
  rtb_VectorConcatenate[1] = 0.84522918991728324;

  /* UnaryMinus: '<S34>/Unary Minus' incorporates:
   *  Product: '<S34>/u(3)*u(4)'
   */
  rtb_VectorConcatenate[2] = 0.361288038615143;

  /* UnaryMinus: '<S29>/Unary Minus' incorporates:
   *  Product: '<S29>/u(1)*u(2)'
   */
  rtb_VectorConcatenate[3] = 0.62280585651662945;

  /* SignalConversion: '<S38>/ConcatBufferAtVector ConcatenateIn5' */
  rtb_VectorConcatenate[4] = -0.53440398249991838;

  /* UnaryMinus: '<S35>/Unary Minus' incorporates:
   *  Product: '<S35>/u(2)*u(3)'
   */
  rtb_VectorConcatenate[5] = 0.57142387819972529;

  /* SignalConversion: '<S38>/ConcatBufferAtVector ConcatenateIn7' */
  rtb_VectorConcatenate[6] = 0.67605790833566293;

  /* SignalConversion: '<S38>/ConcatBufferAtVector ConcatenateIn8' incorporates:
   *  Constant: '<S33>/Constant'
   */
  rtb_VectorConcatenate[7] = 0.0;

  /* UnaryMinus: '<S36>/Unary Minus' */
  rtb_VectorConcatenate[8] = -0.73684849499527949;

  /* UnaryMinus: '<S17>/Unary Minus' incorporates:
   *  Product: '<S17>/u(1)*u(4)'
   */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  rtb_VectorConcatenate_n[0] = 0.39498108890634259;

  /* UnaryMinus: '<S20>/Unary Minus' */
  rtb_VectorConcatenate_n[1] = 0.84522918991728324;

  /* UnaryMinus: '<S23>/Unary Minus' incorporates:
   *  Product: '<S23>/u(3)*u(4)'
   */
  rtb_VectorConcatenate_n[2] = 0.35996882631435323;

  /* UnaryMinus: '<S18>/Unary Minus' incorporates:
   *  Product: '<S18>/u(1)*u(2)'
   */
  rtb_VectorConcatenate_n[3] = 0.62471380592491255;

  /* SignalConversion: '<S27>/ConcatBufferAtVector ConcatenateIn5' */
  rtb_VectorConcatenate_n[4] = -0.53440398249991838;

  /* UnaryMinus: '<S24>/Unary Minus' incorporates:
   *  Product: '<S24>/u(2)*u(3)'
   */
  rtb_VectorConcatenate_n[5] = 0.56933737289505038;

  /* SignalConversion: '<S27>/ConcatBufferAtVector ConcatenateIn7' */
  rtb_VectorConcatenate_n[6] = 0.6735893408399295;

  /* SignalConversion: '<S27>/ConcatBufferAtVector ConcatenateIn8' incorporates:
   *  Constant: '<S22>/Constant'
   */
  rtb_VectorConcatenate_n[7] = 0.0;

  /* UnaryMinus: '<S25>/Unary Minus' */
  rtb_VectorConcatenate_n[8] = -0.73910581103576056;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    /* Sum: '<S10>/Sum' incorporates:
     *  Math: '<S10>/Math Function1'
     *  Product: '<S10>/Product1'
     */
    rtb_Sum_g[lowAlt] = rtb_VectorConcatenate[3 * lowAlt + 2] *
      -6.3664995496819746E+6 + (rtb_VectorConcatenate[3 * lowAlt + 1] * 0.0 +
      rtb_VectorConcatenate[3 * lowAlt] * 0.0);
  }

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Inport: '<Root>/MET'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  Husky-Sat 1, ADCS Subsystem */
  /*   */
  /*  Convert a JD_J2000_TT in seconds into the GPS epoch and SoW-Week format. */
  /*  */
  /*  T. Reynolds -- 3.9.18 */
  /*  ----------------------------------------------------------------------- % */
  /*  #codegen */
  /* MATLAB Function 'MET_2_GPS_lib/MATLAB Function': '<S8>:1' */
  /*  Constants */
  /* '<S8>:1:13' gps2tai     = 19.0; */
  /*  From MJD 58158 */
  /* '<S8>:1:14' gps2tt      = 32.184 + gps2tai; */
  /* '<S8>:1:15' day2sec     = 86400; */
  /* '<S8>:1:16' sec2day     = 1/day2sec; */
  /* '<S8>:1:17' dayinweek   = 7; */
  /* '<S8>:1:18' JD_GPSepoch = 2444244.5; */
  /* '<S8>:1:19' JD_J2000    = 2451545.0; */
  /*  Map input to JD_J2000_TT_days */
  /* '<S8>:1:22' JD_J2000_TT     = JD_J2000_TT_s * sec2day; */
  /*  Map input to GPS time frame */
  /* '<S8>:1:25' JD_J2000_GPS    = JD_J2000_TT - sec2day*gps2tt; */
  /* '<S8>:1:26' JD_GPS          = JD_J2000_GPS + JD_J2000; */
  /* '<S8>:1:27' JD_GPS_since_epoch    = JD_GPS - JD_GPSepoch; */
  /*  Convert to SoW and Week */
  /* '<S8>:1:30' week    = floorf(JD_GPS_since_epoch/dayinweek); */
  rtb_Abs1 = floorf((((rtU.MET * 1.1574074074074073E-5 - 0.00059240740740740738)
                     + 2.451545E+6) - 2.4442445E+6) / 7.0);

  /* MATLAB Function: '<S149>/time-conversion-lib' incorporates:
   *  Inport: '<Root>/MET'
   *  MATLAB Function: '<S2>/MATLAB Function'
   */
  /* '<S8>:1:32' sec_of_week    = day2sec * (JD_GPS_since_epoch - week*dayinweek); */
  /* '<S8>:1:34' GPS_time = [ sec_of_week; week ]; */
  /*  ----------------------------------------------------------------------- % */
  /*  Husky-Sat 1, ADCS Subsystem */
  /*   */
  /*  Convert a GPS time into date-time, Julian date and Julian century in the */
  /*  UT1 and TT frames. Accounts for leap seconds. */
  /*  */
  /*    dUT1 obtained from: */
  /*        http://maia.usno.navy.mil/ser7/finals.daily */
  /*  */
  /*  T. Reynolds -- 2.28.18 */
  /*  ----------------------------------------------------------------------- % */
  /*  #codegen */
  /* MATLAB Function 'time_conversion_lib/time-conversion-lib': '<S151>:1' */
  /*  Constants */
  /* '<S151>:1:16' dAT         = 37.0; */
  /*  From MJD 58158 (Feb 7, 2018) */
  /* '<S151>:1:17' gps2tai     = 19.0; */
  /*  From MJD 58158 */
  /* '<S151>:1:18' gps2tt      = 32.184 + gps2tai; */
  /* '<S151>:1:19' gps2utc     = gps2tai - dAT; */
  /* '<S151>:1:20' gps2ut1     = gps2tai - dAT + dUT1; */
  /* '<S151>:1:21' sec2day     = 1/86400; */
  /* '<S151>:1:22' week2day    = 7; */
  /* '<S151>:1:23' JD_GPSepoch = 2444244.5; */
  /* '<S151>:1:24' JD_J2000    = 2451545.0; */
  /* '<S151>:1:25' JD2cent     = 1/36525; */
  /* '<S151>:1:27' sec_in = cur_gps(1); */
  /* '<S151>:1:27' week_in = cur_gps(2); */
  /*  --------------------------- % */
  /*  Translate GPS w-s to JD and Gregorian ymd-hms */
  /* '<S151>:1:31' JD_days_gps     = JD_GPSepoch + week2day*week_in + sec2day*sec_in; */
  JD_days_gps = ((((rtU.MET * 1.1574074074074073E-5 - 0.00059240740740740738) +
                   2.451545E+6) - 2.4442445E+6) - rtb_Abs1 * 7.0) * 86400.0 *
    1.1574074074074073E-5 + (7.0 * rtb_Abs1 + 2.4442445E+6);

  /* '<S151>:1:32' JD_utc          = JD_days_gps + sec2day*gps2utc; */
  /* '<S151>:1:33' JD_ut1          = JD_days_gps + sec2day*gps2ut1; */
  /* '<S151>:1:34' JD_TT           = JD_days_gps + sec2day*gps2tt; */
  /* '<S151>:1:36' JD_utc_J2000    = JD_utc - JD_J2000; */
  /* '<S151>:1:37' JD_ut1_J2000    = JD_ut1 - JD_J2000; */
  /* '<S151>:1:38' T_ut1_J2000     = JD_ut1_J2000*JD2cent; */
  rtb_Abs1 = ((JD_days_gps + -0.00020833333333333332) - 2.451545E+6) *
    2.7378507871321012E-5;

  /* '<S151>:1:39' T_TT_J2000      = JD2cent*( JD_TT - JD_J2000 ); */
  rtb_T_TT_J2000 = ((JD_days_gps + 0.00059240740740740738) - 2.451545E+6) *
    2.7378507871321012E-5;

  /* '<S151>:1:41' time_ut1 = JD_2_ymdhms( JD_ut1 ); */
  /*  This algorithm is based on Vallado 4e algorithm 22, pp 202. */
  /*  Constants */
  /* '<S151>:1:49' JD_1900     = 2415019.5; */
  /* '<S151>:1:50' Lmonth      = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
  /*  Convert JD to date-time */
  /* '<S151>:1:53' T1900   = (JD - JD_1900)/365.25; */
  T1900 = ((JD_days_gps + -0.00020833333333333332) - 2.4150195E+6) / 365.25;

  /*  base epoch is 1900 */
  /* '<S151>:1:54' year    = 1900 + floorf(T1900); */
  rtb_z1 = floorf(T1900);
  T1900 = 1900.0 + floorf(T1900);

  /* '<S151>:1:56' leapyrs = floorf((year - 1900 - 1)*0.25); */
  /* '<S151>:1:57' days    = (JD - JD_1900) - ((year - 1900)*365 + leapyrs ); */
  days = ((JD_days_gps + -0.00020833333333333332) - 2.4150195E+6) - (floorf
    ((((1900.0 + rtb_z1) - 1900.0) - 1.0) * 0.25) + ((1900.0 + rtb_z1) - 1900.0)
    * 365.0);

  /* '<S151>:1:59' if days < 1 */
  if (days < 1.0) {
    /* '<S151>:1:60' year    = year - 1; */
    T1900 = (1900.0 + rtb_z1) - 1.0;

    /* '<S151>:1:61' leapyrs = floorf((year - 1900 - 1)*0.25); */
    /* '<S151>:1:62' days    = (JD - JD_1900) - ((year - 1900)*365 + leapyrs ); */
    days = ((JD_days_gps + -0.00020833333333333332) - 2.4150195E+6) - (floorf
      (((((1900.0 + rtb_z1) - 1.0) - 1900.0) - 1.0) * 0.25) + (((1900.0 + rtb_z1)
      - 1.0) - 1900.0) * 365.0);
  }

  /* '<S151>:1:65' if mod(year,4) == 0 */
  /* '<S151>:1:69' dayofyear   = floorf(days); */
  rtb_Sum3 = floorf(days);

  /* '<S151>:1:71' day     = 0; */
  /* '<S151>:1:72' month   = 0; */
  /* '<S151>:1:74' while day < dayofyear */
  /* dayofmonth = dayofyear - (day - Lmonth(month)); */
  /* '<S151>:1:81' tau     = 24*( days-dayofyear ); */
  days = (days - rtb_Sum3) * 24.0;

  /* '<S151>:1:82' hour    = floorf( tau ); */
  rtb_z1 = floorf(days);

  /* '<S151>:1:83' min     = floorf( 60*(tau - hour) ); */
  b_min = floorf((days - rtb_z1) * 60.0);

  /* '<S151>:1:84' sec     = 3600*( tau - hour - (min/60) ); */
  /* '<S151>:1:86' time    = [ year, month, dayofyear, hour, min, sec ]; */
  rtb_time_ut1_idx_0 = T1900;
  rtb_time_ut1_idx_2 = rtb_Sum3;
  rtb_time_ut1_idx_3 = rtb_z1;
  rtb_time_ut1_idx_4 = b_min;
  rtb_time_ut1_idx_5 = ((days - rtb_z1) - b_min / 60.0) * 3600.0;

  /* MATLAB Function: '<S148>/MATLAB Function' */
  /*  */
  /*  UW HuskySat-1, ADCS Subsystem */
  /*    Courtesy: Krish Kaycee 9.11.17 */
  /*    Updated: T. Reynolds 2.10.18 */
  /*  */
  /*  COORD_ROTATIONS excludes the polar motion transformation. Hence it */
  /*  includes the Precession, Nutation, and Sidereal Time rotation matrices. */
  /* MATLAB Function 'coord_rotations_lib/MATLAB Function': '<S150>:1' */
  /*  UPDATED: Excluding polar motion since it would need to be updated ~1 per */
  /*  week to keep accurate. For our mission duration and requirements, */
  /*  accounting for polar motion is not necessary. -T. Reynolds, 9.11.17 */
  /*  Constants */
  /*  Everything should be precomputed in radians */
  /* '<S150>:1:17' deg2rad     = pi/180; */
  /* '<S150>:1:18' twopi       = 2*pi; */
  /* '<S150>:1:19' asec2rad    = 1/3600*deg2rad; */
  /* '<S150>:1:20' o_prec      = 7.292115146706979e-5; */
  /*  Evaluate Delaunay parameters */
  /* '<S150>:1:23' epsb_1980 = deg2rad * ( 23.439291 ... */
  /* '<S150>:1:24'                         - 0.0130042 * jd_tt_j2000_century ... */
  /* '<S150>:1:25'                         - 1.64e-7 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:26'                         + 5.04e-7 * jd_tt_j2000_century^3); */
  T1900 = (((23.439291 - 0.0130042 * rtb_T_TT_J2000) - rtb_T_TT_J2000 *
            rtb_T_TT_J2000 * 1.64E-7) + 5.04E-7 * rt_powd_snf(rtb_T_TT_J2000,
            3.0)) * 0.017453292519943295;

  /*  Evaluate Delaunay parameters (correct equations from 4e errata) */
  /* '<S150>:1:29' M_moon	=   deg2rad * ( 134.96298139 ... */
  /* '<S150>:1:30'                         + (1325 * 360 + 198.8673981) * jd_tt_j2000_century ... */
  /* '<S150>:1:31'                       	+ 0.0086972 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:32'                        	+ 1.78e-5 * jd_tt_j2000_century^3); */
  days = (((477198.8673981 * rtb_T_TT_J2000 + 134.96298139) + rtb_T_TT_J2000 *
           rtb_T_TT_J2000 * 0.0086972) + 1.78E-5 * rt_powd_snf(rtb_T_TT_J2000,
           3.0)) * 0.017453292519943295;

  /* '<S150>:1:34' M_sun	=   deg2rad * ( 357.52772333 ... */
  /* '<S150>:1:35'                         + (99 * 360 + 359.0503400) * jd_tt_j2000_century ... */
  /* '<S150>:1:36'                       	- 0.0001603 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:37'                      	- 3.3e-6 * jd_tt_j2000_century^3); */
  rtb_z1 = (((35999.05034 * rtb_T_TT_J2000 + 357.52772333) - rtb_T_TT_J2000 *
             rtb_T_TT_J2000 * 0.0001603) - 3.3E-6 * rt_powd_snf(rtb_T_TT_J2000,
             3.0)) * 0.017453292519943295;

  /* '<S150>:1:39' u_moon	=   deg2rad * ( 93.27191028 ... */
  /* '<S150>:1:40'                         + (1342 * 360 + 82.0175381) * jd_tt_j2000_century ... */
  /* '<S150>:1:41'                       	- 0.0036825 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:42'                         + 3.1e-6 * jd_tt_j2000_century^3); */
  b_min = (((483202.0175381 * rtb_T_TT_J2000 + 93.27191028) - rtb_T_TT_J2000 *
            rtb_T_TT_J2000 * 0.0036825) + 3.1E-6 * rt_powd_snf(rtb_T_TT_J2000,
            3.0)) * 0.017453292519943295;

  /* '<S150>:1:44' D_sun	=   deg2rad * ( 297.85036306 ... */
  /* '<S150>:1:45'                         + (1236 * 360 + 307.1114800) * jd_tt_j2000_century ... */
  /* '<S150>:1:46'                         - 0.0019142 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:47'                      	+ 5.3e-6 * jd_tt_j2000_century^3); */
  rtb_Sum = (((445267.11148 * rtb_T_TT_J2000 + 297.85036306) - rtb_T_TT_J2000 *
              rtb_T_TT_J2000 * 0.0019142) + 5.3E-6 * rt_powd_snf(rtb_T_TT_J2000,
              3.0)) * 0.017453292519943295;

  /* '<S150>:1:49' O_moon	=   deg2rad * ( 125.04452222 ... */
  /* '<S150>:1:50'                         - (5 * 360 + 134.1362608) * jd_tt_j2000_century ... */
  /* '<S150>:1:51'                     	+ 0.0020708 * jd_tt_j2000_century^2 ... */
  /* '<S150>:1:52'                         + 2.2e-6 * jd_tt_j2000_century^3); */
  rtb_Sum3 = (((125.04452222 - 1934.1362608 * rtb_T_TT_J2000) + rtb_T_TT_J2000 *
               rtb_T_TT_J2000 * 0.0020708) + 2.2E-6 * rt_powd_snf(rtb_T_TT_J2000,
    3.0)) * 0.017453292519943295;

  /*  Largest Nutation coefficents (out of 106)                     */
  /* '<S150>:1:55' nc = [ ... */
  /* '<S150>:1:56' 1   0   0   0   0   1   -171996 -174.2  92025   8.9 */
  /* '<S150>:1:57' 9   0   0   2   -2  2   -13187  -1.6    5736    -3.1 */
  /* '<S150>:1:58' 31  0   0   2   0   2   -2274   -0.2    977     -0.5 */
  /* '<S150>:1:59' 2   0   0   0   0   2   2062    0.2     -895    0.5 */
  /* '<S150>:1:60' 10  0   1   0  	0   0   1426    -3.4    54      -0.1 */
  /* '<S150>:1:61' 32  1   0   0   0   0   712     0.1     -7      0.0 */
  /* '<S150>:1:62' 11  0   1   2   -2  2   -517    1.2     224     -0.6 */
  /* '<S150>:1:63' 33  0   0   2   0   1   -386    -0.4    200     0.0 */
  /* '<S150>:1:64' 34  1   0   2   0   2   -301    0.0     129     -0.1 */
  /* '<S150>:1:65' 12  0   -1  2   -2  2   217     -0.5    -95     0.3 */
  /* '<S150>:1:66' 35  1   0   0   -2  0   -158    0.0     -1      0.0 */
  /* '<S150>:1:67' 13  0   0   2   -2  1   129     0.1     -70     0.0  */
  /* '<S150>:1:68' 36  -1  0   2   0   2   123     0.0     -53     0.0  */
  /* '<S150>:1:69' 38  1   0   0   0   1   63      0.1     -33     0.0 */
  /* '<S150>:1:70' 37  0   0   0   2   0   63      0.0     -2      0.0 */
  /* '<S150>:1:71' 40  -1  0   2   2   2   -59     0.0     26      0.0 */
  /* '<S150>:1:72' 39  -1  0   0   0   1   -58     -0.1    32      0.0 */
  /* '<S150>:1:73' 41  1   0   2   0   1   -51     0.0     27      0.0 */
  /* '<S150>:1:74' 14  2   0   0   -2  0   48      0.0     1       0.0 */
  /* '<S150>:1:75' 3   -2  0   2   0   1   46      0.0     -24     0.0 */
  /* '<S150>:1:76' 42  0   0   2   2   2   -38     0.0     16      0.0 */
  /* '<S150>:1:77' 45  2   0   2   0   2   -31     0.0     13      0.0 */
  /* '<S150>:1:78' 43  2   0   0   0   0   29      0.0     -1      0.0 */
  /* '<S150>:1:79' 44  1   0   2   -2  2   29      0.0     -12     0.0 */
  /* '<S150>:1:80' 46  0   0   2   0   0   26      0.0     -1      0.0 */
  /* '<S150>:1:81' 15  0   0   2   -2  0   -22     0.0     0       0.0 */
  /* '<S150>:1:82' 47  -1  0   2   0   1   21      0.0     -10     0.0 */
  /* '<S150>:1:83' 16  0   2   0   0   0   17      -0.1    0       0.0 */
  /* '<S150>:1:84' 18  0   2   2   -2  2   -16     0.1     7       0.0 */
  /* '<S150>:1:85' 48  -1  0   0   2   1   16      0.0     -8      0.0 ... */
  /* '<S150>:1:86' ]; */
  /*  Map coefficients to radians (this should be embedded in the data) */
  /*  nc(:,end-3:end) = nc(:,end-3:end) * 0.0001 * asec2rad; */
  /*  Compute nutation in longitude (psi), and latitude (eps) */
  /* '<S150>:1:92' dpsi_1980 = 0; */
  rtb_Switch = 0.0;

  /* '<S150>:1:92' deps_1980 = 0; */
  deps_1980 = 0.0;

  /* '<S150>:1:93' for i = 1:size(nc,1) */
  for (lowAlt = 0; lowAlt < 30; lowAlt++) {
    /* '<S150>:1:94' api         =   nc(i,2) * M_moon + nc(i,3) * M_sun + nc(i,4) * u_moon + nc(i,5) * D_sun + nc(i,6) * O_moon; */
    IL2 = (((nc[30 + lowAlt] * days + nc[60 + lowAlt] * rtb_z1) + nc[90 + lowAlt]
            * b_min) + nc[120 + lowAlt] * rtb_Sum) + nc[150 + lowAlt] * rtb_Sum3;

    /* '<S150>:1:95' dpsi_1980   =   0.0001 * (nc(i,7) + nc(i,8) * jd_tt_j2000_century) * sinf(api) + dpsi_1980; */
    rtb_Switch += (nc[210 + lowAlt] * rtb_T_TT_J2000 + nc[180 + lowAlt]) *
      0.0001 * sinf(IL2);

    /* '<S150>:1:96' deps_1980   =   0.0001 * (nc(i,9) + nc(i,10) * jd_tt_j2000_century) * cosf(api) + deps_1980; */
    deps_1980 += (nc[270 + lowAlt] * rtb_T_TT_J2000 + nc[240 + lowAlt]) * 0.0001
      * cosf(IL2);
  }

  /* '<S150>:1:99' eps_1980 = epsb_1980 + asec2rad * deps_1980; */
  rtb_z1 = 4.84813681109536E-6 * deps_1980 + T1900;

  /* '<S150>:1:100' tod_to_mod = rot1(-epsb_1980) * rot3(asec2rad * dpsi_1980) * rot1(eps_1980); */
  /* '<S150>:1:149' y = [1 0 0; 0 cosf(u) sinf(u); 0 -sinf(u) cosf(u)]; */
  days = 4.84813681109536E-6 * rtb_Switch;

  /* '<S150>:1:157' y = [cosf(u) sinf(u) 0; -sinf(u) cosf(u) 0; 0 0 1]; */
  /* '<S150>:1:149' y = [1 0 0; 0 cosf(u) sinf(u); 0 -sinf(u) cosf(u)]; */
  c_0[1] = 0.0;
  c_0[4] = cosf(-T1900);
  c_0[7] = sinf(-T1900);
  c_0[2] = 0.0;
  c_0[5] = -sinf(-T1900);
  c_0[8] = cosf(-T1900);
  rtb_teme_to_gcrf[0] = cosf(days);
  rtb_teme_to_gcrf[3] = sinf(days);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sinf(days);
  rtb_teme_to_gcrf[4] = cosf(days);
  rtb_teme_to_gcrf[7] = 0.0;
  c_0[0] = 1.0;
  rtb_teme_to_gcrf[2] = 0.0;
  c_0[3] = 0.0;
  rtb_teme_to_gcrf[5] = 0.0;
  c_0[6] = 0.0;
  rtb_teme_to_gcrf[8] = 1.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_VectorConcatenate[lowAlt + 3 * LOS] = 0.0;
      rtb_VectorConcatenate[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS] *
        c_0[lowAlt];
      rtb_VectorConcatenate[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS + 1] *
        c_0[lowAlt + 3];
      rtb_VectorConcatenate[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS + 2] *
        c_0[lowAlt + 6];
    }

    c_1[3 * lowAlt] = c[lowAlt];
  }

  c_1[1] = 0.0;
  c_1[4] = cosf(rtb_z1);
  c_1[7] = sinf(rtb_z1);
  c_1[2] = 0.0;
  c_1[5] = -sinf(rtb_z1);
  c_1[8] = cosf(rtb_z1);

  /*  Generate Precession matrix */
  /* '<S150>:1:103' zeta    =   asec2rad * (2306.2181 * jd_tt_j2000_century + 0.30188 * jd_tt_j2000_century^2 + 0.017998 * jd_tt_j2000_century^3); */
  days = ((rtb_T_TT_J2000 * rtb_T_TT_J2000 * 0.30188 + 2306.2181 *
           rtb_T_TT_J2000) + 0.017998 * rt_powd_snf(rtb_T_TT_J2000, 3.0)) *
    4.84813681109536E-6;

  /* '<S150>:1:104' theta   =   asec2rad * (2004.3109 * jd_tt_j2000_century - 0.42665 * jd_tt_j2000_century^2 - 0.041833 * jd_tt_j2000_century^3); */
  /* '<S150>:1:105' z       =   asec2rad * (2306.2181 * jd_tt_j2000_century + 1.09468 * jd_tt_j2000_century^2 + 0.018203 * jd_tt_j2000_century^3); */
  rtb_z1 = ((rtb_T_TT_J2000 * rtb_T_TT_J2000 * 1.09468 + 2306.2181 *
             rtb_T_TT_J2000) + 0.018203 * rt_powd_snf(rtb_T_TT_J2000, 3.0)) *
    4.84813681109536E-6;

  /* '<S150>:1:107' mod_to_gcrf = rot3(zeta) * rot2(-theta) * rot3(z); */
  /* '<S150>:1:157' y = [cosf(u) sinf(u) 0; -sinf(u) cosf(u) 0; 0 0 1]; */
  rtb_T_TT_J2000 = -(((2004.3109 * rtb_T_TT_J2000 - rtb_T_TT_J2000 *
                       rtb_T_TT_J2000 * 0.42665) - 0.041833 * rt_powd_snf
                      (rtb_T_TT_J2000, 3.0)) * 4.84813681109536E-6);

  /* '<S150>:1:153' y = [cosf(u) 0 -sinf(u); 0 1 0; sinf(u) 0 cosf(u)]; */
  /* '<S150>:1:157' y = [cosf(u) sinf(u) 0; -sinf(u) cosf(u) 0; 0 0 1]; */
  rtb_teme_to_gcrf[0] = cosf(days);
  rtb_teme_to_gcrf[3] = sinf(days);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sinf(days);
  rtb_teme_to_gcrf[4] = cosf(days);
  rtb_teme_to_gcrf[7] = 0.0;
  c_0[0] = cosf(rtb_T_TT_J2000);
  c_0[3] = 0.0;
  c_0[6] = -sinf(rtb_T_TT_J2000);
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      tod_to_mod[lowAlt + 3 * LOS] = 0.0;
      tod_to_mod[lowAlt + 3 * LOS] += c_1[3 * LOS] *
        rtb_VectorConcatenate[lowAlt];
      tod_to_mod[lowAlt + 3 * LOS] += c_1[3 * LOS + 1] *
        rtb_VectorConcatenate[lowAlt + 3];
      tod_to_mod[lowAlt + 3 * LOS] += c_1[3 * LOS + 2] *
        rtb_VectorConcatenate[lowAlt + 6];
    }

    rtb_teme_to_gcrf[2 + 3 * lowAlt] = d[lowAlt];
    c_0[1 + 3 * lowAlt] = e[lowAlt];
  }

  c_0[2] = sinf(rtb_T_TT_J2000);
  c_0[5] = 0.0;
  c_0[8] = cosf(rtb_T_TT_J2000);
  rtb_VectorConcatenate_2[0] = cosf(rtb_z1);
  rtb_VectorConcatenate_2[3] = sinf(rtb_z1);
  rtb_VectorConcatenate_2[6] = 0.0;
  rtb_VectorConcatenate_2[1] = -sinf(rtb_z1);
  rtb_VectorConcatenate_2[4] = cosf(rtb_z1);
  rtb_VectorConcatenate_2[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      c_1[lowAlt + 3 * LOS] = 0.0;
      c_1[lowAlt + 3 * LOS] += c_0[3 * LOS] * rtb_teme_to_gcrf[lowAlt];
      c_1[lowAlt + 3 * LOS] += c_0[3 * LOS + 1] * rtb_teme_to_gcrf[lowAlt + 3];
      c_1[lowAlt + 3 * LOS] += c_0[3 * LOS + 2] * rtb_teme_to_gcrf[lowAlt + 6];
    }

    rtb_VectorConcatenate_2[2 + 3 * lowAlt] = d[lowAlt];
  }

  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_VectorConcatenate[LOS + 3 * lowAlt] = 0.0;
      rtb_VectorConcatenate[LOS + 3 * lowAlt] += rtb_VectorConcatenate_2[3 *
        lowAlt] * c_1[LOS];
      rtb_VectorConcatenate[LOS + 3 * lowAlt] += rtb_VectorConcatenate_2[3 *
        lowAlt + 1] * c_1[LOS + 3];
      rtb_VectorConcatenate[LOS + 3 * lowAlt] += rtb_VectorConcatenate_2[3 *
        lowAlt + 2] * c_1[LOS + 6];
    }
  }

  /*  Compute equation of Equinoxes */
  /* '<S150>:1:110' eqe_1980 = asec2rad * (dpsi_1980  * cosf(epsb_1980) + 0.00264 * sinf(O_moon) + 0.000063 * sinf(2 * O_moon)); */
  rtb_T_TT_J2000 = ((rtb_Switch * cosf(T1900) + 0.00264 * sinf(rtb_Sum3)) + sinf
                    (2.0 * rtb_Sum3) * 6.3E-5) * 4.84813681109536E-6;

  /* '<S150>:1:111' teme_to_tod = rot3(-eqe_1980); */
  /* '<S150>:1:157' y = [cosf(u) sinf(u) 0; -sinf(u) cosf(u) 0; 0 0 1]; */
  /*  % Compute sidereal rotation */
  /*  GMST_1982 = asec2rad * (67310.54841 ... */
  /*                          + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /*                          + 0.093104 * jd_ut1_j2000_century^2 ... */
  /*                          - 6.2e-6 * jd_ut1_j2000_century^3); */
  /* '<S150>:1:118' GMST_1982 = (67310.54841 ... */
  /* '<S150>:1:119'                         + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /* '<S150>:1:120'                         + 0.093104 * jd_ut1_j2000_century^2 ... */
  /* '<S150>:1:121'                         - 6.2e-6 * jd_ut1_j2000_century^3); */
  T1900 = ((3.1644001848128662E+9 * rtb_Abs1 + 67310.54841) + rtb_Abs1 *
           rtb_Abs1 * 0.093104) - 6.2E-6 * rt_powd_snf(rtb_Abs1, 3.0);

  /* '<S150>:1:122' GMST_1982 = mod(GMST_1982,sign(GMST_1982)*86400) / 240; */
  if (T1900 < 0.0) {
    rtb_Sum3 = -1.0;
  } else if (T1900 > 0.0) {
    rtb_Sum3 = 1.0;
  } else if (T1900 == 0.0) {
    rtb_Sum3 = 0.0;
  } else {
    rtb_Sum3 = T1900;
  }

  rtb_Sum3 *= 86400.0;
  if (!(rtb_Sum3 == 0.0)) {
    if (rtb_Sum3 == rtb_Sum3) {
      T1900 -= floorf(T1900 / rtb_Sum3) * rtb_Sum3;
    } else {
      T1900 /= rtb_Sum3;
      if (fabsf(T1900 - rt_roundd_snf(T1900)) <= 2.2204460492503131E-16 * fabsf
          (T1900)) {
        T1900 = 0.0;
      } else {
        T1900 = (T1900 - floorf(T1900)) * rtb_Sum3;
      }
    }
  }

  T1900 /= 240.0;

  /* '<S150>:1:123' GMST_1982 = deg2rad * mod(GMST_1982, 360); */
  T1900 = (T1900 - floorf(T1900 / 360.0) * 360.0) * 0.017453292519943295;

  /* '<S150>:1:124' GAST_1982 = GMST_1982 + eqe_1980; */
  /*  Compute sidereal rotation */
  /*  GMST_1982 = asec2rad * (67310.54841 ... */
  /*                          + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /*                          + 0.093104 * jd_ut1_j2000_century^2 ... */
  /*                          - 6.2e-6 * jd_ut1_j2000_century^3); */
  /*  GMST_1982 = mod(GMST_1982, 2*pi); */
  /*  GAST_1982 = GMST_1982 + eqe_1980; */
  /*  Compute rotation from pseudo-Earth fixed frame (does not include polar */
  /*  motion) */
  /* '<S150>:1:136' pef_to_tod = rot3(-GAST_1982); */
  T1900 = -(T1900 + rtb_T_TT_J2000);

  /* '<S150>:1:157' y = [cosf(u) sinf(u) 0; -sinf(u) cosf(u) 0; 0 0 1]; */
  /*  % Compute polar motion (small angle approximation is assumed) */
  /*  itrf_to_pef = polar(polar_motion_rad(1), polar_motion_rad(2)); */
  /*  Compute composite rotations */
  /* '<S150>:1:142' teme_to_gcrf = mod_to_gcrf * tod_to_mod * teme_to_tod; */
  /* '<S150>:1:143' itrf_to_gcrf = mod_to_gcrf * tod_to_mod * pef_to_tod; */
  rtb_teme_to_gcrf[0] = cosf(T1900);
  rtb_teme_to_gcrf[3] = sinf(T1900);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sinf(T1900);
  rtb_teme_to_gcrf[4] = cosf(T1900);
  rtb_teme_to_gcrf[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_VectorConcatenate_2[lowAlt + 3 * LOS] = 0.0;
      rtb_VectorConcatenate_2[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS] *
        rtb_VectorConcatenate[lowAlt];
      rtb_VectorConcatenate_2[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 1] *
        rtb_VectorConcatenate[lowAlt + 3];
      rtb_VectorConcatenate_2[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 2] *
        rtb_VectorConcatenate[lowAlt + 6];
    }

    rtb_teme_to_gcrf[2 + 3 * lowAlt] = d[lowAlt];
  }

  /*  * itrf_to_pef;   % for vecef */
  /* '<S150>:1:144' ppef_to_vgcrf = itrf_to_gcrf * skew([0; 0; o_prec]); */
  /*  for w x recef */
  c_0[0] = cosf(-rtb_T_TT_J2000);
  c_0[3] = sinf(-rtb_T_TT_J2000);
  c_0[6] = 0.0;
  c_0[1] = -sinf(-rtb_T_TT_J2000);
  c_0[4] = cosf(-rtb_T_TT_J2000);
  c_0[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_VectorConcatenate_n[lowAlt + 3 * LOS] = 0.0;
      c_1[lowAlt + 3 * LOS] = 0.0;
      rtb_VectorConcatenate_n[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS] *
        rtb_VectorConcatenate_2[lowAlt];
      c_1[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS] *
        rtb_VectorConcatenate[lowAlt];
      rtb_VectorConcatenate_n[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS + 1]
        * rtb_VectorConcatenate_2[lowAlt + 3];
      c_1[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 1] *
        rtb_VectorConcatenate[lowAlt + 3];
      rtb_VectorConcatenate_n[lowAlt + 3 * LOS] += rtb_teme_to_gcrf[3 * LOS + 2]
        * rtb_VectorConcatenate_2[lowAlt + 6];
      c_1[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 2] *
        rtb_VectorConcatenate[lowAlt + 6];
    }

    c_0[2 + 3 * lowAlt] = d[lowAlt];
  }

  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_teme_to_gcrf[LOS + 3 * lowAlt] = 0.0;
      rtb_teme_to_gcrf[LOS + 3 * lowAlt] += c_0[3 * lowAlt] * c_1[LOS];
      rtb_teme_to_gcrf[LOS + 3 * lowAlt] += c_0[3 * lowAlt + 1] * c_1[LOS + 3];
      rtb_teme_to_gcrf[LOS + 3 * lowAlt] += c_0[3 * lowAlt + 2] * c_1[LOS + 6];
    }
  }

  /* End of MATLAB Function: '<S148>/MATLAB Function' */

  /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
   *  Inport: '<Root>/orbit_tle'
   *  MATLAB Function: '<S149>/time-conversion-lib'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  LOAD CONSTANTS */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'sgp4_lib_fsw/MATLAB Function': '<S145>:1' */
  /*  SGP4 Orbit Propagator */
  /*  UW Husky-Sat1 - ADCS Subsystem team */
  /*    Author:     Taylor Reynolds */
  /*    Updated:    7/3/18 */
  /*  Based on "SpaceTrack Report #3: Models for Propagation of NORAD Element */
  /*  Sets", Hoots & Roerhrich, 1980. */
  /*  */
  /*  TLE Format: */
  /*    Line 1: Column | Description */
  /*            01          line number of element data */
  /*            03-07       satellite number */
  /*            08          classification (u = unclassified) */
  /*            10-11       last two digits of launch year */
  /*            12-14       launch number of year */
  /*            15-17       piece of launch */
  /*            19-20       epoch year (last two digits) */
  /*            21-32       epoch (DOY and fractional portion of the day) */
  /*            34-43       first derivative of the mean motion */
  /*            45-52       second time derivative of mean motion (decimal */
  /*                            assumed) */
  /*            54-61       BS drag term (decimal point assumed) */
  /*            63          ephemeris type */
  /*            65-68       element number */
  /*            69          checksum (mod 10) */
  /*     */
  /*    Line 2: Column  | Description */
  /*            01          line number of element data */
  /*            03-07       satellite number */
  /*            09-16       inclination (degrees) */
  /*            18-25       right ascension of the ascending node (degrees) */
  /*            27-33       eccentricity (decimal assumed) */
  /*            35-42       argument of perigee (degrees) */
  /*            44-51       mean anomaly (degrees) */
  /*            53-63       mean motion (revs per day) */
  /*            64-68       revolution number at epoch (revs) */
  /*            69          checksum (mod 10) */
  /*  */
  /*  ----------------------------------------------------------------------- % */
  /*  Inputs: */
  /*    - orbit_tle: NORAD two line element set containing orbit info */
  /*    - tsince: Time since epoch of TLE in MINUTES */
  /*  Outputs: */
  /*    - pos_frame: Position vector in the TEME frame   */
  /*    - vel_frame: Velocity vector in the TEME frame */
  /*  */
  /*  Variables: */
  /*    - n_o = SGP type "mean" mean motion at epoch */
  /*    - e_o = the "mean" eccentricity at epoch */
  /*    - i_o = the "mean" inclination at epoch */
  /*    - M_o = the "mean" mean anomaly at epoch */
  /*    - w_o = the "mean" argument of perigee at epoch */
  /*    - Om_o = the "mean" longitude of ascending node at epoch */
  /*    - nd_o  = the TRoC of n_o */
  /*    - ndd_o = the TRoC of nd_o */
  /*    - Bs = the SGP4 type drag coefficient */
  /*    - k_e = gravitational constant of Earth, sqrtf(GM_e) */
  /*    - a_E = the equatorial radius of Earth in ER */
  /*    - J_2 = the second gravitation zonal harmonic of Earth */
  /*    - J_3 = the third gravitational zonal harmonic of Earth */
  /*    - J_4 = the fourth gravitational zonal harmonic of Earth */
  /*    - (t-t_0) = time since epoch */
  /*    - k_2   = 0.5*J_2*a_E^2 */
  /*    - k_4   = -(3/8)*J_4*a_E^4 */
  /*    - A_30  = -J_3*a_E^3 */
  /*    - q_0   = parameter for the SGP4 density function */
  /*    - s     = parameter for the SGP4 density function */
  /*  ----------------------------------------------------------------------- % */
  /*  Function Body */
  /* '<S145>:1:75' k_2     = 5.413080e-4; */
  /*  CK2 = 0.5*J_2*a_E^2 */
  /* '<S145>:1:76' k_4     = 0.62098875e-6; */
  /*  CK4 = -3/8*J_4*a_E^4 */
  /* '<S145>:1:77' QOMS4   = 1.88027916e-9; */
  days = 1.88027916E-9;

  /*  QOMS2T = (q_0 - s)^4 * ER^4 */
  /* '<S145>:1:78' s       = 1.01222928; */
  T1900 = 1.01222928;

  /*  S = s*ER */
  /* '<S145>:1:79' XJ3     = -0.2538815e-5; */
  /*  J_3 */
  /* '<S145>:1:80' XKE     = 0.743669161e-1; */
  /*  XKE = k_e*(ER/min)^{3/2} */
  /* '<S145>:1:81' XKMPER  = 6378.137; */
  /*  km/ER */
  /* '<S145>:1:82' XMNPDA  = 1440; */
  /*  time_units/day */
  /* '<S145>:1:83' a_E     = 1; */
  /*  distance units/ER */
  /* '<S145>:1:84' DE2RA   = 0.174532925e-1; */
  /*  radians/degree */
  /* '<S145>:1:85' TRUE    = 1; */
  /* '<S145>:1:86' FALSE   = 0; */
  /* '<S145>:1:87' r_E     = 6378.137; */
  /*  Earth's equatorial radius [km] */
  /* '<S145>:1:88' A_30    = -XJ3*a_E^3; */
  /*  see nomenclature */
  /* '<S145>:1:89' VKMPS   = XKMPER/60; */
  /*  Convert ER/min velocities to km/s */
  /* '<S145>:1:90' J2000   = 2451545.0; */
  /*  Julian date for J2000 */
  /* '<S145>:1:91' FLAG    = 0; */
  /* '<S145>:1:92' pos_invalid     = [ XKMPER; XKMPER; XKMPER + 500 ]; */
  /* '<S145>:1:93' vel_invalid     = [ 0.0; 0.0; 0.0 ]; */
  /*  ----------------------------------------------------------------------- % */
  /*  Line 1 */
  /* '<S145>:1:98' epoch_year  = orbit_tle(1); */
  /* '<S145>:1:99' epoch_JD    = orbit_tle(2); */
  /* '<S145>:1:100' BS          = orbit_tle(3); */
  /*  Line 2 */
  /* '<S145>:1:103' i_o     = orbit_tle(4)*DE2RA; */
  rtb_T_TT_J2000 = rtU.orbit_tle[3] * 0.0174532925;

  /*  Radians (INC) */
  /* '<S145>:1:104' Om_o    = orbit_tle(5)*DE2RA; */
  /*  Radians (RAAN) */
  /* '<S145>:1:105' e_o     = orbit_tle(6); */
  /*  Non-dimensional (ECC) */
  /* '<S145>:1:106' w_o     = orbit_tle(7)*DE2RA; */
  b_min = rtU.orbit_tle[6] * 0.0174532925;

  /*  Radians (AOP) */
  /* '<S145>:1:107' M_o     = orbit_tle(8)*DE2RA; */
  rtb_Switch = rtU.orbit_tle[7] * 0.0174532925;

  /*  Radians (MNA) */
  /* '<S145>:1:108' n_o     = orbit_tle(9)*2*pi/XMNPDA; */
  rtb_Sum = rtU.orbit_tle[8] * 2.0 * 3.1415926535897931 / 1440.0;

  /*  Revs per time unit (MNM)    */
  /*  Hooks on orbital elements (catch #1) */
  /*  eccentricity */
  /* '<S145>:1:112' if( (e_o >= 1) || (e_o <= 0) ) */
  if ((rtU.orbit_tle[5] >= 1.0) || (rtU.orbit_tle[5] <= 0.0)) {
    /* '<S145>:1:113' pos_teme_km       = pos_invalid; */
    /* '<S145>:1:114' vel_teme_kmps     = vel_invalid; */
    rtb_pos_teme_km[0] = 6378.137;
    deps_1980 = 0.0;
    rtb_pos_teme_km[1] = 6378.137;
    xi = 0.0;
    rtb_pos_teme_km[2] = 6878.137;
    b_o = 0.0;

    /* '<S145>:1:115' FLAG = -1; */
  } else {
    /*  mean motion (catch #1) */
    /* '<S145>:1:119' if( n_o == 0 ) */
    if (rtb_Sum == 0.0) {
      /* '<S145>:1:120' pos_teme_km       = pos_invalid; */
      /* '<S145>:1:121' vel_teme_kmps     = vel_invalid; */
      rtb_pos_teme_km[0] = 6378.137;
      deps_1980 = 0.0;
      rtb_pos_teme_km[1] = 6378.137;
      xi = 0.0;
      rtb_pos_teme_km[2] = 6878.137;
      b_o = 0.0;

      /* '<S145>:1:122' FLAG = -1; */
    } else {
      /*  Time since epoch (catch #1)                              */
      /* '<S145>:1:127' tsince_JD      = (JD_J2000_cur - epoch_JD); */
      JD_days_gps = ((JD_days_gps + -0.00020833333333333332) - 2.451545E+6) -
        rtU.orbit_tle[1];

      /* '<S145>:1:128' if( abs(tsince_JD) < 1e-8 || tsince_JD < 0) */
      if ((fabsf(JD_days_gps) < 1.0E-8) || (JD_days_gps < 0.0)) {
        /* '<S145>:1:129' tsince_JD   = 0; */
        JD_days_gps = 0.0;
      }

      /* '<S145>:1:132' tsince  = XMNPDA*tsince_JD; */
      rtb_Sum3 = 1440.0 * JD_days_gps;

      /* '<S145>:1:134' a_1  = (XKE/n_o)^(2/3); */
      JD_days_gps = rt_powd_snf(0.0743669161 / rtb_Sum, 0.66666666666666663);

      /* '<S145>:1:135' d_1  = 1.5*(k_2/(a_1^2))*((3*cosf(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
      rtb_z1 = cosf(rtb_T_TT_J2000);
      rtb_z1 = (rtb_z1 * rtb_z1 * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5]
        * rtU.orbit_tle[5], 1.5) * (0.000541308 / (JD_days_gps * JD_days_gps) *
        1.5);

      /* '<S145>:1:136' a_o  = a_1*(1 - (1/3)*d_1 - d_1^2 - (134/81)*d_1^3); */
      JD_days_gps *= ((1.0 - 0.33333333333333331 * rtb_z1) - rtb_z1 * rtb_z1) -
        1.654320987654321 * rt_powd_snf(rtb_z1, 3.0);

      /* '<S145>:1:137' d_o  = (3/2)*(k_2/(a_o^2))*((3*cosf(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
      rtb_z1 = cosf(rtb_T_TT_J2000);
      rtb_z1 = (rtb_z1 * rtb_z1 * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5]
        * rtU.orbit_tle[5], 1.5) * (0.000541308 / (JD_days_gps * JD_days_gps) *
        1.5);

      /* '<S145>:1:138' n_o_pp   = n_o/(1 + d_o); */
      rtb_Sum /= 1.0 + rtb_z1;

      /* '<S145>:1:139' a_o_pp   = a_o/(1 - d_o); */
      deps_1980 = JD_days_gps / (1.0 - rtb_z1);

      /* '<S145>:1:141' r_p     = r_E*(a_o_pp*(1 - e_o) - a_E); */
      JD_days_gps = ((1.0 - rtU.orbit_tle[5]) * deps_1980 - 1.0) * 6378.137;

      /*  Radius of perigee */
      /*  Check low altitude condition                                     */
      /* '<S145>:1:144' if( a_o_pp*(1-e_o)/a_E < (220/XKMPER + a_E) ) */
      if ((1.0 - rtU.orbit_tle[5]) * deps_1980 < 1.0344928307435228) {
        /* '<S145>:1:145' lowAlt  = TRUE; */
        lowAlt = 1;

        /* '<S145>:1:146' FLAG    = 1; */
      } else {
        /* '<S145>:1:147' else */
        /* '<S145>:1:148' lowAlt  = FALSE; */
        lowAlt = 0;
      }

      /*  Check if perigee is between [98,156]km or below 98km and adjust the */
      /*  constant s accordingly. */
      /* '<S145>:1:153' if( (r_p >= 98) && (r_p <= 156) ) */
      if ((JD_days_gps >= 98.0) && (JD_days_gps <= 156.0)) {
        /* '<S145>:1:154' s_star  = a_o_pp*(1 - e_o) - s + a_E; */
        T1900 = ((1.0 - rtU.orbit_tle[5]) * deps_1980 - 1.01222928) + 1.0;

        /* '<S145>:1:155' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
        days = rt_powd_snf(1.0188142770258546 - T1900, 4.0);

        /* '<S145>:1:156' s       = s_star; */
      } else {
        if (JD_days_gps < 98.0) {
          /* '<S145>:1:157' elseif( r_p < 98 ) */
          /* '<S145>:1:158' s_star  = 20/XKMPER + a_E; */
          /* '<S145>:1:159' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
          days = 6.0426196528035638E-8;

          /* '<S145>:1:160' s       = s_star; */
          T1900 = 1.0031357118857749;
        }
      }

      /*  Constants */
      /* '<S145>:1:164' tht     = cosf(i_o); */
      JD_days_gps = cosf(rtb_T_TT_J2000);

      /* '<S145>:1:165' xi      = 1/(a_o_pp - s); */
      xi = 1.0 / (deps_1980 - T1900);

      /*  cannot be zero since s_star = s ~= a_o_pp */
      /* '<S145>:1:166' b_o     = (1 - e_o^2)^(0.5); */
      b_o = sqrtf(1.0 - rtU.orbit_tle[5] * rtU.orbit_tle[5]);

      /* '<S145>:1:167' eta     = a_o_pp*e_o*xi; */
      eta = deps_1980 * rtU.orbit_tle[5] * xi;

      /*  cannot be zero, but could be 1. */
      /* '<S145>:1:168' if( eta == 1 ) */
      if (eta == 1.0) {
        /*  check that (1-eta^2) cannot be zero (catch #2) */
        /* '<S145>:1:169' pos_teme_km     = pos_invalid; */
        /* '<S145>:1:170' vel_teme_kmps   = vel_invalid; */
        rtb_pos_teme_km[0] = 6378.137;
        deps_1980 = 0.0;
        rtb_pos_teme_km[1] = 6378.137;
        xi = 0.0;
        rtb_pos_teme_km[2] = 6878.137;
        b_o = 0.0;

        /* '<S145>:1:171' FLAG = -1; */
      } else {
        /* '<S145>:1:174' C_2     = QOMS4*xi^4*n_o_pp*(1-eta^2)^(-7/2)*((a_o_pp*(1 + ... */
        /* '<S145>:1:175'             (3/2)*eta^2 + 4*e_o*eta + e_o*eta^3)) + ... */
        /* '<S145>:1:176'             (3/2)*k_2*xi*(-0.5+1.5*tht^2)*(8+24*eta^2+3*eta^4)/(1-eta^2)); */
        /* '<S145>:1:177' C_1     = BS*C_2; */
        C_1 = ((((eta * eta * 1.5 + 1.0) + 4.0 * rtU.orbit_tle[5] * eta) +
                rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * deps_1980 +
               (JD_days_gps * JD_days_gps * 1.5 + -0.5) *
               (0.00081196200000000006 * xi) * ((eta * eta * 24.0 + 8.0) + 3.0 *
                rt_powd_snf(eta, 4.0)) / (1.0 - eta * eta)) * (days *
          rt_powd_snf(xi, 4.0) * rtb_Sum * rt_powd_snf(1.0 - eta * eta, -3.5)) *
          rtU.orbit_tle[2];

        /* '<S145>:1:178' C_3     = (QOMS4*xi^5*A_30*n_o_pp*a_E*sinf(i_o))/(k_2*e_o); */
        /* '<S145>:1:179' C_4     = 2*n_o_pp*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)* ... */
        /* '<S145>:1:180'             ((2*eta*(1+e_o*eta) + 0.5*e_o + 0.5*eta^3) - ... */
        /* '<S145>:1:181'             (2*k_2*xi)/(a_o_pp*(1-eta^2)) * (3*(1-3*tht^2)* ... */
        /* '<S145>:1:182'             (1-(3/2)*eta^2-2*e_o*eta-0.5*e_o*eta^3) + (3/4)*(1-tht^2)* ... */
        /* '<S145>:1:183'             (2*eta^2-e_o*eta-e_o*eta^3)*cosf(2*w_o))); */
        C_4 = ((((rtU.orbit_tle[5] * eta + 1.0) * (2.0 * eta) + 0.5 *
                 rtU.orbit_tle[5]) + 0.5 * rt_powd_snf(eta, 3.0)) - ((((1.0 -
                   eta * eta * 1.5) - 2.0 * rtU.orbit_tle[5] * eta) - 0.5 *
                 rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * ((1.0 - JD_days_gps
                  * JD_days_gps * 3.0) * 3.0) + ((eta * eta * 2.0 -
                  rtU.orbit_tle[5] * eta) - rtU.orbit_tle[5] * rt_powd_snf(eta,
                  3.0)) * ((1.0 - JD_days_gps * JD_days_gps) * 0.75) * cosf(2.0 *
                 b_min)) * (0.001082616 * xi / ((1.0 - eta * eta) * deps_1980)))
          * (2.0 * rtb_Sum * days * rt_powd_snf(xi, 4.0) * deps_1980 * (b_o *
              b_o) * rt_powd_snf(1.0 - eta * eta, -3.5));

        /* '<S145>:1:184' C_5     = 0; */
        C_5 = 0.0;

        /* '<S145>:1:185' D_2     = 0; */
        D_2 = 0.0;

        /* '<S145>:1:186' D_3     = 0; */
        D_3 = 0.0;

        /* '<S145>:1:187' D_4     = 0; */
        D_4 = 0.0;

        /* '<S145>:1:188' IL2     = 1.5*C_1; */
        IL2 = 1.5 * C_1;

        /* '<S145>:1:189' IL3     = 0; */
        IL3 = 0.0;

        /* '<S145>:1:190' IL4     = 0; */
        IL4 = 0.0;

        /* '<S145>:1:191' IL5     = 0; */
        IL5 = 0.0;

        /* '<S145>:1:193' if( lowAlt  == FALSE ) */
        if (lowAlt == 0) {
          /* '<S145>:1:194' C_5     = 2*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)*... */
          /* '<S145>:1:195'                                     (1+(11/4)*eta*(eta+e_o) + e_o*eta^3); */
          C_5 = 2.0 * days * rt_powd_snf(xi, 4.0) * deps_1980 * (b_o * b_o) *
            rt_powd_snf(1.0 - eta * eta, -3.5) * ((2.75 * eta * (eta +
            rtU.orbit_tle[5]) + 1.0) + rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0));

          /* '<S145>:1:196' D_2     = 4*a_o_pp*xi*C_1^2; */
          D_2 = 4.0 * deps_1980 * xi * (C_1 * C_1);

          /* '<S145>:1:197' D_3     = (4/3)*a_o_pp*xi^2*(17*a_o_pp + s)*C_1^3; */
          D_3 = 1.3333333333333333 * deps_1980 * (xi * xi) * (17.0 * deps_1980 +
            T1900) * rt_powd_snf(C_1, 3.0);

          /* '<S145>:1:198' D_4     = (2/3)*a_o_pp*xi^3*(221*a_o_pp + 31*s)*C_1^4; */
          D_4 = 0.66666666666666663 * deps_1980 * rt_powd_snf(xi, 3.0) * (221.0 *
            deps_1980 + 31.0 * T1900) * rt_powd_snf(C_1, 4.0);

          /* '<S145>:1:199' IL3     = D_2 + 2*C_1^2; */
          IL3 = C_1 * C_1 * 2.0 + D_2;

          /* '<S145>:1:200' IL4     = 0.25*(3*D_3 + 12*C_1*D_2 + 10*C_1^3); */
          IL4 = ((12.0 * C_1 * D_2 + 3.0 * D_3) + 10.0 * rt_powd_snf(C_1, 3.0)) *
            0.25;

          /* '<S145>:1:201' IL5     = 0.2*(3*D_4 + 12*C_1*D_3 + 6*D_2^2 + 30*C_1^2*D_2 + 15*C_1^4); */
          IL5 = ((((12.0 * C_1 * D_3 + 3.0 * D_4) + D_2 * D_2 * 6.0) + C_1 * C_1
                  * 30.0 * D_2) + 15.0 * rt_powd_snf(C_1, 4.0)) * 0.2;
        }

        /*  Secular effects of atm drag and gravitation. Note that a_o_pp ~= 0 and */
        /*  b_o ~= 0 so no denoms here are zero. */
        /* '<S145>:1:206' M_DF    = M_o + ( 1 + (3*k_2*(3*tht^2-1))/(2*a_o_pp^2*b_o^3) + ... */
        /* '<S145>:1:207'             (3*k_2^2*(13-78*tht^2+137*tht^4))/(16*a_o_pp^4*b_o^7) )* ... */
        /* '<S145>:1:208'             n_o_pp*tsince; */
        M_DF = (((JD_days_gps * JD_days_gps * 3.0 - 1.0) * 0.0016239240000000001
                 / (deps_1980 * deps_1980 * 2.0 * rt_powd_snf(b_o, 3.0)) + 1.0)
                + ((13.0 - JD_days_gps * JD_days_gps * 78.0) + 137.0 *
                   rt_powd_snf(JD_days_gps, 4.0)) * 8.7904305259200008E-7 /
                (16.0 * rt_powd_snf(deps_1980, 4.0) * rt_powd_snf(b_o, 7.0))) *
          rtb_Sum * rtb_Sum3 + rtb_Switch;

        /* '<S145>:1:209' w_DF    = w_o + ( (-3*k_2*(1-5*tht^2))/(2*a_o_pp^2*b_o^3) + ... */
        /* '<S145>:1:210'             (3*k_2^2*(7-114*tht^2+395*tht^4))/(16*a_o_pp^4*b_o^8) + ... */
        /* '<S145>:1:211'             (5*k_4*(3-36*tht^2+49*tht^4))/(4*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
        rtb_z1 = ((((7.0 - JD_days_gps * JD_days_gps * 114.0) + 395.0 *
                    rt_powd_snf(JD_days_gps, 4.0)) * 8.7904305259200008E-7 /
                   (16.0 * rt_powd_snf(deps_1980, 4.0) * rt_powd_snf(b_o, 8.0))
                   + (1.0 - JD_days_gps * JD_days_gps * 5.0) *
                   -0.0016239240000000001 / (deps_1980 * deps_1980 * 2.0 *
                    rt_powd_snf(b_o, 3.0))) + ((3.0 - JD_days_gps * JD_days_gps *
                    36.0) + 49.0 * rt_powd_snf(JD_days_gps, 4.0)) *
                  3.1049437500000002E-6 / (4.0 * rt_powd_snf(deps_1980, 4.0) *
                   rt_powd_snf(b_o, 8.0))) * rtb_Sum * rtb_Sum3 + b_min;

        /* '<S145>:1:212' Om_DF   = Om_o + ( (-3*k_2*tht)/(a_o_pp^2*b_o^4) +... */
        /* '<S145>:1:213'             (3*k_2^2*(4*tht-19*tht^3))/(2*a_o_pp^4*b_o^8) +... */
        /* '<S145>:1:214'             (5*k_4*tht*(3-7*tht^2))/(2*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
        /* '<S145>:1:216' Om      = Om_DF - (21/2)*( (n_o_pp*k_2*tht)/(a_o_pp^2*b_o^2) )*C_1*tsince^2; */
        T1900 = ((((4.0 * JD_days_gps - 19.0 * rt_powd_snf(JD_days_gps, 3.0)) *
                   8.7904305259200008E-7 / (2.0 * rt_powd_snf(deps_1980, 4.0) *
                    rt_powd_snf(b_o, 8.0)) + -0.0016239240000000001 *
                   JD_days_gps / (deps_1980 * deps_1980 * rt_powd_snf(b_o, 4.0)))
                  + (3.0 - JD_days_gps * JD_days_gps * 7.0) *
                  (3.1049437500000002E-6 * JD_days_gps) / (2.0 * rt_powd_snf
                   (deps_1980, 4.0) * rt_powd_snf(b_o, 8.0))) * rtb_Sum *
                 rtb_Sum3 + rtU.orbit_tle[4] * 0.0174532925) - rtb_Sum *
          0.000541308 * JD_days_gps / (deps_1980 * deps_1980 * (b_o * b_o)) *
          10.5 * C_1 * (rtb_Sum3 * rtb_Sum3);

        /* '<S145>:1:218' if( lowAlt == FALSE ) */
        if (lowAlt == 0) {
          /*  #5 %     */
          /* '<S145>:1:219' d_w     = BS*C_3*cosf(w_o)*tsince; */
          b_min = days * rt_powd_snf(xi, 5.0) * 2.538815E-6 * rtb_Sum * sinf
            (rtb_T_TT_J2000) / (0.000541308 * rtU.orbit_tle[5]) * rtU.orbit_tle
            [2] * cosf(b_min) * rtb_Sum3;

          /* '<S145>:1:220' d_M     = -(2/3)*QOMS4*BS*xi^4*(a_E/(e_o*eta))* ... */
          /* '<S145>:1:221'                 ( (1+eta*cosf(M_DF))^3 - (1 + eta*cosf(M_o))^3 ); */
          days = -0.66666666666666663 * days * rtU.orbit_tle[2] * rt_powd_snf(xi,
            4.0) * (1.0 / (rtU.orbit_tle[5] * eta)) * (rt_powd_snf(eta * cosf
            (M_DF) + 1.0, 3.0) - rt_powd_snf(eta * cosf(rtb_Switch) + 1.0, 3.0));

          /* '<S145>:1:222' M_p     = M_DF + d_w + d_M; */
          M_DF = (M_DF + b_min) + days;

          /* '<S145>:1:223' w       = w_DF - d_w - d_M; */
          rtb_z1 = (rtb_z1 - b_min) - days;

          /* '<S145>:1:224' e       = e_o - BS*C_4*tsince - BS*C_5*( sinf(M_p) - sinf(M_o) ); */
          days = (rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * rtb_Sum3) -
            rtU.orbit_tle[2] * C_5 * (sinf(M_DF) - sinf(rtb_Switch));

          /* '<S145>:1:225' a       = a_o_pp*( 1 - C_1*tsince - D_2*tsince^2 - D_3*tsince^3 - ... */
          /* '<S145>:1:226'                 D_4*tsince^4 )^2; */
          rtb_Switch = (((1.0 - C_1 * rtb_Sum3) - rtb_Sum3 * rtb_Sum3 * D_2) -
                        D_3 * rt_powd_snf(rtb_Sum3, 3.0)) - D_4 * rt_powd_snf
            (rtb_Sum3, 4.0);
          rtb_Switch = rtb_Switch * rtb_Switch * deps_1980;

          /* '<S145>:1:227' L       = M_p + w + Om + n_o_pp*( IL2*tsince^2 + IL3*tsince^3 +... */
          /* '<S145>:1:228'             IL4*tsince^4 + IL5*tsince^5 ); */
          rtb_Sum3 = (((rtb_Sum3 * rtb_Sum3 * IL2 + IL3 * rt_powd_snf(rtb_Sum3,
            3.0)) + IL4 * rt_powd_snf(rtb_Sum3, 4.0)) + IL5 * rt_powd_snf
                      (rtb_Sum3, 5.0)) * rtb_Sum + ((M_DF + rtb_z1) + T1900);
        } else {
          /* '<S145>:1:229' else */
          /* '<S145>:1:230' M_p     = M_DF; */
          /* '<S145>:1:231' w       = w_DF; */
          /* '<S145>:1:232' a       = a_o_pp*(1 - C_1*tsince)^2; */
          rtb_Switch = 1.0 - C_1 * rtb_Sum3;
          rtb_Switch = rtb_Switch * rtb_Switch * deps_1980;

          /* '<S145>:1:233' e       = e_o - BS*C_4*tsince; */
          days = rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * rtb_Sum3;

          /* '<S145>:1:234' L       = M_p + w + Om + n_o_pp*IL2*tsince^2; */
          rtb_Sum3 = ((M_DF + rtb_z1) + T1900) + rtb_Sum * IL2 * (rtb_Sum3 *
            rtb_Sum3);
        }

        /* '<S145>:1:237' if( (a == 0) || (e >= 1) ) */
        if ((rtb_Switch == 0.0) || (days >= 1.0)) {
          /*  catch divide by zero conditions (catch #3) */
          /* '<S145>:1:238' pos_teme_km       = pos_invalid; */
          /* '<S145>:1:239' vel_teme_kmps     = vel_invalid; */
          rtb_pos_teme_km[0] = 6378.137;
          deps_1980 = 0.0;
          rtb_pos_teme_km[1] = 6378.137;
          xi = 0.0;
          rtb_pos_teme_km[2] = 6878.137;
          b_o = 0.0;

          /* '<S145>:1:240' FLAG = -1; */
        } else {
          /* '<S145>:1:243' b       = sqrtf(1-e^2); */
          IL2 = sqrtf(1.0 - days * days);

          /* '<S145>:1:244' n       = XKE/(a^(3/2)); */
          b_min = 0.0743669161 / rt_powd_snf(rtb_Switch, 1.5);

          /*  Add the long-period periodic terms */
          /* '<S145>:1:247' a_xN    = e*cosf(w); */
          rtb_Sum = days * cosf(rtb_z1);

          /* '<S145>:1:248' L_L     = (A_30*sinf(i_o)/(8*k_2*a*b^2))*e*cosf(w)*((3+5*tht)/(1+tht)); */
          /* '<S145>:1:249' a_yNL   = A_30*sinf(i_o)/(4*k_2*a*b^2); */
          /* '<S145>:1:250' L_T     = L + L_L; */
          /* '<S145>:1:251' a_yN    = e*sinf(w) + a_yNL; */
          deps_1980 = 2.538815E-6 * sinf(rtb_T_TT_J2000) / (0.002165232 *
            rtb_Switch * (IL2 * IL2)) + days * sinf(rtb_z1);

          /* '<S145>:1:253' U       = mod(L_T - Om,2*pi); */
          rtb_Sum3 = ((2.538815E-6 * sinf(rtb_T_TT_J2000) / (0.004330464 *
            rtb_Switch * (IL2 * IL2)) * days * cosf(rtb_z1) * ((5.0 * JD_days_gps
            + 3.0) / (1.0 + JD_days_gps)) + rtb_Sum3) - T1900) /
            6.2831853071795862;
          if (fabsf(rtb_Sum3 - rt_roundd_snf(rtb_Sum3)) <= 2.2204460492503131E-16
              * fabsf(rtb_Sum3)) {
            rtb_Sum3 = 0.0;
          } else {
            rtb_Sum3 = (rtb_Sum3 - floorf(rtb_Sum3)) * 6.2831853071795862;
          }

          /* '<S145>:1:254' Epw     = kepler(U,a_yN,a_xN); */
          /*  Solves Kepler's equation for Epw = E + w using iterative method */
          /* '<S145>:1:304' tol         = 1e-8; */
          /* '<S145>:1:305' iter_max    = 10; */
          /* '<S145>:1:306' Epw         = U; */
          rtb_z1 = rtb_Sum3;

          /* '<S145>:1:307' Epw_new     = 0; */
          days = 0.0;

          /* '<S145>:1:308' iter        = 0; */
          lowAlt = 0;

          /* '<S145>:1:310' while( (abs(Epw - Epw_new) > tol) && (iter < iter_max) ) */
          while ((fabsf(rtb_z1 - days) > 1.0E-8) && (lowAlt < 10)) {
            /* '<S145>:1:311' Epw     = Epw_new; */
            rtb_z1 = days;

            /* '<S145>:1:312' dEpw    = (U-a_yN*cosf(Epw) + a_xN*sinf(Epw) - Epw)/ ... */
            /* '<S145>:1:313'                     (-a_yN*sinf(Epw) - a_xN*cosf(Epw) + 1); */
            /* '<S145>:1:314' Epw_new = Epw + dEpw; */
            days += (((rtb_Sum3 - deps_1980 * cosf(days)) + rtb_Sum * sinf(days))
                     - days) / ((-deps_1980 * sinf(days) - rtb_Sum * cosf(days)) +
                                1.0);

            /* '<S145>:1:315' iter    = iter +1; */
            lowAlt++;
          }

          /* '<S145>:1:318' Epw = Epw_new; */
          /*  Preliminary quantities for short-period periodics */
          /* '<S145>:1:257' ecE     = a_xN*cosf(Epw) + a_yN*sinf(Epw); */
          /* '<S145>:1:258' esE     = a_xN*sinf(Epw) - a_yN*cosf(Epw); */
          IL3 = rtb_Sum * sinf(days) - deps_1980 * cosf(days);

          /* '<S145>:1:259' e_L     = (a_xN^2 + a_yN^2)^(1/2); */
          rtb_z1 = sqrtf(rtb_Sum * rtb_Sum + deps_1980 * deps_1980);

          /* '<S145>:1:260' p_L     = a*(1 - e_L^2); */
          IL2 = (1.0 - rtb_z1 * rtb_z1) * rtb_Switch;

          /* '<S145>:1:261' r       = a*(1 - ecE); */
          rtb_Sum3 = (1.0 - (rtb_Sum * cosf(days) + deps_1980 * sinf(days))) *
            rtb_Switch;

          /* '<S145>:1:262' if( (r == 0) || (p_L == 0) ) */
          if ((rtb_Sum3 == 0.0) || (IL2 == 0.0)) {
            /*  (catch #4) */
            /* '<S145>:1:263' pos_teme_km       = pos_invalid; */
            /* '<S145>:1:264' vel_teme_kmps     = vel_invalid; */
            rtb_pos_teme_km[0] = 6378.137;
            deps_1980 = 0.0;
            rtb_pos_teme_km[1] = 6378.137;
            xi = 0.0;
            rtb_pos_teme_km[2] = 6878.137;
            b_o = 0.0;

            /* '<S145>:1:265' FLAG = -1; */
          } else {
            /* '<S145>:1:268' r_dot   = XKE*sqrtf(a)*esE/r; */
            /* '<S145>:1:269' r_f_dot = XKE*sqrtf(p_L)/r; */
            /* '<S145>:1:270' cosu    = (a/r)*( cosf(Epw) - a_xN + ((a_yN*esE)/(1+sqrtf(1 - e_L^2))) ); */
            /* '<S145>:1:271' sinu    = (a/r)*( sinf(Epw) - a_yN - ((a_xN*esE)/(1 + sqrtf(1 - e_L^2))) ); */
            /* '<S145>:1:272' u       = atan2f(sinu,cosu); */
            days = rt_atan2d_snf(((sinf(days) - deps_1980) - rtb_Sum * IL3 /
                                  (sqrtf(1.0 - rtb_z1 * rtb_z1) + 1.0)) *
                                 (rtb_Switch / rtb_Sum3), (deps_1980 * IL3 /
              (sqrtf(1.0 - rtb_z1 * rtb_z1) + 1.0) + (cosf(days) - rtb_Sum)) *
                                 (rtb_Switch / rtb_Sum3));

            /* '<S145>:1:273' dr      = (k_2/(2*p_L))*(1 - tht^2)*cosf(2*u); */
            /* '<S145>:1:274' du      = -(k_2/(4*p_L^2))*(7*tht^2 - 1)*sinf(2*u); */
            /* '<S145>:1:275' dOm     = (3*k_2*tht/(2*p_L^2))*sinf(2*u); */
            /* '<S145>:1:276' di      = (3*k_2*tht/(2*p_L^2))*sinf(i_o)*cosf(2*u); */
            /* '<S145>:1:277' dr_dot  = -((k_2*n)/p_L)*(1-tht^2)*sinf(2*u); */
            /* '<S145>:1:278' dr_f_dot    = (k_2*n/p_L)*( (1-tht^2)*cosf(2*u) - (3/2)*(1-3*tht^2) ); */
            /*  Add the short-period periodics to get osculating elements */
            /* '<S145>:1:281' r_k     = r*(1 - (3/2)*k_2*sqrtf(1 - e_L^2)*(3*tht^2 - 1)/p_L^2) + dr; */
            rtb_z1 = (1.0 - sqrtf(1.0 - rtb_z1 * rtb_z1) * 0.00081196200000000006
                      * (JD_days_gps * JD_days_gps * 3.0 - 1.0) / (IL2 * IL2)) *
              rtb_Sum3 + 0.000541308 / (2.0 * IL2) * (1.0 - JD_days_gps *
              JD_days_gps) * cosf(2.0 * days);

            /* '<S145>:1:282' u_k     = u + du; */
            rtb_Sum = -(0.000541308 / (IL2 * IL2 * 4.0)) * (JD_days_gps *
              JD_days_gps * 7.0 - 1.0) * sinf(2.0 * days) + days;

            /* '<S145>:1:283' Om_k    = Om + dOm; */
            T1900 += 0.0016239240000000001 * JD_days_gps / (IL2 * IL2 * 2.0) *
              sinf(2.0 * days);

            /* '<S145>:1:284' i_k     = i_o + di; */
            deps_1980 = 0.0016239240000000001 * JD_days_gps / (IL2 * IL2 * 2.0) *
              sinf(rtb_T_TT_J2000) * cosf(2.0 * days) + rtb_T_TT_J2000;

            /* '<S145>:1:285' r_dot_k     = r_dot + dr_dot; */
            rtb_T_TT_J2000 = -(0.000541308 * b_min / IL2) * (1.0 - JD_days_gps *
              JD_days_gps) * sinf(2.0 * days) + 0.0743669161 * sqrtf(rtb_Switch) *
              IL3 / rtb_Sum3;

            /* '<S145>:1:286' r_f_dot_k   = r_f_dot + dr_f_dot; */
            JD_days_gps = ((1.0 - JD_days_gps * JD_days_gps) * cosf(2.0 * days) -
                           (1.0 - JD_days_gps * JD_days_gps * 3.0) * 1.5) *
              (0.000541308 * b_min / IL2) + 0.0743669161 * sqrtf(IL2) / rtb_Sum3;

            /*  Create unit orientation vectors */
            /* '<S145>:1:289' M       = [ -sinf(Om_k)*cosf(i_k); cosf(Om_k)*cosf(i_k); sinf(i_k)]; */
            M[0] = -sinf(T1900) * cosf(deps_1980);
            M[1] = cosf(T1900) * cosf(deps_1980);
            M[2] = sinf(deps_1980);

            /* '<S145>:1:290' N       = [ cosf(Om_k); sinf(Om_k); 0 ]; */
            eta = cosf(T1900);
            C_1 = sinf(T1900);

            /* '<S145>:1:292' U       = M.*sinf(u_k) + N.*cosf(u_k); */
            T1900 = sinf(rtb_Sum);
            rtb_Sum3 = cosf(rtb_Sum);

            /* '<S145>:1:293' V       = M.*cosf(u_k) - N.*sinf(u_k); */
            days = M[0] * T1900 + eta * rtb_Sum3;
            rtb_pos_teme_km[0] = rtb_z1 * days * 6378.137;
            deps_1980 = days;
            days = M[1] * T1900 + C_1 * rtb_Sum3;
            rtb_pos_teme_km[1] = rtb_z1 * days * 6378.137;
            xi = days;
            days = M[2] * T1900 + 0.0 * rtb_Sum3;
            rtb_pos_teme_km[2] = rtb_z1 * days * 6378.137;
            T1900 = cosf(rtb_Sum);
            rtb_Sum3 = sinf(rtb_Sum);

            /*  Find position and velocity in km & km/s */
            /* '<S145>:1:296' pos_teme_km     = r_k.*U.*XKMPER; */
            /* '<S145>:1:297' vel_teme_kmps   = (r_dot_k.*U + r_f_dot_k.*V).*(VKMPS); */
            deps_1980 = ((M[0] * T1900 - eta * rtb_Sum3) * JD_days_gps +
                         rtb_T_TT_J2000 * deps_1980) * 106.30228333333334;
            xi = ((M[1] * T1900 - C_1 * rtb_Sum3) * JD_days_gps + rtb_T_TT_J2000
                  * xi) * 106.30228333333334;
            b_o = ((M[2] * T1900 - 0.0 * rtb_Sum3) * JD_days_gps +
                   rtb_T_TT_J2000 * days) * 106.30228333333334;
          }
        }
      }
    }
  }

  /* End of MATLAB Function: '<S5>/MATLAB Function' */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    /* Gain: '<S5>/Gain' incorporates:
     *  Product: '<S5>/Product'
     */
    M[lowAlt] = 1000.0 * (rtb_teme_to_gcrf[lowAlt + 6] * rtb_pos_teme_km[2] +
                          (rtb_teme_to_gcrf[lowAlt + 3] * rtb_pos_teme_km[1] +
      rtb_teme_to_gcrf[lowAlt] * rtb_pos_teme_km[0]));
  }

  /*  Constants */
  /* MATLAB Function 'gs_prediction/gs_prediction_lib/MATLAB Function1': '<S11>:1' */
  /* '<S11>:1:4' M2KM        = 1000; */
  /* '<S11>:1:5' ECCE2       = 0.006694385000; */
  /* '<S11>:1:6' RE          = 6378.137; */
  /* '<S11>:1:7' TRUE        = 1; */
  /* '<S11>:1:8' FALSE       = 0; */
  /*  ----- */
  /*  Main Function */
  /*  Remnant from sun_vector, may not be relevant here? */
  /*  gs_eci_km(3)  = gs_eci_km(3)/(sqrtf(1 - ECCE2)); */
  /*  sc_pos_eci_km(3)        = sc_pos_eci_km(3)/(sqrtf(1 - ECCE2)); */
  /* '<S11>:1:17' gs_vec     = gs_ecef_m; */
  /* '<S11>:1:18' sc_vec      = sc_pos_ecef_m; */
  /* In ECI coordinates */
  /* '<S11>:1:21' sc2gs_unit  = (gs_vec - sc_vec)/norm(gs_vec - sc_vec); */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    /* Product: '<S3>/Product3' incorporates:
     *  Math: '<S3>/Math Function1'
     */
    JD_days_gps = rtb_VectorConcatenate_n[3 * lowAlt + 2] * M[2] +
      (rtb_VectorConcatenate_n[3 * lowAlt + 1] * M[1] + rtb_VectorConcatenate_n
       [3 * lowAlt] * M[0]);

    /* MATLAB Function: '<S9>/MATLAB Function1' */
    rtb_Sum_j_0[lowAlt] = rtb_Sum_g[lowAlt] - JD_days_gps;

    /* Product: '<S3>/Product3' incorporates:
     *  Math: '<S3>/Math Function1'
     */
    rtb_pos_teme_km[lowAlt] = JD_days_gps;
  }

  /* MATLAB Function: '<S9>/MATLAB Function1' */
  rtb_T_TT_J2000 = norm(rtb_Sum_j_0);

  /*  Determine if the sc is above the horizon in Seattle */
  /* '<S11>:1:25' sc2gs_gs_dot  = dot(sc2gs_unit,gs_vec)/norm(gs_vec); */
  rtb_Sum = (rtb_Sum_g[0] - rtb_pos_teme_km[0]) / rtb_T_TT_J2000;
  T1900 = rtb_Sum * rtb_Sum_g[0];
  eta = rtb_Sum;
  rtb_Sum = (rtb_Sum_g[1] - rtb_pos_teme_km[1]) / rtb_T_TT_J2000;
  T1900 += rtb_Sum * rtb_Sum_g[1];
  C_1 = rtb_Sum;
  rtb_Sum = (rtb_Sum_g[2] - rtb_pos_teme_km[2]) / rtb_T_TT_J2000;
  T1900 += rtb_Sum * rtb_Sum_g[2];

  /*  Check LoS */
  /* '<S11>:1:28' if sc2gs_gs_dot < 0 */
  if (T1900 / norm(rtb_Sum_g) < 0.0) {
    /* '<S11>:1:29' ABOVE     = TRUE; */
    lowAlt = 1;
  } else {
    /* '<S11>:1:30' else */
    /* '<S11>:1:31' ABOVE     = FALSE; */
    lowAlt = 0;
  }

  /* '<S11>:1:34' sc_above_gs = ABOVE; */
  /* '<S11>:1:36' pr = sc_vec - gs_vec; */
  rtb_pos_teme_km[0] -= rtb_Sum_g[0];
  rtb_pos_teme_km[1] -= rtb_Sum_g[1];
  JD_days_gps = rtb_pos_teme_km[2] - rtb_Sum_g[2];
  rtb_pos_teme_km[2] = JD_days_gps;

  /* '<S11>:1:37' fs = norm(pr)*cosd(fov_ang) - gs_vec'*pr/norm(gs_vec); */
  /* '<S11>:1:38' if (fs < 0) */
  if (norm(rtb_pos_teme_km) * 0.86602540378443871 - ((rtb_Sum_g[0] *
        rtb_pos_teme_km[0] + rtb_Sum_g[1] * rtb_pos_teme_km[1]) + rtb_Sum_g[2] *
       JD_days_gps) / norm(rtb_Sum_g) < 0.0) {
    /* '<S11>:1:39' sc_in_fov = TRUE; */
    LOS = 1;
  } else {
    /* '<S11>:1:40' else */
    /* '<S11>:1:41' sc_in_fov = FALSE; */
    LOS = 0;
  }

  /* Outport: '<Root>/sc_above_gs' incorporates:
   *  MATLAB Function: '<S9>/MATLAB Function1'
   */
  rtY.sc_above_gs = lowAlt;

  /* Outport: '<Root>/sc_in_fov' incorporates:
   *  MATLAB Function: '<S9>/MATLAB Function1'
   */
  rtY.sc_in_fov = LOS;

  /* MATLAB Function: '<S6>/MATLAB Function' */
  /*  ----------------------------------------------------------------------- % */
  /*  UW HuskySat-1, ADCS Team */
  /*  Algorithm to find the sun vector in the ECI frame. Based on algorithm 29  */
  /*    from Vallado 4e (pp. 279-280). */
  /*  */
  /*  T. Reynolds -- 2.23.18 */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'sun_vector_lib/MATLAB Function': '<S146>:1' */
  /*  Constants */
  /* '<S146>:1:11' AU2KM   = 149597870.700; */
  /* '<S146>:1:12' DEG2RAD     = pi/180.0; */
  /* '<S146>:1:13' TWOPI   = 2*pi; */
  /*  Mean longitude in a mean-equator of date frame */
  /* '<S146>:1:16' mean_long_sun  = rem(280.460 + 36000.771*JD_cent_ut1,360); */
  /*  [deg] */
  /*  Mean anomaly for the sun. Low precision formula since we're approximating */
  /*  T_TDB with T_UT1. */
  /* '<S146>:1:20' mean_anom_sun       = rem(357.5291092 + 35999.05034*JD_cent_ut1,360); */
  /* [deg] */
  /* '<S146>:1:21' mean_anom_sun_rad   = mod(DEG2RAD*mean_anom_sun,TWOPI); */
  T1900 = rt_remd_snf(35999.05034 * rtb_Abs1 + 357.5291092, 360.0) *
    0.017453292519943295 / 6.2831853071795862;
  if (fabsf(T1900 - rt_roundd_snf(T1900)) <= 2.2204460492503131E-16 * fabsf(T1900))
  {
    T1900 = 0.0;
  } else {
    T1900 = (T1900 - floorf(T1900)) * 6.2831853071795862;
  }

  /*  Approximate the ecliptic longitude of the sun */
  /* '<S146>:1:24' long_eclp       = mean_long_sun + 1.914666471*sinf(mean_anom_sun_rad) + ... */
  /* '<S146>:1:25'                     0.019994643*sinf(2*mean_anom_sun_rad); */
  /* [deg] */
  /* '<S146>:1:26' long_eclp       = rem(DEG2RAD*long_eclp,360); */
  /*  [rad]    */
  /* '<S146>:1:27' long_eclp_rad   = mod(DEG2RAD*long_eclp,TWOPI); */
  rtb_T_TT_J2000 = rt_remd_snf(((rt_remd_snf(36000.771 * rtb_Abs1 + 280.46,
    360.0) + 1.914666471 * sinf(T1900)) + sinf(2.0 * T1900) * 0.019994643) *
    0.017453292519943295, 360.0) * 0.017453292519943295 / 6.2831853071795862;
  if (fabsf(rtb_T_TT_J2000 - rt_roundd_snf(rtb_T_TT_J2000)) <=
      2.2204460492503131E-16 * fabsf(rtb_T_TT_J2000)) {
    rtb_T_TT_J2000 = 0.0;
  } else {
    rtb_T_TT_J2000 = (rtb_T_TT_J2000 - floorf(rtb_T_TT_J2000)) *
      6.2831853071795862;
  }

  /*  Assume the ecliptic latitude is zero */
  /* '<S146>:1:30' lat_eclp     = 0; */
  /* [deg] */
  /*  Approximate the obliquity of the ecliptic using */
  /* '<S146>:1:33' oblq        = 23.439291 - 0.0130042*JD_cent_ut1; */
  /* '<S146>:1:34' oblq_rad    = DEG2RAD*oblq; */
  JD_days_gps = (23.439291 - 0.0130042 * rtb_Abs1) * 0.017453292519943295;

  /*  Find the distance in AU from E to S */
  /* '<S146>:1:37' r           =  1.000140612 - 0.016708617*cosf(mean_anom_sun_rad) - ... */
  /* '<S146>:1:38'                     0.000139589*cosf(2*mean_anom_sun_rad); */
  rtb_Sum3 = (1.000140612 - 0.016708617 * cosf(T1900)) - cosf(2.0 * T1900) *
    0.000139589;

  /* '<S146>:1:39' x_MOD_km    = r*cosf(long_eclp_rad)*AU2KM; */
  /* '<S146>:1:40' y_MOD_km    = r*cosf(oblq_rad)*sinf(long_eclp_rad)*AU2KM; */
  /* '<S146>:1:41' z_MOD_km    = r*sinf(oblq_rad)*sinf(long_eclp_rad)*AU2KM; */
  /* '<S146>:1:43' sun_vector_mod_km = [x_MOD_km; y_MOD_km; z_MOD_km]; */
  /*  Convert to ECI (mean-equator-mean-equinox) frame  */
  /* '<S146>:1:46' sun_vector_eci_km   = mod2eci(sun_vector_mod_km,JD_cent_ut1); */
  /* '<S146>:1:52' [prec,~,~,~,~] = precess ( JD_cent_ut1 ); */
  /*  " to rad */
  /* '<S146>:1:60' convrt = pi / (180.0*3600.0); */
  /*  ------------------ iau 03 precession angles ------------------- */
  /* '<S146>:1:63' oblo =  84381.406; */
  /*  " */
  /* '<S146>:1:64' psia =  (((( -0.0000000951 * ttt + 0.000132851 ) * ttt - 0.00114045 ) *... */
  /* '<S146>:1:65'                         ttt - 1.0790069 ) * ttt + 5038.481507 ) * ttt; */
  /*  " */
  /* '<S146>:1:66' wa   =  ((((  0.0000003337 * ttt - 0.000000467 ) * ttt - 0.00772503 ) *... */
  /* '<S146>:1:67'                         ttt + 0.0512623 ) * ttt -    0.025754 ) * ttt + oblo; */
  /* '<S146>:1:68' ea   =  (((( -0.0000000434 * ttt - 0.000000576 ) * ttt + 0.00200340 ) *... */
  /* '<S146>:1:69'                         ttt - 0.0001831 ) * ttt -   46.836769 ) * ttt + oblo; */
  /* '<S146>:1:70' xa   =  (((( - 0.0000000560 * ttt + 0.000170663 ) * ttt - 0.00121197 ) *... */
  /* '<S146>:1:71'                         ttt - 2.3814292 ) * ttt +   10.556403 ) * ttt; */
  /* '<S146>:1:73' zeta =  (((( - 0.0000003173 * ttt - 0.000005971 ) * ttt + 0.01801828 ) *... */
  /* '<S146>:1:74'                 ttt + 0.2988499 ) * ttt + 2306.083227 ) * ttt + 2.650545; */
  /*  " */
  /* '<S146>:1:75' theta=  (((( - 0.0000001274 * ttt - 0.000007089 ) * ttt - 0.04182264 ) *... */
  /* '<S146>:1:76'                 ttt - 0.4294934 ) * ttt + 2004.191903 ) * ttt; */
  /* '<S146>:1:77' z    =  ((((   0.0000002904 * ttt - 0.000028596 ) * ttt + 0.01826837 ) *... */
  /* '<S146>:1:78'                 ttt + 1.0927348 ) * ttt + 2306.077181 ) * ttt - 2.650545; */
  /*  convert all units to rad */
  /* '<S146>:1:81' psia = psia  * convrt; */
  rtb_Switch = ((((-9.51E-8 * rtb_Abs1 + 0.000132851) * rtb_Abs1 - 0.00114045) *
                 rtb_Abs1 - 1.0790069) * rtb_Abs1 + 5038.481507) * rtb_Abs1 *
    4.84813681109536E-6;

  /*  rad */
  /* '<S146>:1:82' wa   = wa    * convrt; */
  rtb_z1 = (((((3.337E-7 * rtb_Abs1 - 4.67E-7) * rtb_Abs1 - 0.00772503) *
              rtb_Abs1 + 0.0512623) * rtb_Abs1 - 0.025754) * rtb_Abs1 +
            84381.406) * 4.84813681109536E-6;

  /* '<S146>:1:83' ea   = ea    * convrt; */
  /* '<S146>:1:84' xa   = xa    * convrt; */
  T1900 = ((((-5.6E-8 * rtb_Abs1 + 0.000170663) * rtb_Abs1 - 0.00121197) *
            rtb_Abs1 - 2.3814292) * rtb_Abs1 + 10.556403) * rtb_Abs1 *
    4.84813681109536E-6;

  /* '<S146>:1:86' oblo = oblo * convrt; */
  /*  " to rad */
  /* '<S146>:1:87' a4  = rot3mat(-xa); */
  /* '<S146>:1:108' s   = sinf( z_rad ); */
  rtb_Abs1 = sinf(-T1900);

  /* '<S146>:1:109' c   = cosf( z_rad ); */
  T1900 = cosf(-T1900);

  /* '<S146>:1:111' DCMz    = [ c   s   0; */
  /* '<S146>:1:112'            -s   c   0; */
  /* '<S146>:1:113'             0   0   1 ]; */
  /* '<S146>:1:88' a5  = rot1mat(wa); */
  /*  precess */
  /* '<S146>:1:97' s   = sinf( x_rad ); */
  days = sinf(rtb_z1);

  /* '<S146>:1:98' c   = cosf( x_rad ); */
  rtb_z1 = cosf(rtb_z1);

  /* '<S146>:1:100' DCMx    = [ 1   0   0 */
  /* '<S146>:1:101'             0   c   s; */
  /* '<S146>:1:102'             0  -s   c ]; */
  /* '<S146>:1:89' a6  = rot3mat(psia); */
  /* '<S146>:1:108' s   = sinf( z_rad ); */
  b_min = sinf(rtb_Switch);

  /* '<S146>:1:109' c   = cosf( z_rad ); */
  rtb_Switch = cosf(rtb_Switch);

  /* '<S146>:1:111' DCMz    = [ c   s   0; */
  /* '<S146>:1:112'            -s   c   0; */
  /* '<S146>:1:113'             0   0   1 ]; */
  /* '<S146>:1:90' a7  = rot1mat(-oblo); */
  /* '<S146>:1:91' prec = a7*a6*a5*a4; */
  /* '<S146>:1:52' ~ */
  /* '<S146>:1:54' vec_eci = prec*vec_mod; */
  rtb_Switch_0[0] = rtb_Switch;
  rtb_Switch_0[3] = b_min;
  rtb_Switch_0[6] = 0.0;
  rtb_Switch_0[1] = -b_min;
  rtb_Switch_0[4] = rtb_Switch;
  rtb_Switch_0[7] = 0.0;

  /* Outport: '<Root>/sc2gs_unit' */
  rtY.sc2gs_unit[0] = eta;

  /* MATLAB Function: '<S6>/MATLAB Function' */
  rtb_Switch_0[2] = 0.0;

  /* Outport: '<Root>/sc2gs_unit' */
  rtY.sc2gs_unit[1] = C_1;

  /* MATLAB Function: '<S6>/MATLAB Function' */
  rtb_Switch_0[5] = 0.0;

  /* Outport: '<Root>/sc2gs_unit' */
  rtY.sc2gs_unit[2] = rtb_Sum;

  /* MATLAB Function: '<S6>/MATLAB Function' */
  rtb_Switch_0[8] = 1.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      c_0[lowAlt + 3 * LOS] = 0.0;
      c_0[lowAlt + 3 * LOS] += rtb_Switch_0[3 * LOS] * a[lowAlt];
      c_0[lowAlt + 3 * LOS] += rtb_Switch_0[3 * LOS + 1] * a[lowAlt + 3];
      c_0[lowAlt + 3 * LOS] += rtb_Switch_0[3 * LOS + 2] * a[lowAlt + 6];
    }

    tod_to_mod[3 * lowAlt] = b[lowAlt];
  }

  tod_to_mod[1] = 0.0;
  tod_to_mod[4] = rtb_z1;
  tod_to_mod[7] = days;
  tod_to_mod[2] = 0.0;
  tod_to_mod[5] = -days;
  tod_to_mod[8] = rtb_z1;
  rtb_VectorConcatenate[0] = T1900;
  rtb_VectorConcatenate[3] = rtb_Abs1;
  rtb_VectorConcatenate[6] = 0.0;
  rtb_VectorConcatenate[1] = -rtb_Abs1;
  rtb_VectorConcatenate[4] = T1900;
  rtb_VectorConcatenate[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (LOS = 0; LOS < 3; LOS++) {
      rtb_Switch_0[lowAlt + 3 * LOS] = 0.0;
      rtb_Switch_0[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS] * c_0[lowAlt];
      rtb_Switch_0[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 1] * c_0[lowAlt + 3];
      rtb_Switch_0[lowAlt + 3 * LOS] += tod_to_mod[3 * LOS + 2] * c_0[lowAlt + 6];
    }

    rtb_VectorConcatenate[2 + 3 * lowAlt] = d_0[lowAlt];
  }

  rtb_Sum3_0[0] = rtb_Sum3 * cosf(rtb_T_TT_J2000) * 1.495978707E+8;
  rtb_Sum3_0[1] = rtb_Sum3 * cosf(JD_days_gps) * sinf(rtb_T_TT_J2000) *
    1.495978707E+8;
  rtb_Sum3_0[2] = rtb_Sum3 * sinf(JD_days_gps) * sinf(rtb_T_TT_J2000) *
    1.495978707E+8;

  /*  Constants */
  /* MATLAB Function 'sun_vector_lib/MATLAB Function1': '<S147>:1' */
  /* '<S147>:1:4' M2KM        = 1e-3; */
  /* '<S147>:1:5' ECCE2       = 0.006694385000; */
  /* '<S147>:1:6' RE          = 6378.137; */
  /* '<S147>:1:7' TRUE        = 1; */
  /* '<S147>:1:8' FALSE       = 0; */
  /*  ----- */
  /*  Convert to sc position vector to km if necessary */
  /* '<S147>:1:12' sc_pos_eci_km   = sc_pos_eci_m*M2KM; */
  for (i = 0; i < 3; i++) {
    rtb_Sum_g[i] = 0.0;
    for (lowAlt = 0; lowAlt < 3; lowAlt++) {
      c_0[i + 3 * lowAlt] = 0.0;
      c_0[i + 3 * lowAlt] += rtb_VectorConcatenate[3 * lowAlt] * rtb_Switch_0[i];
      c_0[i + 3 * lowAlt] += rtb_VectorConcatenate[3 * lowAlt + 1] *
        rtb_Switch_0[i + 3];
      c_0[i + 3 * lowAlt] += rtb_VectorConcatenate[3 * lowAlt + 2] *
        rtb_Switch_0[i + 6];
      rtb_Sum_g[i] += c_0[3 * lowAlt + i] * rtb_Sum3_0[lowAlt];
    }

    /* MATLAB Function: '<S6>/MATLAB Function1' */
    rtb_pos_teme_km[i] = M[i] * 0.001;
  }

  /* MATLAB Function: '<S6>/MATLAB Function1' */
  /* ----- */
  /*  Main Function */
  /* '<S147>:1:16' LOS     = FALSE; */
  LOS = 0;

  /* '<S147>:1:18' sun_vector_eci_km(3)    = sun_vector_eci_km(3)/(sqrtf(1 - ECCE2)); */
  rtb_Sum_g[2] /= 0.99664718682189635;

  /* '<S147>:1:19' sc_pos_eci_km(3)        = sc_pos_eci_km(3)/(sqrtf(1 - ECCE2)); */
  rtb_pos_teme_km[2] /= 0.99664718682189635;

  /* '<S147>:1:21' sun_vec     = sun_vector_eci_km; */
  /* '<S147>:1:22' sc_vec      = sc_pos_eci_km; */
  /* '<S147>:1:24' sc2sun_unit  = (sun_vec - sc_vec)/norm(sun_vec - sc_vec); */
  rtb_Sum_j[0] = rtb_Sum_g[0] - rtb_pos_teme_km[0];
  rtb_Sum_j[1] = rtb_Sum_g[1] - rtb_pos_teme_km[1];
  rtb_Sum_j[2] = rtb_Sum_g[2] - rtb_pos_teme_km[2];
  rtb_T_TT_J2000 = norm_f(rtb_Sum_j);

  /*  Determine if the sc is in the light or shadow */
  /*    Call vector 1 the sun, vector 2 the sc */
  /* '<S147>:1:29' mag1    = norm(sun_vec); */
  JD_days_gps = norm_f(rtb_Sum_g);

  /* '<S147>:1:30' mag2    = norm(sc_vec); */
  rtb_Abs1 = norm_f(rtb_pos_teme_km);

  /* '<S147>:1:32' mag1_sq     = mag1*mag1; */
  JD_days_gps *= JD_days_gps;

  /* '<S147>:1:33' mag2_sq     = mag2*mag2; */
  /* '<S147>:1:35' sun_sc_dot  = dot(sun_vec,sc_vec); */
  T1900 = (rtb_Sum_g[0] * rtb_pos_teme_km[0] + rtb_Sum_g[1] * rtb_pos_teme_km[1])
    + rtb_Sum_g[2] * rtb_pos_teme_km[2];

  /*  Find t_min */
  /* '<S147>:1:38' t_min_num   = mag1_sq - sun_sc_dot; */
  /* '<S147>:1:39' t_min_den   = mag1_sq + mag2_sq - 2*sun_sc_dot; */
  rtb_Abs1 = (rtb_Abs1 * rtb_Abs1 + JD_days_gps) - 2.0 * T1900;

  /* '<S147>:1:41' if ( t_min_den < 0.0001  ) */
  if (rtb_Abs1 < 0.0001) {
    /* '<S147>:1:42' t_min = 0.0; */
    rtb_Abs1 = 0.0;
  } else {
    /* '<S147>:1:43' else */
    /* '<S147>:1:44' t_min = ( t_min_num ) / ( t_min_den ); */
    rtb_Abs1 = (JD_days_gps - T1900) / rtb_Abs1;
  }

  /* '<S147>:1:47' c_t_min_sq  = ((1-t_min)*mag1_sq + sun_sc_dot*t_min)/RE^2; */
  /*  Check LoS */
  /* '<S147>:1:50' if( t_min < 0 || t_min > 0.999979 ) */
  if ((rtb_Abs1 < 0.0) || (rtb_Abs1 > 0.999979)) {
    /* '<S147>:1:51' LOS     = TRUE; */
    LOS = 1;
  }

  /* Outport: '<Root>/sc_in_sun' incorporates:
   *  MATLAB Function: '<S6>/MATLAB Function1'
   */
  /* '<S147>:1:56' sc_in_sun   = LOS; */
  rtY.sc_in_sun = LOS;
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sc2sun_unit' incorporates:
     *  MATLAB Function: '<S6>/MATLAB Function1'
     */
    rtY.sc2sun_unit[i] = (rtb_Sum_g[i] - rtb_pos_teme_km[i]) / rtb_T_TT_J2000;

    /* Product: '<S4>/Product2' incorporates:
     *  Math: '<S4>/Math Function1'
     */
    rtb_Sum_g[i] = rtb_VectorConcatenate_n[3 * i + 2] * M[2] +
      (rtb_VectorConcatenate_n[3 * i + 1] * M[1] + rtb_VectorConcatenate_n[3 * i]
       * M[0]);
  }

  /* Sqrt: '<S68>/sqrt' incorporates:
   *  Product: '<S68>/Product2'
   *  Product: '<S68>/Product3'
   *  Sum: '<S68>/Sum2'
   */
  rtb_Switch = sqrtf(rtb_Sum_g[0] * rtb_Sum_g[0] + rtb_Sum_g[1] * rtb_Sum_g[1]);

  /* Outputs for Iterator SubSystem: '<S53>/While Iterator Subsystem' incorporates:
   *  WhileIterator: '<S70>/While Iterator'
   */
  s70_iter = 1;
  do {
    rtb_Abs1 = rtDW.Memory_PreviousInput;
    if (!(s70_iter > 0)) {
      rtb_Abs1 = rt_atan2d_snf(rtb_Sum_g[2], 0.99664718933525254 * rtb_Switch);
    }

    rtb_T_TT_J2000 = sinf(rtb_Abs1);
    JD_days_gps = cosf(rtb_Abs1);
    days = rt_atan2d_snf(42841.31151331366 * rtb_T_TT_J2000 * rtb_T_TT_J2000 *
                         rtb_T_TT_J2000 + rtb_Sum_g[2], rtb_Switch - JD_days_gps
                         * JD_days_gps * JD_days_gps * 0.00669437999014133 *
                         6.378137E+6);
    rtb_T_TT_J2000 = rt_atan2d_snf(0.99664718933525254 * sinf(days), cosf(days));
    rtDW.Memory_PreviousInput = rtb_T_TT_J2000;
    s70_iter++;
  } while ((rtb_Abs1 != rtb_T_TT_J2000) && (s70_iter <= 5));

  /* End of Outputs for SubSystem: '<S53>/While Iterator Subsystem' */

  /* UnitConversion: '<S67>/Unit Conversion' incorporates:
   *  Trigonometry: '<S53>/Trigonometric Function2'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  rtb_T_TT_J2000 = 57.295779513082323 * days;
  eta = 57.295779513082323 * rt_atan2d_snf(rtb_Sum_g[1], rtb_Sum_g[0]);

  /* UnitConversion: '<S65>/Unit Conversion' */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  rtb_Abs1 = 0.017453292519943295 * rtb_T_TT_J2000;

  /* Trigonometry: '<S52>/sincos' */
  b_min = cosf(rtb_Abs1);
  rtb_Abs1 = sinf(rtb_Abs1);
  C_1 = b_min;

  /* UnitConversion: '<S65>/Unit Conversion' */
  rtb_Sum = rtb_Abs1;
  rtb_Abs1 = 0.017453292519943295 * eta;

  /* Trigonometry: '<S52>/sincos' */
  b_min = cosf(rtb_Abs1);
  rtb_Abs1 = sinf(rtb_Abs1);

  /* UnaryMinus: '<S56>/Unary Minus' incorporates:
   *  Product: '<S56>/u(1)*u(4)'
   *  Trigonometry: '<S52>/sincos'
   */
  rtb_VectorConcatenate_o[0] = -(rtb_Sum * b_min);

  /* UnaryMinus: '<S59>/Unary Minus' */
  rtb_VectorConcatenate_o[1] = -rtb_Abs1;

  /* UnaryMinus: '<S62>/Unary Minus' incorporates:
   *  Product: '<S62>/u(3)*u(4)'
   *  Trigonometry: '<S52>/sincos'
   */
  rtb_VectorConcatenate_o[2] = -(C_1 * b_min);

  /* UnaryMinus: '<S57>/Unary Minus' incorporates:
   *  Product: '<S57>/u(1)*u(2)'
   */
  rtb_VectorConcatenate_o[3] = -(rtb_Sum * rtb_Abs1);

  /* SignalConversion: '<S66>/ConcatBufferAtVector ConcatenateIn5' incorporates:
   *  Trigonometry: '<S52>/sincos'
   */
  rtb_VectorConcatenate_o[4] = b_min;

  /* UnaryMinus: '<S63>/Unary Minus' incorporates:
   *  Product: '<S63>/u(2)*u(3)'
   */
  rtb_VectorConcatenate_o[5] = -(rtb_Abs1 * C_1);

  /* SignalConversion: '<S66>/ConcatBufferAtVector ConcatenateIn7' */
  rtb_VectorConcatenate_o[6] = C_1;

  /* SignalConversion: '<S66>/ConcatBufferAtVector ConcatenateIn8' incorporates:
   *  Constant: '<S61>/Constant'
   */
  rtb_VectorConcatenate_o[7] = 0.0;

  /* UnaryMinus: '<S64>/Unary Minus' */
  rtb_VectorConcatenate_o[8] = -rtb_Sum;

  /* MATLAB Function: '<S55>/MATLAB Function' */
  /*  UW HuskySat 1, ADCS Subsystem */
  /*    T. Reynolds, Updated: 10.28.17 */
  /* MATLAB Function 'YMDHMS_2_dec_year_lib/MATLAB Function': '<S144>:1' */
  /*  Converts a time vector into a decimal year number. See Vallado pp 200-201 */
  /*  for reference algorithms. Uses 6 inputs to avoid 1 based indexing. */
  /*  Input 'day' is day of year, not of month.  */
  /*  find day of year - valid until 2100 */
  /* Lmonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
  /* '<S144>:1:12' days_in_year    = 365.25; */
  rtb_Abs1 = 365.25;

  /* '<S144>:1:14' if mod(year,4) == 0 */
  if (rtb_time_ut1_idx_0 - floorf(rtb_time_ut1_idx_0 / 4.0) * 4.0 == 0.0) {
    /* Lmonth(2) = 29; */
    /* '<S144>:1:16' days_in_year    = 366; */
    rtb_Abs1 = 366.0;
  }

  /*  --- Use this if the input 'day' is the day of the month --- % */
  /*  day_of_year = 0; */
  /*  for i = 1:month-1 */
  /*      day_of_year = day_of_year + Lmonth(i); */
  /*  end */
  /*   */
  /*  % Add number of days into the month */
  /*  day_of_year = day_of_year + day; */
  /*  --- % */
  /*  Add HMS info to the day of year */
  /* '<S144>:1:30' day_of_year    = day + hour/24 + min/1440 + sec/86400; */
  /*  Find the year fraction represented by day_of_year */
  /* '<S144>:1:33' day_frac    = day_of_year/days_in_year; */
  /*  Sum to get total decimal year */
  /* '<S144>:1:36' dec_year = year + day_frac; */
  rtb_Abs1 = (((rtb_time_ut1_idx_3 / 24.0 + rtb_time_ut1_idx_2) +
               rtb_time_ut1_idx_4 / 1440.0) + rtb_time_ut1_idx_5 / 86400.0) /
    rtb_Abs1 + rtb_time_ut1_idx_0;

  /* End of MATLAB Function: '<S55>/MATLAB Function' */

  /* RelationalOperator: '<S93>/Relational Operator' incorporates:
   *  Memory: '<S93>/otime'
   */
  rtb_Compare_k = (rtb_Abs1 != rtDW.otime_PreviousInput);

  /* Saturate: '<S54>/+//- 180 deg' */
  if (eta > 180.0) {
    JD_days_gps = 180.0;
  } else if (eta < -180.0) {
    JD_days_gps = -180.0;
  } else {
    JD_days_gps = eta;
  }

  /* End of Saturate: '<S54>/+//- 180 deg' */

  /* Saturate: '<S54>/+//- 90 deg' */
  if (rtb_T_TT_J2000 > 90.0) {
    T1900 = 90.0;
  } else if (rtb_T_TT_J2000 < -90.0) {
    T1900 = -90.0;
  } else {
    T1900 = rtb_T_TT_J2000;
  }

  /* End of Saturate: '<S54>/+//- 90 deg' */

  /* UnitConversion: '<S137>/Unit Conversion' */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  C_1 = 0.017453292519943295 * JD_days_gps;
  rtb_time_ut1_idx_3 = 0.017453292519943295 * T1900;

  /* Trigonometry: '<S90>/sincos' */
  rtb_Sum3 = sinf(C_1);
  rtb_z1 = cosf(C_1);
  rtb_time_ut1_idx_4 = sinf(rtb_time_ut1_idx_3);
  rtb_time_ut1_idx_5 = cosf(rtb_time_ut1_idx_3);

  /* Outputs for Enabled SubSystem: '<S85>/Convert from geodetic to  spherical coordinates ' incorporates:
   *  EnablePort: '<S89>/Enable'
   */
  /* RelationalOperator: '<S92>/Relational Operator' incorporates:
   *  Memory: '<S92>/olon'
   */
  if (JD_days_gps != rtDW.olon_PreviousInput) {
    /* Outputs for Iterator SubSystem: '<S89>/For Iterator Subsystem' incorporates:
     *  ForIterator: '<S136>/For Iterator'
     */
    for (s136_iter = 1L; (int16_T)s136_iter < 12; s136_iter = (int16_T)s136_iter
         + 1) {
      /* Switch: '<S136>/cp[m-1] sp[m-1]' incorporates:
       *  Inport: '<S89>/cp[2]'
       *  Inport: '<S89>/sp[2]'
       *  UnitDelay: '<S136>/Unit Delay1'
       */
      if ((int16_T)s136_iter > 1) {
        C_1 = rtDW.UnitDelay1_DSTATE[0];
        rtb_time_ut1_idx_3 = rtDW.UnitDelay1_DSTATE[1];
      } else {
        C_1 = rtb_z1;
        rtb_time_ut1_idx_3 = rtb_Sum3;
      }

      /* End of Switch: '<S136>/cp[m-1] sp[m-1]' */

      /* Sum: '<S136>/Sum2' incorporates:
       *  Inport: '<S89>/cp[2]'
       *  Inport: '<S89>/sp[2]'
       *  Product: '<S136>/Product1'
       *  Product: '<S136>/Product2'
       */
      b_min = C_1 * rtb_Sum3 + rtb_time_ut1_idx_3 * rtb_z1;

      /* Assignment: '<S136>/Assignment' incorporates:
       *  Assignment: '<S136>/Assignment1'
       *  Constant: '<S136>/Constant'
       *  Constant: '<S136>/Constant1'
       */
      if ((int16_T)s136_iter == 1) {
        memcpy(&rtb_Assignment[0], &rtConstP.pooled15[0], 11U * sizeof(real_T));
        memcpy(&rtb_Assignment1[0], &rtConstP.pooled15[0], 11U * sizeof(real_T));
      }

      rtb_Assignment[(int16_T)s136_iter - 1] = b_min;

      /* End of Assignment: '<S136>/Assignment' */

      /* Sum: '<S136>/Sum1' incorporates:
       *  Inport: '<S89>/cp[2]'
       *  Inport: '<S89>/sp[2]'
       *  Product: '<S136>/Product3'
       *  Product: '<S136>/Product8'
       */
      rtb_Sum = C_1 * rtb_z1 - rtb_time_ut1_idx_3 * rtb_Sum3;

      /* Assignment: '<S136>/Assignment1' */
      rtb_Assignment1[(int16_T)s136_iter - 1] = rtb_Sum;

      /* Update for UnitDelay: '<S136>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE[0] = rtb_Sum;
      rtDW.UnitDelay1_DSTATE[1] = b_min;
    }

    /* End of Outputs for SubSystem: '<S89>/For Iterator Subsystem' */

    /* SignalConversion: '<S89>/OutportBufferForcp[13]' incorporates:
     *  Constant: '<S89>/cp[1]'
     *  Inport: '<S89>/cp[2]'
     */
    rtDW.OutportBufferForcp13[0] = 1.0;
    rtDW.OutportBufferForcp13[1] = rtb_z1;

    /* SignalConversion: '<S89>/OutportBufferForsp[13]' incorporates:
     *  Constant: '<S89>/sp[1]'
     *  Inport: '<S89>/sp[2]'
     */
    rtDW.OutportBufferForsp13[0] = 0.0;
    rtDW.OutportBufferForsp13[1] = rtb_Sum3;

    /* SignalConversion: '<S89>/OutportBufferForcp[13]' */
    memcpy(&rtDW.OutportBufferForcp13[2], &rtb_Assignment1[0], 11U * sizeof
           (real_T));

    /* SignalConversion: '<S89>/OutportBufferForsp[13]' */
    memcpy(&rtDW.OutportBufferForsp13[2], &rtb_Assignment[0], 11U * sizeof
           (real_T));
  }

  /* End of RelationalOperator: '<S92>/Relational Operator' */
  /* End of Outputs for SubSystem: '<S85>/Convert from geodetic to  spherical coordinates ' */

  /* Trigonometry: '<S69>/sincos' */
  rtb_z1 = sinf(days);

  /* Product: '<S69>/Product2' */
  rtb_Sum = rtb_z1 * 0.00669437999014133;

  /* Product: '<S69>/Product3' incorporates:
   *  Constant: '<S69>/Constant1'
   *  Constant: '<S69>/f'
   *  Product: '<S69>/Product1'
   *  Sqrt: '<S69>/sqrt'
   *  Sum: '<S69>/Sum'
   */
  rtb_Sum3 = 6.378137E+6 / sqrtf(1.0 - rtb_Sum * rtb_z1);

  /* Sum: '<S69>/Sum3' incorporates:
   *  Product: '<S69>/Product4'
   *  Product: '<S69>/Product5'
   *  Product: '<S69>/Product6'
   *  Sum: '<S69>/Sum2'
   *  Trigonometry: '<S69>/sincos'
   */
  rtb_Sum3 = ((rtb_Sum * rtb_Sum3 + rtb_Sum_g[2]) * rtb_z1 - rtb_Sum3) + cosf
    (days) * rtb_Switch;

  /* Saturate: '<S54>/0 to 1,000,000 m' */
  if (rtb_Sum3 > 1.0E+6) {
    rtb_z1 = 1.0E+6;
  } else if (rtb_Sum3 < 0.0) {
    rtb_z1 = 0.0;
  } else {
    rtb_z1 = rtb_Sum3;
  }

  /* End of Saturate: '<S54>/0 to 1,000,000 m' */

  /* Gain: '<S54>/Gain' */
  rtb_time_ut1_idx_0 = 0.001 * rtb_z1;

  /* Logic: '<S91>/Logical Operator' incorporates:
   *  Memory: '<S91>/oalt'
   *  Memory: '<S91>/olat'
   *  RelationalOperator: '<S91>/Relational Operator'
   *  RelationalOperator: '<S91>/Relational Operator1'
   */
  rtb_LogicalOperator = ((T1900 != rtDW.olat_PreviousInput) ||
    (rtb_time_ut1_idx_0 != rtDW.oalt_PreviousInput));

  /* Product: '<S90>/Product' */
  rtb_time_ut1_idx_2 = rtb_time_ut1_idx_4 * rtb_time_ut1_idx_4;

  /* Product: '<S90>/Product1' */
  rtb_Switch = rtb_time_ut1_idx_5 * rtb_time_ut1_idx_5;

  /* Outputs for Enabled SubSystem: '<S85>/Convert from geodetic to  spherical coordinates' incorporates:
   *  EnablePort: '<S88>/Enable'
   */
  if (rtb_LogicalOperator) {
    /* Sqrt: '<S131>/sqrt' incorporates:
     *  Product: '<S131>/Product'
     *  Sum: '<S131>/Sum'
     */
    days = sqrtf(4.0680631590768993E+7 - rtb_time_ut1_idx_2 * 272331.60668193549);

    /* Product: '<S88>/Product1' */
    rtb_z1 = days * rtb_time_ut1_idx_0;

    /* Sqrt: '<S130>/sqrt' incorporates:
     *  Product: '<S130>/Product10'
     *  Product: '<S130>/Product9'
     *  Sum: '<S130>/Sum7'
     */
    b_min = sqrtf(rtb_Switch * 4.0680631590768993E+7 + 4.0408299984087057E+7 *
                 rtb_time_ut1_idx_2);

    /* Sqrt: '<S88>/sqrt' incorporates:
     *  Gain: '<S133>/Gain'
     *  Product: '<S133>/Product1'
     *  Product: '<S133>/Product6'
     *  Product: '<S133>/Product7'
     *  Product: '<S133>/Product8'
     *  Sum: '<S133>/Sum5'
     *  Sum: '<S133>/Sum6'
     */
    rtDW.sqrt_o = sqrtf((1.6549137866238722E+15 - 2.208307901990225E+13 *
                        rtb_time_ut1_idx_2) / (days * days) + (2.0 * rtb_z1 +
      rtb_time_ut1_idx_0 * rtb_time_ut1_idx_0));

    /* Product: '<S128>/Product11' incorporates:
     *  Sum: '<S128>/Sum8'
     */
    rtDW.Product11 = (rtb_time_ut1_idx_0 + b_min) / rtDW.sqrt_o;

    /* Sum: '<S132>/Sum2' */
    days = 4.0680631590768993E+7 + rtb_z1;

    /* Sum: '<S132>/Sum1' */
    rtb_z1 += 4.0408299984087057E+7;

    /* Product: '<S129>/Product4' incorporates:
     *  Product: '<S129>/Product3'
     *  Product: '<S132>/Product1'
     *  Product: '<S132>/Product2'
     *  Sqrt: '<S129>/sqrt'
     *  Sum: '<S129>/Sum3'
     */
    rtDW.Product4 = rtb_time_ut1_idx_4 / sqrtf(days * days / (rtb_z1 * rtb_z1) *
      rtb_Switch + rtb_time_ut1_idx_2);

    /* Product: '<S134>/Product12' incorporates:
     *  Product: '<S134>/Product1'
     */
    rtDW.Product12 = 272331.60668193549 / (rtDW.sqrt_o * b_min) *
      rtb_time_ut1_idx_5 * rtb_time_ut1_idx_4;

    /* Sqrt: '<S135>/sqrt' incorporates:
     *  Constant: '<S135>/Constant'
     *  Product: '<S135>/Product5'
     *  Sum: '<S135>/Sum4'
     */
    rtDW.sqrt_od = sqrtf(1.0 - rtDW.Product4 * rtDW.Product4);
  }

  /* End of Outputs for SubSystem: '<S85>/Convert from geodetic to  spherical coordinates' */

  /* Product: '<S85>/aor' incorporates:
   *  Constant: '<S85>/re'
   */
  rtb_time_ut1_idx_2 = 6371.2 / rtDW.sqrt_o;

  /* Product: '<S85>/ar' */
  rtb_Switch = rtb_time_ut1_idx_2 * rtb_time_ut1_idx_2;

  /* Outputs for Iterator SubSystem: '<S85>/Compute magnetic vector in spherical coordinates' incorporates:
   *  ForIterator: '<S87>/For Iterator'
   */
  /* InitializeConditions for UnitDelay: '<S87>/Unit Delay' */
  days = 0.0;

  /* InitializeConditions for UnitDelay: '<S87>/Unit Delay2' */
  rtb_time_ut1_idx_3 = 0.0;
  rtb_time_ut1_idx_4 = 0.0;
  rtb_time_ut1_idx_5 = 0.0;
  C_1 = 0.0;
  for (s136_iter = 1L; (int16_T)s136_iter < 13; s136_iter = (int16_T)s136_iter +
       1) {
    /* Switch: '<S87>/ar(n)' */
    if (!((int16_T)s136_iter > 1)) {
      days = rtb_Switch;
    }

    /* End of Switch: '<S87>/ar(n)' */

    /* Product: '<S87>/Product8' */
    days *= rtb_time_ut1_idx_2;

    /* Sum: '<S87>/Sum' */
    lowAlt = (int16_T)s136_iter + 1;

    /* Outputs for Iterator SubSystem: '<S87>/For Iterator Subsystem' incorporates:
     *  ForIterator: '<S95>/For Iterator'
     */
    if (rtDW.ForIterator_IterationMarker[4] != 0) {
      /* InitializeConditions for UnitDelay: '<S96>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_k = 0.0;

      /* InitializeConditions for UnitDelay: '<S96>/Unit Delay3' */
      rtDW.UnitDelay3_DSTATE = 0.0;

      /* InitializeConditions for UnitDelay: '<S96>/Unit Delay2' */
      rtDW.UnitDelay2_DSTATE_d = 0.0;

      /* InitializeConditions for UnitDelay: '<S96>/Unit Delay4' */
      rtDW.UnitDelay4_DSTATE = 0.0;
    }

    for (i = 0; i < 7; i++) {
      rtDW.ForIterator_IterationMarker[i] = 1U;
    }

    /* Sum: '<S87>/Sum' incorporates:
     *  Constant: '<S102>/Constant'
     *  Constant: '<S102>/Constant1'
     *  Logic: '<S102>/Logical Operator'
     *  RelationalOperator: '<S102>/Relational Operator'
     *  RelationalOperator: '<S102>/Relational Operator1'
     *  Sum: '<S95>/Sum1'
     */
    for (idx1 = 1L; (int16_T)idx1 <= lowAlt; idx1 = (int16_T)idx1 + 1) {
      /* Sum: '<S95>/Sum1' */
      LOS = (int16_T)idx1 - 1;
      idx2 = (int16_T)idx1 - 1;

      /* Outputs for Enabled SubSystem: '<S95>/Time adjust the gauss coefficients' incorporates:
       *  EnablePort: '<S98>/Enable'
       */
      if (rtb_Compare_k) {
        /* Outputs for Atomic SubSystem: '<S98>/If Action Subsystem' */
        /* Sum: '<S124>/Sum1' */
        i = (int16_T)idx2 + 1;

        /* Sum: '<S124>/Sum2' */
        qY_0 = (int16_T)s136_iter + 1;

        /* End of Outputs for SubSystem: '<S98>/If Action Subsystem' */

        /* Assignment: '<S98>/Assignment' incorporates:
         *  Constant: '<S85>/epoch'
         *  Constant: '<S98>/c[maxdef][maxdef]'
         *  Constant: '<S98>/cd[maxdef][maxdef]'
         *  Product: '<S124>/Product'
         *  Selector: '<S124>/c[m][n]'
         *  Selector: '<S124>/cd[m][n]'
         *  Sum: '<S124>/Sum'
         *  Sum: '<S124>/Sum1'
         *  Sum: '<S124>/Sum2'
         *  Sum: '<S85>/Sum'
         *  UnitDelay: '<S98>/Unit Delay'
         */
        if (rtDW.ForIterator_IterationMarker[5] < 2) {
          rtDW.ForIterator_IterationMarker[5L] = 2U;
          memcpy(&Assignment[0], &rtDW.UnitDelay_DSTATE_p[0], 169U * sizeof
                 (real_T));
        }

        /* Outputs for Atomic SubSystem: '<S98>/If Action Subsystem' */
        Assignment[(i + 13 * (qY_0 - 1)) - 1] = rtConstP.cdmaxdefmaxdef_Value
          [((qY_0 - 1) * 13 + i) - 1] * (rtb_Abs1 - 2015.0) +
          rtConstP.cmaxdefmaxdef_Value[((qY_0 - 1) * 13 + i) - 1];

        /* End of Assignment: '<S98>/Assignment' */
        /* End of Outputs for SubSystem: '<S98>/If Action Subsystem' */

        /* Switch: '<S125>/tc_old' incorporates:
         *  UnitDelay: '<S125>/Unit Delay'
         */
        for (i = 0; i < 169; i++) {
          if ((int16_T)s136_iter > 1) {
            rtb_tc_old[i] = rtDW.UnitDelay_DSTATE_g[i];
          } else {
            rtb_tc_old[i] = 0.0;
          }
        }

        /* End of Switch: '<S125>/tc_old' */

        /* If: '<S125>/If' incorporates:
         *  Sum: '<S95>/Sum1'
         */
        if (LOS != 0) {
          /* Outputs for IfAction SubSystem: '<S125>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S126>/Action Port'
           */
          /* Sum: '<S126>/Sum2' */
          i = (int16_T)s136_iter + 1;

          /* Assignment: '<S126>/Assignment2' incorporates:
           *  Constant: '<S85>/epoch'
           *  Constant: '<S98>/c[maxdef][maxdef]'
           *  Constant: '<S98>/cd[maxdef][maxdef]'
           *  Product: '<S126>/Product'
           *  Selector: '<S126>/c[m][n]'
           *  Selector: '<S126>/cd[m][n]'
           *  Sum: '<S126>/Sum'
           *  Sum: '<S126>/Sum2'
           *  Sum: '<S85>/Sum'
           */
          if (rtDW.ForIterator_IterationMarker[6] < 2) {
            rtDW.ForIterator_IterationMarker[6L] = 2U;
            memcpy(&Assignment2[0], &rtb_tc_old[0], 169U * sizeof(real_T));
          }

          Assignment2[(i + 13 * (LOS - 1)) - 1] = rtConstP.cdmaxdefmaxdef_Value
            [((LOS - 1) * 13 + i) - 1] * (rtb_Abs1 - 2015.0) +
            rtConstP.cmaxdefmaxdef_Value[((LOS - 1) * 13 + i) - 1];

          /* End of Assignment: '<S126>/Assignment2' */

          /* Gain: '<S126>/Gain' */
          memcpy(&rtb_tc_old[0], &Assignment2[0], 169U * sizeof(real_T));

          /* End of Outputs for SubSystem: '<S125>/If Action Subsystem1' */
        }

        /* End of If: '<S125>/If' */
        for (i = 0; i < 169; i++) {
          /* Sum: '<S98>/Sum2' */
          rtDW.Sum2_o[i] = Assignment[i] + rtb_tc_old[i];

          /* Update for UnitDelay: '<S98>/Unit Delay' */
          rtDW.UnitDelay_DSTATE_p[i] = Assignment[i];

          /* Update for UnitDelay: '<S125>/Unit Delay' */
          rtDW.UnitDelay_DSTATE_g[i] = rtb_tc_old[i];
        }
      }

      /* End of Outputs for SubSystem: '<S95>/Time adjust the gauss coefficients' */

      /* If: '<S101>/If' incorporates:
       *  Sum: '<S95>/Sum1'
       */
      if (LOS == 0) {
        /* Outputs for IfAction SubSystem: '<S101>/If Action Subsystem' incorporates:
         *  ActionPort: '<S107>/Action Port'
         */
        /* Gain: '<S107>/Gain1' incorporates:
         *  Constant: '<S101>/Constant1'
         *  Constant: '<S107>/Constant'
         *  Product: '<S107>/Product'
         *  Selector: '<S101>/cp[m+1]'
         *  Selector: '<S107>/Selector'
         *  Sum: '<S101>/Sum4'
         *  Sum: '<S107>/Sum'
         */
        rtb_z1 = rtDW.Sum2_o[((int16_T)((real_T)s136_iter + 1.0) - 1) * 13] *
          rtDW.OutportBufferForcp13[(int16_T)((real_T)idx2 + 1.0) - 1];

        /* Gain: '<S107>/Gain2' incorporates:
         *  Constant: '<S101>/Constant1'
         *  Constant: '<S107>/Constant'
         *  Product: '<S107>/Product'
         *  Selector: '<S101>/sp[m+1]'
         *  Selector: '<S107>/Selector'
         *  Sum: '<S101>/Sum4'
         *  Sum: '<S107>/Sum'
         */
        b_min = rtDW.Sum2_o[((int16_T)((real_T)s136_iter + 1.0) - 1) * 13] *
          rtDW.OutportBufferForsp13[(int16_T)((real_T)idx2 + 1.0) - 1];

        /* End of Outputs for SubSystem: '<S101>/If Action Subsystem' */
      } else {
        /* Outputs for IfAction SubSystem: '<S101>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S108>/Action Port'
         */
        /* Sum: '<S108>/Sum' incorporates:
         *  Constant: '<S101>/Constant1'
         *  Constant: '<S109>/Constant'
         *  Product: '<S108>/Product'
         *  Product: '<S108>/Product1'
         *  Selector: '<S101>/cp[m+1]'
         *  Selector: '<S101>/sp[m+1]'
         *  Selector: '<S108>/Selector'
         *  Selector: '<S108>/Selector1'
         *  Sum: '<S101>/Sum4'
         *  Sum: '<S109>/Sum'
         *  Sum: '<S110>/Sum'
         */
        rtb_z1 = rtDW.Sum2_o[(((int16_T)((real_T)s136_iter + 1.0) - 1) * 13 +
                              (LOS + 1)) - 1] * rtDW.OutportBufferForcp13
          [(int16_T)((real_T)idx2 + 1.0) - 1] + rtDW.Sum2_o[(LOS - 1) * 13 +
          (int16_T)s136_iter] * rtDW.OutportBufferForsp13[(int16_T)((real_T)idx2
          + 1.0) - 1];

        /* Sum: '<S108>/Sum1' incorporates:
         *  Constant: '<S101>/Constant1'
         *  Constant: '<S109>/Constant'
         *  Product: '<S108>/Product'
         *  Product: '<S108>/Product1'
         *  Selector: '<S101>/cp[m+1]'
         *  Selector: '<S101>/sp[m+1]'
         *  Selector: '<S108>/Selector'
         *  Selector: '<S108>/Selector1'
         *  Sum: '<S101>/Sum4'
         *  Sum: '<S109>/Sum'
         *  Sum: '<S110>/Sum'
         */
        b_min = rtDW.Sum2_o[(((int16_T)((real_T)s136_iter + 1.0) - 1) * 13 +
                             (LOS + 1)) - 1] * rtDW.OutportBufferForsp13
          [(int16_T)((real_T)idx2 + 1.0) - 1] - rtDW.Sum2_o[(LOS - 1) * 13 +
          (int16_T)s136_iter] * rtDW.OutportBufferForcp13[(int16_T)((real_T)idx2
          + 1.0) - 1];

        /* End of Outputs for SubSystem: '<S101>/If Action Subsystem1' */
      }

      /* End of If: '<S101>/If' */

      /* Outputs for Enabled SubSystem: '<S95>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' incorporates:
       *  EnablePort: '<S97>/Enable'
       */
      if (rtb_LogicalOperator) {
        /* If: '<S97>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' incorporates:
         *  Sum: '<S95>/Sum1'
         */
        if ((int16_T)s136_iter == LOS) {
          /* Outputs for IfAction SubSystem: '<S97>/If Action Subsystem' incorporates:
           *  ActionPort: '<S111>/Action Port'
           */
          /* Gain: '<S115>/Gain' incorporates:
           *  Sum: '<S115>/Sum2'
           */
          j = mul_s32_sat(13L, LOS - 1);

          /* Selector: '<S111>/Selector' incorporates:
           *  Sum: '<S115>/Sum1'
           */
          if (j > MAX_int32_T - (int16_T)s136_iter) {
            j_0 = MAX_int32_T;
            j = MAX_int32_T;
          } else {
            j_0 = (int16_T)s136_iter + j;
            j += (int16_T)s136_iter;
          }

          /* Product: '<S111>/Product1' incorporates:
           *  Selector: '<S111>/Selector'
           *  UnitDelay: '<S97>/Unit Delay1'
           */
          rtDW.Merge_d = rtDW.UnitDelay1_DSTATE_g[(int16_T)j_0 - 1] *
            rtDW.sqrt_od;

          /* Sum: '<S111>/Sum' incorporates:
           *  Product: '<S111>/Product'
           *  Product: '<S111>/Product2'
           *  Selector: '<S111>/Selector'
           *  Selector: '<S111>/Selector1'
           *  UnitDelay: '<S97>/Unit Delay'
           *  UnitDelay: '<S97>/Unit Delay1'
           */
          rtDW.Merge1_f = rtDW.UnitDelay_DSTATE_k[(((int16_T)s136_iter - 1) * 13
            + LOS) - 1] * rtDW.sqrt_od + rtDW.UnitDelay1_DSTATE_g[(int16_T)j - 1]
            * rtDW.Product4;

          /* End of Outputs for SubSystem: '<S97>/If Action Subsystem' */
        } else if (((int16_T)s136_iter == 1) && (LOS == 0)) {
          /* Outputs for IfAction SubSystem: '<S97>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S112>/Action Port'
           */
          /* Selector: '<S112>/Selector' incorporates:
           *  Gain: '<S117>/Gain'
           *  Sum: '<S117>/Sum1'
           */
          j = mul_s32_sat(13L, 0L);
          if (j > 2147483646L) {
            j = MAX_int32_T;
          } else {
            j++;
          }

          /* Product: '<S112>/Product3' incorporates:
           *  Selector: '<S112>/Selector'
           *  UnitDelay: '<S97>/Unit Delay1'
           */
          rtDW.Merge_d = rtDW.UnitDelay1_DSTATE_g[(int16_T)j - 1] *
            rtDW.Product4;

          /* Selector: '<S112>/Selector' incorporates:
           *  Gain: '<S117>/Gain'
           *  Sum: '<S117>/Sum1'
           */
          j = mul_s32_sat(13L, 0L);
          if (j > 2147483646L) {
            j = MAX_int32_T;
          } else {
            j++;
          }

          /* Sum: '<S112>/Sum' incorporates:
           *  Product: '<S112>/Product'
           *  Product: '<S112>/Product2'
           *  Selector: '<S112>/Selector'
           *  Selector: '<S112>/Selector1'
           *  UnitDelay: '<S97>/Unit Delay'
           *  UnitDelay: '<S97>/Unit Delay1'
           */
          rtDW.Merge1_f = rtDW.Product4 * rtDW.UnitDelay_DSTATE_k[0] -
            rtDW.UnitDelay1_DSTATE_g[(int16_T)j - 1] * rtDW.sqrt_od;

          /* End of Outputs for SubSystem: '<S97>/If Action Subsystem1' */
        } else {
          if (((int16_T)s136_iter > 1) && ((int16_T)s136_iter != LOS)) {
            /* Outputs for IfAction SubSystem: '<S97>/If Action Subsystem2' incorporates:
             *  ActionPort: '<S113>/Action Port'
             */
            /* Sum: '<S120>/Sum' */
            i = (int16_T)s136_iter + 1;
            qY_0 = (int16_T)idx2 + 1;

            /* Gain: '<S119>/Gain' */
            j = mul_s32_sat(13L, LOS);

            /* Selector: '<S113>/Selector' incorporates:
             *  Sum: '<S119>/Sum1'
             */
            if (j > MAX_int32_T - (int16_T)s136_iter) {
              j_0 = MAX_int32_T;
            } else {
              j_0 = (int16_T)s136_iter + j;
            }

            /* Switch: '<S113>/Switch' incorporates:
             *  Constant: '<S113>/Constant'
             *  DataTypeConversion: '<S122>/Data Type Conversion'
             *  RelationalOperator: '<S122>/Relational Operator'
             *  Selector: '<S113>/Selector1'
             *  Sum: '<S121>/Sum'
             *  Sum: '<S121>/Sum2'
             *  Sum: '<S122>/Sum2'
             *  UnitDelay: '<S97>/Unit Delay'
             */
            if (((int16_T)s136_iter - 2 >= (int16_T)idx2) > 0.5) {
              rtb_Sum1_i_idx_3 = rtDW.UnitDelay_DSTATE_k[((int16_T)s136_iter - 2)
                * 13 + (int16_T)idx2];
            } else {
              rtb_Sum1_i_idx_3 = 0.0;
            }

            /* End of Switch: '<S113>/Switch' */

            /* Sum: '<S113>/Sum' incorporates:
             *  Constant: '<S113>/k[13][13]'
             *  Product: '<S113>/Product'
             *  Product: '<S113>/Product1'
             *  Product: '<S113>/Product4'
             *  Selector: '<S113>/Selector'
             *  Selector: '<S113>/Selector1'
             *  Selector: '<S113>/Selector2'
             *  SignalConversion: '<S113>/TmpSignal ConversionAtSelector1Inport3'
             *  Sum: '<S120>/Sum'
             *  UnitDelay: '<S97>/Unit Delay'
             *  UnitDelay: '<S97>/Unit Delay1'
             */
            rtDW.Merge1_f = (rtDW.UnitDelay_DSTATE_k[((int16_T)s136_iter - 1) *
                             13 + LOS] * rtDW.Product4 -
                             rtDW.UnitDelay1_DSTATE_g[(int16_T)j_0 - 1] *
                             rtDW.sqrt_od) - rtConstP.pooled9[((i - 1) * 13 +
              qY_0) - 1] * rtb_Sum1_i_idx_3;

            /* Switch: '<S113>/Switch1' incorporates:
             *  Constant: '<S113>/Constant1'
             *  DataTypeConversion: '<S123>/Data Type Conversion'
             *  RelationalOperator: '<S123>/Relational Operator'
             *  Selector: '<S113>/Selector'
             *  Sum: '<S123>/Sum2'
             *  UnitDelay: '<S97>/Unit Delay1'
             */
            if (((int16_T)s136_iter - 2 >= LOS) > 0.5) {
              /* Selector: '<S113>/Selector' incorporates:
               *  Sum: '<S119>/Sum1'
               *  Sum: '<S119>/Sum2'
               */
              qY = (int16_T)s136_iter - 1;
              if (j > MAX_int32_T - qY) {
                j_0 = MAX_int32_T;
              } else {
                j_0 = qY + j;
              }

              rtb_Sum1_i_idx_3 = rtDW.UnitDelay1_DSTATE_g[(int16_T)j_0 - 1];
            } else {
              rtb_Sum1_i_idx_3 = 0.0;
            }

            /* End of Switch: '<S113>/Switch1' */

            /* Selector: '<S113>/Selector' incorporates:
             *  Sum: '<S119>/Sum1'
             */
            if (j > MAX_int32_T - (int16_T)s136_iter) {
              j = MAX_int32_T;
            } else {
              j += (int16_T)s136_iter;
            }

            /* Sum: '<S113>/Sum1' incorporates:
             *  Constant: '<S113>/k[13][13]'
             *  Product: '<S113>/Product2'
             *  Product: '<S113>/Product3'
             *  Selector: '<S113>/Selector'
             *  Selector: '<S113>/Selector2'
             *  Sum: '<S120>/Sum'
             *  UnitDelay: '<S97>/Unit Delay1'
             */
            rtDW.Merge_d = rtDW.UnitDelay1_DSTATE_g[(int16_T)j - 1] *
              rtDW.Product4 - rtConstP.pooled9[((i - 1) * 13 + qY_0) - 1] *
              rtb_Sum1_i_idx_3;

            /* End of Outputs for SubSystem: '<S97>/If Action Subsystem2' */
          }
        }

        /* End of If: '<S97>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' */

        /* Sum: '<S97>/Sum' */
        i = (int16_T)s136_iter + 1;
        qY_0 = (int16_T)idx2 + 1;

        /* Assignment: '<S97>/Assignment' incorporates:
         *  Sum: '<S97>/Sum'
         *  UnitDelay: '<S97>/Unit Delay'
         */
        if (rtDW.ForIterator_IterationMarker[2] < 2) {
          rtDW.ForIterator_IterationMarker[2L] = 2U;
          memcpy(&rtDW.Assignment_k[0], &rtDW.UnitDelay_DSTATE_k[0], 169U *
                 sizeof(real_T));
        }

        rtDW.Assignment_k[(qY_0 + 13 * (i - 1)) - 1] = rtDW.Merge1_f;

        /* End of Assignment: '<S97>/Assignment' */

        /* Assignment: '<S97>/Assignment_snorm' incorporates:
         *  Gain: '<S114>/Gain'
         *  Sum: '<S114>/Sum1'
         *  Sum: '<S114>/Sum2'
         *  Sum: '<S97>/Sum'
         *  UnitDelay: '<S97>/Unit Delay1'
         */
        if (rtDW.ForIterator_IterationMarker[3] < 2) {
          rtDW.ForIterator_IterationMarker[3L] = 2U;
          memcpy(&rtDW.Assignment_snorm[0], &rtDW.UnitDelay1_DSTATE_g[0], 169U *
                 sizeof(real_T));
        }

        rtDW.Assignment_snorm[(int16_T)((real_T)i + (real_T)mul_s32_sat(13L,
          qY_0 - 1)) - 1] = rtDW.Merge_d;

        /* End of Assignment: '<S97>/Assignment_snorm' */

        /* Update for UnitDelay: '<S97>/Unit Delay' */
        memcpy(&rtDW.UnitDelay_DSTATE_k[0], &rtDW.Assignment_k[0], 169U * sizeof
               (real_T));

        /* Update for UnitDelay: '<S97>/Unit Delay1' */
        memcpy(&rtDW.UnitDelay1_DSTATE_g[0], &rtDW.Assignment_snorm[0], 169U *
               sizeof(real_T));
      }

      /* End of Outputs for SubSystem: '<S95>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

      /* Selector: '<S96>/snorm[n+m*13]' incorporates:
       *  Gain: '<S100>/Gain'
       *  Sum: '<S100>/Sum1'
       *  Sum: '<S95>/Sum1'
       */
      i = 1 + (int16_T)s136_iter;
      j = mul_s32_sat(13L, LOS);
      if (j > MAX_int32_T - i) {
        j = MAX_int32_T;
      } else {
        j += i;
      }

      /* Product: '<S96>/par' incorporates:
       *  Selector: '<S96>/snorm[n+m*13]'
       */
      rtb_Sum = rtDW.Assignment_snorm[(int16_T)j - 1] * days;

      /* Outputs for Enabled SubSystem: '<S96>/Special case - North//South Geographic Pole' incorporates:
       *  EnablePort: '<S99>/Enable'
       */
      if ((rtDW.sqrt_od == 0.0) && (1 == LOS)) {
        if (!rtDW.SpecialcaseNorthSouthGeographic) {
          rtDW.SpecialcaseNorthSouthGeographic = true;
        }

        /* If: '<S99>/n ==1' incorporates:
         *  Assignment: '<S104>/Assignment2'
         */
        if ((int16_T)s136_iter == 1) {
          /* Outputs for IfAction SubSystem: '<S99>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S103>/Action Port'
           */
          /* Assignment: '<S103>/Assignment2' incorporates:
           *  Selector: '<S103>/pp[n-1]'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          if (rtDW.ForIterator_IterationMarker[0] < 2) {
            rtDW.ForIterator_IterationMarker[0L] = 2U;
            memcpy(&rtDW.Assignment2_p[0], &rtDW.UnitDelay1_DSTATE_j[0], 13U *
                   sizeof(real_T));
          }

          rtDW.Assignment2_p[1] = rtDW.UnitDelay1_DSTATE_j[0];

          /* End of Assignment: '<S103>/Assignment2' */
          /* End of Outputs for SubSystem: '<S99>/If Action Subsystem1' */
        } else {
          /* Outputs for IfAction SubSystem: '<S99>/If Action Subsystem2' incorporates:
           *  ActionPort: '<S104>/Action Port'
           */
          if (rtDW.ForIterator_IterationMarker[1] < 2) {
            /* Assignment: '<S104>/Assignment2' incorporates:
             *  UnitDelay: '<S99>/Unit Delay1'
             */
            rtDW.ForIterator_IterationMarker[1L] = 2U;
            memcpy(&rtDW.Assignment2_j[0], &rtDW.UnitDelay1_DSTATE_j[0], 13U *
                   sizeof(real_T));
          }

          /* Assignment: '<S104>/Assignment2' incorporates:
           *  Constant: '<S104>/k[13][13]'
           *  Product: '<S104>/Product1'
           *  Product: '<S104>/Product2'
           *  Selector: '<S104>/Selector2'
           *  Selector: '<S104>/pp[n-2] pp[n-1]'
           *  Sum: '<S104>/Sum1'
           *  Sum: '<S104>/Sum2'
           *  Sum: '<S105>/Sum2'
           *  Sum: '<S106>/Sum'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          rtDW.Assignment2_j[(int16_T)s136_iter] = rtDW.UnitDelay1_DSTATE_j
            [(int16_T)s136_iter - 1] * rtDW.Product4 - rtConstP.pooled9[(int16_T)
            s136_iter * 13 + 1] * rtDW.UnitDelay1_DSTATE_j[(int16_T)s136_iter -
            2];

          /* End of Outputs for SubSystem: '<S99>/If Action Subsystem2' */
        }

        /* End of If: '<S99>/n ==1' */

        /* SignalConversion: '<S99>/TmpSignal ConversionAtpp[n]Inport1' incorporates:
         *  UnitDelay: '<S99>/Unit Delay1'
         */
        rtb_TmpSignalConversionAtppnInp[0] = rtDW.UnitDelay1_DSTATE_j[0];
        rtb_TmpSignalConversionAtppnInp[1] = rtDW.Assignment2_p[1];
        memcpy(&rtb_TmpSignalConversionAtppnInp[2], &rtDW.Assignment2_j[2], 11U *
               sizeof(real_T));

        /* Product: '<S99>/Product2' incorporates:
         *  Selector: '<S99>/pp[n]'
         *  Sum: '<S99>/Sum2'
         */
        rtDW.Product2 = days * rtb_TmpSignalConversionAtppnInp[(int16_T)
          s136_iter] * b_min;

        /* Update for UnitDelay: '<S99>/Unit Delay1' */
        memcpy(&rtDW.UnitDelay1_DSTATE_j[0], &rtb_TmpSignalConversionAtppnInp[0],
               13U * sizeof(real_T));
      } else {
        if (rtDW.SpecialcaseNorthSouthGeographic) {
          /* Disable for Outport: '<S99>/bpp' */
          rtDW.Product2 = 0.0;
          rtDW.SpecialcaseNorthSouthGeographic = false;
        }
      }

      /* End of Outputs for SubSystem: '<S96>/Special case - North//South Geographic Pole' */

      /* Sum: '<S96>/Sum1' incorporates:
       *  Constant: '<S102>/Constant'
       *  Constant: '<S102>/Constant1'
       *  Logic: '<S102>/Logical Operator'
       *  Product: '<S96>/Product'
       *  RelationalOperator: '<S102>/Relational Operator'
       *  RelationalOperator: '<S102>/Relational Operator1'
       *  Selector: '<S96>/dp[n][m]'
       *  Sum: '<S95>/Sum1'
       *  Sum: '<S96>/Sum'
       *  UnitDelay: '<S96>/Unit Delay1'
       */
      rtDW.Sum1 = rtDW.UnitDelay1_DSTATE_k - rtDW.Assignment_k[(int16_T)
        s136_iter * 13 + (int16_T)idx2] * rtb_z1 * days;

      /* Sum: '<S96>/Sum2' incorporates:
       *  Constant: '<S96>/fm'
       *  Product: '<S96>/Product1'
       *  Selector: '<S96>/fm[m]'
       *  Sum: '<S96>/Sum4'
       *  UnitDelay: '<S96>/Unit Delay3'
       */
      rtDW.Sum2 = rtConstP.fm_Value[(int16_T)idx2] * rtb_Sum * b_min +
        rtDW.UnitDelay3_DSTATE;

      /* Sum: '<S96>/Sum3' incorporates:
       *  Constant: '<S96>/fn'
       *  Product: '<S96>/Product2'
       *  Selector: '<S96>/fn[m]'
       *  Sum: '<S96>/Sum4'
       *  UnitDelay: '<S96>/Unit Delay2'
       */
      rtDW.Sum3 = rtConstP.fn_Value[(int16_T)s136_iter] * rtb_Sum * rtb_z1 +
        rtDW.UnitDelay2_DSTATE_d;

      /* Sum: '<S96>/Sum5' incorporates:
       *  UnitDelay: '<S96>/Unit Delay4'
       */
      rtDW.Sum5 = rtDW.UnitDelay4_DSTATE + rtDW.Product2;

      /* Update for UnitDelay: '<S96>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_k = rtDW.Sum1;

      /* Update for UnitDelay: '<S96>/Unit Delay3' */
      rtDW.UnitDelay3_DSTATE = rtDW.Sum2;

      /* Update for UnitDelay: '<S96>/Unit Delay2' */
      rtDW.UnitDelay2_DSTATE_d = rtDW.Sum3;

      /* Update for UnitDelay: '<S96>/Unit Delay4' */
      rtDW.UnitDelay4_DSTATE = rtDW.Sum5;
    }

    /* End of Outputs for SubSystem: '<S87>/For Iterator Subsystem' */

    /* Sum: '<S87>/Sum1' incorporates:
     *  UnitDelay: '<S87>/Unit Delay2'
     */
    rtb_Sum1_i_idx_0 = rtb_time_ut1_idx_3 + rtDW.Sum1;
    rtb_Sum1_i_idx_1 = rtb_time_ut1_idx_4 + rtDW.Sum2;
    rtb_Sum1_i_idx_2 = rtb_time_ut1_idx_5 + rtDW.Sum3;
    rtb_Sum1_i_idx_3 = C_1 + rtDW.Sum5;

    /* Update for UnitDelay: '<S87>/Unit Delay2' */
    rtb_time_ut1_idx_3 = rtb_Sum1_i_idx_0;
    rtb_time_ut1_idx_4 = rtb_Sum1_i_idx_1;
    rtb_time_ut1_idx_5 = rtb_Sum1_i_idx_2;
    C_1 = rtb_Sum1_i_idx_3;
  }

  /* End of Outputs for SubSystem: '<S85>/Compute magnetic vector in spherical coordinates' */

  /* Switch: '<S139>/Switch' incorporates:
   *  Product: '<S139>/Product'
   */
  if (rtDW.sqrt_od != 0.0) {
    rtb_z1 = rtb_Sum1_i_idx_1 / rtDW.sqrt_od;
  } else {
    rtb_z1 = rtb_Sum1_i_idx_3;
  }

  /* End of Switch: '<S139>/Switch' */

  /* Sum: '<S138>/Sum1' incorporates:
   *  Product: '<S138>/Product1'
   *  Product: '<S138>/Product4'
   */
  rtb_Sum = (0.0 - rtDW.Product11 * rtb_Sum1_i_idx_0) - rtb_Sum1_i_idx_2 *
    rtDW.Product12;

  /* UnitConversion: '<S143>/Unit Conversion' incorporates:
   *  Trigonometry: '<S141>/Trigonometric Function1'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  rtb_Switch = 57.295779513082323 * rt_atan2d_snf(rtb_z1, rtb_Sum);

  /* Sum: '<S140>/Sum1' incorporates:
   *  Product: '<S140>/Product1'
   *  Product: '<S140>/Product4'
   */
  rtb_time_ut1_idx_2 = rtDW.Product12 * rtb_Sum1_i_idx_0 - rtb_Sum1_i_idx_2 *
    rtDW.Product11;

  /* Sum: '<S141>/Sum' incorporates:
   *  Product: '<S141>/Product'
   *  Product: '<S141>/Product1'
   */
  rtb_z1 = rtb_z1 * rtb_z1 + rtb_Sum * rtb_Sum;

  /* UnitConversion: '<S86>/Unit Conversion' incorporates:
   *  Sqrt: '<S141>/sqrt1'
   *  Trigonometry: '<S141>/Trigonometric Function'
   *  UnitConversion: '<S142>/Unit Conversion'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  C_1 = 0.017453292519943295 * rtb_Switch;
  rtb_time_ut1_idx_3 = 57.295779513082323 * rt_atan2d_snf(rtb_time_ut1_idx_2,
    sqrtf(rtb_z1)) * 0.017453292519943295;

  /* Trigonometry: '<S80>/sincos' */
  rtb_Sum = cosf(C_1);
  C_1 = sinf(C_1);

  /* Sum: '<S141>/Sum1' incorporates:
   *  Product: '<S141>/Product2'
   */
  rtb_z1 += rtb_time_ut1_idx_2 * rtb_time_ut1_idx_2;

  /* Sqrt: '<S141>/sqrt' */
  rtb_z1 = sqrtf(rtb_z1);

  /* Product: '<S80>/h1' incorporates:
   *  Trigonometry: '<S80>/sincos'
   */
  rtb_time_ut1_idx_2 = cosf(rtb_time_ut1_idx_3) * rtb_z1;

  /* Product: '<S80>/x1' */
  rtb_Sum *= rtb_time_ut1_idx_2;

  /* Product: '<S80>/y1' */
  rtb_time_ut1_idx_2 *= C_1;

  /* Product: '<S80>/z1' incorporates:
   *  Trigonometry: '<S80>/sincos'
   */
  rtb_z1 *= sinf(rtb_time_ut1_idx_3);

  /* Gain: '<S4>/Gain' */
  rtb_Sum1_i_idx_3 = 1.0E-9 * rtb_Sum;
  rtb_Sum1_i_idx_1 = 1.0E-9 * rtb_time_ut1_idx_2;
  rtb_Sum1_i_idx_0 = 1.0E-9 * rtb_z1;

  /* Product: '<S4>/Product' incorporates:
   *  Math: '<S4>/Math Function'
   */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    M[lowAlt] = rtb_VectorConcatenate_o[3 * lowAlt + 2] * rtb_Sum1_i_idx_0 +
      (rtb_VectorConcatenate_o[3 * lowAlt + 1] * rtb_Sum1_i_idx_1 +
       rtb_VectorConcatenate_o[3 * lowAlt] * rtb_Sum1_i_idx_3);
  }

  /* End of Product: '<S4>/Product' */

  /* Product: '<S4>/Product1' */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    rtb_Sum_g[lowAlt] = rtb_VectorConcatenate_n[lowAlt + 6] * M[2] +
      (rtb_VectorConcatenate_n[lowAlt + 3] * M[1] +
       rtb_VectorConcatenate_n[lowAlt] * M[0]);
  }

  /* End of Product: '<S4>/Product1' */

  /* S-Function (sdsp2norm2): '<S4>/Normalization' */
  s136_iter = 0L;
  i = 0;
  while (i < 1) {
    idx1 = s136_iter;
    idx2 = s136_iter;
    j = 0L;
    while ((int16_T)j < 1) {
      rtb_time_ut1_idx_2 = 0.0;
      for (lowAlt = 0; lowAlt < 3; lowAlt++) {
        rtb_time_ut1_idx_2 += rtb_Sum_g[idx1] * rtb_Sum_g[idx1];
        idx1++;
      }

      rtb_time_ut1_idx_2 = 1.0 / (sqrtf(rtb_time_ut1_idx_2) + 1.0E-10);
      for (lowAlt = 0; lowAlt < 3; lowAlt++) {
        M[idx2] = rtb_Sum_g[idx2] * rtb_time_ut1_idx_2;
        idx2++;
      }

      j = 1L;
    }

    s136_iter++;
    i = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S4>/Normalization' */
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/mag_unit_vector_eci' */
    rtY.mag_unit_vector_eci[i] = M[i];

    /* Outport: '<Root>/mag_vector_eci' */
    rtY.mag_vector_eci[i] = rtb_Sum_g[i];

    /* Outport: '<Root>/vel_eci_mps' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Product: '<S5>/Product1'
     */
    rtY.vel_eci_mps[i] = 1000.0 * (rtb_teme_to_gcrf[i + 6] * b_o +
      (rtb_teme_to_gcrf[i + 3] * xi + rtb_teme_to_gcrf[i] * deps_1980));
  }

  /* Assertion: '<S77>/Assertion' incorporates:
   *  Constant: '<S77>/max_val'
   *  Constant: '<S77>/min_val'
   *  Logic: '<S77>/conjunction'
   *  RelationalOperator: '<S77>/max_relop'
   *  RelationalOperator: '<S77>/min_relop'
   */
  utAssert((0.0 <= rtb_Sum3) && (rtb_Sum3 <= 1.0E+6));

  /* Assertion: '<S78>/Assertion' incorporates:
   *  Constant: '<S78>/max_val'
   *  Constant: '<S78>/min_val'
   *  Logic: '<S78>/conjunction'
   *  RelationalOperator: '<S78>/max_relop'
   *  RelationalOperator: '<S78>/min_relop'
   */
  utAssert((-90.0 <= rtb_T_TT_J2000) && (rtb_T_TT_J2000 <= 90.0));

  /* Assertion: '<S79>/Assertion' incorporates:
   *  Constant: '<S79>/max_val'
   *  Constant: '<S79>/min_val'
   *  Logic: '<S79>/conjunction'
   *  RelationalOperator: '<S79>/max_relop'
   *  RelationalOperator: '<S79>/min_relop'
   */
  utAssert((-180.0 <= eta) && (eta <= 180.0));

  /* Assertion: '<S81>/Assertion' incorporates:
   *  Constant: '<S81>/max_val'
   *  Constant: '<S81>/min_val'
   *  Constant: '<S85>/epoch'
   *  Logic: '<S81>/conjunction'
   *  RelationalOperator: '<S81>/max_relop'
   *  RelationalOperator: '<S81>/min_relop'
   *  Sum: '<S85>/Sum'
   */
  utAssert((0.0 <= rtb_Abs1 - 2015.0) && (rtb_Abs1 - 2015.0 <= 5.0));

  /* Update for Memory: '<S93>/otime' */
  rtDW.otime_PreviousInput = rtb_Abs1;

  /* Update for Memory: '<S92>/olon' */
  rtDW.olon_PreviousInput = JD_days_gps;

  /* Update for Memory: '<S91>/olat' */
  rtDW.olat_PreviousInput = T1900;

  /* Update for Memory: '<S91>/oalt' */
  rtDW.oalt_PreviousInput = rtb_time_ut1_idx_0;
}

/* Model initialize function */
void env_estimation_lib_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    int16_T i;

    /* InitializeConditions for Memory: '<S93>/otime' */
    rtDW.otime_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S92>/olon' */
    rtDW.olon_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S91>/olat' */
    rtDW.olat_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S91>/oalt' */
    rtDW.oalt_PreviousInput = -1000.0;

    /* SystemInitialize for Enabled SubSystem: '<S85>/Convert from geodetic to  spherical coordinates ' */
    /* SystemInitialize for Outport: '<S89>/sp[13]' */
    memcpy(&rtDW.OutportBufferForsp13[0], &rtConstP.sp13_Y0[0], 13U * sizeof
           (real_T));

    /* SystemInitialize for Outport: '<S89>/cp[13]' */
    memcpy(&rtDW.OutportBufferForcp13[0], &rtConstP.cp13_Y0[0], 13U * sizeof
           (real_T));

    /* End of SystemInitialize for SubSystem: '<S85>/Convert from geodetic to  spherical coordinates ' */

    /* SystemInitialize for Enabled SubSystem: '<S85>/Convert from geodetic to  spherical coordinates' */
    /* SystemInitialize for Outport: '<S88>/r' */
    rtDW.sqrt_o = 6378.137;

    /* SystemInitialize for Outport: '<S88>/ct' */
    rtDW.Product4 = 1.0;

    /* End of SystemInitialize for SubSystem: '<S85>/Convert from geodetic to  spherical coordinates' */

    /* SystemInitialize for Iterator SubSystem: '<S85>/Compute magnetic vector in spherical coordinates' */
    /* SystemInitialize for Iterator SubSystem: '<S87>/For Iterator Subsystem' */
    /* SystemInitialize for Enabled SubSystem: '<S95>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */
    /* InitializeConditions for UnitDelay: '<S97>/Unit Delay1' */
    memcpy(&rtDW.UnitDelay1_DSTATE_g[0], &rtConstP.pooled12[0], 169U * sizeof
           (real_T));

    /* SystemInitialize for Outport: '<S97>/snorm[169]' */
    memcpy(&rtDW.Assignment_snorm[0], &rtConstP.pooled12[0], 169U * sizeof
           (real_T));

    /* End of SystemInitialize for SubSystem: '<S95>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

    /* SystemInitialize for Enabled SubSystem: '<S96>/Special case - North//South Geographic Pole' */
    /* SystemInitialize for IfAction SubSystem: '<S99>/If Action Subsystem2' */
    /* SystemInitialize for IfAction SubSystem: '<S99>/If Action Subsystem1' */
    for (i = 0; i < 13; i++) {
      /* InitializeConditions for UnitDelay: '<S99>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_j[i] = 1.0;

      /* SystemInitialize for Outport: '<S103>/pp[13]' */
      rtDW.Assignment2_p[i] = 1.0;

      /* SystemInitialize for Outport: '<S104>/pp[13]' */
      rtDW.Assignment2_j[i] = 1.0;
    }

    /* End of SystemInitialize for SubSystem: '<S99>/If Action Subsystem1' */
    /* End of SystemInitialize for SubSystem: '<S99>/If Action Subsystem2' */
    /* End of SystemInitialize for SubSystem: '<S96>/Special case - North//South Geographic Pole' */
    /* End of SystemInitialize for SubSystem: '<S87>/For Iterator Subsystem' */
    /* End of SystemInitialize for SubSystem: '<S85>/Compute magnetic vector in spherical coordinates' */
  }
}

/* Model terminate function */
void env_estimation_lib_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
