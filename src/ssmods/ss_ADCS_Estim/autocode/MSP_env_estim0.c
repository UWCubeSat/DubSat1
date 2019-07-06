/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_env_estim0.c
 *
 * Code generated for Simulink model 'MSP_env_estim0'.
 *
 * Model version                  : 1.368
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Tue May  1 18:59:45 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MSP_env_estim0.h"
#define NumBitsPerChar                 8U

/* Block signals and states (auto storage) */
#pragma NOINIT(rtDW)
DW rtDW;

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern real_T rt_roundd_snf(real_T u);
extern real_T rt_remd_snf(real_T u0, real_T u1);
extern real_T rt_powd_snf(real_T u0, real_T u1);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);
void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo);
int32_T mul_s32_sat(int32_T a, int32_T b);

/* Forward declaration for local functions */
static real_T norm_i(const real_T x[3]);
static real_T norm(const real_T x[3]);
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

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
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
      u1_0 = ceil(u1);
    } else {
      u1_0 = floor(u1);
    }

    if ((u1 != 0.0) && (u1 != u1_0)) {
      u1_0 = u0 / u1;
      if (fabs(u1_0 - rt_roundd_snf(u1_0)) <= DBL_EPSILON * fabs(u1_0)) {
        y = 0.0;
      } else {
        y = fmod(u0, u1);
      }
    } else {
      y = fmod(u0, u1);
    }
  }

  return y;
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

    y = atan2(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Function for MATLAB Function: '<S6>/MATLAB Function1' */
static real_T norm_i(const real_T x[3])
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

/* Function for MATLAB Function: '<S9>/MATLAB Function1' */
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

/* Model step function */
void MSP_env_estim0_step(void)
{
  real_T week;
  real_T T1900;
  real_T days;
  real_T dayofyear;
  real_T day;
  real_T month;
  real_T b_min;
  real_T b_s;
  real_T b_c;
  real_T c_s;
  static const real_T a[9] = { 1.0, 0.0, 0.0, 0.0, 0.91748214306524178,
    0.397776969112606, 0.0, -0.397776969112606, 0.91748214306524178 };

  static const int8_T b[3] = { 1, 0, 0 };

  static const int8_T d[3] = { 0, 0, 1 };

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

  static const int8_T d_0[3] = { 0, 0, 1 };

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
  real_T IL4;
  real_T IL5;
  real_T M_DF;
  real_T M[3];
  int8_T s72_iter;
  real_T rtb_VectorConcatenate[9];
  boolean_T rtb_RelationalOperator;
  boolean_T rtb_LogicalOperator;
  int32_T idx1;
  int32_T idx2;
  int32_T j;
  real_T rtb_VectorConcatenate_b[9];
  int8_T rtb_FLAG;
  int32_T s138_iter;
  real_T rtb_Sum;
  real_T rtb_T_TT_J2000;
  real_T rtb_sun_vector_eci_km[3];
  real_T rtb_pos_teme_km[3];
  real_T rtb_Assignment[11];
  real_T rtb_Assignment1[11];
  real_T rtb_TmpSignalConversionAtppnInp[13];
  real_T rtb_itrf_to_gcrf[9];
  real_T rtb_mod_to_gcrf[9];
  real_T rtb_teme_to_gcrf[9];
  real_T rtb_tc_old[169];
  real_T Assignment[169];
  real_T Assignment2[169];
  int16_T i;
  real_T rtb_sun_vector_eci_km_0[3];
  real_T b_min_0[9];
  real_T b_0[9];
  real_T dayofyear_0[9];
  real_T T1900_0[3];
  real_T c_0[9];
  real_T rtb_time_ut1_idx_0;
  real_T rtb_time_ut1_idx_2;
  real_T rtb_time_ut1_idx_3;
  real_T rtb_time_ut1_idx_4;
  real_T rtb_time_ut1_idx_5;
  real_T rtb_Sum1_a_idx_3;
  real_T rtb_Sum1_a_idx_1;
  real_T rtb_Sum1_a_idx_0;
  real_T rtb_Sum1_a_idx_2;
  int16_T qY;
  int16_T qY_0;
  int16_T qY_1;
  int32_T j_0;

  /* Sum: '<S2>/Sum' incorporates:
   *  Inport: '<Root>/MET'
   *  Inport: '<Root>/MET_epoch'
   */
  rtb_Sum = rtU.MET + rtU.MET_epoch;

  /* MATLAB Function: '<S2>/MATLAB Function' */
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
  /* '<S8>:1:30' week    = floor(JD_GPS_since_epoch/dayinweek); */
  week = floor((((rtb_Sum * 1.1574074074074073E-5 - 0.00059240740740740738) +
                 2.451545E+6) - 2.4442445E+6) / 7.0);

  /* MATLAB Function: '<S152>/time-conversion-lib' incorporates:
   *  Constant: '<S7>/dut1'
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
  /* MATLAB Function 'time_conversion_lib/time-conversion-lib': '<S154>:1' */
  /*  Constants */
  /* '<S154>:1:16' dAT         = 37.0; */
  /*  From MJD 58158 (Feb 7, 2018) */
  /* '<S154>:1:17' gps2tai     = 19.0; */
  /*  From MJD 58158 */
  /* '<S154>:1:18' gps2tt      = 32.184 + gps2tai; */
  /* '<S154>:1:19' gps2utc     = gps2tai - dAT; */
  /* '<S154>:1:20' gps2ut1     = gps2tai - dAT + dUT1; */
  /* '<S154>:1:21' sec2day     = 1/86400; */
  /* '<S154>:1:22' week2day    = 7; */
  /* '<S154>:1:23' JD_GPSepoch = 2444244.5; */
  /* '<S154>:1:24' JD_J2000    = 2451545.0; */
  /* '<S154>:1:25' JD2cent     = 1/36525; */
  /* '<S154>:1:27' sec_in = cur_gps(1); */
  /* '<S154>:1:27' week_in = cur_gps(2); */
  /*  --------------------------- % */
  /*  Translate GPS w-s to JD and Gregorian ymd-hms */
  /* '<S154>:1:31' JD_days_gps     = JD_GPSepoch + week2day*week_in + sec2day*sec_in; */
  week = ((((rtb_Sum * 1.1574074074074073E-5 - 0.00059240740740740738) +
            2.451545E+6) - 2.4442445E+6) - week * 7.0) * 86400.0 *
    1.1574074074074073E-5 + (7.0 * week + 2.4442445E+6);

  /* '<S154>:1:32' JD_utc          = JD_days_gps + sec2day*gps2utc; */
  /* '<S154>:1:33' JD_ut1          = JD_days_gps + sec2day*gps2ut1; */
  /* '<S154>:1:34' JD_TT           = JD_days_gps + sec2day*gps2tt; */
  /* '<S154>:1:36' JD_utc_J2000    = JD_utc - JD_J2000; */
  /* '<S154>:1:37' JD_ut1_J2000    = JD_ut1 - JD_J2000; */
  /* '<S154>:1:38' T_ut1_J2000     = JD_ut1_J2000*JD2cent; */
  rtb_Sum = ((week + -0.00020759027777777776) - 2.451545E+6) *
    2.7378507871321012E-5;

  /* '<S154>:1:39' T_TT_J2000      = JD2cent*( JD_TT - JD_J2000 ); */
  rtb_T_TT_J2000 = ((week + 0.00059240740740740738) - 2.451545E+6) *
    2.7378507871321012E-5;

  /* '<S154>:1:41' time_ut1 = JD_2_ymdhms( JD_ut1 ); */
  /*  This algorithm is based on Vallado 4e algorithm 22, pp 202. */
  /*  Constants */
  /* '<S154>:1:49' JD_1900     = 2415019.5; */
  /* '<S154>:1:50' Lmonth      = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
  /*  Convert JD to date-time */
  /* '<S154>:1:53' T1900   = (JD - JD_1900)/365.25; */
  T1900 = ((week + -0.00020759027777777776) - 2.4150195E+6) / 365.25;

  /*  base epoch is 1900 */
  /* '<S154>:1:54' year    = 1900 + floor(T1900); */
  day = floor(T1900);
  T1900 = 1900.0 + floor(T1900);

  /* '<S154>:1:56' leapyrs = floor((year - 1900 - 1)*0.25); */
  /* '<S154>:1:57' days    = (JD - JD_1900) - ((year - 1900)*365 + leapyrs ); */
  days = ((week + -0.00020759027777777776) - 2.4150195E+6) - (floor((((1900.0 +
    day) - 1900.0) - 1.0) * 0.25) + ((1900.0 + day) - 1900.0) * 365.0);

  /* '<S154>:1:59' if days < 1 */
  if (days < 1.0) {
    /* '<S154>:1:60' year    = year - 1; */
    T1900 = (1900.0 + day) - 1.0;

    /* '<S154>:1:61' leapyrs = floor((year - 1900 - 1)*0.25); */
    /* '<S154>:1:62' days    = (JD - JD_1900) - ((year - 1900)*365 + leapyrs ); */
    days = ((week + -0.00020759027777777776) - 2.4150195E+6) - (floor(((((1900.0
      + day) - 1.0) - 1900.0) - 1.0) * 0.25) + (((1900.0 + day) - 1.0) - 1900.0)
      * 365.0);
  }

  /* '<S154>:1:65' if mod(year,4) == 0 */
  /* '<S154>:1:69' dayofyear   = floor(days); */
  dayofyear = floor(days);

  /* '<S154>:1:71' day     = 0; */
  /* '<S154>:1:72' month   = 0; */
  /* '<S154>:1:74' while day < dayofyear */
  /* dayofmonth = dayofyear - (day - Lmonth(month)); */
  /* '<S154>:1:81' tau     = 24*( days-dayofyear ); */
  day = (days - dayofyear) * 24.0;

  /* '<S154>:1:82' hour    = floor( tau ); */
  days = floor(day);

  /* '<S154>:1:83' min     = floor( 60*(tau - hour) ); */
  b_min = floor((day - days) * 60.0);

  /* '<S154>:1:84' sec     = 3600*( tau - hour - (min/60) ); */
  /* '<S154>:1:86' time    = [ year, month, dayofyear, hour, min, sec ]; */
  rtb_time_ut1_idx_0 = T1900;
  rtb_time_ut1_idx_2 = dayofyear;
  rtb_time_ut1_idx_3 = days;
  rtb_time_ut1_idx_4 = b_min;
  rtb_time_ut1_idx_5 = ((day - days) - b_min / 60.0) * 3600.0;

  /* MATLAB Function: '<S6>/MATLAB Function' */
  /*  ----------------------------------------------------------------------- % */
  /*  UW HuskySat-1, ADCS Team */
  /*  Algorithm to find the sun vector in the ECI frame. Based on algorithm 29  */
  /*    from Vallado 4e (pp. 279-280). */
  /*  */
  /*  T. Reynolds -- 2.23.18 */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'sun_vector_lib/MATLAB Function': '<S148>:1' */
  /*  Constants */
  /* '<S148>:1:11' AU2KM   = 149597870.700; */
  /* '<S148>:1:12' DEG2RAD     = pi/180.0; */
  /* '<S148>:1:13' TWOPI   = 2*pi; */
  /*  Mean longitude in a mean-equator of date frame */
  /* '<S148>:1:16' mean_long_sun  = rem(280.460 + 36000.771*JD_cent_ut1,360); */
  /*  [deg] */
  /*  Mean anomaly for the sun. Low precision formula since we're approximating */
  /*  T_TDB with T_UT1. */
  /* '<S148>:1:20' mean_anom_sun       = rem(357.5291092 + 35999.05034*JD_cent_ut1,360); */
  /* [deg] */
  /* '<S148>:1:21' mean_anom_sun_rad   = mod(DEG2RAD*mean_anom_sun,TWOPI); */
  T1900 = rt_remd_snf(35999.05034 * rtb_Sum + 357.5291092, 360.0) *
    0.017453292519943295 / 6.2831853071795862;
  if (fabs(T1900 - rt_roundd_snf(T1900)) <= 2.2204460492503131E-16 * fabs(T1900))
  {
    T1900 = 0.0;
  } else {
    T1900 = (T1900 - floor(T1900)) * 6.2831853071795862;
  }

  /*  Approximate the ecliptic longitude of the sun */
  /* '<S148>:1:24' long_eclp       = mean_long_sun + 1.914666471*sin(mean_anom_sun_rad) + ... */
  /* '<S148>:1:25'                     0.019994643*sin(2*mean_anom_sun_rad); */
  /* [deg] */
  /* '<S148>:1:26' long_eclp       = rem(DEG2RAD*long_eclp,360); */
  /*  [rad]    */
  /* '<S148>:1:27' long_eclp_rad   = mod(DEG2RAD*long_eclp,TWOPI); */
  month = rt_remd_snf(((rt_remd_snf(36000.771 * rtb_Sum + 280.46, 360.0) +
                        1.914666471 * sin(T1900)) + sin(2.0 * T1900) *
                       0.019994643) * 0.017453292519943295, 360.0) *
    0.017453292519943295 / 6.2831853071795862;
  if (fabs(month - rt_roundd_snf(month)) <= 2.2204460492503131E-16 * fabs(month))
  {
    month = 0.0;
  } else {
    month = (month - floor(month)) * 6.2831853071795862;
  }

  /*  Assume the ecliptic latitude is zero */
  /* '<S148>:1:30' lat_eclp     = 0; */
  /* [deg] */
  /*  Approximate the obliquity of the ecliptic using */
  /* '<S148>:1:33' oblq        = 23.439291 - 0.0130042*JD_cent_ut1; */
  /* '<S148>:1:34' oblq_rad    = DEG2RAD*oblq; */
  day = (23.439291 - 0.0130042 * rtb_Sum) * 0.017453292519943295;

  /*  Find the distance in AU from E to S */
  /* '<S148>:1:37' r           =  1.000140612 - 0.016708617*cos(mean_anom_sun_rad) - ... */
  /* '<S148>:1:38'                     0.000139589*cos(2*mean_anom_sun_rad); */
  T1900 = (1.000140612 - 0.016708617 * cos(T1900)) - cos(2.0 * T1900) *
    0.000139589;

  /* '<S148>:1:39' x_MOD_km    = r*cos(long_eclp_rad)*AU2KM; */
  /* '<S148>:1:40' y_MOD_km    = r*cos(oblq_rad)*sin(long_eclp_rad)*AU2KM; */
  /* '<S148>:1:41' z_MOD_km    = r*sin(oblq_rad)*sin(long_eclp_rad)*AU2KM; */
  /* '<S148>:1:43' sun_vector_mod_km = [x_MOD_km; y_MOD_km; z_MOD_km]; */
  /*  Convert to ECI (mean-equator-mean-equinox) frame  */
  /* '<S148>:1:46' sun_vector_eci_km   = mod2eci(sun_vector_mod_km,JD_cent_ut1); */
  /* '<S148>:1:52' [prec,~,~,~,~] = precess ( JD_cent_ut1 ); */
  /*  " to rad */
  /* '<S148>:1:60' convrt = pi / (180.0*3600.0); */
  /*  ------------------ iau 03 precession angles ------------------- */
  /* '<S148>:1:63' oblo =  84381.406; */
  /*  " */
  /* '<S148>:1:64' psia =  (((( -0.0000000951 * ttt + 0.000132851 ) * ttt - 0.00114045 ) *... */
  /* '<S148>:1:65'                         ttt - 1.0790069 ) * ttt + 5038.481507 ) * ttt; */
  /*  " */
  /* '<S148>:1:66' wa   =  ((((  0.0000003337 * ttt - 0.000000467 ) * ttt - 0.00772503 ) *... */
  /* '<S148>:1:67'                         ttt + 0.0512623 ) * ttt -    0.025754 ) * ttt + oblo; */
  /* '<S148>:1:68' ea   =  (((( -0.0000000434 * ttt - 0.000000576 ) * ttt + 0.00200340 ) *... */
  /* '<S148>:1:69'                         ttt - 0.0001831 ) * ttt -   46.836769 ) * ttt + oblo; */
  /* '<S148>:1:70' xa   =  (((( - 0.0000000560 * ttt + 0.000170663 ) * ttt - 0.00121197 ) *... */
  /* '<S148>:1:71'                         ttt - 2.3814292 ) * ttt +   10.556403 ) * ttt; */
  /* '<S148>:1:73' zeta =  (((( - 0.0000003173 * ttt - 0.000005971 ) * ttt + 0.01801828 ) *... */
  /* '<S148>:1:74'                 ttt + 0.2988499 ) * ttt + 2306.083227 ) * ttt + 2.650545; */
  /*  " */
  /* '<S148>:1:75' theta=  (((( - 0.0000001274 * ttt - 0.000007089 ) * ttt - 0.04182264 ) *... */
  /* '<S148>:1:76'                 ttt - 0.4294934 ) * ttt + 2004.191903 ) * ttt; */
  /* '<S148>:1:77' z    =  ((((   0.0000002904 * ttt - 0.000028596 ) * ttt + 0.01826837 ) *... */
  /* '<S148>:1:78'                 ttt + 1.0927348 ) * ttt + 2306.077181 ) * ttt - 2.650545; */
  /*  convert all units to rad */
  /* '<S148>:1:81' psia = psia  * convrt; */
  b_min = ((((-9.51E-8 * rtb_Sum + 0.000132851) * rtb_Sum - 0.00114045) *
            rtb_Sum - 1.0790069) * rtb_Sum + 5038.481507) * rtb_Sum *
    4.84813681109536E-6;

  /*  rad */
  /* '<S148>:1:82' wa   = wa    * convrt; */
  b_c = (((((3.337E-7 * rtb_Sum - 4.67E-7) * rtb_Sum - 0.00772503) * rtb_Sum +
           0.0512623) * rtb_Sum - 0.025754) * rtb_Sum + 84381.406) *
    4.84813681109536E-6;

  /* '<S148>:1:83' ea   = ea    * convrt; */
  /* '<S148>:1:84' xa   = xa    * convrt; */
  dayofyear = ((((-5.6E-8 * rtb_Sum + 0.000170663) * rtb_Sum - 0.00121197) *
                rtb_Sum - 2.3814292) * rtb_Sum + 10.556403) * rtb_Sum *
    4.84813681109536E-6;

  /* '<S148>:1:86' oblo = oblo * convrt; */
  /*  " to rad */
  /* '<S148>:1:87' a4  = rot3mat(-xa); */
  /* '<S148>:1:108' s   = sin( z_rad ); */
  days = sin(-dayofyear);

  /* '<S148>:1:109' c   = cos( z_rad ); */
  dayofyear = cos(-dayofyear);

  /* '<S148>:1:111' DCMz    = [ c   s   0; */
  /* '<S148>:1:112'            -s   c   0; */
  /* '<S148>:1:113'             0   0   1 ]; */
  /* '<S148>:1:88' a5  = rot1mat(wa); */
  /*  precess */
  /* '<S148>:1:97' s   = sin( x_rad ); */
  b_s = sin(b_c);

  /* '<S148>:1:98' c   = cos( x_rad ); */
  b_c = cos(b_c);

  /* '<S148>:1:100' DCMx    = [ 1   0   0 */
  /* '<S148>:1:101'             0   c   s; */
  /* '<S148>:1:102'             0  -s   c ]; */
  /* '<S148>:1:89' a6  = rot3mat(psia); */
  /* '<S148>:1:108' s   = sin( z_rad ); */
  c_s = sin(b_min);

  /* '<S148>:1:109' c   = cos( z_rad ); */
  b_min = cos(b_min);

  /* '<S148>:1:111' DCMz    = [ c   s   0; */
  /* '<S148>:1:112'            -s   c   0; */
  /* '<S148>:1:113'             0   0   1 ]; */
  /* '<S148>:1:90' a7  = rot1mat(-oblo); */
  /* '<S148>:1:91' prec = a7*a6*a5*a4; */
  /* '<S148>:1:52' ~ */
  /* '<S148>:1:54' vec_eci = prec*vec_mod; */
  b_min_0[0] = b_min;
  b_min_0[3] = c_s;
  b_min_0[6] = 0.0;
  b_min_0[1] = -c_s;
  b_min_0[4] = b_min;
  b_min_0[7] = 0.0;
  b_min_0[2] = 0.0;
  b_min_0[5] = 0.0;
  b_min_0[8] = 1.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] = 0.0;
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += b_min_0[3 * qY_1] * a[lowAlt];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += b_min_0[3 * qY_1 + 1] * a[lowAlt + 3];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += b_min_0[3 * qY_1 + 2] * a[lowAlt + 6];
    }

    b_0[3 * lowAlt] = b[lowAlt];
  }

  b_0[1] = 0.0;
  b_0[4] = b_c;
  b_0[7] = b_s;
  b_0[2] = 0.0;
  b_0[5] = -b_s;
  b_0[8] = b_c;
  dayofyear_0[0] = dayofyear;
  dayofyear_0[3] = days;
  dayofyear_0[6] = 0.0;
  dayofyear_0[1] = -days;
  dayofyear_0[4] = dayofyear;
  dayofyear_0[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      b_min_0[lowAlt + 3 * qY_1] = 0.0;
      b_min_0[lowAlt + 3 * qY_1] += b_0[3 * qY_1] * rtb_mod_to_gcrf[lowAlt];
      b_min_0[lowAlt + 3 * qY_1] += b_0[3 * qY_1 + 1] * rtb_mod_to_gcrf[lowAlt +
        3];
      b_min_0[lowAlt + 3 * qY_1] += b_0[3 * qY_1 + 2] * rtb_mod_to_gcrf[lowAlt +
        6];
    }

    dayofyear_0[2 + 3 * lowAlt] = d[lowAlt];
  }

  T1900_0[0] = T1900 * cos(month) * 1.495978707E+8;
  T1900_0[1] = T1900 * cos(day) * sin(month) * 1.495978707E+8;
  T1900_0[2] = T1900 * sin(day) * sin(month) * 1.495978707E+8;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    rtb_sun_vector_eci_km[lowAlt] = 0.0;
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] = 0.0;
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += dayofyear_0[3 * qY_1] *
        b_min_0[lowAlt];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += dayofyear_0[3 * qY_1 + 1] *
        b_min_0[lowAlt + 3];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += dayofyear_0[3 * qY_1 + 2] *
        b_min_0[lowAlt + 6];
      rtb_sun_vector_eci_km[lowAlt] += rtb_mod_to_gcrf[3 * qY_1 + lowAlt] *
        T1900_0[qY_1];
    }
  }

  /* End of MATLAB Function: '<S6>/MATLAB Function' */

  /* MATLAB Function: '<S151>/MATLAB Function' */
  /*  */
  /*  UW HuskySat-1, ADCS Subsystem */
  /*    Courtesy: Krish Kaycee 9.11.17 */
  /*    Updated: T. Reynolds 2.10.18 */
  /*  */
  /*  COORD_ROTATIONS excludes the polar motion transformation. Hence it */
  /*  includes the Precession, Nutation, and Sidereal Time rotation matrices. */
  /* MATLAB Function 'coord_rotations_lib/MATLAB Function': '<S153>:1' */
  /*  UPDATED: Excluding polar motion since it would need to be updated ~1 per */
  /*  week to keep accurate. For our mission duration and requirements, */
  /*  accounting for polar motion is not necessary. -T. Reynolds, 9.11.17 */
  /*  Constants */
  /*  Everything should be precomputed in radians */
  /* '<S153>:1:17' deg2rad     = pi/180; */
  /* '<S153>:1:18' twopi       = 2*pi; */
  /* '<S153>:1:19' asec2rad    = 1/3600*deg2rad; */
  /* '<S153>:1:20' o_prec      = 7.292115146706979e-5; */
  /*  Evaluate Delaunay parameters */
  /* '<S153>:1:23' epsb_1980 = deg2rad * ( 23.439291 ... */
  /* '<S153>:1:24'                         - 0.0130042 * jd_tt_j2000_century ... */
  /* '<S153>:1:25'                         - 1.64e-7 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:26'                         + 5.04e-7 * jd_tt_j2000_century^3); */
  T1900 = (((23.439291 - 0.0130042 * rtb_T_TT_J2000) - rtb_T_TT_J2000 *
            rtb_T_TT_J2000 * 1.64E-7) + 5.04E-7 * rt_powd_snf(rtb_T_TT_J2000,
            3.0)) * 0.017453292519943295;

  /*  Evaluate Delaunay parameters (correct equations from 4e errata) */
  /* '<S153>:1:29' M_moon	=   deg2rad * ( 134.96298139 ... */
  /* '<S153>:1:30'                         + (1325 * 360 + 198.8673981) * jd_tt_j2000_century ... */
  /* '<S153>:1:31'                       	+ 0.0086972 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:32'                        	+ 1.78e-5 * jd_tt_j2000_century^3); */
  month = (((477198.8673981 * rtb_T_TT_J2000 + 134.96298139) + rtb_T_TT_J2000 *
            rtb_T_TT_J2000 * 0.0086972) + 1.78E-5 * rt_powd_snf(rtb_T_TT_J2000,
            3.0)) * 0.017453292519943295;

  /* '<S153>:1:34' M_sun	=   deg2rad * ( 357.52772333 ... */
  /* '<S153>:1:35'                         + (99 * 360 + 359.0503400) * jd_tt_j2000_century ... */
  /* '<S153>:1:36'                       	- 0.0001603 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:37'                      	- 3.3e-6 * jd_tt_j2000_century^3); */
  days = (((35999.05034 * rtb_T_TT_J2000 + 357.52772333) - rtb_T_TT_J2000 *
           rtb_T_TT_J2000 * 0.0001603) - 3.3E-6 * rt_powd_snf(rtb_T_TT_J2000,
           3.0)) * 0.017453292519943295;

  /* '<S153>:1:39' u_moon	=   deg2rad * ( 93.27191028 ... */
  /* '<S153>:1:40'                         + (1342 * 360 + 82.0175381) * jd_tt_j2000_century ... */
  /* '<S153>:1:41'                       	- 0.0036825 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:42'                         + 3.1e-6 * jd_tt_j2000_century^3); */
  b_min = (((483202.0175381 * rtb_T_TT_J2000 + 93.27191028) - rtb_T_TT_J2000 *
            rtb_T_TT_J2000 * 0.0036825) + 3.1E-6 * rt_powd_snf(rtb_T_TT_J2000,
            3.0)) * 0.017453292519943295;

  /* '<S153>:1:44' D_sun	=   deg2rad * ( 297.85036306 ... */
  /* '<S153>:1:45'                         + (1236 * 360 + 307.1114800) * jd_tt_j2000_century ... */
  /* '<S153>:1:46'                         - 0.0019142 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:47'                      	+ 5.3e-6 * jd_tt_j2000_century^3); */
  b_s = (((445267.11148 * rtb_T_TT_J2000 + 297.85036306) - rtb_T_TT_J2000 *
          rtb_T_TT_J2000 * 0.0019142) + 5.3E-6 * rt_powd_snf(rtb_T_TT_J2000, 3.0))
    * 0.017453292519943295;

  /* '<S153>:1:49' O_moon	=   deg2rad * ( 125.04452222 ... */
  /* '<S153>:1:50'                         - (5 * 360 + 134.1362608) * jd_tt_j2000_century ... */
  /* '<S153>:1:51'                     	+ 0.0020708 * jd_tt_j2000_century^2 ... */
  /* '<S153>:1:52'                         + 2.2e-6 * jd_tt_j2000_century^3); */
  dayofyear = (((125.04452222 - 1934.1362608 * rtb_T_TT_J2000) + rtb_T_TT_J2000 *
                rtb_T_TT_J2000 * 0.0020708) + 2.2E-6 * rt_powd_snf
               (rtb_T_TT_J2000, 3.0)) * 0.017453292519943295;

  /*  Largest Nutation coefficents (out of 106)                     */
  /* '<S153>:1:55' nc = [ ... */
  /* '<S153>:1:56' 1   0   0   0   0   1   -171996 -174.2  92025   8.9 */
  /* '<S153>:1:57' 9   0   0   2   -2  2   -13187  -1.6    5736    -3.1 */
  /* '<S153>:1:58' 31  0   0   2   0   2   -2274   -0.2    977     -0.5 */
  /* '<S153>:1:59' 2   0   0   0   0   2   2062    0.2     -895    0.5 */
  /* '<S153>:1:60' 10  0   1   0  	0   0   1426    -3.4    54      -0.1 */
  /* '<S153>:1:61' 32  1   0   0   0   0   712     0.1     -7      0.0 */
  /* '<S153>:1:62' 11  0   1   2   -2  2   -517    1.2     224     -0.6 */
  /* '<S153>:1:63' 33  0   0   2   0   1   -386    -0.4    200     0.0 */
  /* '<S153>:1:64' 34  1   0   2   0   2   -301    0.0     129     -0.1 */
  /* '<S153>:1:65' 12  0   -1  2   -2  2   217     -0.5    -95     0.3 */
  /* '<S153>:1:66' 35  1   0   0   -2  0   -158    0.0     -1      0.0 */
  /* '<S153>:1:67' 13  0   0   2   -2  1   129     0.1     -70     0.0  */
  /* '<S153>:1:68' 36  -1  0   2   0   2   123     0.0     -53     0.0  */
  /* '<S153>:1:69' 38  1   0   0   0   1   63      0.1     -33     0.0 */
  /* '<S153>:1:70' 37  0   0   0   2   0   63      0.0     -2      0.0 */
  /* '<S153>:1:71' 40  -1  0   2   2   2   -59     0.0     26      0.0 */
  /* '<S153>:1:72' 39  -1  0   0   0   1   -58     -0.1    32      0.0 */
  /* '<S153>:1:73' 41  1   0   2   0   1   -51     0.0     27      0.0 */
  /* '<S153>:1:74' 14  2   0   0   -2  0   48      0.0     1       0.0 */
  /* '<S153>:1:75' 3   -2  0   2   0   1   46      0.0     -24     0.0 */
  /* '<S153>:1:76' 42  0   0   2   2   2   -38     0.0     16      0.0 */
  /* '<S153>:1:77' 45  2   0   2   0   2   -31     0.0     13      0.0 */
  /* '<S153>:1:78' 43  2   0   0   0   0   29      0.0     -1      0.0 */
  /* '<S153>:1:79' 44  1   0   2   -2  2   29      0.0     -12     0.0 */
  /* '<S153>:1:80' 46  0   0   2   0   0   26      0.0     -1      0.0 */
  /* '<S153>:1:81' 15  0   0   2   -2  0   -22     0.0     0       0.0 */
  /* '<S153>:1:82' 47  -1  0   2   0   1   21      0.0     -10     0.0 */
  /* '<S153>:1:83' 16  0   2   0   0   0   17      -0.1    0       0.0 */
  /* '<S153>:1:84' 18  0   2   2   -2  2   -16     0.1     7       0.0 */
  /* '<S153>:1:85' 48  -1  0   0   2   1   16      0.0     -8      0.0 ... */
  /* '<S153>:1:86' ]; */
  /*  Map coefficients to radians (this should be embedded in the data) */
  /*  nc(:,end-3:end) = nc(:,end-3:end) * 0.0001 * asec2rad; */
  /*  Compute nutation in longitude (psi), and latitude (eps) */
  /* '<S153>:1:92' dpsi_1980 = 0; */
  day = 0.0;

  /* '<S153>:1:92' deps_1980 = 0; */
  b_c = 0.0;

  /* '<S153>:1:93' for i = 1:size(nc,1) */
  for (lowAlt = 0; lowAlt < 30; lowAlt++) {
    /* '<S153>:1:94' api         =   nc(i,2) * M_moon + nc(i,3) * M_sun + nc(i,4) * u_moon + nc(i,5) * D_sun + nc(i,6) * O_moon; */
    c_s = (((nc[30 + lowAlt] * month + nc[60 + lowAlt] * days) + nc[90 + lowAlt]
            * b_min) + nc[120 + lowAlt] * b_s) + nc[150 + lowAlt] * dayofyear;

    /* '<S153>:1:95' dpsi_1980   =   0.0001 * (nc(i,7) + nc(i,8) * jd_tt_j2000_century) * sin(api) + dpsi_1980; */
    day += (nc[210 + lowAlt] * rtb_T_TT_J2000 + nc[180 + lowAlt]) * 0.0001 * sin
      (c_s);

    /* '<S153>:1:96' deps_1980   =   0.0001 * (nc(i,9) + nc(i,10) * jd_tt_j2000_century) * cos(api) + deps_1980; */
    b_c += (nc[270 + lowAlt] * rtb_T_TT_J2000 + nc[240 + lowAlt]) * 0.0001 * cos
      (c_s);
  }

  /* '<S153>:1:99' eps_1980 = epsb_1980 + asec2rad * deps_1980; */
  days = 4.84813681109536E-6 * b_c + T1900;

  /* '<S153>:1:100' tod_to_mod = rot1(-epsb_1980) * rot3(asec2rad * dpsi_1980) * rot1(eps_1980); */
  /* '<S153>:1:149' y = [1 0 0; 0 cos(u) sin(u); 0 -sin(u) cos(u)]; */
  month = 4.84813681109536E-6 * day;

  /* '<S153>:1:157' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
  /* '<S153>:1:149' y = [1 0 0; 0 cos(u) sin(u); 0 -sin(u) cos(u)]; */
  c_0[1] = 0.0;
  c_0[4] = cos(-T1900);
  c_0[7] = sin(-T1900);
  c_0[2] = 0.0;
  c_0[5] = -sin(-T1900);
  c_0[8] = cos(-T1900);
  rtb_teme_to_gcrf[0] = cos(month);
  rtb_teme_to_gcrf[3] = sin(month);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sin(month);
  rtb_teme_to_gcrf[4] = cos(month);
  rtb_teme_to_gcrf[7] = 0.0;
  c_0[0] = 1.0;
  rtb_teme_to_gcrf[2] = 0.0;
  c_0[3] = 0.0;
  rtb_teme_to_gcrf[5] = 0.0;
  c_0[6] = 0.0;
  rtb_teme_to_gcrf[8] = 1.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] = 0.0;
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1] *
        c_0[lowAlt];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1 + 1] *
        c_0[lowAlt + 3];
      rtb_mod_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1 + 2] *
        c_0[lowAlt + 6];
    }

    b_0[3 * lowAlt] = c[lowAlt];
  }

  b_0[1] = 0.0;
  b_0[4] = cos(days);
  b_0[7] = sin(days);
  b_0[2] = 0.0;
  b_0[5] = -sin(days);
  b_0[8] = cos(days);

  /*  Generate Precession matrix */
  /* '<S153>:1:103' zeta    =   asec2rad * (2306.2181 * jd_tt_j2000_century + 0.30188 * jd_tt_j2000_century^2 + 0.017998 * jd_tt_j2000_century^3); */
  month = ((rtb_T_TT_J2000 * rtb_T_TT_J2000 * 0.30188 + 2306.2181 *
            rtb_T_TT_J2000) + 0.017998 * rt_powd_snf(rtb_T_TT_J2000, 3.0)) *
    4.84813681109536E-6;

  /* '<S153>:1:104' theta   =   asec2rad * (2004.3109 * jd_tt_j2000_century - 0.42665 * jd_tt_j2000_century^2 - 0.041833 * jd_tt_j2000_century^3); */
  /* '<S153>:1:105' z       =   asec2rad * (2306.2181 * jd_tt_j2000_century + 1.09468 * jd_tt_j2000_century^2 + 0.018203 * jd_tt_j2000_century^3); */
  days = ((rtb_T_TT_J2000 * rtb_T_TT_J2000 * 1.09468 + 2306.2181 *
           rtb_T_TT_J2000) + 0.018203 * rt_powd_snf(rtb_T_TT_J2000, 3.0)) *
    4.84813681109536E-6;

  /* '<S153>:1:107' mod_to_gcrf = rot3(zeta) * rot2(-theta) * rot3(z); */
  /* '<S153>:1:157' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
  rtb_T_TT_J2000 = -(((2004.3109 * rtb_T_TT_J2000 - rtb_T_TT_J2000 *
                       rtb_T_TT_J2000 * 0.42665) - 0.041833 * rt_powd_snf
                      (rtb_T_TT_J2000, 3.0)) * 4.84813681109536E-6);

  /* '<S153>:1:153' y = [cos(u) 0 -sin(u); 0 1 0; sin(u) 0 cos(u)]; */
  /* '<S153>:1:157' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
  rtb_teme_to_gcrf[0] = cos(month);
  rtb_teme_to_gcrf[3] = sin(month);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sin(month);
  rtb_teme_to_gcrf[4] = cos(month);
  rtb_teme_to_gcrf[7] = 0.0;
  c_0[0] = cos(rtb_T_TT_J2000);
  c_0[3] = 0.0;
  c_0[6] = -sin(rtb_T_TT_J2000);
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      tod_to_mod[lowAlt + 3 * qY_1] = 0.0;
      tod_to_mod[lowAlt + 3 * qY_1] += b_0[3 * qY_1] * rtb_mod_to_gcrf[lowAlt];
      tod_to_mod[lowAlt + 3 * qY_1] += b_0[3 * qY_1 + 1] *
        rtb_mod_to_gcrf[lowAlt + 3];
      tod_to_mod[lowAlt + 3 * qY_1] += b_0[3 * qY_1 + 2] *
        rtb_mod_to_gcrf[lowAlt + 6];
    }

    rtb_teme_to_gcrf[2 + 3 * lowAlt] = d_0[lowAlt];
    c_0[1 + 3 * lowAlt] = e[lowAlt];
  }

  c_0[2] = sin(rtb_T_TT_J2000);
  c_0[5] = 0.0;
  c_0[8] = cos(rtb_T_TT_J2000);
  b_min_0[0] = cos(days);
  b_min_0[3] = sin(days);
  b_min_0[6] = 0.0;
  b_min_0[1] = -sin(days);
  b_min_0[4] = cos(days);
  b_min_0[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      b_0[lowAlt + 3 * qY_1] = 0.0;
      b_0[lowAlt + 3 * qY_1] += c_0[3 * qY_1] * rtb_teme_to_gcrf[lowAlt];
      b_0[lowAlt + 3 * qY_1] += c_0[3 * qY_1 + 1] * rtb_teme_to_gcrf[lowAlt + 3];
      b_0[lowAlt + 3 * qY_1] += c_0[3 * qY_1 + 2] * rtb_teme_to_gcrf[lowAlt + 6];
    }

    b_min_0[2 + 3 * lowAlt] = d_0[lowAlt];
  }

  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_mod_to_gcrf[qY_1 + 3 * lowAlt] = 0.0;
      rtb_mod_to_gcrf[qY_1 + 3 * lowAlt] += b_min_0[3 * lowAlt] * b_0[qY_1];
      rtb_mod_to_gcrf[qY_1 + 3 * lowAlt] += b_min_0[3 * lowAlt + 1] * b_0[qY_1 +
        3];
      rtb_mod_to_gcrf[qY_1 + 3 * lowAlt] += b_min_0[3 * lowAlt + 2] * b_0[qY_1 +
        6];
    }
  }

  /*  Compute equation of Equinoxes */
  /* '<S153>:1:110' eqe_1980 = asec2rad * (dpsi_1980  * cos(epsb_1980) + 0.00264 * sin(O_moon) + 0.000063 * sin(2 * O_moon)); */
  rtb_T_TT_J2000 = ((day * cos(T1900) + 0.00264 * sin(dayofyear)) + sin(2.0 *
    dayofyear) * 6.3E-5) * 4.84813681109536E-6;

  /* '<S153>:1:111' teme_to_tod = rot3(-eqe_1980); */
  /* '<S153>:1:157' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
  /*  % Compute sidereal rotation */
  /*  GMST_1982 = asec2rad * (67310.54841 ... */
  /*                          + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /*                          + 0.093104 * jd_ut1_j2000_century^2 ... */
  /*                          - 6.2e-6 * jd_ut1_j2000_century^3); */
  /* '<S153>:1:118' GMST_1982 = (67310.54841 ... */
  /* '<S153>:1:119'                         + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /* '<S153>:1:120'                         + 0.093104 * jd_ut1_j2000_century^2 ... */
  /* '<S153>:1:121'                         - 6.2e-6 * jd_ut1_j2000_century^3); */
  rtb_Sum = ((3.1644001848128662E+9 * rtb_Sum + 67310.54841) + rtb_Sum * rtb_Sum
             * 0.093104) - 6.2E-6 * rt_powd_snf(rtb_Sum, 3.0);

  /* '<S153>:1:122' GMST_1982 = mod(GMST_1982,sign(GMST_1982)*86400) / 240; */
  if (rtb_Sum < 0.0) {
    T1900 = -1.0;
  } else if (rtb_Sum > 0.0) {
    T1900 = 1.0;
  } else if (rtb_Sum == 0.0) {
    T1900 = 0.0;
  } else {
    T1900 = rtb_Sum;
  }

  T1900 *= 86400.0;
  if (!(T1900 == 0.0)) {
    if (T1900 == T1900) {
      rtb_Sum -= floor(rtb_Sum / T1900) * T1900;
    } else {
      rtb_Sum /= T1900;
      if (fabs(rtb_Sum - rt_roundd_snf(rtb_Sum)) <= 2.2204460492503131E-16 *
          fabs(rtb_Sum)) {
        rtb_Sum = 0.0;
      } else {
        rtb_Sum = (rtb_Sum - floor(rtb_Sum)) * T1900;
      }
    }
  }

  rtb_Sum /= 240.0;

  /* '<S153>:1:123' GMST_1982 = deg2rad * mod(GMST_1982, 360); */
  rtb_Sum = (rtb_Sum - floor(rtb_Sum / 360.0) * 360.0) * 0.017453292519943295;

  /* '<S153>:1:124' GAST_1982 = GMST_1982 + eqe_1980; */
  /*  Compute sidereal rotation */
  /*  GMST_1982 = asec2rad * (67310.54841 ... */
  /*                          + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
  /*                          + 0.093104 * jd_ut1_j2000_century^2 ... */
  /*                          - 6.2e-6 * jd_ut1_j2000_century^3); */
  /*  GMST_1982 = mod(GMST_1982, 2*pi); */
  /*  GAST_1982 = GMST_1982 + eqe_1980; */
  /*  Compute rotation from pseudo-Earth fixed frame (does not include polar */
  /*  motion) */
  /* '<S153>:1:136' pef_to_tod = rot3(-GAST_1982); */
  rtb_Sum = -(rtb_Sum + rtb_T_TT_J2000);

  /* '<S153>:1:157' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
  /*  % Compute polar motion (small angle approximation is assumed) */
  /*  itrf_to_pef = polar(polar_motion_rad(1), polar_motion_rad(2)); */
  /*  Compute composite rotations */
  /* '<S153>:1:142' teme_to_gcrf = mod_to_gcrf * tod_to_mod * teme_to_tod; */
  /* '<S153>:1:143' itrf_to_gcrf = mod_to_gcrf * tod_to_mod * pef_to_tod; */
  rtb_teme_to_gcrf[0] = cos(rtb_Sum);
  rtb_teme_to_gcrf[3] = sin(rtb_Sum);
  rtb_teme_to_gcrf[6] = 0.0;
  rtb_teme_to_gcrf[1] = -sin(rtb_Sum);
  rtb_teme_to_gcrf[4] = cos(rtb_Sum);
  rtb_teme_to_gcrf[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      b_min_0[lowAlt + 3 * qY_1] = 0.0;
      b_min_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1] *
        rtb_mod_to_gcrf[lowAlt];
      b_min_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1 + 1] *
        rtb_mod_to_gcrf[lowAlt + 3];
      b_min_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1 + 2] *
        rtb_mod_to_gcrf[lowAlt + 6];
    }

    rtb_teme_to_gcrf[2 + 3 * lowAlt] = d_0[lowAlt];
  }

  /*  * itrf_to_pef;   % for vecef */
  /* '<S153>:1:144' ppef_to_vgcrf = itrf_to_gcrf * skew([0; 0; o_prec]); */
  /*  for w x recef */
  c_0[0] = cos(-rtb_T_TT_J2000);
  c_0[3] = sin(-rtb_T_TT_J2000);
  c_0[6] = 0.0;
  c_0[1] = -sin(-rtb_T_TT_J2000);
  c_0[4] = cos(-rtb_T_TT_J2000);
  c_0[7] = 0.0;
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_itrf_to_gcrf[lowAlt + 3 * qY_1] = 0.0;
      b_0[lowAlt + 3 * qY_1] = 0.0;
      rtb_itrf_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1] *
        b_min_0[lowAlt];
      b_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1] * rtb_mod_to_gcrf[lowAlt];
      rtb_itrf_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1 + 1] *
        b_min_0[lowAlt + 3];
      b_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1 + 1] *
        rtb_mod_to_gcrf[lowAlt + 3];
      rtb_itrf_to_gcrf[lowAlt + 3 * qY_1] += rtb_teme_to_gcrf[3 * qY_1 + 2] *
        b_min_0[lowAlt + 6];
      b_0[lowAlt + 3 * qY_1] += tod_to_mod[3 * qY_1 + 2] *
        rtb_mod_to_gcrf[lowAlt + 6];
    }

    c_0[2 + 3 * lowAlt] = d_0[lowAlt];
  }

  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    for (qY_1 = 0; qY_1 < 3; qY_1++) {
      rtb_teme_to_gcrf[qY_1 + 3 * lowAlt] = 0.0;
      rtb_teme_to_gcrf[qY_1 + 3 * lowAlt] += c_0[3 * lowAlt] * b_0[qY_1];
      rtb_teme_to_gcrf[qY_1 + 3 * lowAlt] += c_0[3 * lowAlt + 1] * b_0[qY_1 + 3];
      rtb_teme_to_gcrf[qY_1 + 3 * lowAlt] += c_0[3 * lowAlt + 2] * b_0[qY_1 + 6];
    }
  }

  /* End of MATLAB Function: '<S151>/MATLAB Function' */

  /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
   *  Inport: '<Root>/orbit_tle'
   *  MATLAB Function: '<S152>/time-conversion-lib'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  LOAD CONSTANTS */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'sgp4_lib_fsw/MATLAB Function': '<S147>:1' */
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
  /*    - k_e = gravitational constant of Earth, sqrt(GM_e) */
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
  /* '<S147>:1:75' k_2     = 5.413080e-4; */
  /*  CK2 = 0.5*J_2*a_E^2 */
  /* '<S147>:1:76' k_4     = 0.62098875e-6; */
  /*  CK4 = -3/8*J_4*a_E^4 */
  /* '<S147>:1:77' QOMS4   = 1.88027916e-9; */
  month = 1.88027916E-9;

  /*  QOMS2T = (q_0 - s)^4 * ER^4 */
  /* '<S147>:1:78' s       = 1.01222928; */
  rtb_T_TT_J2000 = 1.01222928;

  /*  S = s*ER */
  /* '<S147>:1:79' XJ3     = -0.2538815e-5; */
  /*  J_3 */
  /* '<S147>:1:80' XKE     = 0.743669161e-1; */
  /*  XKE = k_e*(ER/min)^{3/2} */
  /* '<S147>:1:81' XKMPER  = 6378.137; */
  /*  km/ER */
  /* '<S147>:1:82' XMNPDA  = 1440; */
  /*  time_units/day */
  /* '<S147>:1:83' a_E     = 1; */
  /*  distance units/ER */
  /* '<S147>:1:84' DE2RA   = 0.174532925e-1; */
  /*  radians/degree */
  /* '<S147>:1:85' TRUE    = 1; */
  /* '<S147>:1:86' FALSE   = 0; */
  /* '<S147>:1:87' r_E     = 6378.137; */
  /*  Earth's equatorial radius [km] */
  /* '<S147>:1:88' A_30    = -XJ3*a_E^3; */
  /*  see nomenclature */
  /* '<S147>:1:89' VKMPS   = XKMPER/60; */
  /*  Convert ER/min velocities to km/s */
  /* '<S147>:1:90' FLAG    = int8(0); */
  rtb_FLAG = 0;

  /* '<S147>:1:91' pos_invalid     = [ XKMPER; XKMPER; XKMPER + 500 ]; */
  /* '<S147>:1:92' vel_invalid     = [ 0.0; 0.0; 0.0 ]; */
  /*  ----------------------------------------------------------------------- % */
  /*  Line 1 */
  /* epoch_year  = orbit_tle(1); */
  /* '<S147>:1:98' epoch_JD    = orbit_tle(2); */
  /* '<S147>:1:99' BS          = orbit_tle(3); */
  /*  Line 2 */
  /* '<S147>:1:102' i_o     = orbit_tle(4)*DE2RA; */
  rtb_Sum = rtU.orbit_tle[3] * 0.0174532925;

  /*  Radians (INC) */
  /* '<S147>:1:103' Om_o    = orbit_tle(5)*DE2RA; */
  /*  Radians (RAAN) */
  /* '<S147>:1:104' e_o     = orbit_tle(6); */
  /*  Non-dimensional (ECC) */
  /* '<S147>:1:105' w_o     = orbit_tle(7)*DE2RA; */
  days = rtU.orbit_tle[6] * 0.0174532925;

  /*  Radians (AOP) */
  /* '<S147>:1:106' M_o     = orbit_tle(8)*DE2RA; */
  dayofyear = rtU.orbit_tle[7] * 0.0174532925;

  /*  Radians (MNA) */
  /* '<S147>:1:107' n_o     = orbit_tle(9)*2*pi/XMNPDA; */
  b_min = rtU.orbit_tle[8] * 2.0 * 3.1415926535897931 / 1440.0;

  /*  Revs per time unit (MNM)    */
  /*  Hooks on orbital elements (catch #1) */
  /*  eccentricity */
  /* '<S147>:1:111' if( (e_o >= 1) || (e_o <= 0) ) */
  if ((rtU.orbit_tle[5] >= 1.0) || (rtU.orbit_tle[5] <= 0.0)) {
    /* '<S147>:1:112' pos_teme_km       = pos_invalid; */
    /* '<S147>:1:113' vel_teme_kmps     = vel_invalid; */
    rtb_pos_teme_km[0] = 6378.137;
    b_c = 0.0;
    rtb_pos_teme_km[1] = 6378.137;
    c_s = 0.0;
    rtb_pos_teme_km[2] = 6878.137;
    xi = 0.0;

    /* '<S147>:1:114' FLAG = int8(-1); */
    rtb_FLAG = -1;
  } else {
    /*  mean motion (catch #1) */
    /* '<S147>:1:118' if( n_o == 0 ) */
    if (b_min == 0.0) {
      /* '<S147>:1:119' pos_teme_km       = pos_invalid; */
      /* '<S147>:1:120' vel_teme_kmps     = vel_invalid; */
      rtb_pos_teme_km[0] = 6378.137;
      b_c = 0.0;
      rtb_pos_teme_km[1] = 6378.137;
      c_s = 0.0;
      rtb_pos_teme_km[2] = 6878.137;
      xi = 0.0;

      /* '<S147>:1:121' FLAG = int8(-1); */
      rtb_FLAG = -1;
    } else {
      /*  Time since epoch (catch #1)                              */
      /* '<S147>:1:126' tsince_JD      = (JD_J2000_cur - epoch_JD); */
      week = ((week + -0.00020833333333333332) - 2.451545E+6) - rtU.orbit_tle[1];

      /* '<S147>:1:127' if( abs(tsince_JD) < 1e-8 || tsince_JD < 0) */
      if ((fabs(week) < 1.0E-8) || (week < 0.0)) {
        /* '<S147>:1:128' tsince_JD   = 0; */
        week = 0.0;
      }

      /* '<S147>:1:131' tsince  = XMNPDA*tsince_JD; */
      T1900 = 1440.0 * week;

      /* '<S147>:1:133' a_1  = (XKE/n_o)^(2/3); */
      week = rt_powd_snf(0.0743669161 / b_min, 0.66666666666666663);

      /* '<S147>:1:134' d_1  = 1.5*(k_2/(a_1^2))*((3*cos(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
      day = cos(rtb_Sum);
      day = (day * day * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5] *
        rtU.orbit_tle[5], 1.5) * (0.000541308 / (week * week) * 1.5);

      /* '<S147>:1:135' a_o  = a_1*(1 - (1/3)*d_1 - d_1^2 - (134/81)*d_1^3); */
      week *= ((1.0 - 0.33333333333333331 * day) - day * day) -
        1.654320987654321 * rt_powd_snf(day, 3.0);

      /* '<S147>:1:136' d_o  = (3/2)*(k_2/(a_o^2))*((3*cos(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
      day = cos(rtb_Sum);
      day = (day * day * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5] *
        rtU.orbit_tle[5], 1.5) * (0.000541308 / (week * week) * 1.5);

      /* '<S147>:1:137' n_o_pp   = n_o/(1 + d_o); */
      b_min /= 1.0 + day;

      /* '<S147>:1:138' a_o_pp   = a_o/(1 - d_o); */
      b_s = week / (1.0 - day);

      /* '<S147>:1:140' r_p     = r_E*(a_o_pp*(1 - e_o) - a_E); */
      week = ((1.0 - rtU.orbit_tle[5]) * b_s - 1.0) * 6378.137;

      /*  Radius of perigee */
      /*  Check low altitude condition                                     */
      /* '<S147>:1:143' if( a_o_pp*(1-e_o)/a_E < (220/XKMPER + a_E) ) */
      if ((1.0 - rtU.orbit_tle[5]) * b_s < 1.0344928307435228) {
        /* '<S147>:1:144' lowAlt  = TRUE; */
        lowAlt = 1;

        /* '<S147>:1:145' FLAG    = int8(1); */
        rtb_FLAG = 1;
      } else {
        /* '<S147>:1:146' else */
        /* '<S147>:1:147' lowAlt  = FALSE; */
        lowAlt = 0;
      }

      /*  Check if perigee is between [98,156]km or below 98km and adjust the */
      /*  constant s accordingly. */
      /* '<S147>:1:152' if( (r_p >= 98) && (r_p <= 156) ) */
      if ((week >= 98.0) && (week <= 156.0)) {
        /* '<S147>:1:153' s_star  = a_o_pp*(1 - e_o) - s + a_E; */
        rtb_T_TT_J2000 = ((1.0 - rtU.orbit_tle[5]) * b_s - 1.01222928) + 1.0;

        /* '<S147>:1:154' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
        month = rt_powd_snf(1.0188142770258546 - rtb_T_TT_J2000, 4.0);

        /* '<S147>:1:155' s       = s_star; */
      } else {
        if (week < 98.0) {
          /* '<S147>:1:156' elseif( r_p < 98 ) */
          /* '<S147>:1:157' s_star  = 20/XKMPER + a_E; */
          /* '<S147>:1:158' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
          month = 6.0426196528035638E-8;

          /* '<S147>:1:159' s       = s_star; */
          rtb_T_TT_J2000 = 1.0031357118857749;
        }
      }

      /*  Constants */
      /* '<S147>:1:163' tht     = cos(i_o); */
      week = cos(rtb_Sum);

      /* '<S147>:1:164' xi      = 1/(a_o_pp - s); */
      xi = 1.0 / (b_s - rtb_T_TT_J2000);

      /*  cannot be zero since s_star = s ~= a_o_pp */
      /* '<S147>:1:165' b_o     = (1 - e_o^2)^(0.5); */
      b_o = sqrt(1.0 - rtU.orbit_tle[5] * rtU.orbit_tle[5]);

      /* '<S147>:1:166' eta     = a_o_pp*e_o*xi; */
      eta = b_s * rtU.orbit_tle[5] * xi;

      /*  cannot be zero, but could be 1. */
      /* '<S147>:1:167' if( eta == 1 ) */
      if (eta == 1.0) {
        /*  check that (1-eta^2) cannot be zero (catch #2) */
        /* '<S147>:1:168' pos_teme_km     = pos_invalid; */
        /* '<S147>:1:169' vel_teme_kmps   = vel_invalid; */
        rtb_pos_teme_km[0] = 6378.137;
        b_c = 0.0;
        rtb_pos_teme_km[1] = 6378.137;
        c_s = 0.0;
        rtb_pos_teme_km[2] = 6878.137;
        xi = 0.0;

        /* '<S147>:1:170' FLAG = int8(-1); */
        rtb_FLAG = -1;
      } else {
        /* '<S147>:1:173' C_2     = QOMS4*xi^4*n_o_pp*(1-eta^2)^(-7/2)*((a_o_pp*(1 + ... */
        /* '<S147>:1:174'             (3/2)*eta^2 + 4*e_o*eta + e_o*eta^3)) + ... */
        /* '<S147>:1:175'             (3/2)*k_2*xi*(-0.5+1.5*tht^2)*(8+24*eta^2+3*eta^4)/(1-eta^2)); */
        /* '<S147>:1:176' C_1     = BS*C_2; */
        C_1 = ((((eta * eta * 1.5 + 1.0) + 4.0 * rtU.orbit_tle[5] * eta) +
                rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * b_s + (week * week *
                1.5 + -0.5) * (0.00081196200000000006 * xi) * ((eta * eta * 24.0
                 + 8.0) + 3.0 * rt_powd_snf(eta, 4.0)) / (1.0 - eta * eta)) *
          (month * rt_powd_snf(xi, 4.0) * b_min * rt_powd_snf(1.0 - eta * eta,
            -3.5)) * rtU.orbit_tle[2];

        /* '<S147>:1:177' C_3     = (QOMS4*xi^5*A_30*n_o_pp*a_E*sin(i_o))/(k_2*e_o); */
        /* '<S147>:1:178' C_4     = 2*n_o_pp*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)* ... */
        /* '<S147>:1:179'             ((2*eta*(1+e_o*eta) + 0.5*e_o + 0.5*eta^3) - ... */
        /* '<S147>:1:180'             (2*k_2*xi)/(a_o_pp*(1-eta^2)) * (3*(1-3*tht^2)* ... */
        /* '<S147>:1:181'             (1-(3/2)*eta^2-2*e_o*eta-0.5*e_o*eta^3) + (3/4)*(1-tht^2)* ... */
        /* '<S147>:1:182'             (2*eta^2-e_o*eta-e_o*eta^3)*cos(2*w_o))); */
        C_4 = ((((rtU.orbit_tle[5] * eta + 1.0) * (2.0 * eta) + 0.5 *
                 rtU.orbit_tle[5]) + 0.5 * rt_powd_snf(eta, 3.0)) - ((((1.0 -
                   eta * eta * 1.5) - 2.0 * rtU.orbit_tle[5] * eta) - 0.5 *
                 rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * ((1.0 - week * week
                  * 3.0) * 3.0) + ((eta * eta * 2.0 - rtU.orbit_tle[5] * eta) -
                 rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * ((1.0 - week * week)
                 * 0.75) * cos(2.0 * days)) * (0.001082616 * xi / ((1.0 - eta *
                  eta) * b_s))) * (2.0 * b_min * month * rt_powd_snf(xi, 4.0) *
          b_s * (b_o * b_o) * rt_powd_snf(1.0 - eta * eta, -3.5));

        /* '<S147>:1:183' C_5     = 0; */
        C_5 = 0.0;

        /* '<S147>:1:184' D_2     = 0; */
        D_2 = 0.0;

        /* '<S147>:1:185' D_3     = 0; */
        D_3 = 0.0;

        /* '<S147>:1:186' D_4     = 0; */
        D_4 = 0.0;

        /* '<S147>:1:187' IL2     = 1.5*C_1; */
        b_c = 1.5 * C_1;

        /* '<S147>:1:188' IL3     = 0; */
        c_s = 0.0;

        /* '<S147>:1:189' IL4     = 0; */
        IL4 = 0.0;

        /* '<S147>:1:190' IL5     = 0; */
        IL5 = 0.0;

        /* '<S147>:1:192' if( lowAlt  == FALSE ) */
        if (lowAlt == 0) {
          /* '<S147>:1:193' C_5     = 2*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)*... */
          /* '<S147>:1:194'                                     (1+(11/4)*eta*(eta+e_o) + e_o*eta^3); */
          C_5 = 2.0 * month * rt_powd_snf(xi, 4.0) * b_s * (b_o * b_o) *
            rt_powd_snf(1.0 - eta * eta, -3.5) * ((2.75 * eta * (eta +
            rtU.orbit_tle[5]) + 1.0) + rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0));

          /* '<S147>:1:195' D_2     = 4*a_o_pp*xi*C_1^2; */
          D_2 = 4.0 * b_s * xi * (C_1 * C_1);

          /* '<S147>:1:196' D_3     = (4/3)*a_o_pp*xi^2*(17*a_o_pp + s)*C_1^3; */
          D_3 = 1.3333333333333333 * b_s * (xi * xi) * (17.0 * b_s +
            rtb_T_TT_J2000) * rt_powd_snf(C_1, 3.0);

          /* '<S147>:1:197' D_4     = (2/3)*a_o_pp*xi^3*(221*a_o_pp + 31*s)*C_1^4; */
          D_4 = 0.66666666666666663 * b_s * rt_powd_snf(xi, 3.0) * (221.0 * b_s
            + 31.0 * rtb_T_TT_J2000) * rt_powd_snf(C_1, 4.0);

          /* '<S147>:1:198' IL3     = D_2 + 2*C_1^2; */
          c_s = C_1 * C_1 * 2.0 + D_2;

          /* '<S147>:1:199' IL4     = 0.25*(3*D_3 + 12*C_1*D_2 + 10*C_1^3); */
          IL4 = ((12.0 * C_1 * D_2 + 3.0 * D_3) + 10.0 * rt_powd_snf(C_1, 3.0)) *
            0.25;

          /* '<S147>:1:200' IL5     = 0.2*(3*D_4 + 12*C_1*D_3 + 6*D_2^2 + 30*C_1^2*D_2 + 15*C_1^4); */
          IL5 = ((((12.0 * C_1 * D_3 + 3.0 * D_4) + D_2 * D_2 * 6.0) + C_1 * C_1
                  * 30.0 * D_2) + 15.0 * rt_powd_snf(C_1, 4.0)) * 0.2;
        }

        /*  Secular effects of atm drag and gravitation. Note that a_o_pp ~= 0 and */
        /*  b_o ~= 0 so no denoms here are zero. */
        /* '<S147>:1:205' M_DF    = M_o + ( 1 + (3*k_2*(3*tht^2-1))/(2*a_o_pp^2*b_o^3) + ... */
        /* '<S147>:1:206'             (3*k_2^2*(13-78*tht^2+137*tht^4))/(16*a_o_pp^4*b_o^7) )* ... */
        /* '<S147>:1:207'             n_o_pp*tsince; */
        M_DF = (((week * week * 3.0 - 1.0) * 0.0016239240000000001 / (b_s * b_s *
                  2.0 * rt_powd_snf(b_o, 3.0)) + 1.0) + ((13.0 - week * week *
                  78.0) + 137.0 * rt_powd_snf(week, 4.0)) *
                8.7904305259200008E-7 / (16.0 * rt_powd_snf(b_s, 4.0) *
                 rt_powd_snf(b_o, 7.0))) * b_min * T1900 + dayofyear;

        /* '<S147>:1:208' w_DF    = w_o + ( (-3*k_2*(1-5*tht^2))/(2*a_o_pp^2*b_o^3) + ... */
        /* '<S147>:1:209'             (3*k_2^2*(7-114*tht^2+395*tht^4))/(16*a_o_pp^4*b_o^8) + ... */
        /* '<S147>:1:210'             (5*k_4*(3-36*tht^2+49*tht^4))/(4*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
        day = ((((7.0 - week * week * 114.0) + 395.0 * rt_powd_snf(week, 4.0)) *
                8.7904305259200008E-7 / (16.0 * rt_powd_snf(b_s, 4.0) *
                 rt_powd_snf(b_o, 8.0)) + (1.0 - week * week * 5.0) *
                -0.0016239240000000001 / (b_s * b_s * 2.0 * rt_powd_snf(b_o, 3.0)))
               + ((3.0 - week * week * 36.0) + 49.0 * rt_powd_snf(week, 4.0)) *
               3.1049437500000002E-6 / (4.0 * rt_powd_snf(b_s, 4.0) *
                rt_powd_snf(b_o, 8.0))) * b_min * T1900 + days;

        /* '<S147>:1:211' Om_DF   = Om_o + ( (-3*k_2*tht)/(a_o_pp^2*b_o^4) +... */
        /* '<S147>:1:212'             (3*k_2^2*(4*tht-19*tht^3))/(2*a_o_pp^4*b_o^8) +... */
        /* '<S147>:1:213'             (5*k_4*tht*(3-7*tht^2))/(2*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
        /* '<S147>:1:215' Om      = Om_DF - (21/2)*( (n_o_pp*k_2*tht)/(a_o_pp^2*b_o^2) )*C_1*tsince^2; */
        rtb_T_TT_J2000 = ((((4.0 * week - 19.0 * rt_powd_snf(week, 3.0)) *
                            8.7904305259200008E-7 / (2.0 * rt_powd_snf(b_s, 4.0)
          * rt_powd_snf(b_o, 8.0)) + -0.0016239240000000001 * week / (b_s * b_s *
          rt_powd_snf(b_o, 4.0))) + (3.0 - week * week * 7.0) *
                           (3.1049437500000002E-6 * week) / (2.0 * rt_powd_snf
          (b_s, 4.0) * rt_powd_snf(b_o, 8.0))) * b_min * T1900 + rtU.orbit_tle[4]
                          * 0.0174532925) - b_min * 0.000541308 * week / (b_s *
          b_s * (b_o * b_o)) * 10.5 * C_1 * (T1900 * T1900);

        /* '<S147>:1:217' if( lowAlt == FALSE ) */
        if (lowAlt == 0) {
          /*  #5 %     */
          /* '<S147>:1:218' d_w     = BS*C_3*cos(w_o)*tsince; */
          days = month * rt_powd_snf(xi, 5.0) * 2.538815E-6 * b_min * sin
            (rtb_Sum) / (0.000541308 * rtU.orbit_tle[5]) * rtU.orbit_tle[2] *
            cos(days) * T1900;

          /* '<S147>:1:219' d_M     = -(2/3)*QOMS4*BS*xi^4*(a_E/(e_o*eta))* ... */
          /* '<S147>:1:220'                 ( (1+eta*cos(M_DF))^3 - (1 + eta*cos(M_o))^3 ); */
          month = -0.66666666666666663 * month * rtU.orbit_tle[2] * rt_powd_snf
            (xi, 4.0) * (1.0 / (rtU.orbit_tle[5] * eta)) * (rt_powd_snf(eta *
            cos(M_DF) + 1.0, 3.0) - rt_powd_snf(eta * cos(dayofyear) + 1.0, 3.0));

          /* '<S147>:1:221' M_p     = M_DF + d_w + d_M; */
          M_DF = (M_DF + days) + month;

          /* '<S147>:1:222' w       = w_DF - d_w - d_M; */
          day = (day - days) - month;

          /* '<S147>:1:223' e       = e_o - BS*C_4*tsince - BS*C_5*( sin(M_p) - sin(M_o) ); */
          month = (rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * T1900) -
            rtU.orbit_tle[2] * C_5 * (sin(M_DF) - sin(dayofyear));

          /* '<S147>:1:224' a       = a_o_pp*( 1 - C_1*tsince - D_2*tsince^2 - D_3*tsince^3 - ... */
          /* '<S147>:1:225'                 D_4*tsince^4 )^2; */
          dayofyear = (((1.0 - C_1 * T1900) - T1900 * T1900 * D_2) - D_3 *
                       rt_powd_snf(T1900, 3.0)) - D_4 * rt_powd_snf(T1900, 4.0);
          dayofyear = dayofyear * dayofyear * b_s;

          /* '<S147>:1:226' L       = M_p + w + Om + n_o_pp*( IL2*tsince^2 + IL3*tsince^3 +... */
          /* '<S147>:1:227'             IL4*tsince^4 + IL5*tsince^5 ); */
          T1900 = (((T1900 * T1900 * b_c + c_s * rt_powd_snf(T1900, 3.0)) + IL4 *
                    rt_powd_snf(T1900, 4.0)) + IL5 * rt_powd_snf(T1900, 5.0)) *
            b_min + ((M_DF + day) + rtb_T_TT_J2000);
        } else {
          /* '<S147>:1:228' else */
          /* '<S147>:1:229' M_p     = M_DF; */
          /* '<S147>:1:230' w       = w_DF; */
          /* '<S147>:1:231' a       = a_o_pp*(1 - C_1*tsince)^2; */
          dayofyear = 1.0 - C_1 * T1900;
          dayofyear = dayofyear * dayofyear * b_s;

          /* '<S147>:1:232' e       = e_o - BS*C_4*tsince; */
          month = rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * T1900;

          /* '<S147>:1:233' L       = M_p + w + Om + n_o_pp*IL2*tsince^2; */
          T1900 = ((M_DF + day) + rtb_T_TT_J2000) + b_min * b_c * (T1900 * T1900);
        }

        /* '<S147>:1:236' if( (a == 0) || (e >= 1) ) */
        if ((dayofyear == 0.0) || (month >= 1.0)) {
          /*  catch divide by zero conditions (catch #3) */
          /* '<S147>:1:237' pos_teme_km       = pos_invalid; */
          /* '<S147>:1:238' vel_teme_kmps     = vel_invalid; */
          rtb_pos_teme_km[0] = 6378.137;
          b_c = 0.0;
          rtb_pos_teme_km[1] = 6378.137;
          c_s = 0.0;
          rtb_pos_teme_km[2] = 6878.137;
          xi = 0.0;

          /* '<S147>:1:239' FLAG = int8(-1); */
          rtb_FLAG = -1;
        } else {
          /* '<S147>:1:242' b       = sqrt(1-e^2); */
          b_c = sqrt(1.0 - month * month);

          /* '<S147>:1:243' n       = XKE/(a^(3/2)); */
          days = 0.0743669161 / rt_powd_snf(dayofyear, 1.5);

          /*  Add the long-period periodic terms */
          /* '<S147>:1:246' a_xN    = e*cos(w); */
          b_min = month * cos(day);

          /* '<S147>:1:247' L_L     = (A_30*sin(i_o)/(8*k_2*a*b^2))*e*cos(w)*((3+5*tht)/(1+tht)); */
          /* '<S147>:1:248' a_yNL   = A_30*sin(i_o)/(4*k_2*a*b^2); */
          /* '<S147>:1:249' L_T     = L + L_L; */
          /* '<S147>:1:250' a_yN    = e*sin(w) + a_yNL; */
          b_s = 2.538815E-6 * sin(rtb_Sum) / (0.002165232 * dayofyear * (b_c *
            b_c)) + month * sin(day);

          /* '<S147>:1:252' U       = mod(L_T - Om,2*pi); */
          T1900 = ((2.538815E-6 * sin(rtb_Sum) / (0.004330464 * dayofyear * (b_c
                      * b_c)) * month * cos(day) * ((5.0 * week + 3.0) / (1.0 +
                      week)) + T1900) - rtb_T_TT_J2000) / 6.2831853071795862;
          if (fabs(T1900 - rt_roundd_snf(T1900)) <= 2.2204460492503131E-16 *
              fabs(T1900)) {
            T1900 = 0.0;
          } else {
            T1900 = (T1900 - floor(T1900)) * 6.2831853071795862;
          }

          /* '<S147>:1:253' Epw     = kepler(U,a_yN,a_xN); */
          /*  Solves Kepler's equation for Epw = E + w using iterative method */
          /* '<S147>:1:303' tol         = 1e-8; */
          /* '<S147>:1:304' iter_max    = 10; */
          /* '<S147>:1:305' Epw         = U; */
          day = T1900;

          /* '<S147>:1:306' Epw_new     = 0; */
          month = 0.0;

          /* '<S147>:1:307' iter        = 0; */
          lowAlt = 0;

          /* '<S147>:1:309' while( (abs(Epw - Epw_new) > tol) && (iter < iter_max) ) */
          while ((fabs(day - month) > 1.0E-8) && (lowAlt < 10)) {
            /* '<S147>:1:310' Epw     = Epw_new; */
            day = month;

            /* '<S147>:1:311' dEpw    = (U-a_yN*cos(Epw) + a_xN*sin(Epw) - Epw)/ ... */
            /* '<S147>:1:312'                     (-a_yN*sin(Epw) - a_xN*cos(Epw) + 1); */
            /* '<S147>:1:313' Epw_new = Epw + dEpw; */
            month += (((T1900 - b_s * cos(month)) + b_min * sin(month)) - month)
              / ((-b_s * sin(month) - b_min * cos(month)) + 1.0);

            /* '<S147>:1:314' iter    = iter +1; */
            lowAlt++;
          }

          /* '<S147>:1:317' Epw = Epw_new; */
          /*  Preliminary quantities for short-period periodics */
          /* '<S147>:1:256' ecE     = a_xN*cos(Epw) + a_yN*sin(Epw); */
          /* '<S147>:1:257' esE     = a_xN*sin(Epw) - a_yN*cos(Epw); */
          c_s = b_min * sin(month) - b_s * cos(month);

          /* '<S147>:1:258' e_L     = (a_xN^2 + a_yN^2)^(1/2); */
          day = sqrt(b_min * b_min + b_s * b_s);

          /* '<S147>:1:259' p_L     = a*(1 - e_L^2); */
          b_c = (1.0 - day * day) * dayofyear;

          /* '<S147>:1:260' r       = a*(1 - ecE); */
          T1900 = (1.0 - (b_min * cos(month) + b_s * sin(month))) * dayofyear;

          /* '<S147>:1:261' if( (r == 0) || (p_L == 0) ) */
          if ((T1900 == 0.0) || (b_c == 0.0)) {
            /*  (catch #4) */
            /* '<S147>:1:262' pos_teme_km       = pos_invalid; */
            /* '<S147>:1:263' vel_teme_kmps     = vel_invalid; */
            rtb_pos_teme_km[0] = 6378.137;
            b_c = 0.0;
            rtb_pos_teme_km[1] = 6378.137;
            c_s = 0.0;
            rtb_pos_teme_km[2] = 6878.137;
            xi = 0.0;

            /* '<S147>:1:264' FLAG = int8(-1); */
            rtb_FLAG = -1;
          } else {
            /* '<S147>:1:267' r_dot   = XKE*sqrt(a)*esE/r; */
            /* '<S147>:1:268' r_f_dot = XKE*sqrt(p_L)/r; */
            /* '<S147>:1:269' cosu    = (a/r)*( cos(Epw) - a_xN + ((a_yN*esE)/(1+sqrt(1 - e_L^2))) ); */
            /* '<S147>:1:270' sinu    = (a/r)*( sin(Epw) - a_yN - ((a_xN*esE)/(1 + sqrt(1 - e_L^2))) ); */
            /* '<S147>:1:271' u       = atan2(sinu,cosu); */
            month = rt_atan2d_snf(((sin(month) - b_s) - b_min * c_s / (sqrt(1.0
              - day * day) + 1.0)) * (dayofyear / T1900), (b_s * c_s / (sqrt(1.0
              - day * day) + 1.0) + (cos(month) - b_min)) * (dayofyear / T1900));

            /* '<S147>:1:272' dr      = (k_2/(2*p_L))*(1 - tht^2)*cos(2*u); */
            /* '<S147>:1:273' du      = -(k_2/(4*p_L^2))*(7*tht^2 - 1)*sin(2*u); */
            /* '<S147>:1:274' dOm     = (3*k_2*tht/(2*p_L^2))*sin(2*u); */
            /* '<S147>:1:275' di      = (3*k_2*tht/(2*p_L^2))*sin(i_o)*cos(2*u); */
            /* '<S147>:1:276' dr_dot  = -((k_2*n)/p_L)*(1-tht^2)*sin(2*u); */
            /* '<S147>:1:277' dr_f_dot    = (k_2*n/p_L)*( (1-tht^2)*cos(2*u) - (3/2)*(1-3*tht^2) ); */
            /*  Add the short-period periodics to get osculating elements */
            /* '<S147>:1:280' r_k     = r*(1 - (3/2)*k_2*sqrt(1 - e_L^2)*(3*tht^2 - 1)/p_L^2) + dr; */
            day = (1.0 - sqrt(1.0 - day * day) * 0.00081196200000000006 * (week *
                    week * 3.0 - 1.0) / (b_c * b_c)) * T1900 + 0.000541308 /
              (2.0 * b_c) * (1.0 - week * week) * cos(2.0 * month);

            /* '<S147>:1:281' u_k     = u + du; */
            b_min = -(0.000541308 / (b_c * b_c * 4.0)) * (week * week * 7.0 -
              1.0) * sin(2.0 * month) + month;

            /* '<S147>:1:282' Om_k    = Om + dOm; */
            rtb_T_TT_J2000 += 0.0016239240000000001 * week / (b_c * b_c * 2.0) *
              sin(2.0 * month);

            /* '<S147>:1:283' i_k     = i_o + di; */
            b_s = 0.0016239240000000001 * week / (b_c * b_c * 2.0) * sin(rtb_Sum)
              * cos(2.0 * month) + rtb_Sum;

            /* '<S147>:1:284' r_dot_k     = r_dot + dr_dot; */
            rtb_Sum = -(0.000541308 * days / b_c) * (1.0 - week * week) * sin
              (2.0 * month) + 0.0743669161 * sqrt(dayofyear) * c_s / T1900;

            /* '<S147>:1:285' r_f_dot_k   = r_f_dot + dr_f_dot; */
            week = ((1.0 - week * week) * cos(2.0 * month) - (1.0 - week * week *
                     3.0) * 1.5) * (0.000541308 * days / b_c) + 0.0743669161 *
              sqrt(b_c) / T1900;

            /*  Create unit orientation vectors */
            /* '<S147>:1:288' M       = [ -sin(Om_k)*cos(i_k); cos(Om_k)*cos(i_k); sin(i_k)]; */
            M[0] = -sin(rtb_T_TT_J2000) * cos(b_s);
            M[1] = cos(rtb_T_TT_J2000) * cos(b_s);
            M[2] = sin(b_s);

            /* '<S147>:1:289' N       = [ cos(Om_k); sin(Om_k); 0 ]; */
            dayofyear = cos(rtb_T_TT_J2000);
            month = sin(rtb_T_TT_J2000);

            /* '<S147>:1:291' U       = M.*sin(u_k) + N.*cos(u_k); */
            rtb_T_TT_J2000 = sin(b_min);
            T1900 = cos(b_min);

            /* '<S147>:1:292' V       = M.*cos(u_k) - N.*sin(u_k); */
            days = M[0] * rtb_T_TT_J2000 + dayofyear * T1900;
            rtb_pos_teme_km[0] = day * days * 6378.137;
            b_c = days;
            days = M[1] * rtb_T_TT_J2000 + month * T1900;
            rtb_pos_teme_km[1] = day * days * 6378.137;
            c_s = days;
            days = M[2] * rtb_T_TT_J2000 + 0.0 * T1900;
            rtb_pos_teme_km[2] = day * days * 6378.137;
            rtb_T_TT_J2000 = cos(b_min);
            T1900 = sin(b_min);

            /*  Find position and velocity in km & km/s */
            /* '<S147>:1:295' pos_teme_km     = r_k.*U.*XKMPER; */
            /* '<S147>:1:296' vel_teme_kmps   = (r_dot_k.*U + r_f_dot_k.*V).*(VKMPS); */
            b_c = ((M[0] * rtb_T_TT_J2000 - dayofyear * T1900) * week + rtb_Sum *
                   b_c) * 106.30228333333334;
            c_s = ((M[1] * rtb_T_TT_J2000 - month * T1900) * week + rtb_Sum *
                   c_s) * 106.30228333333334;
            xi = ((M[2] * rtb_T_TT_J2000 - 0.0 * T1900) * week + rtb_Sum * days)
              * 106.30228333333334;
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

  /* MATLAB Function: '<S6>/MATLAB Function1' */
  /*  Constants */
  /* MATLAB Function 'sun_vector_lib/MATLAB Function1': '<S149>:1' */
  /* '<S149>:1:4' M2KM        = 1e-3; */
  /* '<S149>:1:5' ECCE2       = 0.006694385000; */
  /* '<S149>:1:6' RE          = 6378.137; */
  /* '<S149>:1:7' TRUE        = 1; */
  /* '<S149>:1:8' FALSE       = 0; */
  /*  ----- */
  /*  Convert to sc position vector to km if necessary */
  /* '<S149>:1:12' sc_pos_eci_km   = sc_pos_eci_m*M2KM; */
  rtb_pos_teme_km[0] = M[0] * 0.001;
  rtb_pos_teme_km[1] = M[1] * 0.001;
  rtb_pos_teme_km[2] = M[2] * 0.001;

  /* ----- */
  /*  Main Function */
  /* '<S149>:1:16' LOS     = FALSE; */
  lowAlt = 0;

  /* '<S149>:1:18' sun_vector_eci_km(3)    = sun_vector_eci_km(3)/(sqrt(1 - ECCE2)); */
  rtb_sun_vector_eci_km[2] /= 0.99664718682189635;

  /* '<S149>:1:19' sc_pos_eci_km(3)        = sc_pos_eci_km(3)/(sqrt(1 - ECCE2)); */
  rtb_pos_teme_km[2] /= 0.99664718682189635;

  /* '<S149>:1:21' sun_vec     = sun_vector_eci_km; */
  /* '<S149>:1:22' sc_vec      = sc_pos_eci_km; */
  /* '<S149>:1:24' sc2sun_unit  = (sun_vec - sc_vec)/norm(sun_vec - sc_vec); */
  rtb_sun_vector_eci_km_0[0] = rtb_sun_vector_eci_km[0] - rtb_pos_teme_km[0];
  rtb_sun_vector_eci_km_0[1] = rtb_sun_vector_eci_km[1] - rtb_pos_teme_km[1];
  rtb_sun_vector_eci_km_0[2] = rtb_sun_vector_eci_km[2] - rtb_pos_teme_km[2];
  dayofyear = norm_i(rtb_sun_vector_eci_km_0);

  /*  Determine if the sc is in the light or shadow */
  /*    Call vector 1 the sun, vector 2 the sc */
  /* '<S149>:1:29' mag1    = norm(sun_vec); */
  week = norm_i(rtb_sun_vector_eci_km);

  /* '<S149>:1:30' mag2    = norm(sc_vec); */
  rtb_Sum = norm_i(rtb_pos_teme_km);

  /* '<S149>:1:32' mag1_sq     = mag1*mag1; */
  week *= week;

  /* '<S149>:1:33' mag2_sq     = mag2*mag2; */
  /* '<S149>:1:35' sun_sc_dot  = dot(sun_vec,sc_vec); */
  rtb_T_TT_J2000 = (rtb_sun_vector_eci_km[0] * rtb_pos_teme_km[0] +
                    rtb_sun_vector_eci_km[1] * rtb_pos_teme_km[1]) +
    rtb_sun_vector_eci_km[2] * rtb_pos_teme_km[2];

  /*  Find t_min */
  /* '<S149>:1:38' t_min_num   = mag1_sq - sun_sc_dot; */
  /* '<S149>:1:39' t_min_den   = mag1_sq + mag2_sq - 2*sun_sc_dot; */
  rtb_Sum = (rtb_Sum * rtb_Sum + week) - 2.0 * rtb_T_TT_J2000;

  /* '<S149>:1:41' if ( t_min_den < 0.0001  ) */
  if (rtb_Sum < 0.0001) {
    /* '<S149>:1:42' t_min = 0.0; */
    rtb_Sum = 0.0;
  } else {
    /* '<S149>:1:43' else */
    /* '<S149>:1:44' t_min = ( t_min_num ) / ( t_min_den ); */
    rtb_Sum = (week - rtb_T_TT_J2000) / rtb_Sum;
  }

  /* '<S149>:1:47' c_t_min_sq  = ((1-t_min)*mag1_sq + sun_sc_dot*t_min)/RE^2; */
  /*  Check LoS */
  /* '<S149>:1:50' if( t_min < 0 || t_min > 0.999979 ) */
  if ((rtb_Sum < 0.0) || (rtb_Sum > 0.999979)) {
    /* '<S149>:1:51' LOS     = TRUE; */
    lowAlt = 1;
  }

  /* Outport: '<Root>/sc_in_sun' incorporates:
   *  MATLAB Function: '<S6>/MATLAB Function1'
   *  Switch: '<S150>/Switch'
   */
  /* '<S149>:1:56' sc_in_sun   = LOS; */
  rtY.sc_in_sun = (lowAlt > 0.5);
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/sc2sun_unit' incorporates:
     *  MATLAB Function: '<S6>/MATLAB Function1'
     */
    rtY.sc2sun_unit[i] = (rtb_sun_vector_eci_km[i] - rtb_pos_teme_km[i]) /
      dayofyear;

    /* Product: '<S4>/Product2' incorporates:
     *  Math: '<S4>/Math Function1'
     */
    rtb_sun_vector_eci_km[i] = rtb_itrf_to_gcrf[3 * i + 2] * M[2] +
      (rtb_itrf_to_gcrf[3 * i + 1] * M[1] + rtb_itrf_to_gcrf[3 * i] * M[0]);
  }

  /* Sqrt: '<S70>/sqrt' incorporates:
   *  Product: '<S70>/Product2'
   *  Product: '<S70>/Product3'
   *  Sum: '<S70>/Sum2'
   */
  dayofyear = sqrt(rtb_sun_vector_eci_km[0] * rtb_sun_vector_eci_km[0] +
                   rtb_sun_vector_eci_km[1] * rtb_sun_vector_eci_km[1]);

  /* Outputs for Iterator SubSystem: '<S55>/While Iterator Subsystem' incorporates:
   *  WhileIterator: '<S72>/While Iterator'
   */
  s72_iter = 1;
  do {
    rtb_Sum = rtDW.Memory_PreviousInput;
    if (!(s72_iter > 0)) {
      rtb_Sum = rt_atan2d_snf(rtb_sun_vector_eci_km[2], 0.99664718933525254 *
        dayofyear);
    }

    week = sin(rtb_Sum);
    rtb_T_TT_J2000 = cos(rtb_Sum);
    month = rt_atan2d_snf(42841.31151331366 * week * week * week +
                          rtb_sun_vector_eci_km[2], dayofyear - rtb_T_TT_J2000 *
                          rtb_T_TT_J2000 * rtb_T_TT_J2000 * 0.00669437999014133 *
                          6.378137E+6);
    week = rt_atan2d_snf(0.99664718933525254 * sin(month), cos(month));
    rtDW.Memory_PreviousInput = week;
    s72_iter++;
  } while ((rtb_Sum != week) && (s72_iter <= 5));

  /* End of Outputs for SubSystem: '<S55>/While Iterator Subsystem' */

  /* UnitConversion: '<S69>/Unit Conversion' incorporates:
   *  Trigonometry: '<S55>/Trigonometric Function2'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  week = 57.295779513082323 * month;
  b_o = 57.295779513082323 * rt_atan2d_snf(rtb_sun_vector_eci_km[1],
    rtb_sun_vector_eci_km[0]);

  /* UnitConversion: '<S67>/Unit Conversion' */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  rtb_Sum = 0.017453292519943295 * week;

  /* Trigonometry: '<S54>/sincos' */
  day = cos(rtb_Sum);
  rtb_Sum = sin(rtb_Sum);
  b_s = day;

  /* UnitConversion: '<S67>/Unit Conversion' */
  b_min = rtb_Sum;
  rtb_Sum = 0.017453292519943295 * b_o;

  /* Trigonometry: '<S54>/sincos' */
  day = cos(rtb_Sum);
  rtb_Sum = sin(rtb_Sum);

  /* UnaryMinus: '<S58>/Unary Minus' incorporates:
   *  Product: '<S58>/u(1)*u(4)'
   *  Trigonometry: '<S54>/sincos'
   */
  rtb_VectorConcatenate[0] = -(b_min * day);

  /* UnaryMinus: '<S61>/Unary Minus' */
  rtb_VectorConcatenate[1] = -rtb_Sum;

  /* UnaryMinus: '<S64>/Unary Minus' incorporates:
   *  Product: '<S64>/u(3)*u(4)'
   *  Trigonometry: '<S54>/sincos'
   */
  rtb_VectorConcatenate[2] = -(b_s * day);

  /* UnaryMinus: '<S59>/Unary Minus' incorporates:
   *  Product: '<S59>/u(1)*u(2)'
   */
  rtb_VectorConcatenate[3] = -(b_min * rtb_Sum);

  /* SignalConversion: '<S68>/ConcatBufferAtVector ConcatenateIn5' incorporates:
   *  Trigonometry: '<S54>/sincos'
   */
  rtb_VectorConcatenate[4] = day;

  /* UnaryMinus: '<S65>/Unary Minus' incorporates:
   *  Product: '<S65>/u(2)*u(3)'
   */
  rtb_VectorConcatenate[5] = -(rtb_Sum * b_s);

  /* SignalConversion: '<S68>/ConcatBufferAtVector ConcatenateIn7' */
  rtb_VectorConcatenate[6] = b_s;

  /* SignalConversion: '<S68>/ConcatBufferAtVector ConcatenateIn8' incorporates:
   *  Constant: '<S63>/Constant'
   */
  rtb_VectorConcatenate[7] = 0.0;

  /* UnaryMinus: '<S66>/Unary Minus' */
  rtb_VectorConcatenate[8] = -b_min;

  /* MATLAB Function: '<S57>/MATLAB Function' */
  /*  UW HuskySat 1, ADCS Subsystem */
  /*    T. Reynolds, Updated: 10.28.17 */
  /* MATLAB Function 'YMDHMS_2_dec_year_lib/MATLAB Function': '<S146>:1' */
  /*  Converts a time vector into a decimal year number. See Vallado pp 200-201 */
  /*  for reference algorithms. Uses 6 inputs to avoid 1 based indexing. */
  /*  Input 'day' is day of year, not of month.  */
  /*  find day of year - valid until 2100 */
  /* Lmonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
  /* '<S146>:1:12' days_in_year    = 365.25; */
  rtb_Sum = 365.25;

  /* '<S146>:1:14' if mod(year,4) == 0 */
  if (rtb_time_ut1_idx_0 - floor(rtb_time_ut1_idx_0 / 4.0) * 4.0 == 0.0) {
    /* Lmonth(2) = 29; */
    /* '<S146>:1:16' days_in_year    = 366; */
    rtb_Sum = 366.0;
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
  /* '<S146>:1:30' day_of_year    = day + hour/24 + min/1440 + sec/86400; */
  /*  Find the year fraction represented by day_of_year */
  /* '<S146>:1:33' day_frac    = day_of_year/days_in_year; */
  /*  Sum to get total decimal year */
  /* '<S146>:1:36' dec_year = year + day_frac; */
  rtb_Sum = (((rtb_time_ut1_idx_3 / 24.0 + rtb_time_ut1_idx_2) +
              rtb_time_ut1_idx_4 / 1440.0) + rtb_time_ut1_idx_5 / 86400.0) /
    rtb_Sum + rtb_time_ut1_idx_0;

  /* End of MATLAB Function: '<S57>/MATLAB Function' */

  /* RelationalOperator: '<S95>/Relational Operator' incorporates:
   *  Memory: '<S95>/otime'
   */
  rtb_RelationalOperator = (rtb_Sum != rtDW.otime_PreviousInput);

  /* Saturate: '<S56>/+//- 180 deg' */
  if (b_o > 180.0) {
    rtb_T_TT_J2000 = 180.0;
  } else if (b_o < -180.0) {
    rtb_T_TT_J2000 = -180.0;
  } else {
    rtb_T_TT_J2000 = b_o;
  }

  /* End of Saturate: '<S56>/+//- 180 deg' */

  /* Saturate: '<S56>/+//- 90 deg' */
  if (week > 90.0) {
    T1900 = 90.0;
  } else if (week < -90.0) {
    T1900 = -90.0;
  } else {
    T1900 = week;
  }

  /* End of Saturate: '<S56>/+//- 90 deg' */

  /* UnitConversion: '<S139>/Unit Conversion' */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  b_s = 0.017453292519943295 * rtb_T_TT_J2000;
  day = 0.017453292519943295 * T1900;

  /* Trigonometry: '<S92>/sincos' */
  days = sin(b_s);
  rtb_time_ut1_idx_0 = cos(b_s);
  rtb_time_ut1_idx_3 = sin(day);
  rtb_time_ut1_idx_4 = cos(day);

  /* Outputs for Enabled SubSystem: '<S87>/Convert from geodetic to  spherical coordinates ' incorporates:
   *  EnablePort: '<S91>/Enable'
   */
  /* RelationalOperator: '<S94>/Relational Operator' incorporates:
   *  Memory: '<S94>/olon'
   */
  if (rtb_T_TT_J2000 != rtDW.olon_PreviousInput) {
    /* Outputs for Iterator SubSystem: '<S91>/For Iterator Subsystem' incorporates:
     *  ForIterator: '<S138>/For Iterator'
     */
    for (s138_iter = 1L; (int16_T)s138_iter < 12; s138_iter = (int16_T)s138_iter
         + 1) {
      /* Switch: '<S138>/cp[m-1] sp[m-1]' incorporates:
       *  Inport: '<S91>/cp[2]'
       *  Inport: '<S91>/sp[2]'
       *  UnitDelay: '<S138>/Unit Delay1'
       */
      if ((int16_T)s138_iter > 1) {
        b_s = rtDW.UnitDelay1_DSTATE[0];
        day = rtDW.UnitDelay1_DSTATE[1];
      } else {
        b_s = rtb_time_ut1_idx_0;
        day = days;
      }

      /* End of Switch: '<S138>/cp[m-1] sp[m-1]' */

      /* Sum: '<S138>/Sum2' incorporates:
       *  Inport: '<S91>/cp[2]'
       *  Inport: '<S91>/sp[2]'
       *  Product: '<S138>/Product1'
       *  Product: '<S138>/Product2'
       */
      b_min = b_s * days + day * rtb_time_ut1_idx_0;

      /* Assignment: '<S138>/Assignment' incorporates:
       *  Assignment: '<S138>/Assignment1'
       *  Constant: '<S138>/Constant'
       *  Constant: '<S138>/Constant1'
       */
      if ((int16_T)s138_iter == 1) {
        memcpy(&rtb_Assignment[0], &rtConstP.pooled15[0], 11U * sizeof(real_T));
        memcpy(&rtb_Assignment1[0], &rtConstP.pooled15[0], 11U * sizeof(real_T));
      }

      rtb_Assignment[(int16_T)s138_iter - 1] = b_min;

      /* End of Assignment: '<S138>/Assignment' */

      /* Sum: '<S138>/Sum1' incorporates:
       *  Inport: '<S91>/cp[2]'
       *  Inport: '<S91>/sp[2]'
       *  Product: '<S138>/Product3'
       *  Product: '<S138>/Product8'
       */
      b_s = b_s * rtb_time_ut1_idx_0 - day * days;

      /* Assignment: '<S138>/Assignment1' */
      rtb_Assignment1[(int16_T)s138_iter - 1] = b_s;

      /* Update for UnitDelay: '<S138>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE[0] = b_s;
      rtDW.UnitDelay1_DSTATE[1] = b_min;
    }

    /* End of Outputs for SubSystem: '<S91>/For Iterator Subsystem' */

    /* SignalConversion: '<S91>/OutportBufferForcp[13]' incorporates:
     *  Constant: '<S91>/cp[1]'
     *  Inport: '<S91>/cp[2]'
     */
    rtDW.OutportBufferForcp13[0] = 1.0;
    rtDW.OutportBufferForcp13[1] = rtb_time_ut1_idx_0;

    /* SignalConversion: '<S91>/OutportBufferForsp[13]' incorporates:
     *  Constant: '<S91>/sp[1]'
     *  Inport: '<S91>/sp[2]'
     */
    rtDW.OutportBufferForsp13[0] = 0.0;
    rtDW.OutportBufferForsp13[1] = days;

    /* SignalConversion: '<S91>/OutportBufferForcp[13]' */
    memcpy(&rtDW.OutportBufferForcp13[2], &rtb_Assignment1[0], 11U * sizeof
           (real_T));

    /* SignalConversion: '<S91>/OutportBufferForsp[13]' */
    memcpy(&rtDW.OutportBufferForsp13[2], &rtb_Assignment[0], 11U * sizeof
           (real_T));
  }

  /* End of RelationalOperator: '<S94>/Relational Operator' */
  /* End of Outputs for SubSystem: '<S87>/Convert from geodetic to  spherical coordinates ' */

  /* Trigonometry: '<S71>/sincos' */
  day = sin(month);

  /* Product: '<S71>/Product2' */
  days = day * 0.00669437999014133;

  /* Product: '<S71>/Product3' incorporates:
   *  Constant: '<S71>/Constant1'
   *  Constant: '<S71>/f'
   *  Product: '<S71>/Product1'
   *  Sqrt: '<S71>/sqrt'
   *  Sum: '<S71>/Sum'
   */
  rtb_time_ut1_idx_0 = 6.378137E+6 / sqrt(1.0 - days * day);

  /* Sum: '<S71>/Sum3' incorporates:
   *  Product: '<S71>/Product4'
   *  Product: '<S71>/Product5'
   *  Product: '<S71>/Product6'
   *  Sum: '<S71>/Sum2'
   *  Trigonometry: '<S71>/sincos'
   */
  rtb_time_ut1_idx_0 = ((days * rtb_time_ut1_idx_0 + rtb_sun_vector_eci_km[2]) *
                        day - rtb_time_ut1_idx_0) + cos(month) * dayofyear;

  /* Saturate: '<S56>/0 to 1,000,000 m' */
  if (rtb_time_ut1_idx_0 > 1.0E+6) {
    dayofyear = 1.0E+6;
  } else if (rtb_time_ut1_idx_0 < 0.0) {
    dayofyear = 0.0;
  } else {
    dayofyear = rtb_time_ut1_idx_0;
  }

  /* End of Saturate: '<S56>/0 to 1,000,000 m' */

  /* Gain: '<S56>/Gain' */
  rtb_time_ut1_idx_2 = 0.001 * dayofyear;

  /* Logic: '<S93>/Logical Operator' incorporates:
   *  Memory: '<S93>/oalt'
   *  Memory: '<S93>/olat'
   *  RelationalOperator: '<S93>/Relational Operator'
   *  RelationalOperator: '<S93>/Relational Operator1'
   */
  rtb_LogicalOperator = ((T1900 != rtDW.olat_PreviousInput) ||
    (rtb_time_ut1_idx_2 != rtDW.oalt_PreviousInput));

  /* Product: '<S92>/Product' */
  dayofyear = rtb_time_ut1_idx_3 * rtb_time_ut1_idx_3;

  /* Product: '<S92>/Product1' */
  month = rtb_time_ut1_idx_4 * rtb_time_ut1_idx_4;

  /* Outputs for Enabled SubSystem: '<S87>/Convert from geodetic to  spherical coordinates' incorporates:
   *  EnablePort: '<S90>/Enable'
   */
  if (rtb_LogicalOperator) {
    /* Sqrt: '<S133>/sqrt' incorporates:
     *  Product: '<S133>/Product'
     *  Sum: '<S133>/Sum'
     */
    day = sqrt(4.0680631590768993E+7 - dayofyear * 272331.60668193549);

    /* Product: '<S90>/Product1' */
    days = day * rtb_time_ut1_idx_2;

    /* Sqrt: '<S132>/sqrt' incorporates:
     *  Product: '<S132>/Product10'
     *  Product: '<S132>/Product9'
     *  Sum: '<S132>/Sum7'
     */
    b_min = sqrt(month * 4.0680631590768993E+7 + 4.0408299984087057E+7 *
                 dayofyear);

    /* Sqrt: '<S90>/sqrt' incorporates:
     *  Gain: '<S135>/Gain'
     *  Product: '<S135>/Product1'
     *  Product: '<S135>/Product6'
     *  Product: '<S135>/Product7'
     *  Product: '<S135>/Product8'
     *  Sum: '<S135>/Sum5'
     *  Sum: '<S135>/Sum6'
     */
    rtDW.sqrt_e = sqrt((1.6549137866238722E+15 - 2.208307901990225E+13 *
                        dayofyear) / (day * day) + (2.0 * days +
      rtb_time_ut1_idx_2 * rtb_time_ut1_idx_2));

    /* Product: '<S130>/Product11' incorporates:
     *  Sum: '<S130>/Sum8'
     */
    rtDW.Product11 = (rtb_time_ut1_idx_2 + b_min) / rtDW.sqrt_e;

    /* Sum: '<S134>/Sum2' */
    day = 4.0680631590768993E+7 + days;

    /* Sum: '<S134>/Sum1' */
    days += 4.0408299984087057E+7;

    /* Product: '<S131>/Product4' incorporates:
     *  Product: '<S131>/Product3'
     *  Product: '<S134>/Product1'
     *  Product: '<S134>/Product2'
     *  Sqrt: '<S131>/sqrt'
     *  Sum: '<S131>/Sum3'
     */
    rtDW.Product4 = rtb_time_ut1_idx_3 / sqrt(day * day / (days * days) * month
      + dayofyear);

    /* Product: '<S136>/Product12' incorporates:
     *  Product: '<S136>/Product1'
     */
    rtDW.Product12 = 272331.60668193549 / (rtDW.sqrt_e * b_min) *
      rtb_time_ut1_idx_4 * rtb_time_ut1_idx_3;

    /* Sqrt: '<S137>/sqrt' incorporates:
     *  Constant: '<S137>/Constant'
     *  Product: '<S137>/Product5'
     *  Sum: '<S137>/Sum4'
     */
    rtDW.sqrt_j = sqrt(1.0 - rtDW.Product4 * rtDW.Product4);
  }

  /* End of Outputs for SubSystem: '<S87>/Convert from geodetic to  spherical coordinates' */

  /* Product: '<S87>/aor' incorporates:
   *  Constant: '<S87>/re'
   */
  dayofyear = 6371.2 / rtDW.sqrt_e;

  /* Outputs for Iterator SubSystem: '<S87>/Compute magnetic vector in spherical coordinates' incorporates:
   *  ForIterator: '<S89>/For Iterator'
   */
  /* InitializeConditions for UnitDelay: '<S89>/Unit Delay' */
  month = 0.0;

  /* InitializeConditions for UnitDelay: '<S89>/Unit Delay2' */
  rtb_time_ut1_idx_3 = 0.0;
  rtb_time_ut1_idx_4 = 0.0;
  rtb_time_ut1_idx_5 = 0.0;
  b_s = 0.0;
  for (s138_iter = 1L; (int16_T)s138_iter < 13; s138_iter = (int16_T)s138_iter +
       1) {
    /* Switch: '<S89>/ar(n)' incorporates:
     *  Product: '<S87>/ar'
     */
    if (!((int16_T)s138_iter > 1)) {
      month = dayofyear * dayofyear;
    }

    /* End of Switch: '<S89>/ar(n)' */

    /* Product: '<S89>/Product8' */
    month *= dayofyear;

    /* Sum: '<S89>/Sum' */
    lowAlt = (int16_T)s138_iter + 1;

    /* Outputs for Iterator SubSystem: '<S89>/For Iterator Subsystem' incorporates:
     *  ForIterator: '<S97>/For Iterator'
     */
    if (rtDW.ForIterator_IterationMarker[4] != 0) {
      /* InitializeConditions for UnitDelay: '<S98>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_a = 0.0;

      /* InitializeConditions for UnitDelay: '<S98>/Unit Delay3' */
      rtDW.UnitDelay3_DSTATE = 0.0;

      /* InitializeConditions for UnitDelay: '<S98>/Unit Delay2' */
      rtDW.UnitDelay2_DSTATE_e = 0.0;

      /* InitializeConditions for UnitDelay: '<S98>/Unit Delay4' */
      rtDW.UnitDelay4_DSTATE = 0.0;
    }

    for (i = 0; i < 7; i++) {
      rtDW.ForIterator_IterationMarker[i] = 1U;
    }

    /* Sum: '<S89>/Sum' incorporates:
     *  Constant: '<S104>/Constant'
     *  Constant: '<S104>/Constant1'
     *  Logic: '<S104>/Logical Operator'
     *  RelationalOperator: '<S104>/Relational Operator'
     *  RelationalOperator: '<S104>/Relational Operator1'
     *  Sum: '<S97>/Sum1'
     */
    for (idx1 = 1L; (int16_T)idx1 <= lowAlt; idx1 = (int16_T)idx1 + 1) {
      /* Sum: '<S97>/Sum1' */
      qY_1 = (int16_T)idx1 - 1;
      idx2 = (int16_T)idx1 - 1;

      /* Outputs for Enabled SubSystem: '<S97>/Time adjust the gauss coefficients' incorporates:
       *  EnablePort: '<S100>/Enable'
       */
      if (rtb_RelationalOperator) {
        /* Outputs for Atomic SubSystem: '<S100>/If Action Subsystem' */
        /* Sum: '<S126>/Sum1' */
        i = (int16_T)idx2 + 1;

        /* Sum: '<S126>/Sum2' */
        qY_0 = (int16_T)s138_iter + 1;

        /* End of Outputs for SubSystem: '<S100>/If Action Subsystem' */

        /* Assignment: '<S100>/Assignment' incorporates:
         *  Constant: '<S100>/c[maxdef][maxdef]'
         *  Constant: '<S100>/cd[maxdef][maxdef]'
         *  Constant: '<S87>/epoch'
         *  Product: '<S126>/Product'
         *  Selector: '<S126>/c[m][n]'
         *  Selector: '<S126>/cd[m][n]'
         *  Sum: '<S126>/Sum'
         *  Sum: '<S126>/Sum1'
         *  Sum: '<S126>/Sum2'
         *  Sum: '<S87>/Sum'
         *  UnitDelay: '<S100>/Unit Delay'
         */
        if (rtDW.ForIterator_IterationMarker[5] < 2) {
          rtDW.ForIterator_IterationMarker[5L] = 2U;
          memcpy(&Assignment[0], &rtDW.UnitDelay_DSTATE_p[0], 169U * sizeof
                 (real_T));
        }

        /* Outputs for Atomic SubSystem: '<S100>/If Action Subsystem' */
        Assignment[(i + 13 * (qY_0 - 1)) - 1] = rtConstP.cdmaxdefmaxdef_Value
          [((qY_0 - 1) * 13 + i) - 1] * (rtb_Sum - 2015.0) +
          rtConstP.cmaxdefmaxdef_Value[((qY_0 - 1) * 13 + i) - 1];

        /* End of Assignment: '<S100>/Assignment' */
        /* End of Outputs for SubSystem: '<S100>/If Action Subsystem' */

        /* Switch: '<S127>/tc_old' incorporates:
         *  UnitDelay: '<S127>/Unit Delay'
         */
        for (i = 0; i < 169; i++) {
          if ((int16_T)s138_iter > 1) {
            rtb_tc_old[i] = rtDW.UnitDelay_DSTATE_n[i];
          } else {
            rtb_tc_old[i] = 0.0;
          }
        }

        /* End of Switch: '<S127>/tc_old' */

        /* If: '<S127>/If' incorporates:
         *  Sum: '<S97>/Sum1'
         */
        if (qY_1 != 0) {
          /* Outputs for IfAction SubSystem: '<S127>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S128>/Action Port'
           */
          /* Sum: '<S128>/Sum2' */
          i = (int16_T)s138_iter + 1;

          /* Assignment: '<S128>/Assignment2' incorporates:
           *  Constant: '<S100>/c[maxdef][maxdef]'
           *  Constant: '<S100>/cd[maxdef][maxdef]'
           *  Constant: '<S87>/epoch'
           *  Product: '<S128>/Product'
           *  Selector: '<S128>/c[m][n]'
           *  Selector: '<S128>/cd[m][n]'
           *  Sum: '<S128>/Sum'
           *  Sum: '<S128>/Sum2'
           *  Sum: '<S87>/Sum'
           */
          if (rtDW.ForIterator_IterationMarker[6] < 2) {
            rtDW.ForIterator_IterationMarker[6L] = 2U;
            memcpy(&Assignment2[0], &rtb_tc_old[0], 169U * sizeof(real_T));
          }

          Assignment2[(i + 13 * (qY_1 - 1)) - 1] =
            rtConstP.cdmaxdefmaxdef_Value[((qY_1 - 1) * 13 + i) - 1] * (rtb_Sum
            - 2015.0) + rtConstP.cmaxdefmaxdef_Value[((qY_1 - 1) * 13 + i) - 1];

          /* End of Assignment: '<S128>/Assignment2' */

          /* Gain: '<S128>/Gain' */
          memcpy(&rtb_tc_old[0], &Assignment2[0], 169U * sizeof(real_T));

          /* End of Outputs for SubSystem: '<S127>/If Action Subsystem1' */
        }

        /* End of If: '<S127>/If' */
        for (i = 0; i < 169; i++) {
          /* Sum: '<S100>/Sum2' */
          rtDW.Sum2_n[i] = Assignment[i] + rtb_tc_old[i];

          /* Update for UnitDelay: '<S100>/Unit Delay' */
          rtDW.UnitDelay_DSTATE_p[i] = Assignment[i];

          /* Update for UnitDelay: '<S127>/Unit Delay' */
          rtDW.UnitDelay_DSTATE_n[i] = rtb_tc_old[i];
        }
      }

      /* End of Outputs for SubSystem: '<S97>/Time adjust the gauss coefficients' */

      /* If: '<S103>/If' incorporates:
       *  Sum: '<S97>/Sum1'
       */
      if (qY_1 == 0) {
        /* Outputs for IfAction SubSystem: '<S103>/If Action Subsystem' incorporates:
         *  ActionPort: '<S109>/Action Port'
         */
        /* Gain: '<S109>/Gain1' incorporates:
         *  Constant: '<S103>/Constant1'
         *  Constant: '<S109>/Constant'
         *  Product: '<S109>/Product'
         *  Selector: '<S103>/cp[m+1]'
         *  Selector: '<S109>/Selector'
         *  Sum: '<S103>/Sum4'
         *  Sum: '<S109>/Sum'
         */
        day = rtDW.Sum2_n[((int16_T)((real_T)s138_iter + 1.0) - 1) * 13] *
          rtDW.OutportBufferForcp13[(int16_T)((real_T)idx2 + 1.0) - 1];

        /* Gain: '<S109>/Gain2' incorporates:
         *  Constant: '<S103>/Constant1'
         *  Constant: '<S109>/Constant'
         *  Product: '<S109>/Product'
         *  Selector: '<S103>/sp[m+1]'
         *  Selector: '<S109>/Selector'
         *  Sum: '<S103>/Sum4'
         *  Sum: '<S109>/Sum'
         */
        days = rtDW.Sum2_n[((int16_T)((real_T)s138_iter + 1.0) - 1) * 13] *
          rtDW.OutportBufferForsp13[(int16_T)((real_T)idx2 + 1.0) - 1];

        /* End of Outputs for SubSystem: '<S103>/If Action Subsystem' */
      } else {
        /* Outputs for IfAction SubSystem: '<S103>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S110>/Action Port'
         */
        /* Sum: '<S110>/Sum' incorporates:
         *  Constant: '<S103>/Constant1'
         *  Constant: '<S111>/Constant'
         *  Product: '<S110>/Product'
         *  Product: '<S110>/Product1'
         *  Selector: '<S103>/cp[m+1]'
         *  Selector: '<S103>/sp[m+1]'
         *  Selector: '<S110>/Selector'
         *  Selector: '<S110>/Selector1'
         *  Sum: '<S103>/Sum4'
         *  Sum: '<S111>/Sum'
         *  Sum: '<S112>/Sum'
         */
        day = rtDW.Sum2_n[(((int16_T)((real_T)s138_iter + 1.0) - 1) * 13 + (qY_1
          + 1)) - 1] * rtDW.OutportBufferForcp13[(int16_T)((real_T)idx2 + 1.0) -
          1] + rtDW.Sum2_n[(qY_1 - 1) * 13 + (int16_T)s138_iter] *
          rtDW.OutportBufferForsp13[(int16_T)((real_T)idx2 + 1.0) - 1];

        /* Sum: '<S110>/Sum1' incorporates:
         *  Constant: '<S103>/Constant1'
         *  Constant: '<S111>/Constant'
         *  Product: '<S110>/Product'
         *  Product: '<S110>/Product1'
         *  Selector: '<S103>/cp[m+1]'
         *  Selector: '<S103>/sp[m+1]'
         *  Selector: '<S110>/Selector'
         *  Selector: '<S110>/Selector1'
         *  Sum: '<S103>/Sum4'
         *  Sum: '<S111>/Sum'
         *  Sum: '<S112>/Sum'
         */
        days = rtDW.Sum2_n[(((int16_T)((real_T)s138_iter + 1.0) - 1) * 13 +
                            (qY_1 + 1)) - 1] * rtDW.OutportBufferForsp13
          [(int16_T)((real_T)idx2 + 1.0) - 1] - rtDW.Sum2_n[(qY_1 - 1) * 13 +
          (int16_T)s138_iter] * rtDW.OutportBufferForcp13[(int16_T)((real_T)idx2
          + 1.0) - 1];

        /* End of Outputs for SubSystem: '<S103>/If Action Subsystem1' */
      }

      /* End of If: '<S103>/If' */

      /* Outputs for Enabled SubSystem: '<S97>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' incorporates:
       *  EnablePort: '<S99>/Enable'
       */
      if (rtb_LogicalOperator) {
        /* If: '<S99>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' incorporates:
         *  Sum: '<S97>/Sum1'
         */
        if ((int16_T)s138_iter == qY_1) {
          /* Outputs for IfAction SubSystem: '<S99>/If Action Subsystem' incorporates:
           *  ActionPort: '<S113>/Action Port'
           */
          /* Gain: '<S117>/Gain' incorporates:
           *  Sum: '<S117>/Sum2'
           */
          j = mul_s32_sat(13L, qY_1 - 1);

          /* Selector: '<S113>/Selector' incorporates:
           *  Sum: '<S117>/Sum1'
           */
          if (j > MAX_int32_T - (int16_T)s138_iter) {
            j_0 = MAX_int32_T;
            j = MAX_int32_T;
          } else {
            j_0 = (int16_T)s138_iter + j;
            j += (int16_T)s138_iter;
          }

          /* Product: '<S113>/Product1' incorporates:
           *  Selector: '<S113>/Selector'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          rtDW.Merge_p = rtDW.UnitDelay1_DSTATE_k[(int16_T)j_0 - 1] *
            rtDW.sqrt_j;

          /* Sum: '<S113>/Sum' incorporates:
           *  Product: '<S113>/Product'
           *  Product: '<S113>/Product2'
           *  Selector: '<S113>/Selector'
           *  Selector: '<S113>/Selector1'
           *  UnitDelay: '<S99>/Unit Delay'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          rtDW.Merge1_b = rtDW.UnitDelay_DSTATE_f[(((int16_T)s138_iter - 1) * 13
            + qY_1) - 1] * rtDW.sqrt_j + rtDW.UnitDelay1_DSTATE_k[(int16_T)j - 1]
            * rtDW.Product4;

          /* End of Outputs for SubSystem: '<S99>/If Action Subsystem' */
        } else if (((int16_T)s138_iter == 1) && (qY_1 == 0)) {
          /* Outputs for IfAction SubSystem: '<S99>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S114>/Action Port'
           */
          /* Selector: '<S114>/Selector' incorporates:
           *  Gain: '<S119>/Gain'
           *  Sum: '<S119>/Sum1'
           */
          j = mul_s32_sat(13L, 0L);
          if (j > 2147483646L) {
            j = MAX_int32_T;
          } else {
            j++;
          }

          /* Product: '<S114>/Product3' incorporates:
           *  Selector: '<S114>/Selector'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          rtDW.Merge_p = rtDW.UnitDelay1_DSTATE_k[(int16_T)j - 1] *
            rtDW.Product4;

          /* Selector: '<S114>/Selector' incorporates:
           *  Gain: '<S119>/Gain'
           *  Sum: '<S119>/Sum1'
           */
          j = mul_s32_sat(13L, 0L);
          if (j > 2147483646L) {
            j = MAX_int32_T;
          } else {
            j++;
          }

          /* Sum: '<S114>/Sum' incorporates:
           *  Product: '<S114>/Product'
           *  Product: '<S114>/Product2'
           *  Selector: '<S114>/Selector'
           *  Selector: '<S114>/Selector1'
           *  UnitDelay: '<S99>/Unit Delay'
           *  UnitDelay: '<S99>/Unit Delay1'
           */
          rtDW.Merge1_b = rtDW.Product4 * rtDW.UnitDelay_DSTATE_f[0] -
            rtDW.UnitDelay1_DSTATE_k[(int16_T)j - 1] * rtDW.sqrt_j;

          /* End of Outputs for SubSystem: '<S99>/If Action Subsystem1' */
        } else {
          if (((int16_T)s138_iter > 1) && ((int16_T)s138_iter != qY_1)) {
            /* Outputs for IfAction SubSystem: '<S99>/If Action Subsystem2' incorporates:
             *  ActionPort: '<S115>/Action Port'
             */
            /* Sum: '<S122>/Sum' */
            i = (int16_T)s138_iter + 1;
            qY_0 = (int16_T)idx2 + 1;

            /* Gain: '<S121>/Gain' */
            j = mul_s32_sat(13L, qY_1);

            /* Selector: '<S115>/Selector' incorporates:
             *  Sum: '<S121>/Sum1'
             */
            if (j > MAX_int32_T - (int16_T)s138_iter) {
              j_0 = MAX_int32_T;
            } else {
              j_0 = (int16_T)s138_iter + j;
            }

            /* Switch: '<S115>/Switch' incorporates:
             *  Constant: '<S115>/Constant'
             *  DataTypeConversion: '<S124>/Data Type Conversion'
             *  RelationalOperator: '<S124>/Relational Operator'
             *  Selector: '<S115>/Selector1'
             *  Sum: '<S123>/Sum'
             *  Sum: '<S123>/Sum2'
             *  Sum: '<S124>/Sum2'
             *  UnitDelay: '<S99>/Unit Delay'
             */
            if (((int16_T)s138_iter - 2 >= (int16_T)idx2) > 0.5) {
              rtb_Sum1_a_idx_3 = rtDW.UnitDelay_DSTATE_f[((int16_T)s138_iter - 2)
                * 13 + (int16_T)idx2];
            } else {
              rtb_Sum1_a_idx_3 = 0.0;
            }

            /* End of Switch: '<S115>/Switch' */

            /* Sum: '<S115>/Sum' incorporates:
             *  Constant: '<S115>/k[13][13]'
             *  Product: '<S115>/Product'
             *  Product: '<S115>/Product1'
             *  Product: '<S115>/Product4'
             *  Selector: '<S115>/Selector'
             *  Selector: '<S115>/Selector1'
             *  Selector: '<S115>/Selector2'
             *  SignalConversion: '<S115>/TmpSignal ConversionAtSelector1Inport3'
             *  Sum: '<S122>/Sum'
             *  UnitDelay: '<S99>/Unit Delay'
             *  UnitDelay: '<S99>/Unit Delay1'
             */
            rtDW.Merge1_b = (rtDW.UnitDelay_DSTATE_f[((int16_T)s138_iter - 1) *
                             13 + qY_1] * rtDW.Product4 -
                             rtDW.UnitDelay1_DSTATE_k[(int16_T)j_0 - 1] *
                             rtDW.sqrt_j) - rtConstP.pooled9[((i - 1) * 13 +
              qY_0) - 1] * rtb_Sum1_a_idx_3;

            /* Switch: '<S115>/Switch1' incorporates:
             *  Constant: '<S115>/Constant1'
             *  DataTypeConversion: '<S125>/Data Type Conversion'
             *  RelationalOperator: '<S125>/Relational Operator'
             *  Selector: '<S115>/Selector'
             *  Sum: '<S125>/Sum2'
             *  UnitDelay: '<S99>/Unit Delay1'
             */
            if (((int16_T)s138_iter - 2 >= qY_1) > 0.5) {
              /* Selector: '<S115>/Selector' incorporates:
               *  Sum: '<S121>/Sum1'
               *  Sum: '<S121>/Sum2'
               */
              qY = (int16_T)s138_iter - 1;
              if (j > MAX_int32_T - qY) {
                j_0 = MAX_int32_T;
              } else {
                j_0 = qY + j;
              }

              rtb_Sum1_a_idx_3 = rtDW.UnitDelay1_DSTATE_k[(int16_T)j_0 - 1];
            } else {
              rtb_Sum1_a_idx_3 = 0.0;
            }

            /* End of Switch: '<S115>/Switch1' */

            /* Selector: '<S115>/Selector' incorporates:
             *  Sum: '<S121>/Sum1'
             */
            if (j > MAX_int32_T - (int16_T)s138_iter) {
              j = MAX_int32_T;
            } else {
              j += (int16_T)s138_iter;
            }

            /* Sum: '<S115>/Sum1' incorporates:
             *  Constant: '<S115>/k[13][13]'
             *  Product: '<S115>/Product2'
             *  Product: '<S115>/Product3'
             *  Selector: '<S115>/Selector'
             *  Selector: '<S115>/Selector2'
             *  Sum: '<S122>/Sum'
             *  UnitDelay: '<S99>/Unit Delay1'
             */
            rtDW.Merge_p = rtDW.UnitDelay1_DSTATE_k[(int16_T)j - 1] *
              rtDW.Product4 - rtConstP.pooled9[((i - 1) * 13 + qY_0) - 1] *
              rtb_Sum1_a_idx_3;

            /* End of Outputs for SubSystem: '<S99>/If Action Subsystem2' */
          }
        }

        /* End of If: '<S99>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' */

        /* Sum: '<S99>/Sum' */
        i = (int16_T)s138_iter + 1;
        qY_0 = (int16_T)idx2 + 1;

        /* Assignment: '<S99>/Assignment' incorporates:
         *  Sum: '<S99>/Sum'
         *  UnitDelay: '<S99>/Unit Delay'
         */
        if (rtDW.ForIterator_IterationMarker[2] < 2) {
          rtDW.ForIterator_IterationMarker[2L] = 2U;
          memcpy(&rtDW.Assignment_o[0], &rtDW.UnitDelay_DSTATE_f[0], 169U *
                 sizeof(real_T));
        }

        rtDW.Assignment_o[(qY_0 + 13 * (i - 1)) - 1] = rtDW.Merge1_b;

        /* End of Assignment: '<S99>/Assignment' */

        /* Assignment: '<S99>/Assignment_snorm' incorporates:
         *  Gain: '<S116>/Gain'
         *  Sum: '<S116>/Sum1'
         *  Sum: '<S116>/Sum2'
         *  Sum: '<S99>/Sum'
         *  UnitDelay: '<S99>/Unit Delay1'
         */
        if (rtDW.ForIterator_IterationMarker[3] < 2) {
          rtDW.ForIterator_IterationMarker[3L] = 2U;
          memcpy(&rtDW.Assignment_snorm[0], &rtDW.UnitDelay1_DSTATE_k[0], 169U *
                 sizeof(real_T));
        }

        rtDW.Assignment_snorm[(int16_T)((real_T)i + (real_T)mul_s32_sat(13L,
          qY_0 - 1)) - 1] = rtDW.Merge_p;

        /* End of Assignment: '<S99>/Assignment_snorm' */

        /* Update for UnitDelay: '<S99>/Unit Delay' */
        memcpy(&rtDW.UnitDelay_DSTATE_f[0], &rtDW.Assignment_o[0], 169U * sizeof
               (real_T));

        /* Update for UnitDelay: '<S99>/Unit Delay1' */
        memcpy(&rtDW.UnitDelay1_DSTATE_k[0], &rtDW.Assignment_snorm[0], 169U *
               sizeof(real_T));
      }

      /* End of Outputs for SubSystem: '<S97>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

      /* Selector: '<S98>/snorm[n+m*13]' incorporates:
       *  Gain: '<S102>/Gain'
       *  Sum: '<S102>/Sum1'
       *  Sum: '<S97>/Sum1'
       */
      i = 1 + (int16_T)s138_iter;
      j = mul_s32_sat(13L, qY_1);
      if (j > MAX_int32_T - i) {
        j = MAX_int32_T;
      } else {
        j += i;
      }

      /* Product: '<S98>/par' incorporates:
       *  Selector: '<S98>/snorm[n+m*13]'
       */
      b_min = rtDW.Assignment_snorm[(int16_T)j - 1] * month;

      /* Outputs for Enabled SubSystem: '<S98>/Special case - North//South Geographic Pole' incorporates:
       *  EnablePort: '<S101>/Enable'
       */
      if ((rtDW.sqrt_j == 0.0) && (1 == qY_1)) {
        if (!rtDW.SpecialcaseNorthSouthGeographic) {
          rtDW.SpecialcaseNorthSouthGeographic = true;
        }

        /* If: '<S101>/n ==1' incorporates:
         *  Assignment: '<S106>/Assignment2'
         */
        if ((int16_T)s138_iter == 1) {
          /* Outputs for IfAction SubSystem: '<S101>/If Action Subsystem1' incorporates:
           *  ActionPort: '<S105>/Action Port'
           */
          /* Assignment: '<S105>/Assignment2' incorporates:
           *  Selector: '<S105>/pp[n-1]'
           *  UnitDelay: '<S101>/Unit Delay1'
           */
          if (rtDW.ForIterator_IterationMarker[0] < 2) {
            rtDW.ForIterator_IterationMarker[0L] = 2U;
            memcpy(&rtDW.Assignment2_en[0], &rtDW.UnitDelay1_DSTATE_m[0], 13U *
                   sizeof(real_T));
          }

          rtDW.Assignment2_en[1] = rtDW.UnitDelay1_DSTATE_m[0];

          /* End of Assignment: '<S105>/Assignment2' */
          /* End of Outputs for SubSystem: '<S101>/If Action Subsystem1' */
        } else {
          /* Outputs for IfAction SubSystem: '<S101>/If Action Subsystem2' incorporates:
           *  ActionPort: '<S106>/Action Port'
           */
          if (rtDW.ForIterator_IterationMarker[1] < 2) {
            /* Assignment: '<S106>/Assignment2' incorporates:
             *  UnitDelay: '<S101>/Unit Delay1'
             */
            rtDW.ForIterator_IterationMarker[1L] = 2U;
            memcpy(&rtDW.Assignment2_e[0], &rtDW.UnitDelay1_DSTATE_m[0], 13U *
                   sizeof(real_T));
          }

          /* Assignment: '<S106>/Assignment2' incorporates:
           *  Constant: '<S106>/k[13][13]'
           *  Product: '<S106>/Product1'
           *  Product: '<S106>/Product2'
           *  Selector: '<S106>/Selector2'
           *  Selector: '<S106>/pp[n-2] pp[n-1]'
           *  Sum: '<S106>/Sum1'
           *  Sum: '<S106>/Sum2'
           *  Sum: '<S107>/Sum2'
           *  Sum: '<S108>/Sum'
           *  UnitDelay: '<S101>/Unit Delay1'
           */
          rtDW.Assignment2_e[(int16_T)s138_iter] = rtDW.UnitDelay1_DSTATE_m
            [(int16_T)s138_iter - 1] * rtDW.Product4 - rtConstP.pooled9[(int16_T)
            s138_iter * 13 + 1] * rtDW.UnitDelay1_DSTATE_m[(int16_T)s138_iter -
            2];

          /* End of Outputs for SubSystem: '<S101>/If Action Subsystem2' */
        }

        /* End of If: '<S101>/n ==1' */

        /* SignalConversion: '<S101>/TmpSignal ConversionAtpp[n]Inport1' incorporates:
         *  UnitDelay: '<S101>/Unit Delay1'
         */
        rtb_TmpSignalConversionAtppnInp[0] = rtDW.UnitDelay1_DSTATE_m[0];
        rtb_TmpSignalConversionAtppnInp[1] = rtDW.Assignment2_en[1];
        memcpy(&rtb_TmpSignalConversionAtppnInp[2], &rtDW.Assignment2_e[2], 11U *
               sizeof(real_T));

        /* Product: '<S101>/Product2' incorporates:
         *  Selector: '<S101>/pp[n]'
         *  Sum: '<S101>/Sum2'
         */
        rtDW.Product2 = month * rtb_TmpSignalConversionAtppnInp[(int16_T)
          s138_iter] * days;

        /* Update for UnitDelay: '<S101>/Unit Delay1' */
        memcpy(&rtDW.UnitDelay1_DSTATE_m[0], &rtb_TmpSignalConversionAtppnInp[0],
               13U * sizeof(real_T));
      } else {
        if (rtDW.SpecialcaseNorthSouthGeographic) {
          /* Disable for Outport: '<S101>/bpp' */
          rtDW.Product2 = 0.0;
          rtDW.SpecialcaseNorthSouthGeographic = false;
        }
      }

      /* End of Outputs for SubSystem: '<S98>/Special case - North//South Geographic Pole' */

      /* Sum: '<S98>/Sum1' incorporates:
       *  Constant: '<S104>/Constant'
       *  Constant: '<S104>/Constant1'
       *  Logic: '<S104>/Logical Operator'
       *  Product: '<S98>/Product'
       *  RelationalOperator: '<S104>/Relational Operator'
       *  RelationalOperator: '<S104>/Relational Operator1'
       *  Selector: '<S98>/dp[n][m]'
       *  Sum: '<S97>/Sum1'
       *  Sum: '<S98>/Sum'
       *  UnitDelay: '<S98>/Unit Delay1'
       */
      rtDW.Sum1 = rtDW.UnitDelay1_DSTATE_a - rtDW.Assignment_o[(int16_T)
        s138_iter * 13 + (int16_T)idx2] * day * month;

      /* Sum: '<S98>/Sum2' incorporates:
       *  Constant: '<S98>/fm'
       *  Product: '<S98>/Product1'
       *  Selector: '<S98>/fm[m]'
       *  Sum: '<S98>/Sum4'
       *  UnitDelay: '<S98>/Unit Delay3'
       */
      rtDW.Sum2 = rtConstP.fm_Value[(int16_T)idx2] * b_min * days +
        rtDW.UnitDelay3_DSTATE;

      /* Sum: '<S98>/Sum3' incorporates:
       *  Constant: '<S98>/fn'
       *  Product: '<S98>/Product2'
       *  Selector: '<S98>/fn[m]'
       *  Sum: '<S98>/Sum4'
       *  UnitDelay: '<S98>/Unit Delay2'
       */
      rtDW.Sum3 = rtConstP.fn_Value[(int16_T)s138_iter] * b_min * day +
        rtDW.UnitDelay2_DSTATE_e;

      /* Sum: '<S98>/Sum5' incorporates:
       *  UnitDelay: '<S98>/Unit Delay4'
       */
      rtDW.Sum5 = rtDW.UnitDelay4_DSTATE + rtDW.Product2;

      /* Update for UnitDelay: '<S98>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_a = rtDW.Sum1;

      /* Update for UnitDelay: '<S98>/Unit Delay3' */
      rtDW.UnitDelay3_DSTATE = rtDW.Sum2;

      /* Update for UnitDelay: '<S98>/Unit Delay2' */
      rtDW.UnitDelay2_DSTATE_e = rtDW.Sum3;

      /* Update for UnitDelay: '<S98>/Unit Delay4' */
      rtDW.UnitDelay4_DSTATE = rtDW.Sum5;
    }

    /* End of Outputs for SubSystem: '<S89>/For Iterator Subsystem' */

    /* Sum: '<S89>/Sum1' incorporates:
     *  UnitDelay: '<S89>/Unit Delay2'
     */
    rtb_Sum1_a_idx_0 = rtb_time_ut1_idx_3 + rtDW.Sum1;
    rtb_Sum1_a_idx_1 = rtb_time_ut1_idx_4 + rtDW.Sum2;
    rtb_Sum1_a_idx_2 = rtb_time_ut1_idx_5 + rtDW.Sum3;
    rtb_Sum1_a_idx_3 = b_s + rtDW.Sum5;

    /* Update for UnitDelay: '<S89>/Unit Delay2' */
    rtb_time_ut1_idx_3 = rtb_Sum1_a_idx_0;
    rtb_time_ut1_idx_4 = rtb_Sum1_a_idx_1;
    rtb_time_ut1_idx_5 = rtb_Sum1_a_idx_2;
    b_s = rtb_Sum1_a_idx_3;
  }

  /* End of Outputs for SubSystem: '<S87>/Compute magnetic vector in spherical coordinates' */

  /* Switch: '<S141>/Switch' incorporates:
   *  Product: '<S141>/Product'
   */
  if (rtDW.sqrt_j != 0.0) {
    days = rtb_Sum1_a_idx_1 / rtDW.sqrt_j;
  } else {
    days = rtb_Sum1_a_idx_3;
  }

  /* End of Switch: '<S141>/Switch' */

  /* Sum: '<S140>/Sum1' incorporates:
   *  Product: '<S140>/Product1'
   *  Product: '<S140>/Product4'
   */
  day = (0.0 - rtDW.Product11 * rtb_Sum1_a_idx_0) - rtb_Sum1_a_idx_2 *
    rtDW.Product12;

  /* UnitConversion: '<S145>/Unit Conversion' incorporates:
   *  Trigonometry: '<S143>/Trigonometric Function1'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  month = 57.295779513082323 * rt_atan2d_snf(days, day);

  /* Sum: '<S142>/Sum1' incorporates:
   *  Product: '<S142>/Product1'
   *  Product: '<S142>/Product4'
   */
  dayofyear = rtDW.Product12 * rtb_Sum1_a_idx_0 - rtb_Sum1_a_idx_2 *
    rtDW.Product11;

  /* Sum: '<S143>/Sum' incorporates:
   *  Product: '<S143>/Product'
   *  Product: '<S143>/Product1'
   */
  days = days * days + day * day;

  /* UnitConversion: '<S88>/Unit Conversion' incorporates:
   *  Sqrt: '<S143>/sqrt1'
   *  Trigonometry: '<S143>/Trigonometric Function'
   *  UnitConversion: '<S144>/Unit Conversion'
   */
  /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
  b_s = 0.017453292519943295 * month;
  day = 57.295779513082323 * rt_atan2d_snf(dayofyear, sqrt(days)) *
    0.017453292519943295;

  /* Trigonometry: '<S82>/sincos' */
  b_min = cos(b_s);
  b_s = sin(b_s);

  /* Sum: '<S143>/Sum1' incorporates:
   *  Product: '<S143>/Product2'
   */
  days += dayofyear * dayofyear;

  /* Sqrt: '<S143>/sqrt' */
  days = sqrt(days);

  /* Product: '<S82>/h1' incorporates:
   *  Trigonometry: '<S82>/sincos'
   */
  dayofyear = cos(day) * days;

  /* Product: '<S82>/x1' */
  month = b_min * dayofyear;

  /* Product: '<S82>/y1' */
  dayofyear *= b_s;

  /* Product: '<S82>/z1' incorporates:
   *  Trigonometry: '<S82>/sincos'
   */
  days *= sin(day);

  /* Gain: '<S4>/Gain' */
  rtb_Sum1_a_idx_3 = 1.0E-9 * month;
  rtb_Sum1_a_idx_1 = 1.0E-9 * dayofyear;
  rtb_Sum1_a_idx_0 = 1.0E-9 * days;

  /* Product: '<S4>/Product' incorporates:
   *  Math: '<S4>/Math Function'
   *  Product: '<S4>/Product1'
   */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    T1900_0[lowAlt] = rtb_VectorConcatenate[3 * lowAlt + 2] * rtb_Sum1_a_idx_0 +
      (rtb_VectorConcatenate[3 * lowAlt + 1] * rtb_Sum1_a_idx_1 +
       rtb_VectorConcatenate[3 * lowAlt] * rtb_Sum1_a_idx_3);
  }

  /* End of Product: '<S4>/Product' */

  /* Product: '<S4>/Product1' */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    rtb_sun_vector_eci_km[lowAlt] = rtb_itrf_to_gcrf[lowAlt + 6] * T1900_0[2] +
      (rtb_itrf_to_gcrf[lowAlt + 3] * T1900_0[1] + rtb_itrf_to_gcrf[lowAlt] *
       T1900_0[0]);
  }

  /* S-Function (sdsp2norm2): '<S4>/Normalization' */
  s138_iter = 0L;
  i = 0;
  while (i < 1) {
    idx1 = s138_iter;
    idx2 = s138_iter;
    j = 0L;
    while ((int16_T)j < 1) {
      dayofyear = 0.0;
      for (lowAlt = 0; lowAlt < 3; lowAlt++) {
        dayofyear += rtb_sun_vector_eci_km[idx1] * rtb_sun_vector_eci_km[idx1];
        idx1++;
      }

      dayofyear = 1.0 / (sqrt(dayofyear) + 1.0E-10);
      for (lowAlt = 0; lowAlt < 3; lowAlt++) {
        rtb_sun_vector_eci_km[idx2] *= dayofyear;
        idx2++;
      }

      j = 1L;
    }

    s138_iter++;
    i = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S4>/Normalization' */

  /* Outport: '<Root>/mag_eci_unit' */
  rtY.mag_eci_unit[0] = rtb_sun_vector_eci_km[0];
  rtY.mag_eci_unit[1] = rtb_sun_vector_eci_km[1];
  rtY.mag_eci_unit[2] = rtb_sun_vector_eci_km[2];

  /* UnaryMinus: '<S30>/Unary Minus' incorporates:
   *  Product: '<S30>/u(1)*u(4)'
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
  rtb_VectorConcatenate_b[0] = 0.39377477022454854;

  /* UnaryMinus: '<S33>/Unary Minus' */
  rtb_VectorConcatenate_b[1] = 0.84522918991728324;

  /* UnaryMinus: '<S36>/Unary Minus' incorporates:
   *  Product: '<S36>/u(3)*u(4)'
   */
  rtb_VectorConcatenate_b[2] = 0.361288038615143;

  /* UnaryMinus: '<S31>/Unary Minus' incorporates:
   *  Product: '<S31>/u(1)*u(2)'
   */
  rtb_VectorConcatenate_b[3] = 0.62280585651662945;

  /* SignalConversion: '<S40>/ConcatBufferAtVector ConcatenateIn5' */
  rtb_VectorConcatenate_b[4] = -0.53440398249991838;

  /* UnaryMinus: '<S37>/Unary Minus' incorporates:
   *  Product: '<S37>/u(2)*u(3)'
   */
  rtb_VectorConcatenate_b[5] = 0.57142387819972529;

  /* SignalConversion: '<S40>/ConcatBufferAtVector ConcatenateIn7' */
  rtb_VectorConcatenate_b[6] = 0.67605790833566293;

  /* SignalConversion: '<S40>/ConcatBufferAtVector ConcatenateIn8' incorporates:
   *  Constant: '<S35>/Constant'
   */
  rtb_VectorConcatenate_b[7] = 0.0;

  /* UnaryMinus: '<S38>/Unary Minus' */
  rtb_VectorConcatenate_b[8] = -0.73684849499527949;

  /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
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
  /*  gs_eci_km(3)  = gs_eci_km(3)/(sqrt(1 - ECCE2)); */
  /*  sc_pos_eci_km(3)        = sc_pos_eci_km(3)/(sqrt(1 - ECCE2)); */
  /* '<S11>:1:17' gs_vec     = gs_ecef_m; */
  /* '<S11>:1:18' sc_vec      = sc_pos_ecef_m; */
  /* In ECI coordinates */
  /* '<S11>:1:21' sc2gs_unit  = (gs_vec - sc_vec)/norm(gs_vec - sc_vec); */
  for (lowAlt = 0; lowAlt < 3; lowAlt++) {
    /* Product: '<S10>/Product1' incorporates:
     *  Math: '<S10>/Math Function1'
     *  Sum: '<S10>/Sum'
     */
    rtb_Sum1_a_idx_3 = rtb_VectorConcatenate_b[3 * lowAlt + 2] *
      -6.3664995496819746E+6 + (rtb_VectorConcatenate_b[3 * lowAlt + 1] * 0.0 +
      rtb_VectorConcatenate_b[3 * lowAlt] * 0.0);

    /* Product: '<S3>/Product3' incorporates:
     *  Math: '<S3>/Math Function1'
     */
    rtb_Sum1_a_idx_1 = rtb_itrf_to_gcrf[3 * lowAlt + 2] * M[2] +
      (rtb_itrf_to_gcrf[3 * lowAlt + 1] * M[1] + rtb_itrf_to_gcrf[3 * lowAlt] *
       M[0]);

    /* MATLAB Function: '<S9>/MATLAB Function1' */
    rtb_sun_vector_eci_km_0[lowAlt] = rtb_Sum1_a_idx_3 - rtb_Sum1_a_idx_1;

    /* Sum: '<S10>/Sum' */
    rtb_sun_vector_eci_km[lowAlt] = rtb_Sum1_a_idx_3;

    /* Product: '<S3>/Product3' incorporates:
     *  Math: '<S3>/Math Function1'
     */
    rtb_pos_teme_km[lowAlt] = rtb_Sum1_a_idx_1;
  }

  /* MATLAB Function: '<S9>/MATLAB Function1' */
  dayofyear = norm(rtb_sun_vector_eci_km_0);
  rtb_pos_teme_km[0] = (rtb_sun_vector_eci_km[0] - rtb_pos_teme_km[0]) /
    dayofyear;
  rtb_pos_teme_km[1] = (rtb_sun_vector_eci_km[1] - rtb_pos_teme_km[1]) /
    dayofyear;

  /*  Determine if the sc is above the horizon in Seattle */
  /* '<S11>:1:25' sc2gs_gs_dot  = dot(sc2gs_unit,gs_vec)/norm(gs_vec); */
  /*  Check LoS */
  /* '<S11>:1:28' if sc2gs_gs_dot < 0 */
  if (((rtb_sun_vector_eci_km[2] - rtb_pos_teme_km[2]) / dayofyear *
       rtb_sun_vector_eci_km[2] + (rtb_pos_teme_km[0] * rtb_sun_vector_eci_km[0]
        + rtb_pos_teme_km[1] * rtb_sun_vector_eci_km[1])) / norm
      (rtb_sun_vector_eci_km) < 0.0) {
    /* '<S11>:1:29' ABOVE     = TRUE; */
    lowAlt = 1;
  } else {
    /* '<S11>:1:30' else */
    /* '<S11>:1:31' ABOVE     = FALSE; */
    lowAlt = 0;
  }

  /* Outport: '<Root>/sc_above_gs' incorporates:
   *  MATLAB Function: '<S9>/MATLAB Function1'
   *  Switch: '<S12>/Switch'
   */
  /* '<S11>:1:34' sc_above_gs = ABOVE; */
  /* '<S11>:1:36' pr = sc_vec - gs_vec; */
  /* '<S11>:1:37' fs = norm(pr)*cosd(fov_ang) - gs_vec'*pr/norm(gs_vec); */
  /* '<S11>:1:38' if (fs < 0) */
  rtY.sc_above_gs = (lowAlt > 0.5);
  for (i = 0; i < 3; i++) {
    /* Outport: '<Root>/pos_eci_m' */
    rtY.pos_eci_m[i] = M[i];

    /* Outport: '<Root>/vel_eci_mps' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Product: '<S5>/Product1'
     */
    rtY.vel_eci_mps[i] = 1000.0 * (rtb_teme_to_gcrf[i + 6] * xi +
      (rtb_teme_to_gcrf[i + 3] * c_s + rtb_teme_to_gcrf[i] * b_c));
  }

  /* Outport: '<Root>/SGP4_flag' */
  rtY.SGP4_flag = rtb_FLAG;

  /* Assertion: '<S79>/Assertion' incorporates:
   *  Constant: '<S79>/max_val'
   *  Constant: '<S79>/min_val'
   *  Logic: '<S79>/conjunction'
   *  RelationalOperator: '<S79>/max_relop'
   *  RelationalOperator: '<S79>/min_relop'
   */
  utAssert((0.0 <= rtb_time_ut1_idx_0) && (rtb_time_ut1_idx_0 <= 1.0E+6));

  /* Assertion: '<S80>/Assertion' incorporates:
   *  Constant: '<S80>/max_val'
   *  Constant: '<S80>/min_val'
   *  Logic: '<S80>/conjunction'
   *  RelationalOperator: '<S80>/max_relop'
   *  RelationalOperator: '<S80>/min_relop'
   */
  utAssert((-90.0 <= week) && (week <= 90.0));

  /* Assertion: '<S81>/Assertion' incorporates:
   *  Constant: '<S81>/max_val'
   *  Constant: '<S81>/min_val'
   *  Logic: '<S81>/conjunction'
   *  RelationalOperator: '<S81>/max_relop'
   *  RelationalOperator: '<S81>/min_relop'
   */
  utAssert((-180.0 <= b_o) && (b_o <= 180.0));

  /* Assertion: '<S83>/Assertion' incorporates:
   *  Constant: '<S83>/max_val'
   *  Constant: '<S83>/min_val'
   *  Constant: '<S87>/epoch'
   *  Logic: '<S83>/conjunction'
   *  RelationalOperator: '<S83>/max_relop'
   *  RelationalOperator: '<S83>/min_relop'
   *  Sum: '<S87>/Sum'
   */
  utAssert((0.0 <= rtb_Sum - 2015.0) && (rtb_Sum - 2015.0 <= 5.0));

  /* Update for Memory: '<S95>/otime' */
  rtDW.otime_PreviousInput = rtb_Sum;

  /* Update for Memory: '<S94>/olon' */
  rtDW.olon_PreviousInput = rtb_T_TT_J2000;

  /* Update for Memory: '<S93>/olat' */
  rtDW.olat_PreviousInput = T1900;

  /* Update for Memory: '<S93>/oalt' */
  rtDW.oalt_PreviousInput = rtb_time_ut1_idx_2;
}

/* Model initialize function */
void MSP_env_estim0_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    int16_T i;

    /* InitializeConditions for Memory: '<S95>/otime' */
    rtDW.otime_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S94>/olon' */
    rtDW.olon_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S93>/olat' */
    rtDW.olat_PreviousInput = -1000.0;

    /* InitializeConditions for Memory: '<S93>/oalt' */
    rtDW.oalt_PreviousInput = -1000.0;

    /* SystemInitialize for Enabled SubSystem: '<S87>/Convert from geodetic to  spherical coordinates ' */
    /* SystemInitialize for Outport: '<S91>/sp[13]' */
    memcpy(&rtDW.OutportBufferForsp13[0], &rtConstP.sp13_Y0[0], 13U * sizeof
           (real_T));

    /* SystemInitialize for Outport: '<S91>/cp[13]' */
    memcpy(&rtDW.OutportBufferForcp13[0], &rtConstP.cp13_Y0[0], 13U * sizeof
           (real_T));

    /* End of SystemInitialize for SubSystem: '<S87>/Convert from geodetic to  spherical coordinates ' */

    /* SystemInitialize for Enabled SubSystem: '<S87>/Convert from geodetic to  spherical coordinates' */
    /* SystemInitialize for Outport: '<S90>/r' */
    rtDW.sqrt_e = 6378.137;

    /* SystemInitialize for Outport: '<S90>/ct' */
    rtDW.Product4 = 1.0;

    /* End of SystemInitialize for SubSystem: '<S87>/Convert from geodetic to  spherical coordinates' */

    /* SystemInitialize for Iterator SubSystem: '<S87>/Compute magnetic vector in spherical coordinates' */
    /* SystemInitialize for Iterator SubSystem: '<S89>/For Iterator Subsystem' */
    /* SystemInitialize for Enabled SubSystem: '<S97>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */
    /* InitializeConditions for UnitDelay: '<S99>/Unit Delay1' */
    memcpy(&rtDW.UnitDelay1_DSTATE_k[0], &rtConstP.pooled12[0], 169U * sizeof
           (real_T));

    /* SystemInitialize for Outport: '<S99>/snorm[169]' */
    memcpy(&rtDW.Assignment_snorm[0], &rtConstP.pooled12[0], 169U * sizeof
           (real_T));

    /* End of SystemInitialize for SubSystem: '<S97>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

    /* SystemInitialize for Enabled SubSystem: '<S98>/Special case - North//South Geographic Pole' */
    /* SystemInitialize for IfAction SubSystem: '<S101>/If Action Subsystem2' */
    /* SystemInitialize for IfAction SubSystem: '<S101>/If Action Subsystem1' */
    for (i = 0; i < 13; i++) {
      /* InitializeConditions for UnitDelay: '<S101>/Unit Delay1' */
      rtDW.UnitDelay1_DSTATE_m[i] = 1.0;

      /* SystemInitialize for Outport: '<S105>/pp[13]' */
      rtDW.Assignment2_en[i] = 1.0;

      /* SystemInitialize for Outport: '<S106>/pp[13]' */
      rtDW.Assignment2_e[i] = 1.0;
    }

    /* End of SystemInitialize for SubSystem: '<S101>/If Action Subsystem1' */
    /* End of SystemInitialize for SubSystem: '<S101>/If Action Subsystem2' */
    /* End of SystemInitialize for SubSystem: '<S98>/Special case - North//South Geographic Pole' */
    /* End of SystemInitialize for SubSystem: '<S89>/For Iterator Subsystem' */
    /* End of SystemInitialize for SubSystem: '<S87>/Compute magnetic vector in spherical coordinates' */
  }
}

/* Model terminate function */
void MSP_env_estim0_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
