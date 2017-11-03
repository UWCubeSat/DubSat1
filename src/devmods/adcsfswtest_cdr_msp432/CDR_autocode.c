/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CDR_autocode.c
 *
 * Code generated for Simulink model 'CDR_autocode'.
 *
 * Model version                  : 1.217
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Tue Oct 31 19:47:41 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "CDR_autocode.h"
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
extern real_T rt_roundd_snf(real_T u);
extern real_T rt_remd_snf(real_T u0, real_T u1);
extern real_T rt_powd_snf(real_T u0, real_T u1);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);
void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo);
int32_T mul_s32_sat(int32_T a, int32_T b);

/* Forward declaration for local functions */
static real_T norm(const real_T x[3]);
static void mrdivide(real_T A[36], const real_T B_0[36]);
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

/* Function for MATLAB Function: '<S21>/MATLAB Function1' */
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

/* Function for MATLAB Function: '<S12>/kalman_gain' */
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

/* Model step function */
void CDR_autocode_step(void)
{
    real_T JD_days_gps;
    real_T T1900;
    real_T days;
    real_T dayofyear;
    real_T i;
    real_T day_num;
    static const int8_T b[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    real_T oblq_rad;
    real_T s;
    real_T b_s;
    real_T c_s;
    real_T c_c;
    static const real_T a[9] = { 1.0, 0.0, 0.0, 0.0, 0.91748214306524178,
                                 0.397776969112606, 0.0, -0.397776969112606, 0.91748214306524178 };

    static const int8_T b_0[3] = { 1, 0, 0 };

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
    real_T IL2;
    real_T IL3;
    real_T IL4;
    real_T IL5;
    real_T M_DF;
    real_T sc_pos_eci_km[3];
    int8_T Lmonth[12];
    static const int8_T b_1[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    int8_T Phi_22[9];
    int8_T I[9];
    static const int8_T a_0[9] = { -1, 0, 0, 0, -1, 0, 0, 0, -1 };

    static const int8_T b_a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

    static const int8_T b_2[16] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0
    };

    static const int8_T a_1[16] = { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0
    };

    static const int8_T b_b[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

    real_T KHI[36];
    int8_T I_0[36];
    real_T rtb_Gain[3];
    int8_T s31_iter;
    real_T rtb_VectorConcatenate[9];
    boolean_T rtb_RelationalOperator;
    boolean_T rtb_LogicalOperator_h;
    real_T rtb_Sum1_dv[4];
    int16_T s108_iter;
    int16_T s67_iter;
    real_T rtb_MatrixMultiply[6];
    real_T rtb_sun_vector_eci_km[3];
    real_T rtb_time_vec_ut1[6];
    real_T rtb_Assignment[11];
    real_T rtb_Assignment1[11];
    int32_T rtb_Sum1_jr;
    real_T rtb_TmpSignalConversionAtppnInp[13];
    real_T rtb_itrf_to_gcrf[9];
    real_T rtb_Merge2[36];
    real_T rtb_mod_to_gcrf[9];
    real_T rtb_K[36];
    real_T rtb_tc_old[169];
    real_T UnitDelay2_DSTATE_n[4];
    real_T Assignment[169];
    real_T Assignment2[169];
    real_T rtb_Merge2_0[36];
    real_T rtb_sun_vector_eci_km_0[3];
    real_T c_c_0[9];
    real_T a_2[9];
    real_T i_0[9];
    real_T dayofyear_0[3];
    int16_T i_1;
    real_T UnitDelay2_DSTATE_n_0[6];
    real_T rtb_Merge2_1[36];
    real_T tmp[6];
    real_T KHI_0[36];
    real_T rtb_K_0[36];
    real_T tod_to_mod_0[9];
    real_T tmp_0[9];
    real_T s_0[16];
    real_T a_3[16];
    real_T a_4[16];
    int16_T qY;
    int16_T qY_0;
    int16_T qY_1;
    int16_T qY_2;
    int32_T rtb_Sum1_b;

    /* MATLAB Function: '<S122>/Translate to UT1' incorporates:
     *  Inport: '<Root>/gps_time'
     */
    /*  ------ Prelim Stuff ------ % */
    /*  Account for \Delta UT1 to get closest estimate to UT1 */
    /*    \Delta UT1 obtained from: */
    /*        http://maia.usno.navy.mil/ser7/finals.daily */
    /*    on Feb 24, 2017: 0.5236028 */
    /* MATLAB Function 'gps_to_ut1/Translate to UT1': '<S125>:1' */
    /*  Account for leap seconds GPS --> UT1 */
    /* '<S125>:1:11' dut1 = -0.07; */
    /*  estimated for Oct 1, 2018 */
    /* '<S125>:1:12' gps2ut1     = dut1 - 18; */
    /*  sec --> JD conversion */
    /* '<S125>:1:15' sec2JD  = (0.0001/8.64); */
    /*  JD/sec */
    /* '<S125>:1:17' feb = 28; */
    /* '<S125>:1:18' cal_days = [31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
    /* '<S125>:1:20' sec_in = cur_gps(1); */
    /* '<S125>:1:20' week_in = cur_gps(2); */
    /*  --------------------------- % */
    /*  Translate GPS w-s to JD and Gregorian ymd-hms */
    /* '<S125>:1:25' JD_jan61980 = 2.444244500000000e+06; */
    /* '<S125>:1:26' JD_J2000    = 2451545; */
    /* '<S125>:1:28' JD_days_gps     = JD_jan61980 + 7*week_in + sec_in/3600/24; */
    JD_days_gps = (7.0 * rtU.gps_time[1] + 2.4442445E+6) + rtU.gps_time[0] /
            3600.0 / 24.0;

    /* '<S125>:1:29' JD_days_ut1     = JD_days_gps; */
    /* + sec2JD*gps2ut1; */
    /* '<S125>:1:31' JD_ut1_J2000_days       = JD_days_ut1 - JD_J2000; */
    /* '<S125>:1:32' JD_ut1_J2000_century    = (JD_days_ut1 - 2451545)/36525; */
    /* '<S125>:1:34' T1900   = (JD_days_ut1 - 2415019.5)/365.25; */
    T1900 = (JD_days_gps - 2.4150195E+6) / 365.25;

    /* '<S125>:1:36' year    = 1900 + floor(T1900); */
    s = floor(T1900);
    T1900 = 1900.0 + floor(T1900);

    /* '<S125>:1:38' leapyrs     = floor((year - 1900 - 1)*0.25); */
    /* '<S125>:1:39' days    = (JD_days_ut1 - 2415019.5) - ((year - 1900)*365 + leapyrs ); */
    days = (JD_days_gps - 2.4150195E+6) - (floor((((1900.0 + s) - 1900.0) - 1.0) *
                                                 0.25) + ((1900.0 + s) - 1900.0) * 365.0);

    /* '<S125>:1:41' if days < 1 */
    if (days < 1.0) {
        /* '<S125>:1:42' year = year - 1; */
        T1900 = (1900.0 + s) - 1.0;

        /* '<S125>:1:43' leapyrs = floor((year - 1900 - 1)*0.25); */
        /* '<S125>:1:44' days    = (JD_days_ut1 - 2415019.5) - ((year - 1900)*365 + leapyrs ); */
        days = (JD_days_gps - 2.4150195E+6) - (floor(((((1900.0 + s) - 1.0) - 1900.0)
                - 1.0) * 0.25) + (((1900.0 + s) - 1.0) - 1900.0) * 365.0);
    }

    /* '<S125>:1:47' if mod(year,4) == 0 */
    /* '<S125>:1:51' dayofyear   = floor(days); */
    dayofyear = floor(days);

    /* '<S125>:1:53' day     = 0; */
    days = 0.0;

    /* '<S125>:1:54' i       = 0; */
    i = 0.0;

    /* '<S125>:1:56' while day < dayofyear */
    while (days < dayofyear) {
        /* '<S125>:1:57' i   = i + 1; */
        i++;

        /* '<S125>:1:58' day = day + cal_days(i); */
        days += (real_T)b[(int16_T)i - 1];
    }

    /* '<S125>:1:61' month   = i - 1; */
    /* '<S125>:1:63' day_num = 0; */
    day_num = 0.0;

    /* '<S125>:1:64' for i = 1:month */
    for (lowAlt = 0; lowAlt < (int16_T)(i - 1.0); lowAlt++) {
        /* '<S125>:1:65' day_num = day_num + cal_days(i); */
        day_num += (real_T)b[lowAlt];
    }

    /* '<S125>:1:68' day     = day - day_num; */
    days -= day_num;

    /* '<S125>:1:70' tau     = 24*( day - dayofyear ); */
    dayofyear = (days - dayofyear) * 24.0;

    /* '<S125>:1:72' hour    = floor( tau ); */
    /* '<S125>:1:73' min     = floor( 60*(tau - hour) ); */
    day_num = (dayofyear - dayofyear) * 60.0;

    /* '<S125>:1:74' sec     = 3600*( tau - hour - (min/60) ); */
    /*  % Account for leap seconds GPS --> UT1 */
    /*  sec = sec + gps2ut1; */
    /*   */
    /*  if sec < 0 */
    /*      sec = mod(sec,60); */
    /*      min = min - 1; */
    /*  end */
    /*   */
    /*  if min < 0 */
    /*      min = mod(min,60); */
    /*      hour = hour - 1; */
    /*  end */
    /*   */
    /*  if hour < 0 */
    /*      hour = mod(hour,24); */
    /*      day = day - 1; */
    /*  end */
    /*   */
    /*  if day < 0 */
    /*      month = month - 1; */
    /*      if month < 0 */
    /*          month = 12; */
    /*          year = year - 1; */
    /*      end */
    /*  end */
    /* '<S125>:1:102' time_vec_ut1 = [year,month,day,hour,min,sec]; */
    rtb_time_vec_ut1[0] = T1900;
    rtb_time_vec_ut1[1] = i - 1.0;
    rtb_time_vec_ut1[2] = days;
    rtb_time_vec_ut1[3] = dayofyear;
    rtb_time_vec_ut1[4] = day_num;
    rtb_time_vec_ut1[5] = ((dayofyear - dayofyear) - day_num / 60.0) * 3600.0;
    days = (JD_days_gps - 2.451545E+6) / 36525.0;

    /* MATLAB Function: '<S21>/MATLAB Function' */
    /*  Constants */
    /* MATLAB Function 'sun_vector_lib/MATLAB Function': '<S118>:1' */
    /* '<S118>:1:4' AU2KM  = 149597870.700; */
    /* '<S118>:1:5' DEG2RAD     = pi/180.0; */
    /*  ----- */
    /*  Convert to Julian Centuries since J2000 */
    /*  JD_cent_ut1     = (JD_ut1 - 2451545)/36525; */
    /*  Mean longitude in a mean-equator of date frame */
    /* '<S118>:1:12' mean_long_sun  = rem(280.460 + 36000.771*JD_cent_ut1,360); */
    /*  [deg] */
    /*  Mean anomaly for the sun. Low precision formula since we're approximating */
    /*  T_TDB with T_UT1. */
    /* '<S118>:1:16' mean_anom_sun   = rem(357.5291092 + 35999.05034*JD_cent_ut1,360); */
    /* [deg] */
    /* '<S118>:1:17' mean_anom_sun_rad   = DEG2RAD*mean_anom_sun; */
    i = rt_remd_snf(35999.05034 * days + 357.5291092, 360.0) *
            0.017453292519943295;

    /*  Make sure we're \in [0,2pi] */
    /* '<S118>:1:20' if ( mean_anom_sun_rad < 0.0  ) */
    if (i < 0.0) {
        /* '<S118>:1:21' mean_anom_sun_rad = 2*pi + mean_anom_sun_rad; */
        i += 6.2831853071795862;
    }

    /*  Approximate the ecliptic longitude of the sun */
    /* '<S118>:1:25' long_eclp       = mean_long_sun + 1.914666471*sin(mean_anom_sun_rad) + ... */
    /* '<S118>:1:26'                     0.019994643*sin(2*mean_anom_sun_rad); */
    /* [deg] */
    /* '<S118>:1:27' long_eclp_rad   = rem(DEG2RAD*long_eclp,360); */
    T1900 = rt_remd_snf(((rt_remd_snf(36000.771 * days + 280.46, 360.0) +
            1.914666471 * sin(i)) + sin(2.0 * i) * 0.019994643) *
                        0.017453292519943295, 360.0);

    /*  [rad]             */
    /*  Assume the ecliptic latitude is zero */
    /* '<S118>:1:30' lat_eclp     = 0; */
    /* [deg] */
    /*  Approximate the obliquity of the ecliptic using */
    /* '<S118>:1:33' oblq        = 23.439291 - 0.0130042*JD_cent_ut1; */
    /* '<S118>:1:34' oblq_rad    = DEG2RAD*oblq; */
    oblq_rad = (23.439291 - 0.0130042 * days) * 0.017453292519943295;

    /*  Find the distance in AU from E to S */
    /* '<S118>:1:37' r           =  1.000140612 - 0.016708617*cos(mean_anom_sun_rad) - ... */
    /* '<S118>:1:38'                     0.000139589*cos(2*mean_anom_sun_rad); */
    dayofyear = (1.000140612 - 0.016708617 * cos(i)) - cos(2.0 * i) * 0.000139589;

    /* '<S118>:1:40' x_MOD_km    = r*cos(long_eclp_rad)*AU2KM; */
    /* '<S118>:1:41' y_MOD_km    = r*cos(oblq_rad)*sin(long_eclp_rad)*AU2KM; */
    /* '<S118>:1:42' z_MOD_km    = r*sin(oblq_rad)*sin(long_eclp_rad)*AU2KM; */
    /* '<S118>:1:44' sun_vector_mod_km = [x_MOD_km; y_MOD_km; z_MOD_km]; */
    /*  Convert to ECI (mean-equator-mean-equinox) frame  */
    /* '<S118>:1:48' sun_vector_eci_km   = mod2eci(sun_vector_mod_km,JD_cent_ut1); */
    /* '<S118>:1:54' [prec,psia,wa,ea,xa] = precess ( JD_cent_ut1 ); */
    /*  " to rad */
    /* '<S118>:1:64' convrt = pi / (180.0*3600.0); */
    /*  ------------------ iau 03 precession angles ------------------- */
    /* '<S118>:1:67' oblo =  84381.406; */
    /*  " */
    /* '<S118>:1:68' psia =  (((( -0.0000000951 * ttt + 0.000132851 ) * ttt - 0.00114045 ) * ttt - 1.0790069 ) * ttt + 5038.481507 ) * ttt; */
    /*  " */
    /* '<S118>:1:69' wa   =  ((((  0.0000003337 * ttt - 0.000000467 ) * ttt - 0.00772503 ) * ttt + 0.0512623 ) * ttt -    0.025754 ) * ttt + oblo; */
    /* '<S118>:1:70' ea   =  (((( -0.0000000434 * ttt - 0.000000576 ) * ttt + 0.00200340 ) * ttt - 0.0001831 ) * ttt -   46.836769 ) * ttt + oblo; */
    /* '<S118>:1:71' xa   =  (((( - 0.0000000560 * ttt + 0.000170663 ) * ttt - 0.00121197 ) * ttt - 2.3814292 ) * ttt +   10.556403 ) * ttt; */
    /* '<S118>:1:73' zeta =  (((( - 0.0000003173 * ttt - 0.000005971 ) * ttt + 0.01801828 ) * ttt + 0.2988499 ) * ttt + 2306.083227 ) * ttt + 2.650545; */
    /*  " */
    /* '<S118>:1:74' theta=  (((( - 0.0000001274 * ttt - 0.000007089 ) * ttt - 0.04182264 ) * ttt - 0.4294934 ) * ttt + 2004.191903 ) * ttt; */
    /* '<S118>:1:75' z    =  ((((   0.0000002904 * ttt - 0.000028596 ) * ttt + 0.01826837 ) * ttt + 1.0927348 ) * ttt + 2306.077181 ) * ttt - 2.650545; */
    /*  convert all units to rad */
    /* '<S118>:1:78' psia = psia  * convrt; */
    c_c = ((((-9.51E-8 * days + 0.000132851) * days - 0.00114045) * days -
            1.0790069) * days + 5038.481507) * days * 4.84813681109536E-6;

    /*  rad */
    /* '<S118>:1:79' wa   = wa    * convrt; */
    day_num = (((((3.337E-7 * days - 4.67E-7) * days - 0.00772503) * days +
            0.0512623) * days - 0.025754) * days + 84381.406) *
                    4.84813681109536E-6;

    /* '<S118>:1:80' ea   = ea    * convrt; */
    /* '<S118>:1:81' xa   = xa    * convrt; */
    i = ((((-5.6E-8 * days + 0.000170663) * days - 0.00121197) * days - 2.3814292)
            * days + 10.556403) * days * 4.84813681109536E-6;

    /* '<S118>:1:83' oblo = oblo * convrt; */
    /*  " to rad */
    /* '<S118>:1:84' a4  = rot3mat(-xa); */
    /* '<S118>:1:105' s   = sin( z_rad ); */
    s = sin(-i);

    /* '<S118>:1:106' c   = cos( z_rad ); */
    i = cos(-i);

    /* '<S118>:1:108' DCMz    = [ c   s   0; */
    /* '<S118>:1:109'            -s   c   0; */
    /* '<S118>:1:110'             0   0   1 ]; */
    /* '<S118>:1:85' a5  = rot1mat(wa); */
    /*  precess */
    /* '<S118>:1:94' s   = sin( x_rad ); */
    b_s = sin(day_num);

    /* '<S118>:1:95' c   = cos( x_rad ); */
    day_num = cos(day_num);

    /* '<S118>:1:97' DCMx    = [ 1   0   0 */
    /* '<S118>:1:98'             0   c   s; */
    /* '<S118>:1:99'             0  -s   c ]; */
    /* '<S118>:1:86' a6  = rot3mat(psia); */
    /* '<S118>:1:105' s   = sin( z_rad ); */
    c_s = sin(c_c);

    /* '<S118>:1:106' c   = cos( z_rad ); */
    c_c = cos(c_c);

    /* '<S118>:1:108' DCMz    = [ c   s   0; */
    /* '<S118>:1:109'            -s   c   0; */
    /* '<S118>:1:110'             0   0   1 ]; */
    /* '<S118>:1:87' a7  = rot1mat(-oblo); */
    /* '<S118>:1:88' prec = a7*a6*a5*a4; */
    /* '<S118>:1:56' vec_eci = prec*vec_mod; */
    c_c_0[0] = c_c;
    c_c_0[3] = c_s;
    c_c_0[6] = 0.0;
    c_c_0[1] = -c_s;
    c_c_0[4] = c_c;
    c_c_0[7] = 0.0;
    c_c_0[2] = 0.0;
    c_c_0[5] = 0.0;
    c_c_0[8] = 1.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += c_c_0[3 * qY] * a[s108_iter];
            a_2[s108_iter + 3 * qY] += c_c_0[3 * qY + 1] * a[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += c_c_0[3 * qY + 2] * a[s108_iter + 6];
        }

        rtb_itrf_to_gcrf[3 * s108_iter] = b_0[s108_iter];
    }

    rtb_itrf_to_gcrf[1] = 0.0;
    rtb_itrf_to_gcrf[4] = day_num;
    rtb_itrf_to_gcrf[7] = b_s;
    rtb_itrf_to_gcrf[2] = 0.0;
    rtb_itrf_to_gcrf[5] = -b_s;
    rtb_itrf_to_gcrf[8] = day_num;
    i_0[0] = i;
    i_0[3] = s;
    i_0[6] = 0.0;
    i_0[1] = -s;
    i_0[4] = i;
    i_0[7] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            c_c_0[s108_iter + 3 * qY] = 0.0;
            c_c_0[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY] * a_2[s108_iter];
            c_c_0[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY + 1] * a_2[s108_iter
                                                                            + 3];
            c_c_0[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY + 2] * a_2[s108_iter
                                                                            + 6];
        }

        i_0[2 + 3 * s108_iter] = d[s108_iter];
    }

    dayofyear_0[0] = dayofyear * cos(T1900) * 1.495978707E+8;
    dayofyear_0[1] = dayofyear * cos(oblq_rad) * sin(T1900) * 1.495978707E+8;
    dayofyear_0[2] = dayofyear * sin(oblq_rad) * sin(T1900) * 1.495978707E+8;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        rtb_sun_vector_eci_km[s108_iter] = 0.0;
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += i_0[3 * qY] * c_c_0[s108_iter];
            a_2[s108_iter + 3 * qY] += i_0[3 * qY + 1] * c_c_0[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += i_0[3 * qY + 2] * c_c_0[s108_iter + 6];
            rtb_sun_vector_eci_km[s108_iter] += a_2[3 * qY + s108_iter] *
                    dayofyear_0[qY];
        }
    }

    /* End of MATLAB Function: '<S21>/MATLAB Function' */

    /* MATLAB Function: '<S121>/Translate to TT' incorporates:
     *  Inport: '<Root>/gps_time'
     */
    /*  ------ Prelim Stuff ------ % */
    /*  Account for \Delta UT1 to get closest estimate to UT1 */
    /*    \Delta UT1 obtained from: */
    /*        http://maia.usno.navy.mil/ser7/finals.daily */
    /*    on Feb 24, 2017: 0.5236028 */
    /* MATLAB Function 'gps_to_tt/Translate to TT': '<S124>:1' */
    /*  Account for leap seconds GPS --> TT */
    /* '<S124>:1:11' gps2tt     = 32.184 + (19); */
    /*  sec --> JD conversion */
    /* '<S124>:1:14' sec2JD  = (0.0001/8.64); */
    /*  JD/sec */
    /* '<S124>:1:16' feb = 28; */
    /* '<S124>:1:17' cal_days = [31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
    /* '<S124>:1:19' sec_in = cur_gps(1); */
    /* '<S124>:1:19' week_in = cur_gps(2); */
    /*  Translate GPS w-s to JD and Gregorian ymd-hms */
    /* '<S124>:1:23' JD_jan61980 = 2.444244500000000e+06; */
    /* '<S124>:1:24' JD_J2000    = 2451545; */
    /* '<S124>:1:26' JD_days_gps     = JD_jan61980 + 7*week_in + sec_in/3600/24; */
    /* '<S124>:1:27' JD_days_TT     = JD_days_gps + sec2JD*gps2tt; */
    /* '<S124>:1:29' JD_TT_J2000_days       = JD_days_TT - JD_J2000; */
    /* '<S124>:1:30' JD_TT_J2000_century    = (JD_days_TT - 2451545)/36525; */
    /* '<S124>:1:32' T1900   = (JD_days_TT - 2415019.5)/365.25; */
    /* '<S124>:1:34' year    = 1900 + floor(T1900); */
    /* '<S124>:1:36' leapyrs     = floor((year - 1900 - 1)*0.25); */
    /* '<S124>:1:37' days    = (JD_days_TT - 2415019.5) - ((year - 1900)*365 + leapyrs ); */
    /* '<S124>:1:39' if days < 1 */
    /* '<S124>:1:45' if mod(year,4) == 0 */
    /* '<S124>:1:49' dayofyear   = floor(days); */
    /* '<S124>:1:51' day     = 0; */
    /* '<S124>:1:52' i       = 0; */
    /* '<S124>:1:54' while day < dayofyear */
    /* '<S124>:1:59' month   = i - 1; */
    /* '<S124>:1:61' day_num = 0; */
    /* '<S124>:1:62' for i = 1:month */
    /* '<S124>:1:66' day     = day - day_num; */
    /* '<S124>:1:68' tau     = 24*( day - dayofyear ); */
    /* '<S124>:1:70' hour    = floor( tau ); */
    /* '<S124>:1:71' min     = floor( 60*(tau - hour) ); */
    /* '<S124>:1:72' sec     = 3600*( tau - hour - (min/60) ); */
    /*  % Account for leap seconds GPS --> UT1 */
    /*  sec = sec + gps2ut1; */
    /*   */
    /*  if sec < 0 */
    /*      sec = mod(sec,60); */
    /*      min = min - 1; */
    /*  end */
    /*   */
    /*  if min < 0 */
    /*      min = mod(min,60); */
    /*      hour = hour - 1; */
    /*  end */
    /*   */
    /*  if hour < 0 */
    /*      hour = mod(hour,24); */
    /*      day = day - 1; */
    /*  end */
    /*   */
    /*  if day < 0 */
    /*      month = month - 1; */
    /*      if month < 0 */
    /*          month = 12; */
    /*          year = year - 1; */
    /*      end */
    /*  end */
    /* '<S124>:1:100' time_vec_TT = [year,month,day,hour,min,sec]; */
    c_c = ((((7.0 * rtU.gps_time[1] + 2.4442445E+6) + rtU.gps_time[0] / 3600.0 /
            24.0) + 0.00059240740740740738) - 2.451545E+6) / 36525.0;

    /* MATLAB Function: '<S120>/MATLAB Function' */
    /*  */
    /*  UW HuskySat-1, ADCS Subsystem */
    /*    Courtesy: Krish Kaycee 9.11.17 */
    /*  */
    /*  COORD_ROTATIONS excludes the polar motion transformation. Hence it */
    /*  includes the Precession, Nutation, and Sidereal Time rotation matrices. */
    /* MATLAB Function 'coord_rotations_lib/MATLAB Function': '<S123>:1' */
    /*  UPDATED: Excluding polar motion since it would need to be updated ~1 per */
    /*  week to keep accurate. For our mission duration and requirements, */
    /*  accounting for polar motion is not necessary. -T. Reynolds, 9.11.17 */
    /*  Constants */
    /*  Everything should be precomputed in radians */
    /* '<S123>:1:16' deg2rad = pi/180; */
    /* '<S123>:1:17' asec2rad = 1/3600*deg2rad; */
    /* '<S123>:1:18' o_prec = 7.292115146706979e-5; */
    /*  Evaluate Delaunay parameters */
    /* '<S123>:1:21' epsb_1980 = deg2rad * ( 23.439291 ... */
    /* '<S123>:1:22'                         - 0.0130042 * jd_tt_j2000_century ... */
    /* '<S123>:1:23'                         - 1.64e-7 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:24'                         + 5.04e-7 * jd_tt_j2000_century^3); */
    T1900 = (((23.439291 - 0.0130042 * c_c) - c_c * c_c * 1.64E-7) + 5.04E-7 *
            rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /*  Evaluate Delaunay parameters (correct equations from 4e errata) */
    /* '<S123>:1:27' M_moon	=   deg2rad * ( 134.96298139 ... */
    /* '<S123>:1:28'                         + (1325 * 360 + 198.8673981) * jd_tt_j2000_century ... */
    /* '<S123>:1:29'                       	+ 0.0086972 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:30'                        	+ 1.78e-5 * jd_tt_j2000_century^3); */
    day_num = (((477198.8673981 * c_c + 134.96298139) + c_c * c_c * 0.0086972) +
            1.78E-5 * rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /* '<S123>:1:31' M_sun	=   deg2rad * ( 357.52772333 ... */
    /* '<S123>:1:32'                         + (99 * 360 + 359.0503400) * jd_tt_j2000_century ... */
    /* '<S123>:1:33'                       	- 0.0001603 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:34'                      	- 3.3e-6 * jd_tt_j2000_century^3); */
    oblq_rad = (((35999.05034 * c_c + 357.52772333) - c_c * c_c * 0.0001603) -
            3.3E-6 * rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /* '<S123>:1:35' u_moon	=   deg2rad * ( 93.27191028 ... */
    /* '<S123>:1:36'                         + (1342 * 360 + 82.0175381) * jd_tt_j2000_century ... */
    /* '<S123>:1:37'                       	- 0.0036825 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:38'                         + 3.1e-6 * jd_tt_j2000_century^3); */
    s = (((483202.0175381 * c_c + 93.27191028) - c_c * c_c * 0.0036825) + 3.1E-6 *
            rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /* '<S123>:1:39' D_sun	=   deg2rad * ( 297.85036306 ... */
    /* '<S123>:1:40'                         + (1236 * 360 + 307.1114800) * jd_tt_j2000_century ... */
    /* '<S123>:1:41'                         - 0.0019142 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:42'                      	+ 5.3e-6 * jd_tt_j2000_century^3); */
    b_s = (((445267.11148 * c_c + 297.85036306) - c_c * c_c * 0.0019142) + 5.3E-6 *
            rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /* '<S123>:1:43' O_moon	=   deg2rad * ( 125.04452222 ... */
    /* '<S123>:1:44'                         - (5 * 360 + 134.1362608) * jd_tt_j2000_century ... */
    /* '<S123>:1:45'                     	+ 0.0020708 * jd_tt_j2000_century^2 ... */
    /* '<S123>:1:46'                         + 2.2e-6 * jd_tt_j2000_century^3); */
    i = (((125.04452222 - 1934.1362608 * c_c) + c_c * c_c * 0.0020708) + 2.2E-6 *
            rt_powd_snf(c_c, 3.0)) * 0.017453292519943295;

    /*  Largest Nutation coefficents (out of 106)                     */
    /* '<S123>:1:49' nc = [ ... */
    /* '<S123>:1:50' 1   0   0   0   0   1   -171996 -174.2  92025   8.9 */
    /* '<S123>:1:51' 9   0   0   2   -2  2   -13187  -1.6    5736    -3.1 */
    /* '<S123>:1:52' 31  0   0   2   0   2   -2274   -0.2    977     -0.5 */
    /* '<S123>:1:53' 2   0   0   0   0   2   2062    0.2     -895    0.5 */
    /* '<S123>:1:54' 10  0   1   0  	0   0   1426    -3.4    54      -0.1 */
    /* '<S123>:1:55' 32  1   0   0   0   0   712     0.1     -7      0.0 */
    /* '<S123>:1:56' 11  0   1   2   -2  2   -517    1.2     224     -0.6 */
    /* '<S123>:1:57' 33  0   0   2   0   1   -386    -0.4    200     0.0 */
    /* '<S123>:1:58' 34  1   0   2   0   2   -301    0.0     129     -0.1 */
    /* '<S123>:1:59' 12  0   -1  2   -2  2   217     -0.5    -95     0.3 */
    /* '<S123>:1:60' 35  1   0   0   -2  0   -158    0.0     -1      0.0 */
    /* '<S123>:1:61' 13  0   0   2   -2  1   129     0.1     -70     0.0  */
    /* '<S123>:1:62' 36  -1  0   2   0   2   123     0.0     -53     0.0  */
    /* '<S123>:1:63' 38  1   0   0   0   1   63      0.1     -33     0.0 */
    /* '<S123>:1:64' 37  0   0   0   2   0   63      0.0     -2      0.0 */
    /* '<S123>:1:65' 40  -1  0   2   2   2   -59     0.0     26      0.0 */
    /* '<S123>:1:66' 39  -1  0   0   0   1   -58     -0.1    32      0.0 */
    /* '<S123>:1:67' 41  1   0   2   0   1   -51     0.0     27      0.0 */
    /* '<S123>:1:68' 14  2   0   0   -2  0   48      0.0     1       0.0 */
    /* '<S123>:1:69' 3   -2  0   2   0   1   46      0.0     -24     0.0 */
    /* '<S123>:1:70' 42  0   0   2   2   2   -38     0.0     16      0.0 */
    /* '<S123>:1:71' 45  2   0   2   0   2   -31     0.0     13      0.0 */
    /* '<S123>:1:72' 43  2   0   0   0   0   29      0.0     -1      0.0 */
    /* '<S123>:1:73' 44  1   0   2   -2  2   29      0.0     -12     0.0 */
    /* '<S123>:1:74' 46  0   0   2   0   0   26      0.0     -1      0.0 */
    /* '<S123>:1:75' 15  0   0   2   -2  0   -22     0.0     0       0.0 */
    /* '<S123>:1:76' 47  -1  0   2   0   1   21      0.0     -10     0.0 */
    /* '<S123>:1:77' 16  0   2   0   0   0   17      -0.1    0       0.0 */
    /* '<S123>:1:78' 18  0   2   2   -2  2   -16     0.1     7       0.0 */
    /* '<S123>:1:79' 48  -1  0   0   2   1   16      0.0     -8      0.0 ... */
    /* '<S123>:1:80' ]; */
    /*  Map coefficients to radians (this should be embedded in the data) */
    /*  nc(:,end-3:end) = nc(:,end-3:end) * 0.0001 * asec2rad; */
    /*  Compute nutation in longitude (psi), and latitude (eps) */
    /* '<S123>:1:86' dpsi_1980 = 0; */
    dayofyear = 0.0;

    /* '<S123>:1:86' deps_1980 = 0; */
    c_s = 0.0;

    /* '<S123>:1:87' for i = 1:size(nc,1) */
    for (s108_iter = 0; s108_iter < 30; s108_iter++) {
        /* '<S123>:1:88' api         =   nc(i,2) * M_moon + nc(i,3) * M_sun + nc(i,4) * u_moon + nc(i,5) * D_sun + nc(i,6) * O_moon; */
        IL2 = (((nc[30 + s108_iter] * day_num + nc[60 + s108_iter] * oblq_rad) + nc
                [90 + s108_iter] * s) + nc[120 + s108_iter] * b_s) + nc[150 +
                                                                        s108_iter] * i;

        /* '<S123>:1:89' dpsi_1980   =   0.0001 * (nc(i,7) + nc(i,8) * jd_tt_j2000_century) * sin(api) + dpsi_1980; */
        dayofyear += (nc[210 + s108_iter] * c_c + nc[180 + s108_iter]) * 0.0001 *
                sin(IL2);

        /* '<S123>:1:90' deps_1980   =   0.0001 * (nc(i,9) + nc(i,10) * jd_tt_j2000_century) * cos(api) + deps_1980; */
        c_s += (nc[270 + s108_iter] * c_c + nc[240 + s108_iter]) * 0.0001 * cos(IL2);
    }

    /* '<S123>:1:93' eps_1980 = epsb_1980 + asec2rad * deps_1980; */
    oblq_rad = 4.84813681109536E-6 * c_s + T1900;

    /* '<S123>:1:94' tod_to_mod = rot1(-epsb_1980) * rot3(asec2rad * dpsi_1980) * rot1(eps_1980); */
    /* '<S123>:1:132' y = [1 0 0; 0 cos(u) sin(u); 0 -sin(u) cos(u)]; */
    day_num = 4.84813681109536E-6 * dayofyear;

    /* '<S123>:1:140' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
    /* '<S123>:1:132' y = [1 0 0; 0 cos(u) sin(u); 0 -sin(u) cos(u)]; */
    rtb_mod_to_gcrf[1] = 0.0;
    rtb_mod_to_gcrf[4] = cos(-T1900);
    rtb_mod_to_gcrf[7] = sin(-T1900);
    rtb_mod_to_gcrf[2] = 0.0;
    rtb_mod_to_gcrf[5] = -sin(-T1900);
    rtb_mod_to_gcrf[8] = cos(-T1900);
    tod_to_mod_0[0] = cos(day_num);
    tod_to_mod_0[3] = sin(day_num);
    tod_to_mod_0[6] = 0.0;
    tod_to_mod_0[1] = -sin(day_num);
    tod_to_mod_0[4] = cos(day_num);
    tod_to_mod_0[7] = 0.0;
    rtb_mod_to_gcrf[0] = 1.0;
    tod_to_mod_0[2] = 0.0;
    rtb_mod_to_gcrf[3] = 0.0;
    tod_to_mod_0[5] = 0.0;
    rtb_mod_to_gcrf[6] = 0.0;
    tod_to_mod_0[8] = 1.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY] *
                    rtb_mod_to_gcrf[s108_iter];
            a_2[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY + 1] *
                    rtb_mod_to_gcrf[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY + 2] *
                    rtb_mod_to_gcrf[s108_iter + 6];
        }

        rtb_itrf_to_gcrf[3 * s108_iter] = c[s108_iter];
    }

    rtb_itrf_to_gcrf[1] = 0.0;
    rtb_itrf_to_gcrf[4] = cos(oblq_rad);
    rtb_itrf_to_gcrf[7] = sin(oblq_rad);
    rtb_itrf_to_gcrf[2] = 0.0;
    rtb_itrf_to_gcrf[5] = -sin(oblq_rad);
    rtb_itrf_to_gcrf[8] = cos(oblq_rad);

    /*  Generate Precession matrix */
    /* '<S123>:1:97' zeta    =   asec2rad * (2306.2181 * jd_tt_j2000_century + 0.30188 * jd_tt_j2000_century^2 + 0.017998 * jd_tt_j2000_century^3); */
    day_num = ((c_c * c_c * 0.30188 + 2306.2181 * c_c) + 0.017998 * rt_powd_snf
            (c_c, 3.0)) * 4.84813681109536E-6;

    /* '<S123>:1:98' theta   =   asec2rad * (2004.3109 * jd_tt_j2000_century - 0.42665 * jd_tt_j2000_century^2 - 0.041833 * jd_tt_j2000_century^3); */
    /* '<S123>:1:99' z       =   asec2rad * (2306.2181 * jd_tt_j2000_century + 1.09468 * jd_tt_j2000_century^2 + 0.018203 * jd_tt_j2000_century^3); */
    oblq_rad = ((c_c * c_c * 1.09468 + 2306.2181 * c_c) + 0.018203 * rt_powd_snf
            (c_c, 3.0)) * 4.84813681109536E-6;

    /* '<S123>:1:101' mod_to_gcrf = rot3(zeta) * rot2(-theta) * rot3(z); */
    /* '<S123>:1:140' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
    c_c = -(((2004.3109 * c_c - c_c * c_c * 0.42665) - 0.041833 * rt_powd_snf(c_c,
                                                                              3.0)) * 4.84813681109536E-6);

    /* '<S123>:1:136' y = [cos(u) 0 -sin(u); 0 1 0; sin(u) 0 cos(u)]; */
    /* '<S123>:1:140' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
    tod_to_mod_0[0] = cos(day_num);
    tod_to_mod_0[3] = sin(day_num);
    tod_to_mod_0[6] = 0.0;
    tod_to_mod_0[1] = -sin(day_num);
    tod_to_mod_0[4] = cos(day_num);
    tod_to_mod_0[7] = 0.0;
    rtb_mod_to_gcrf[0] = cos(c_c);
    rtb_mod_to_gcrf[3] = 0.0;
    rtb_mod_to_gcrf[6] = -sin(c_c);
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            tod_to_mod[s108_iter + 3 * qY] = 0.0;
            tod_to_mod[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY] * a_2[s108_iter];
            tod_to_mod[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY + 1] *
                    a_2[s108_iter + 3];
            tod_to_mod[s108_iter + 3 * qY] += rtb_itrf_to_gcrf[3 * qY + 2] *
                    a_2[s108_iter + 6];
        }

        tod_to_mod_0[2 + 3 * s108_iter] = d_0[s108_iter];
        rtb_mod_to_gcrf[1 + 3 * s108_iter] = e[s108_iter];
    }

    rtb_mod_to_gcrf[2] = sin(c_c);
    rtb_mod_to_gcrf[5] = 0.0;
    rtb_mod_to_gcrf[8] = cos(c_c);
    rtb_itrf_to_gcrf[0] = cos(oblq_rad);
    rtb_itrf_to_gcrf[3] = sin(oblq_rad);
    rtb_itrf_to_gcrf[6] = 0.0;
    rtb_itrf_to_gcrf[1] = -sin(oblq_rad);
    rtb_itrf_to_gcrf[4] = cos(oblq_rad);
    rtb_itrf_to_gcrf[7] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += rtb_mod_to_gcrf[3 * qY] *
                    tod_to_mod_0[s108_iter];
            a_2[s108_iter + 3 * qY] += rtb_mod_to_gcrf[3 * qY + 1] *
                    tod_to_mod_0[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += rtb_mod_to_gcrf[3 * qY + 2] *
                    tod_to_mod_0[s108_iter + 6];
        }

        rtb_itrf_to_gcrf[2 + 3 * s108_iter] = d_0[s108_iter];
    }

    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            rtb_mod_to_gcrf[qY + 3 * s108_iter] = 0.0;
            rtb_mod_to_gcrf[qY + 3 * s108_iter] += rtb_itrf_to_gcrf[3 * s108_iter] *
                    a_2[qY];
            rtb_mod_to_gcrf[qY + 3 * s108_iter] += rtb_itrf_to_gcrf[3 * s108_iter + 1]
                                                                    * a_2[qY + 3];
            rtb_mod_to_gcrf[qY + 3 * s108_iter] += rtb_itrf_to_gcrf[3 * s108_iter + 2]
                                                                    * a_2[qY + 6];
        }
    }

    /*  Compute equation of Equinoxes */
    /* '<S123>:1:104' eqe_1980 = asec2rad * (dpsi_1980  * cos(epsb_1980) + 0.00264 * sin(O_moon) + 0.000063 * sin(2 * O_moon)); */
    T1900 = ((dayofyear * cos(T1900) + 0.00264 * sin(i)) + sin(2.0 * i) * 6.3E-5) *
            4.84813681109536E-6;

    /* '<S123>:1:105' teme_to_tod = rot3(-eqe_1980); */
    /* '<S123>:1:140' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
    /*  Compute sidereal rotation */
    /*  GMST_1982 = asec2rad * (67310.54841 ... */
    /*                          + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
    /*                          + 0.093104 * jd_ut1_j2000_century^2 ... */
    /*                          - 6.2e-6 * jd_ut1_j2000_century^3); */
    /* '<S123>:1:112' GMST_1982 = (67310.54841 ... */
    /* '<S123>:1:113'                         + (876600 * 3600 + 8640184.812866) * jd_ut1_j2000_century ... */
    /* '<S123>:1:114'                         + 0.093104 * jd_ut1_j2000_century^2 ... */
    /* '<S123>:1:115'                         - 6.2e-6 * jd_ut1_j2000_century^3); */
    days = ((3.1644001848128662E+9 * days + 67310.54841) + days * days * 0.093104)
            - 6.2E-6 * rt_powd_snf(days, 3.0);

    /* '<S123>:1:116' GMST_1982 = mod(GMST_1982,sign(GMST_1982)*86400) / 240; */
    if (days < 0.0) {
        dayofyear = -1.0;
    } else if (days > 0.0) {
        dayofyear = 1.0;
    } else if (days == 0.0) {
        dayofyear = 0.0;
    } else {
        dayofyear = days;
    }

    i = dayofyear * 86400.0;
    if (!(i == 0.0)) {
        if (i == i) {
            days -= floor(days / i) * i;
        } else {
            days /= i;
            if (fabs(days - rt_roundd_snf(days)) <= 2.2204460492503131E-16 * fabs(days))
            {
                days = 0.0;
            } else {
                days = (days - floor(days)) * i;
            }
        }
    }

    days /= 240.0;

    /* '<S123>:1:117' GMST_1982 = deg2rad * mod(GMST_1982, 360); */
    days = (days - floor(days / 360.0) * 360.0) * 0.017453292519943295;

    /* '<S123>:1:118' GAST_1982 = GMST_1982 + eqe_1980; */
    /*  Compute rotation from pseudo-Earth fixed frame (does not include polar */
    /*  motion) */
    /* '<S123>:1:122' pef_to_tod = rot3(-GAST_1982); */
    days = -(days + T1900);

    /* '<S123>:1:140' y = [cos(u) sin(u) 0; -sin(u) cos(u) 0; 0 0 1]; */
    /*  Compute composite rotations */
    /* '<S123>:1:125' teme_to_gcrf = mod_to_gcrf * tod_to_mod * teme_to_tod; */
    /* '<S123>:1:126' itrf_to_gcrf = mod_to_gcrf * tod_to_mod * pef_to_tod; */
    tod_to_mod_0[0] = cos(days);
    tod_to_mod_0[3] = sin(days);
    tod_to_mod_0[6] = 0.0;
    tod_to_mod_0[1] = -sin(days);
    tod_to_mod_0[4] = cos(days);
    tod_to_mod_0[7] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY] * rtb_mod_to_gcrf[s108_iter];
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY + 1] *
                    rtb_mod_to_gcrf[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY + 2] *
                    rtb_mod_to_gcrf[s108_iter + 6];
        }

        tod_to_mod_0[2 + 3 * s108_iter] = d_0[s108_iter];
    }

    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            rtb_itrf_to_gcrf[qY + 3 * s108_iter] = 0.0;
            rtb_itrf_to_gcrf[qY + 3 * s108_iter] += tod_to_mod_0[3 * s108_iter] *
                    a_2[qY];
            rtb_itrf_to_gcrf[qY + 3 * s108_iter] += tod_to_mod_0[3 * s108_iter + 1] *
                    a_2[qY + 3];
            rtb_itrf_to_gcrf[qY + 3 * s108_iter] += tod_to_mod_0[3 * s108_iter + 2] *
                    a_2[qY + 6];
        }
    }

    /* MATLAB Function: '<S20>/MATLAB Function' incorporates:
     *  Inport: '<Root>/orbit_tle'
     *  MATLAB Function: '<S122>/Translate to UT1'
     */
    /*  for vecef */
    /* '<S123>:1:127' ppef_to_vgcrf = itrf_to_gcrf * skew([0; 0; o_prec]); */
    /*  for w x recef */
    /*  ----------------------------------------------------------------------- % */
    /*  LOAD CONSTANTS */
    /*  ----------------------------------------------------------------------- % */
    /* MATLAB Function 'sgp4_lib_fsw/MATLAB Function': '<S117>:1' */
    /*  SGP4 Orbit Propagator */
    /*  UW Husky-Sat1 - ADCS Subsystem team */
    /*    Author:     Taylor Reynolds */
    /*    Updated:    5/9/17 */
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
    /*    - pos_frame: Position vector in the teme frame   */
    /*    - vel_frame: Velocity vector in the teme frame */
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
    /*    - a_E = the equatorial radius of Earth */
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
    /* '<S117>:1:75' k_2     = 5.413080e-4; */
    /*  CK2 = 0.5*J_2*a_E^2 */
    /* '<S117>:1:76' k_4     = 0.62098875e-6; */
    /*  CK4 = -3/8*J_4*a_E^4 */
    /* '<S117>:1:77' QOMS4   = 1.88027916e-9; */
    c_c = 1.88027916E-9;

    /*  QOMS2T = (q_0 - s)^4 * ER^4 */
    /* '<S117>:1:78' s       = 1.01222928; */
    i = 1.01222928;

    /*  S = s*ER */
    /* '<S117>:1:79' XJ3     = -0.2538815e-5; */
    /*  J_3 */
    /* '<S117>:1:80' XKE     = 0.743669161e-1; */
    /*  XKE = k_e*(ER/min)^{3/2} */
    /* '<S117>:1:81' XKMPER  = 6378.137; */
    /*  km/ER */
    /* '<S117>:1:82' XMNPDA  = 1440; */
    /*  time_units/day */
    /* '<S117>:1:83' a_E     = 1; */
    /*  distance units/ER */
    /* '<S117>:1:84' DE2RA   = 0.174532925e-1; */
    /*  radians/degree */
    /* '<S117>:1:85' TRUE    = 1; */
    /* '<S117>:1:86' FALSE   = 0; */
    /* '<S117>:1:87' r_E     = 6378.137; */
    /*  Earth's equatorial radius [km] */
    /* '<S117>:1:88' A_30    = -XJ3*a_E^3; */
    /*  see nomenclature */
    /* '<S117>:1:89' VKMPS   = XKMPER/60; */
    /*  Convert ER/min velocities to km/s */
    /*  ----------------------------------------------------------------------- % */
    /*  Line 1 */
    /* '<S117>:1:94' epoch_year  = orbit_tle(1); */
    /* '<S117>:1:95' epoch_JD    = orbit_tle(2); */
    /* '<S117>:1:96' BS          = orbit_tle(3); */
    /*  Line 2 */
    /* '<S117>:1:99' i_o     = orbit_tle(4)*DE2RA; */
    days = rtU.orbit_tle[3] * 0.0174532925;

    /*  Radians (INC) */
    /* '<S117>:1:100' Om_o    = orbit_tle(5)*DE2RA; */
    /*  Radians (RAAN) */
    /* '<S117>:1:101' e_o     = orbit_tle(6); */
    /*  Non-dimensional (ECC) */
    /* '<S117>:1:102' w_o     = orbit_tle(7)*DE2RA; */
    oblq_rad = rtU.orbit_tle[6] * 0.0174532925;

    /*  Radians (AOP) */
    /* '<S117>:1:103' M_o     = orbit_tle(8)*DE2RA; */
    day_num = rtU.orbit_tle[7] * 0.0174532925;

    /*  Radians (MNA) */
    /* '<S117>:1:104' n_o     = orbit_tle(9)*2*pi/XMNPDA; */
    c_s = rtU.orbit_tle[8] * 2.0 * 3.1415926535897931 / 1440.0;

    /*  Revs per time unit (MNM)    */
    /*  Time since epoch                                   */
    /* '<S117>:1:107' tsince_JD      = (JD_J2000_cur - (epoch_JD-2451545)); */
    JD_days_gps = (JD_days_gps - 2.451545E+6) - (rtU.orbit_tle[1] - 2.451545E+6);

    /* '<S117>:1:108' if( abs(tsince_JD) < 1e-8 || tsince_JD < 0) */
    if ((fabs(JD_days_gps) < 1.0E-8) || (JD_days_gps < 0.0)) {
        /* '<S117>:1:109' tsince_JD   = 0; */
        JD_days_gps = 0.0;
    }

    /* '<S117>:1:112' tsince  = XMNPDA*tsince_JD; */
    dayofyear = 1440.0 * JD_days_gps;

    /* '<S117>:1:114' a_1  = (XKE/n_o)^(2/3); */
    JD_days_gps = rt_powd_snf(0.0743669161 / c_s, 0.66666666666666663);

    /* '<S117>:1:115' d_1  = 1.5*(k_2/(a_1^2))*((3*cos(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
    s = cos(days);
    s = (s * s * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5] * rtU.orbit_tle[5],
                                          1.5) * (0.000541308 / (JD_days_gps * JD_days_gps) * 1.5);

    /* '<S117>:1:116' a_o  = a_1*(1 - (1/3)*d_1 - d_1^2 - (134/81)*d_1^3); */
    JD_days_gps *= ((1.0 - 0.33333333333333331 * s) - s * s) - 1.654320987654321 *
            rt_powd_snf(s, 3.0);

    /* '<S117>:1:117' d_o  = (3/2)*(k_2/(a_o^2))*((3*cos(i_o)^2 - 1)/(1 - e_o^2)^(3/2)); */
    b_s = cos(days);
    b_s = (b_s * b_s * 3.0 - 1.0) / rt_powd_snf(1.0 - rtU.orbit_tle[5] *
                                                rtU.orbit_tle[5], 1.5) * (0.000541308 / (JD_days_gps * JD_days_gps) * 1.5);

    /* '<S117>:1:118' n_o_pp   = n_o/(1 + d_o); */
    s = c_s / (1.0 + b_s);

    /* '<S117>:1:119' a_o_pp   = a_o/(1 - d_o); */
    c_s = JD_days_gps / (1.0 - b_s);

    /* '<S117>:1:121' r_p     = r_E*(a_o_pp*(1 - e_o) - a_E); */
    JD_days_gps = ((1.0 - rtU.orbit_tle[5]) * c_s - 1.0) * 6378.137;

    /*  Radius of perigee */
    /*  Check low altitude condition                                     */
    /* '<S117>:1:124' if( a_o_pp*(1-e_o)/a_E < (220/XKMPER + a_E) ) */
    if ((1.0 - rtU.orbit_tle[5]) * c_s < 1.0344928307435228) {
        /* '<S117>:1:125' lowAlt  = TRUE; */
        lowAlt = 1;
    } else {
        /* '<S117>:1:126' else */
        /* '<S117>:1:127' lowAlt  = FALSE; */
        lowAlt = 0;
    }

    /*  Check if perigee is between [98,156]km or below 98km and adjust the */
    /*  constant s accordingly. */
    /* '<S117>:1:133' if( (r_p > 98) && (r_p < 156) ) */
    if ((JD_days_gps > 98.0) && (JD_days_gps < 156.0)) {
        /* '<S117>:1:134' s_star  = a_o_pp*(1 - e_o) - s + a_E; */
        i = ((1.0 - rtU.orbit_tle[5]) * c_s - 1.01222928) + 1.0;

        /* '<S117>:1:135' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
        c_c = rt_powd_snf(1.0188142770258546 - i, 4.0);

        /* '<S117>:1:136' s       = s_star; */
    } else {
        if (JD_days_gps < 98.0) {
            /* '<S117>:1:137' elseif( r_p < 98 ) */
            /* '<S117>:1:138' s_star  = 20/XKMPER + a_E; */
            /* '<S117>:1:139' QOMS4   = (QOMS4^(0.25) + s - s_star)^4; */
            c_c = 6.0426196528035638E-8;

            /* '<S117>:1:140' s       = s_star; */
            i = 1.0031357118857749;
        }
    }

    /*  Constants */
    /* '<S117>:1:144' tht     = cos(i_o); */
    JD_days_gps = cos(days);

    /* '<S117>:1:145' xi      = 1/(a_o_pp - s); */
    xi = 1.0 / (c_s - i);

    /* '<S117>:1:146' b_o     = (1 - e_o^2)^(0.5); */
    b_o = sqrt(1.0 - rtU.orbit_tle[5] * rtU.orbit_tle[5]);

    /* '<S117>:1:147' eta     = a_o_pp*e_o*xi; */
    eta = c_s * rtU.orbit_tle[5] * xi;

    /* '<S117>:1:148' C_2     = QOMS4*xi^4*n_o_pp*(1-eta^2)^(-7/2)*((a_o_pp*(1 + ... */
    /* '<S117>:1:149'             (3/2)*eta^2 + 4*e_o*eta + e_o*eta^3)) + ... */
    /* '<S117>:1:150'             (3/2)*k_2*xi*(-0.5+1.5*tht^2)*(8+24*eta^2+3*eta^4)/(1-eta^2)); */
    /* '<S117>:1:151' C_1     = BS*C_2; */
    C_1 = ((((eta * eta * 1.5 + 1.0) + 4.0 * rtU.orbit_tle[5] * eta) +
            rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0)) * c_s + (JD_days_gps *
                    JD_days_gps * 1.5 + -0.5) * (0.00081196200000000006 * xi) * ((eta *
                            eta * 24.0 + 8.0) + 3.0 * rt_powd_snf(eta, 4.0)) / (1.0 - eta * eta))
            * (c_c * rt_powd_snf(xi, 4.0) * s * rt_powd_snf(1.0 - eta * eta, -3.5)) *
            rtU.orbit_tle[2];

    /* '<S117>:1:152' C_3     = (QOMS4*xi^5*A_30*n_o_pp*a_E*sin(i_o))/(k_2*e_o); */
    /* '<S117>:1:153' C_4     = 2*n_o_pp*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)*... */
    /* '<S117>:1:154'             ((2*eta*(1+e_o*eta) + 0.5*e_o + 0.5*eta^3) - (2*k_2*xi)/(a_o_pp*(1-eta^2))*... */
    /* '<S117>:1:155'             (3*(1-3*tht^2)*(1-(3/2)*eta^2-2*e_o*eta-0.5*e_o*eta^3)+(3/4)*(1-tht^2)*... */
    /* '<S117>:1:156'             (2*eta^2-e_o*eta-e_o*eta^3)*cos(2*w_o))); */
    C_4 = ((((rtU.orbit_tle[5] * eta + 1.0) * (2.0 * eta) + 0.5 * rtU.orbit_tle[5])
            + 0.5 * rt_powd_snf(eta, 3.0)) - ((((1.0 - eta * eta * 1.5) - 2.0 *
                    rtU.orbit_tle[5] * eta) - 0.5 * rtU.orbit_tle[5] * rt_powd_snf(eta,
                                                                                   3.0)) * ((1.0 - JD_days_gps * JD_days_gps * 3.0) * 3.0) + ((eta *
                                                                                           eta * 2.0 - rtU.orbit_tle[5] * eta) - rtU.orbit_tle[5] * rt_powd_snf
                                                                                           (eta, 3.0)) * ((1.0 - JD_days_gps * JD_days_gps) * 0.75) * cos(2.0 *
                                                                                                                                                          oblq_rad)) * (0.001082616 * xi / ((1.0 - eta * eta) * c_s))) * (2.0 *
                                                                                                                                                                  s * c_c * rt_powd_snf(xi, 4.0) * c_s * (b_o * b_o) * rt_powd_snf(1.0 - eta *
                                                                                                                                                                                                                                   eta, -3.5));

    /* '<S117>:1:157' C_5     = 0; */
    C_5 = 0.0;

    /* '<S117>:1:158' D_2     = 0; */
    D_2 = 0.0;

    /* '<S117>:1:159' D_3     = 0; */
    D_3 = 0.0;

    /* '<S117>:1:160' D_4     = 0; */
    D_4 = 0.0;

    /* '<S117>:1:161' IL2     = 1.5*C_1; */
    IL2 = 1.5 * C_1;

    /* '<S117>:1:162' IL3     = 0; */
    IL3 = 0.0;

    /* '<S117>:1:163' IL4     = 0; */
    IL4 = 0.0;

    /* '<S117>:1:164' IL5     = 0; */
    IL5 = 0.0;

    /* '<S117>:1:166' if( lowAlt  == FALSE ) */
    if (lowAlt == 0) {
        /* '<S117>:1:167' C_5     = 2*QOMS4*xi^4*a_o_pp*b_o^2*(1-eta^2)^(-7/2)*(1+(11/4)*eta*(eta+e_o) + e_o*eta^3); */
        C_5 = 2.0 * c_c * rt_powd_snf(xi, 4.0) * c_s * (b_o * b_o) * rt_powd_snf(1.0
                                                                                 - eta * eta, -3.5) * ((2.75 * eta * (eta + rtU.orbit_tle[5]) + 1.0) +
                                                                                         rtU.orbit_tle[5] * rt_powd_snf(eta, 3.0));

        /* '<S117>:1:168' D_2     = 4*a_o_pp*xi*C_1^2; */
        D_2 = 4.0 * c_s * xi * (C_1 * C_1);

        /* '<S117>:1:169' D_3     = (4/3)*a_o_pp*xi^2*(17*a_o_pp + s)*C_1^3; */
        D_3 = 1.3333333333333333 * c_s * (xi * xi) * (17.0 * c_s + i) * rt_powd_snf
                (C_1, 3.0);

        /* '<S117>:1:170' D_4     = (2/3)*a_o_pp*xi^3*(221*a_o_pp + 31*s)*C_1^4; */
        D_4 = 0.66666666666666663 * c_s * rt_powd_snf(xi, 3.0) * (221.0 * c_s + 31.0
                * i) * rt_powd_snf(C_1, 4.0);

        /* '<S117>:1:171' IL3     = D_2 + 2*C_1^2; */
        IL3 = C_1 * C_1 * 2.0 + D_2;

        /* '<S117>:1:172' IL4     = 0.25*(3*D_3 + 12*C_1*D_2 + 10*C_1^3); */
        IL4 = ((12.0 * C_1 * D_2 + 3.0 * D_3) + 10.0 * rt_powd_snf(C_1, 3.0)) * 0.25;

        /* '<S117>:1:173' IL5     = 0.2*(3*D_4 + 12*C_1*D_3 + 6*D_2^2 + 30*C_1^2*D_2 + 15*C_1^4); */
        IL5 = ((((12.0 * C_1 * D_3 + 3.0 * D_4) + D_2 * D_2 * 6.0) + C_1 * C_1 *
                30.0 * D_2) + 15.0 * rt_powd_snf(C_1, 4.0)) * 0.2;
    }

    /*  Secular effects of atm drag and gravitation */
    /* '<S117>:1:178' M_DF    = M_o + ( 1 + (3*k_2*(3*tht^2-1))/(2*a_o_pp^2*b_o^3) + ... */
    /* '<S117>:1:179'              (3*k_2^2*(13-78*tht^2+137*tht^4))/(16*a_o_pp^4*b_o^7) )*n_o_pp*tsince; */
    M_DF = (((JD_days_gps * JD_days_gps * 3.0 - 1.0) * 0.0016239240000000001 /
            (c_s * c_s * 2.0 * rt_powd_snf(b_o, 3.0)) + 1.0) + ((13.0 -
                    JD_days_gps * JD_days_gps * 78.0) + 137.0 * rt_powd_snf(JD_days_gps,
                                                                            4.0)) * 8.7904305259200008E-7 / (16.0 * rt_powd_snf(c_s, 4.0) *
                                                                                    rt_powd_snf(b_o, 7.0))) * s * dayofyear + day_num;

    /* '<S117>:1:180' w_DF    = w_o + ( (-3*k_2*(1-5*tht^2))/(2*a_o_pp^2*b_o^3) +... */
    /* '<S117>:1:181'                    (3*k_2^2*(7-114*tht^2+395*tht^4))/(16*a_o_pp^4*b_o^8) + ... */
    /* '<S117>:1:182'                     (5*k_4*(3-36*tht^2+49*tht^4))/(4*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
    b_s = ((((7.0 - JD_days_gps * JD_days_gps * 114.0) + 395.0 * rt_powd_snf
            (JD_days_gps, 4.0)) * 8.7904305259200008E-7 / (16.0 * rt_powd_snf(c_s,
                                                                              4.0) * rt_powd_snf(b_o, 8.0)) + (1.0 - JD_days_gps * JD_days_gps *
                                                                                      5.0) * -0.0016239240000000001 / (c_s * c_s * 2.0 * rt_powd_snf(b_o,
                                                                                                                                                     3.0))) + ((3.0 - JD_days_gps * JD_days_gps * 36.0) + 49.0 *
                                                                                                                                                             rt_powd_snf(JD_days_gps, 4.0)) * 3.1049437500000002E-6 /
                                                                                                                                                             (4.0 * rt_powd_snf(c_s, 4.0) * rt_powd_snf(b_o, 8.0))) * s * dayofyear
                                                                                                                                                                     + oblq_rad;

    /* '<S117>:1:183' Om_DF   = Om_o + ( (-3*k_2*tht)/(a_o_pp^2*b_o^4) +... */
    /* '<S117>:1:184'                     (3*k_2^2*(4*tht-19*tht^3))/(2*a_o_pp^4*b_o^8) +... */
    /* '<S117>:1:185'                      (5*k_4*tht*(3-7*tht^2))/(2*a_o_pp^4*b_o^8) )*n_o_pp*tsince; */
    /* '<S117>:1:187' Om      = Om_DF - (21/2)*( (n_o_pp*k_2*tht)/(a_o_pp^2*b_o^2) )*C_1*tsince^2; */
    i = ((((4.0 * JD_days_gps - 19.0 * rt_powd_snf(JD_days_gps, 3.0)) *
            8.7904305259200008E-7 / (2.0 * rt_powd_snf(c_s, 4.0) * rt_powd_snf(b_o,
                                                                               8.0)) + -0.0016239240000000001 * JD_days_gps / (c_s * c_s *
                                                                                       rt_powd_snf(b_o, 4.0))) + (3.0 - JD_days_gps * JD_days_gps * 7.0) *
            (3.1049437500000002E-6 * JD_days_gps) / (2.0 * rt_powd_snf(c_s, 4.0) *
                    rt_powd_snf(b_o, 8.0))) * s * dayofyear + rtU.orbit_tle[4] *
            0.0174532925) - s * 0.000541308 * JD_days_gps / (c_s * c_s * (b_o * b_o))
            * 10.5 * C_1 * (dayofyear * dayofyear);

    /* '<S117>:1:189' if( lowAlt == FALSE ) */
    if (lowAlt == 0) {
        /*  #5 %     */
        /* '<S117>:1:190' d_w     = BS*C_3*cos(w_o)*tsince; */
        oblq_rad = c_c * rt_powd_snf(xi, 5.0) * 2.538815E-6 * s * sin(days) /
                (0.000541308 * rtU.orbit_tle[5]) * rtU.orbit_tle[2] * cos(oblq_rad) *
                dayofyear;

        /* '<S117>:1:191' d_M     = -(2/3)*QOMS4*BS*xi^4*(a_E/(e_o*eta))*( (1+eta*cos(M_DF))^3 - (1 + eta*cos(M_o))^3 ); */
        c_c = -0.66666666666666663 * c_c * rtU.orbit_tle[2] * rt_powd_snf(xi, 4.0) *
                (1.0 / (rtU.orbit_tle[5] * eta)) * (rt_powd_snf(eta * cos(M_DF) + 1.0, 3.0)
                        - rt_powd_snf(eta * cos(day_num) + 1.0, 3.0));

        /* '<S117>:1:192' M_p     = M_DF + d_w + d_M; */
        M_DF = (M_DF + oblq_rad) + c_c;

        /* '<S117>:1:193' w       = w_DF - d_w - d_M; */
        b_s = (b_s - oblq_rad) - c_c;

        /* '<S117>:1:194' e       = e_o - BS*C_4*tsince - BS*C_5*( sin(M_p) - sin(M_o) ); */
        c_c = (rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * dayofyear) -
                rtU.orbit_tle[2] * C_5 * (sin(M_DF) - sin(day_num));

        /* '<S117>:1:195' a       = a_o_pp*( 1 - C_1*tsince - D_2*tsince^2 - D_3*tsince^3 - D_4*tsince^4 )^2; */
        day_num = (((1.0 - C_1 * dayofyear) - dayofyear * dayofyear * D_2) - D_3 *
                rt_powd_snf(dayofyear, 3.0)) - D_4 * rt_powd_snf(dayofyear, 4.0);
        day_num = day_num * day_num * c_s;

        /* '<S117>:1:196' L       = M_p + w + Om + n_o_pp*( IL2*tsince^2 + IL3*tsince^3 +... */
        /* '<S117>:1:197'             IL4*tsince^4 + IL5*tsince^5 ); */
        c_s = (((dayofyear * dayofyear * IL2 + IL3 * rt_powd_snf(dayofyear, 3.0)) +
                IL4 * rt_powd_snf(dayofyear, 4.0)) + IL5 * rt_powd_snf(dayofyear,
                                                                       5.0)) * s + ((M_DF + b_s) + i);
    } else {
        /* '<S117>:1:198' else */
        /* '<S117>:1:199' M_p     = M_DF; */
        /* '<S117>:1:200' w       = w_DF; */
        /* '<S117>:1:201' a       = a_o_pp*(1 - C_1*tsince)^2; */
        day_num = 1.0 - C_1 * dayofyear;
        day_num = day_num * day_num * c_s;

        /* '<S117>:1:202' e       = e_o - BS*C_4*tsince; */
        c_c = rtU.orbit_tle[5] - rtU.orbit_tle[2] * C_4 * dayofyear;

        /* '<S117>:1:203' L       = M_p + w + Om + n_o_pp*IL2*tsince^2; */
        c_s = ((M_DF + b_s) + i) + s * IL2 * (dayofyear * dayofyear);
    }

    /* '<S117>:1:206' b       = sqrt(1-e^2); */
    dayofyear = sqrt(1.0 - c_c * c_c);

    /* '<S117>:1:207' n       = XKE/(a^(3/2)); */
    /*  Add the long-period periodic terms */
    /* '<S117>:1:211' a_xN    = e*cos(w); */
    oblq_rad = c_c * cos(b_s);

    /* '<S117>:1:212' L_L     = (A_30*sin(i_o)/(8*k_2*a*b^2))*e*cos(w)*((3+5*tht)/(1+tht)); */
    /* '<S117>:1:213' a_yNL   = A_30*sin(i_o)/(4*k_2*a*b^2); */
    /* '<S117>:1:214' L_T     = L + L_L; */
    /* '<S117>:1:215' a_yN    = e*sin(w) + a_yNL; */
    s = 2.538815E-6 * sin(days) / (0.002165232 * day_num * (dayofyear * dayofyear))
            + c_c * sin(b_s);

    /* '<S117>:1:217' U       = mod(L_T - Om,2*pi); */
    dayofyear = ((2.538815E-6 * sin(days) / (0.004330464 * day_num * (dayofyear *
            dayofyear)) * c_c * cos(b_s) * ((5.0 * JD_days_gps + 3.0) / (1.0 +
                    JD_days_gps)) + c_s) - i) / 6.2831853071795862;
    if (fabs(dayofyear - rt_roundd_snf(dayofyear)) <= 2.2204460492503131E-16 *
            fabs(dayofyear)) {
        dayofyear = 0.0;
    } else {
        dayofyear = (dayofyear - floor(dayofyear)) * 6.2831853071795862;
    }

    /* '<S117>:1:218' Epw     = kepler(U,a_yN,a_xN); */
    /*  Solves Kepler's equation for Epw = E + w using iterative method */
    /* '<S117>:1:262' tol         = 1e-8; */
    /* '<S117>:1:263' iter_max    = 1000; */
    /* '<S117>:1:264' Epw         = U; */
    c_c = dayofyear;

    /* '<S117>:1:265' Epw_new     = 0; */
    b_s = 0.0;

    /* '<S117>:1:266' iter        = 0; */
    lowAlt = 0;

    /* '<S117>:1:268' while( (abs(Epw - Epw_new) > tol) && (iter < iter_max) ) */
    while ((fabs(c_c - b_s) > 1.0E-8) && (lowAlt < 1000)) {
        /* '<S117>:1:269' Epw = Epw_new; */
        c_c = b_s;

        /* '<S117>:1:270' dEpw   = (U-a_yN*cos(Epw) + a_xN*sin(Epw) - Epw)/(-a_yN*sin(Epw) - a_xN*cos(Epw) + 1); */
        /* '<S117>:1:271' Epw_new     = Epw + dEpw; */
        b_s += (((dayofyear - s * cos(b_s)) + oblq_rad * sin(b_s)) - b_s) / ((-s *
                sin(b_s) - oblq_rad * cos(b_s)) + 1.0);

        /* '<S117>:1:272' iter = iter +1; */
        lowAlt++;
    }

    /* '<S117>:1:275' Epw     = Epw_new; */
    /*  Preliminary quantities for short-period periodics */
    /* '<S117>:1:221' ecE     = a_xN*cos(Epw) + a_yN*sin(Epw); */
    /* '<S117>:1:222' esE     = a_xN*sin(Epw) - a_yN*cos(Epw); */
    IL2 = oblq_rad * sin(b_s) - s * cos(b_s);

    /* '<S117>:1:223' e_L     = (a_xN^2 + a_yN^2)^(1/2); */
    c_s = sqrt(oblq_rad * oblq_rad + s * s);

    /* '<S117>:1:224' p_L     = a*(1 - e_L^2); */
    c_c = (1.0 - c_s * c_s) * day_num;

    /* '<S117>:1:225' r       = a*(1 - ecE); */
    dayofyear = (1.0 - (oblq_rad * cos(b_s) + s * sin(b_s))) * day_num;

    /* '<S117>:1:226' r_dot   = XKE*sqrt(a)*esE/r; */
    /* '<S117>:1:227' r_f_dot     = XKE*sqrt(p_L)/r; */
    /* '<S117>:1:228' cosu    = (a/r)*( cos(Epw) - a_xN + ((a_yN*esE)/(1+sqrt(1 - e_L^2))) ); */
    /* '<S117>:1:229' sinu    = (a/r)*( sin(Epw) - a_yN - ((a_xN*esE)/(1 + sqrt(1 - e_L^2))) ); */
    /* '<S117>:1:230' u       = atan2(sinu,cosu); */
    day_num = rt_atan2d_snf(((sin(b_s) - s) - oblq_rad * IL2 / (sqrt(1.0 - c_s *
                                                                     c_s) + 1.0)) * (day_num / dayofyear), (s * IL2 / (sqrt(1.0 - c_s * c_s) +
                                                                             1.0) + (cos(b_s) - oblq_rad)) * (day_num / dayofyear));

    /* '<S117>:1:231' dr      = (k_2/(2*p_L))*(1 - tht^2)*cos(2*u); */
    /* '<S117>:1:232' du      = -(k_2/(4*p_L^2))*(7*tht^2 - 1)*sin(2*u); */
    /* '<S117>:1:233' dOm     = (3*k_2*tht/(2*p_L^2))*sin(2*u); */
    /* '<S117>:1:234' di      = (3*k_2*tht/(2*p_L^2))*sin(i_o)*cos(2*u); */
    /* '<S117>:1:235' dr_dot  = -((k_2*n)/p_L)*(1-tht^2)*sin(2*u); */
    /* '<S117>:1:236' dr_f_dot    = (k_2*n/p_L)*( (1-tht^2)*cos(2*u) - (3/2)*(1-3*tht^2) ); */
    /*  Add the short-period periodics to get osculating elements */
    /* '<S117>:1:239' r_k     = r*(1 - (3/2)*k_2*sqrt(1 - e_L^2)*(3*tht^2 - 1)/p_L^2) + dr; */
    dayofyear = (1.0 - sqrt(1.0 - c_s * c_s) * 0.00081196200000000006 *
            (JD_days_gps * JD_days_gps * 3.0 - 1.0) / (c_c * c_c)) *
                    dayofyear + 0.000541308 / (2.0 * c_c) * (1.0 - JD_days_gps * JD_days_gps) *
                    cos(2.0 * day_num);

    /* '<S117>:1:240' u_k     = u + du; */
    s = -(0.000541308 / (c_c * c_c * 4.0)) * (JD_days_gps * JD_days_gps * 7.0 -
            1.0) * sin(2.0 * day_num) + day_num;

    /* '<S117>:1:241' Om_k    = Om + dOm; */
    i += 0.0016239240000000001 * JD_days_gps / (c_c * c_c * 2.0) * sin(2.0 *
                                                                       day_num);

    /* '<S117>:1:242' i_k     = i_o + di; */
    days += 0.0016239240000000001 * JD_days_gps / (c_c * c_c * 2.0) * sin(days) *
            cos(2.0 * day_num);

    /* '<S117>:1:243' r_dot_k     = r_dot + dr_dot; */
    /* '<S117>:1:244' r_f_dot_k   = r_f_dot + dr_f_dot; */
    /*  Create unit orientation vectors */
    /* '<S117>:1:247' M       = [ -sin(Om_k)*cos(i_k); cos(Om_k)*cos(i_k); sin(i_k)]; */
    /* '<S117>:1:248' N       = [ cos(Om_k); sin(Om_k); 0 ]; */
    /* '<S117>:1:250' U       = M.*sin(u_k) + N.*cos(u_k); */
    oblq_rad = sin(s);
    JD_days_gps = cos(s);

    /* MATLAB Function: '<S120>/MATLAB Function' */
    /* '<S117>:1:251' V       = M.*cos(u_k) - N.*sin(u_k); */
    /*  Find position and velocity in km & km/s */
    /* '<S117>:1:254' pos_teme_km     = r_k.*U.*XKMPER; */
    /* '<S117>:1:255' vel_teme_kmps   = (r_dot_k.*U + r_f_dot_k.*V).*(VKMPS); */
    tod_to_mod_0[0] = cos(-T1900);
    tod_to_mod_0[3] = sin(-T1900);
    tod_to_mod_0[6] = 0.0;
    tod_to_mod_0[1] = -sin(-T1900);
    tod_to_mod_0[4] = cos(-T1900);
    tod_to_mod_0[7] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        for (qY = 0; qY < 3; qY++) {
            a_2[s108_iter + 3 * qY] = 0.0;
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY] * rtb_mod_to_gcrf[s108_iter];
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY + 1] *
                    rtb_mod_to_gcrf[s108_iter + 3];
            a_2[s108_iter + 3 * qY] += tod_to_mod[3 * qY + 2] *
                    rtb_mod_to_gcrf[s108_iter + 6];
        }

        tod_to_mod_0[2 + 3 * s108_iter] = d_0[s108_iter];
    }

    /* MATLAB Function: '<S20>/MATLAB Function' */
    rtb_Gain[0] = -sin(i) * cos(days);
    rtb_Gain[1] = cos(i) * cos(days);
    rtb_Gain[2] = sin(days);
    sc_pos_eci_km[0] = cos(i);
    sc_pos_eci_km[1] = sin(i);
    sc_pos_eci_km[2] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        /* MATLAB Function: '<S120>/MATLAB Function' incorporates:
         *  Product: '<S3>/Product1'
         */
        for (qY = 0; qY < 3; qY++) {
            rtb_mod_to_gcrf[s108_iter + 3 * qY] = 0.0;
            rtb_mod_to_gcrf[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY] *
                    a_2[s108_iter];
            rtb_mod_to_gcrf[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY + 1] *
                    a_2[s108_iter + 3];
            rtb_mod_to_gcrf[s108_iter + 3 * qY] += tod_to_mod_0[3 * qY + 2] *
                    a_2[s108_iter + 6];
        }

        /* MATLAB Function: '<S20>/MATLAB Function' incorporates:
         *  Product: '<S3>/Product1'
         */
        dayofyear_0[s108_iter] = (rtb_Gain[s108_iter] * oblq_rad +
                sc_pos_eci_km[s108_iter] * JD_days_gps) * dayofyear * 6378.137;
    }

    /*  Constants */
    /* MATLAB Function 'sun_vector_lib/MATLAB Function1': '<S119>:1' */
    /* '<S119>:1:4' M2KM        = 1e-3; */
    /* '<S119>:1:5' ECCE2       = 0.006694385000; */
    /* '<S119>:1:6' RE          = 6378.137; */
    /* '<S119>:1:7' TRUE        = 1; */
    /* '<S119>:1:8' FALSE       = 0; */
    /*  ----- */
    /*  Convert to sc position vector to km if necessary */
    /* '<S119>:1:12' sc_pos_eci_km   = sc_pos_eci_m*M2KM; */
    for (i_1 = 0; i_1 < 3; i_1++) {
        /* Gain: '<S3>/Gain' incorporates:
         *  Product: '<S3>/Product1'
         */
        dayofyear = 1000.0 * (rtb_mod_to_gcrf[i_1 + 6] * dayofyear_0[2] +
                (rtb_mod_to_gcrf[i_1 + 3] * dayofyear_0[1] +
                        rtb_mod_to_gcrf[i_1] * dayofyear_0[0]));

        /* MATLAB Function: '<S21>/MATLAB Function1' */
        sc_pos_eci_km[i_1] = dayofyear * 0.001;

        /* Gain: '<S3>/Gain' */
        rtb_Gain[i_1] = dayofyear;
    }

    /* MATLAB Function: '<S21>/MATLAB Function1' incorporates:
     *  Math: '<S3>/Math Function'
     *  Product: '<S3>/Product'
     */
    /* ----- */
    /*  Main Function */
    /* '<S119>:1:16' LOS     = FALSE; */
    lowAlt = 0;

    /* '<S119>:1:18' sun_vector_eci_km(3)    = sun_vector_eci_km(3)/(sqrt(1 - ECCE2)); */
    rtb_sun_vector_eci_km[2] /= 0.99664718682189635;

    /* '<S119>:1:19' sc_pos_eci_km(3)        = sc_pos_eci_km(3)/(sqrt(1 - ECCE2)); */
    sc_pos_eci_km[2] /= 0.99664718682189635;

    /* '<S119>:1:21' sun_vec     = sun_vector_eci_km; */
    /* '<S119>:1:22' sc_vec      = sc_pos_eci_km; */
    /* '<S119>:1:24' sc2sun_unit  = (sun_vec - sc_vec)/norm(sun_vec - sc_vec); */
    rtb_sun_vector_eci_km_0[0] = rtb_sun_vector_eci_km[0] - sc_pos_eci_km[0];
    rtb_sun_vector_eci_km_0[1] = rtb_sun_vector_eci_km[1] - sc_pos_eci_km[1];
    rtb_sun_vector_eci_km_0[2] = rtb_sun_vector_eci_km[2] - sc_pos_eci_km[2];
    T1900 = norm(rtb_sun_vector_eci_km_0);

    /*  Determine if the sc is in the light or shadow */
    /*    Call vector 1 the sun, vector 2 the sc */
    /* '<S119>:1:29' mag1    = norm(sun_vec); */
    JD_days_gps = norm(rtb_sun_vector_eci_km);

    /* '<S119>:1:30' mag2    = norm(sc_vec); */
    days = norm(sc_pos_eci_km);

    /* '<S119>:1:32' mag1_sq     = mag1*mag1; */
    JD_days_gps *= JD_days_gps;

    /* '<S119>:1:33' mag2_sq     = mag2*mag2; */
    /* '<S119>:1:35' sun_sc_dot  = dot(sun_vec,sc_vec); */
    i = (rtb_sun_vector_eci_km[0] * sc_pos_eci_km[0] + rtb_sun_vector_eci_km[1] *
            sc_pos_eci_km[1]) + rtb_sun_vector_eci_km[2] * sc_pos_eci_km[2];

    /*  Find t_min */
    /* '<S119>:1:38' t_min_num   = mag1_sq - sun_sc_dot; */
    /* '<S119>:1:39' t_min_den   = mag1_sq + mag2_sq - 2*sun_sc_dot; */
    days = (days * days + JD_days_gps) - 2.0 * i;

    /* '<S119>:1:41' if ( t_min_den < 0.0001  ) */
    if (days < 0.0001) {
        /* '<S119>:1:42' t_min = 0.0; */
        days = 0.0;
    } else {
        /* '<S119>:1:43' else */
        /* '<S119>:1:44' t_min = ( t_min_num ) / ( t_min_den ); */
        days = (JD_days_gps - i) / days;
    }

    /* '<S119>:1:47' c_t_min_sq  = ((1-t_min)*mag1_sq + sun_sc_dot*t_min)/RE^2; */
    /*  Check LoS */
    /* '<S119>:1:50' if( t_min < 0 || t_min > 0.999979 ) */
    if ((days < 0.0) || (days > 0.999979)) {
        /* '<S119>:1:51' LOS     = TRUE; */
        lowAlt = 1;
    }

    /* '<S119>:1:56' sc_in_sun   = LOS; */
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        rtb_sun_vector_eci_km[s108_iter] = (rtb_sun_vector_eci_km[s108_iter] -
                sc_pos_eci_km[s108_iter]) / T1900;
        sc_pos_eci_km[s108_iter] = rtb_itrf_to_gcrf[3 * s108_iter + 2] * rtb_Gain[2]
                                                                                  + (rtb_itrf_to_gcrf[3 * s108_iter + 1] * rtb_Gain[1] + rtb_itrf_to_gcrf[3 *
                                                                                                                                                          s108_iter] * rtb_Gain[0]);
    }

    /* Sqrt: '<S29>/sqrt' incorporates:
     *  Product: '<S29>/Product2'
     *  Product: '<S29>/Product3'
     *  Sum: '<S29>/Sum2'
     */
    dayofyear = sqrt(sc_pos_eci_km[0] * sc_pos_eci_km[0] + sc_pos_eci_km[1] *
                     sc_pos_eci_km[1]);

    /* Outputs for Iterator SubSystem: '<S23>/While Iterator Subsystem' incorporates:
     *  WhileIterator: '<S31>/While Iterator'
     */
    s31_iter = 1;
    do {
        T1900 = rtDW.Memory_PreviousInput;
        if (!(s31_iter > 0)) {
            T1900 = rt_atan2d_snf(sc_pos_eci_km[2], 0.99664718933525254 * dayofyear);
        }

        days = sin(T1900);
        JD_days_gps = cos(T1900);
        day_num = rt_atan2d_snf(42841.31151331366 * days * days * days +
                                sc_pos_eci_km[2], dayofyear - JD_days_gps * JD_days_gps * JD_days_gps *
                                0.00669437999014133 * 6.378137E+6);
        days = rt_atan2d_snf(0.99664718933525254 * sin(day_num), cos(day_num));
        rtDW.Memory_PreviousInput = days;
        s31_iter++;
    } while ((T1900 != days) && (s31_iter <= 5));

    /* End of Outputs for SubSystem: '<S23>/While Iterator Subsystem' */

    /* UnitConversion: '<S28>/Unit Conversion' incorporates:
     *  Trigonometry: '<S23>/Trigonometric Function2'
     */
    /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
    oblq_rad = 57.295779513082323 * day_num;
    b_s = 57.295779513082323 * rt_atan2d_snf(sc_pos_eci_km[1], sc_pos_eci_km[0]);

    /* UnitConversion: '<S47>/Unit Conversion' */
    /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
    JD_days_gps = 0.017453292519943295 * oblq_rad;

    /* Trigonometry: '<S24>/sincos' */
    c_c = cos(JD_days_gps);
    JD_days_gps = sin(JD_days_gps);
    s = c_c;

    /* UnitConversion: '<S47>/Unit Conversion' */
    b_o = JD_days_gps;
    JD_days_gps = 0.017453292519943295 * b_s;

    /* Trigonometry: '<S24>/sincos' */
    c_c = cos(JD_days_gps);
    JD_days_gps = sin(JD_days_gps);

    /* UnaryMinus: '<S38>/Unary Minus' incorporates:
     *  Product: '<S38>/u(1)*u(4)'
     *  Trigonometry: '<S24>/sincos'
     */
    rtb_VectorConcatenate[0] = -(b_o * c_c);

    /* UnaryMinus: '<S41>/Unary Minus' */
    rtb_VectorConcatenate[1] = -JD_days_gps;

    /* UnaryMinus: '<S44>/Unary Minus' incorporates:
     *  Product: '<S44>/u(3)*u(4)'
     *  Trigonometry: '<S24>/sincos'
     */
    rtb_VectorConcatenate[2] = -(s * c_c);

    /* UnaryMinus: '<S39>/Unary Minus' incorporates:
     *  Product: '<S39>/u(1)*u(2)'
     */
    rtb_VectorConcatenate[3] = -(b_o * JD_days_gps);

    /* SignalConversion: '<S48>/ConcatBufferAtVector ConcatenateIn5' incorporates:
     *  Trigonometry: '<S24>/sincos'
     */
    rtb_VectorConcatenate[4] = c_c;

    /* UnaryMinus: '<S45>/Unary Minus' incorporates:
     *  Product: '<S45>/u(2)*u(3)'
     */
    rtb_VectorConcatenate[5] = -(JD_days_gps * s);

    /* SignalConversion: '<S48>/ConcatBufferAtVector ConcatenateIn7' */
    rtb_VectorConcatenate[6] = s;

    /* SignalConversion: '<S48>/ConcatBufferAtVector ConcatenateIn8' incorporates:
     *  Constant: '<S43>/Constant'
     */
    rtb_VectorConcatenate[7] = 0.0;

    /* UnaryMinus: '<S46>/Unary Minus' */
    rtb_VectorConcatenate[8] = -b_o;

    /* MATLAB Function: '<S27>/MATLAB Function' */
    /*  UW HuskySat 1, ADCS Subsystem */
    /*    T. Reynolds, Updated: 10.28.17 */
    /* MATLAB Function 'YMDHMS_2_dec_year_lib/MATLAB Function': '<S116>:1' */
    /*  Converts a time vector into a decimal year number. See Vallado pp 200-201 */
    /*  for reference algorithms. Uses 6 inputs to avoid 1 based indexing. */
    /*  find day of year - valid until 2100 */
    /* '<S116>:1:10' Lmonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]; */
    for (s108_iter = 0; s108_iter < 12; s108_iter++) {
        Lmonth[s108_iter] = b_1[s108_iter];
    }

    /* '<S116>:1:11' days_in_year    = 365.25; */
    T1900 = 365.25;

    /* '<S116>:1:13' if mod(year,4) == 0 */
    if (rtb_time_vec_ut1[0] - floor(rtb_time_vec_ut1[0] / 4.0) * 4.0 == 0.0) {
        /* '<S116>:1:14' Lmonth(2) = 29; */
        Lmonth[1] = 29;

        /* '<S116>:1:15' days_in_year    = 366; */
        T1900 = 366.0;
    }

    /* '<S116>:1:18' day_of_year = 0; */
    days = 0.0;

    /* '<S116>:1:19' for i = 1:month-1 */
    for (s108_iter = 0; s108_iter < (int16_T)(rtb_time_vec_ut1[1] - 1.0);
            s108_iter++) {
        /* '<S116>:1:20' day_of_year = day_of_year + Lmonth(i); */
        days += (real_T)Lmonth[s108_iter];
    }

    /*  Add number of days into the month */
    /* '<S116>:1:24' day_of_year = day_of_year + day; */
    days += rtb_time_vec_ut1[2];

    /*  Add HMS info to the day of year */
    /* '<S116>:1:27' day_of_year    = day_of_year + hour/24 + min/1440 + sec/86400; */
    days = ((rtb_time_vec_ut1[3] / 24.0 + days) + rtb_time_vec_ut1[4] / 1440.0) +
            rtb_time_vec_ut1[5] / 86400.0;

    /*  Find the year fraction represented by day_of_year */
    /* '<S116>:1:30' day_frac    = day_of_year/days_in_year; */
    /*  Sum to get total decimal year */
    /* '<S116>:1:33' dec_year = year + day_frac; */
    T1900 = days / T1900 + rtb_time_vec_ut1[0];

    /* End of MATLAB Function: '<S27>/MATLAB Function' */

    /* RelationalOperator: '<S65>/Relational Operator' incorporates:
     *  Memory: '<S65>/otime'
     */
    rtb_RelationalOperator = (T1900 != rtDW.otime_PreviousInput);

    /* Saturate: '<S26>/+//- 180 deg' */
    if (b_s > 180.0) {
        days = 180.0;
    } else if (b_s < -180.0) {
        days = -180.0;
    } else {
        days = b_s;
    }

    /* End of Saturate: '<S26>/+//- 180 deg' */

    /* Saturate: '<S26>/+//- 90 deg' */
    if (oblq_rad > 90.0) {
        JD_days_gps = 90.0;
    } else if (oblq_rad < -90.0) {
        JD_days_gps = -90.0;
    } else {
        JD_days_gps = oblq_rad;
    }

    /* End of Saturate: '<S26>/+//- 90 deg' */

    /* UnitConversion: '<S109>/Unit Conversion' */
    /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
    s = 0.017453292519943295 * days;
    c_c = 0.017453292519943295 * JD_days_gps;

    /* Trigonometry: '<S62>/sincos' */
    c_s = sin(s);
    xi = cos(s);
    i = sin(c_c);
    b_o = cos(c_c);

    /* Outputs for Enabled SubSystem: '<S57>/Convert from geodetic to  spherical coordinates ' incorporates:
     *  EnablePort: '<S61>/Enable'
     */
    /* RelationalOperator: '<S64>/Relational Operator' incorporates:
     *  Memory: '<S64>/olon'
     */
    if (days != rtDW.olon_PreviousInput) {
        /* Outputs for Iterator SubSystem: '<S61>/For Iterator Subsystem' incorporates:
         *  ForIterator: '<S108>/For Iterator'
         */
        for (s108_iter = 1; s108_iter < 12; s108_iter++) {
            /* Switch: '<S108>/cp[m-1] sp[m-1]' incorporates:
             *  Inport: '<S61>/cp[2]'
             *  Inport: '<S61>/sp[2]'
             *  UnitDelay: '<S108>/Unit Delay1'
             */
            if (s108_iter > 1) {
                oblq_rad = rtDW.UnitDelay1_DSTATE_l[0];
                b_s = rtDW.UnitDelay1_DSTATE_l[1];
            } else {
                oblq_rad = xi;
                b_s = c_s;
            }

            /* End of Switch: '<S108>/cp[m-1] sp[m-1]' */

            /* Sum: '<S108>/Sum2' incorporates:
             *  Inport: '<S61>/cp[2]'
             *  Inport: '<S61>/sp[2]'
             *  Product: '<S108>/Product1'
             *  Product: '<S108>/Product2'
             */
            s = oblq_rad * c_s + b_s * xi;

            /* Assignment: '<S108>/Assignment' incorporates:
             *  Assignment: '<S108>/Assignment1'
             *  Constant: '<S108>/Constant'
             *  Constant: '<S108>/Constant1'
             */
            if (s108_iter == 1) {
                memcpy(&rtb_Assignment[0], &rtConstP.pooled11[0], 11U * sizeof(real_T));
                memcpy(&rtb_Assignment1[0], &rtConstP.pooled11[0], 11U * sizeof(real_T));
            }

            rtb_Assignment[s108_iter - 1] = s;

            /* End of Assignment: '<S108>/Assignment' */

            /* Sum: '<S108>/Sum1' incorporates:
             *  Inport: '<S61>/cp[2]'
             *  Inport: '<S61>/sp[2]'
             *  Product: '<S108>/Product3'
             *  Product: '<S108>/Product8'
             */
            b_s = oblq_rad * xi - b_s * c_s;

            /* Assignment: '<S108>/Assignment1' */
            rtb_Assignment1[s108_iter - 1] = b_s;

            /* Update for UnitDelay: '<S108>/Unit Delay1' */
            rtDW.UnitDelay1_DSTATE_l[0] = b_s;
            rtDW.UnitDelay1_DSTATE_l[1] = s;
        }

        /* End of Outputs for SubSystem: '<S61>/For Iterator Subsystem' */

        /* SignalConversion: '<S61>/OutportBufferForcp[13]' incorporates:
         *  Constant: '<S61>/cp[1]'
         *  Inport: '<S61>/cp[2]'
         */
        rtDW.OutportBufferForcp13[0] = 1.0;
        rtDW.OutportBufferForcp13[1] = xi;

        /* SignalConversion: '<S61>/OutportBufferForsp[13]' incorporates:
         *  Constant: '<S61>/sp[1]'
         *  Inport: '<S61>/sp[2]'
         */
        rtDW.OutportBufferForsp13[0] = 0.0;
        rtDW.OutportBufferForsp13[1] = c_s;

        /* SignalConversion: '<S61>/OutportBufferForcp[13]' */
        memcpy(&rtDW.OutportBufferForcp13[2], &rtb_Assignment1[0], 11U * sizeof
               (real_T));

        /* SignalConversion: '<S61>/OutportBufferForsp[13]' */
        memcpy(&rtDW.OutportBufferForsp13[2], &rtb_Assignment[0], 11U * sizeof
               (real_T));
    }

    /* End of RelationalOperator: '<S64>/Relational Operator' */
    /* End of Outputs for SubSystem: '<S57>/Convert from geodetic to  spherical coordinates ' */

    /* Trigonometry: '<S30>/sincos' */
    c_c = sin(day_num);

    /* Product: '<S30>/Product2' */
    oblq_rad = c_c * 0.00669437999014133;

    /* Product: '<S30>/Product3' incorporates:
     *  Constant: '<S30>/Constant1'
     *  Constant: '<S30>/f'
     *  Product: '<S30>/Product1'
     *  Sqrt: '<S30>/sqrt'
     *  Sum: '<S30>/Sum'
     */
    xi = 6.378137E+6 / sqrt(1.0 - oblq_rad * c_c);

    /* Sum: '<S30>/Sum3' incorporates:
     *  Product: '<S30>/Product4'
     *  Product: '<S30>/Product5'
     *  Product: '<S30>/Product6'
     *  Sum: '<S30>/Sum2'
     *  Trigonometry: '<S30>/sincos'
     */
    xi = ((oblq_rad * xi + sc_pos_eci_km[2]) * c_c - xi) + cos(day_num) *
            dayofyear;

    /* Saturate: '<S26>/0 to 1,000,000 m' */
    if (xi > 1.0E+6) {
        xi = 1.0E+6;
    } else {
        if (xi < 0.0) {
            xi = 0.0;
        }
    }

    /* End of Saturate: '<S26>/0 to 1,000,000 m' */

    /* Gain: '<S26>/Gain' */
    c_s = 0.001 * xi;

    /* Logic: '<S63>/Logical Operator' incorporates:
     *  Memory: '<S63>/oalt'
     *  Memory: '<S63>/olat'
     *  RelationalOperator: '<S63>/Relational Operator'
     *  RelationalOperator: '<S63>/Relational Operator1'
     */
    rtb_LogicalOperator_h = ((JD_days_gps != rtDW.olat_PreviousInput) || (c_s !=
            rtDW.oalt_PreviousInput));

    /* Product: '<S62>/Product' */
    xi = i * i;

    /* Product: '<S62>/Product1' */
    dayofyear = b_o * b_o;

    /* Outputs for Enabled SubSystem: '<S57>/Convert from geodetic to  spherical coordinates' incorporates:
     *  EnablePort: '<S60>/Enable'
     */
    if (rtb_LogicalOperator_h) {
        /* Sqrt: '<S103>/sqrt' incorporates:
         *  Product: '<S103>/Product'
         *  Sum: '<S103>/Sum'
         */
        day_num = sqrt(4.0680631590768993E+7 - xi * 272331.60668193549);

        /* Product: '<S60>/Product1' */
        c_c = day_num * c_s;

        /* Sqrt: '<S102>/sqrt' incorporates:
         *  Product: '<S102>/Product10'
         *  Product: '<S102>/Product9'
         *  Sum: '<S102>/Sum7'
         */
        oblq_rad = sqrt(dayofyear * 4.0680631590768993E+7 + 4.0408299984087057E+7 *
                        xi);

        /* Sqrt: '<S60>/sqrt' incorporates:
         *  Gain: '<S105>/Gain'
         *  Product: '<S105>/Product1'
         *  Product: '<S105>/Product6'
         *  Product: '<S105>/Product7'
         *  Product: '<S105>/Product8'
         *  Sum: '<S105>/Sum5'
         *  Sum: '<S105>/Sum6'
         */
        rtDW.sqrt_a = sqrt((1.6549137866238722E+15 - 2.208307901990225E+13 * xi) /
                           (day_num * day_num) + (2.0 * c_c + c_s * c_s));

        /* Product: '<S100>/Product11' incorporates:
         *  Sum: '<S100>/Sum8'
         */
        rtDW.Product11 = (c_s + oblq_rad) / rtDW.sqrt_a;

        /* Sum: '<S104>/Sum2' */
        day_num = 4.0680631590768993E+7 + c_c;

        /* Sum: '<S104>/Sum1' */
        c_c += 4.0408299984087057E+7;

        /* Product: '<S101>/Product4' incorporates:
         *  Product: '<S101>/Product3'
         *  Product: '<S104>/Product1'
         *  Product: '<S104>/Product2'
         *  Sqrt: '<S101>/sqrt'
         *  Sum: '<S101>/Sum3'
         */
        rtDW.Product4 = i / sqrt(day_num * day_num / (c_c * c_c) * dayofyear + xi);

        /* Product: '<S106>/Product12' incorporates:
         *  Product: '<S106>/Product1'
         */
        rtDW.Product12 = 272331.60668193549 / (rtDW.sqrt_a * oblq_rad) * b_o * i;

        /* Sqrt: '<S107>/sqrt' incorporates:
         *  Constant: '<S107>/Constant'
         *  Product: '<S107>/Product5'
         *  Sum: '<S107>/Sum4'
         */
        rtDW.sqrt_n = sqrt(1.0 - rtDW.Product4 * rtDW.Product4);
    }

    /* End of Outputs for SubSystem: '<S57>/Convert from geodetic to  spherical coordinates' */

    /* Product: '<S57>/aor' incorporates:
     *  Constant: '<S57>/re'
     */
    xi = 6371.2 / rtDW.sqrt_a;

    /* Product: '<S57>/ar' */
    dayofyear = xi * xi;

    /* Outputs for Iterator SubSystem: '<S57>/Compute magnetic vector in spherical coordinates' incorporates:
     *  ForIterator: '<S59>/For Iterator'
     */
    /* InitializeConditions for UnitDelay: '<S59>/Unit Delay' */
    day_num = 0.0;

    /* InitializeConditions for UnitDelay: '<S59>/Unit Delay2' */
    UnitDelay2_DSTATE_n[0] = 0.0;
    UnitDelay2_DSTATE_n[1] = 0.0;
    UnitDelay2_DSTATE_n[2] = 0.0;
    UnitDelay2_DSTATE_n[3] = 0.0;
    for (s108_iter = 1; s108_iter < 13; s108_iter++) {
        /* Switch: '<S59>/ar(n)' */
        if (!(s108_iter > 1)) {
            day_num = dayofyear;
        }

        /* End of Switch: '<S59>/ar(n)' */

        /* Product: '<S59>/Product8' */
        day_num *= xi;

        /* Sum: '<S59>/Sum' */
        qY = s108_iter + 1;

        /* Outputs for Iterator SubSystem: '<S59>/For Iterator Subsystem' incorporates:
         *  ForIterator: '<S67>/For Iterator'
         */
        if (rtDW.ForIterator_IterationMarker[4] != 0) {
            /* InitializeConditions for UnitDelay: '<S68>/Unit Delay1' */
            rtDW.UnitDelay1_DSTATE_k = 0.0;

            /* InitializeConditions for UnitDelay: '<S68>/Unit Delay3' */
            rtDW.UnitDelay3_DSTATE = 0.0;

            /* InitializeConditions for UnitDelay: '<S68>/Unit Delay2' */
            rtDW.UnitDelay2_DSTATE_h = 0.0;

            /* InitializeConditions for UnitDelay: '<S68>/Unit Delay4' */
            rtDW.UnitDelay4_DSTATE = 0.0;
        }

        for (i_1 = 0; i_1 < 7; i_1++) {
            rtDW.ForIterator_IterationMarker[i_1] = 1U;
        }

        /* Sum: '<S59>/Sum' incorporates:
         *  Constant: '<S74>/Constant'
         *  Constant: '<S74>/Constant1'
         *  Logic: '<S74>/Logical Operator'
         *  RelationalOperator: '<S74>/Relational Operator'
         *  RelationalOperator: '<S74>/Relational Operator1'
         *  Sum: '<S67>/Sum1'
         */
        for (s67_iter = 1; s67_iter <= qY; s67_iter++) {
            /* Sum: '<S67>/Sum1' */
            qY_2 = s67_iter - 1;

            /* Outputs for Enabled SubSystem: '<S67>/Time adjust the gauss coefficients' incorporates:
             *  EnablePort: '<S70>/Enable'
             */
            if (rtb_RelationalOperator) {
                /* Outputs for Atomic SubSystem: '<S70>/If Action Subsystem' */
                /* Sum: '<S96>/Sum1' incorporates:
                 *  Sum: '<S67>/Sum1'
                 */
                i_1 = qY_2 + 1;

                /* Sum: '<S96>/Sum2' */
                qY_1 = s108_iter + 1;

                /* End of Outputs for SubSystem: '<S70>/If Action Subsystem' */

                /* Assignment: '<S70>/Assignment' incorporates:
                 *  Constant: '<S57>/epoch'
                 *  Constant: '<S70>/c[maxdef][maxdef]'
                 *  Constant: '<S70>/cd[maxdef][maxdef]'
                 *  Product: '<S96>/Product'
                 *  Selector: '<S96>/c[m][n]'
                 *  Selector: '<S96>/cd[m][n]'
                 *  Sum: '<S57>/Sum'
                 *  Sum: '<S96>/Sum'
                 *  Sum: '<S96>/Sum1'
                 *  Sum: '<S96>/Sum2'
                 *  UnitDelay: '<S70>/Unit Delay'
                 */
                if (rtDW.ForIterator_IterationMarker[5] < 2) {
                    rtDW.ForIterator_IterationMarker[5L] = 2U;
                    memcpy(&Assignment[0], &rtDW.UnitDelay_DSTATE_b[0], 169U * sizeof
                           (real_T));
                }

                /* Outputs for Atomic SubSystem: '<S70>/If Action Subsystem' */
                Assignment[(i_1 + 13 * (qY_1 - 1)) - 1] = rtConstP.cdmaxdefmaxdef_Value
                        [((qY_1 - 1) * 13 + i_1) - 1] * (T1900 - 2015.0) +
                        rtConstP.cmaxdefmaxdef_Value[((qY_1 - 1) * 13 + i_1) - 1];

                /* End of Assignment: '<S70>/Assignment' */
                /* End of Outputs for SubSystem: '<S70>/If Action Subsystem' */

                /* Switch: '<S97>/tc_old' incorporates:
                 *  UnitDelay: '<S97>/Unit Delay'
                 */
                for (i_1 = 0; i_1 < 169; i_1++) {
                    if (s108_iter > 1) {
                        rtb_tc_old[i_1] = rtDW.UnitDelay_DSTATE_g[i_1];
                    } else {
                        rtb_tc_old[i_1] = 0.0;
                    }
                }

                /* End of Switch: '<S97>/tc_old' */

                /* If: '<S97>/If' incorporates:
                 *  Sum: '<S67>/Sum1'
                 */
                if (qY_2 != 0) {
                    /* Outputs for IfAction SubSystem: '<S97>/If Action Subsystem1' incorporates:
                     *  ActionPort: '<S98>/Action Port'
                     */
                    /* Sum: '<S98>/Sum2' */
                    i_1 = s108_iter + 1;

                    /* Assignment: '<S98>/Assignment2' incorporates:
                     *  Constant: '<S57>/epoch'
                     *  Constant: '<S70>/c[maxdef][maxdef]'
                     *  Constant: '<S70>/cd[maxdef][maxdef]'
                     *  Product: '<S98>/Product'
                     *  Selector: '<S98>/c[m][n]'
                     *  Selector: '<S98>/cd[m][n]'
                     *  Sum: '<S57>/Sum'
                     *  Sum: '<S98>/Sum'
                     *  Sum: '<S98>/Sum2'
                     */
                    if (rtDW.ForIterator_IterationMarker[6] < 2) {
                        rtDW.ForIterator_IterationMarker[6L] = 2U;
                        memcpy(&Assignment2[0], &rtb_tc_old[0], 169U * sizeof(real_T));
                    }

                    Assignment2[(i_1 + 13 * (qY_2 - 1)) - 1] =
                            rtConstP.cdmaxdefmaxdef_Value[((qY_2 - 1) * 13 + i_1) - 1] * (T1900
                                    - 2015.0) + rtConstP.cmaxdefmaxdef_Value[((qY_2 - 1) * 13 + i_1) - 1];

                    /* End of Assignment: '<S98>/Assignment2' */

                    /* Gain: '<S98>/Gain' */
                    memcpy(&rtb_tc_old[0], &Assignment2[0], 169U * sizeof(real_T));

                    /* End of Outputs for SubSystem: '<S97>/If Action Subsystem1' */
                }

                /* End of If: '<S97>/If' */
                for (i_1 = 0; i_1 < 169; i_1++) {
                    /* Sum: '<S70>/Sum2' */
                    rtDW.Sum2_m[i_1] = Assignment[i_1] + rtb_tc_old[i_1];

                    /* Update for UnitDelay: '<S70>/Unit Delay' */
                    rtDW.UnitDelay_DSTATE_b[i_1] = Assignment[i_1];

                    /* Update for UnitDelay: '<S97>/Unit Delay' */
                    rtDW.UnitDelay_DSTATE_g[i_1] = rtb_tc_old[i_1];
                }
            }

            /* End of Outputs for SubSystem: '<S67>/Time adjust the gauss coefficients' */

            /* If: '<S73>/If' incorporates:
             *  Sum: '<S67>/Sum1'
             */
            if (qY_2 == 0) {
                /* Outputs for IfAction SubSystem: '<S73>/If Action Subsystem' incorporates:
                 *  ActionPort: '<S79>/Action Port'
                 */
                /* Gain: '<S79>/Gain1' incorporates:
                 *  Constant: '<S73>/Constant1'
                 *  Constant: '<S79>/Constant'
                 *  Product: '<S79>/Product'
                 *  Selector: '<S73>/cp[m+1]'
                 *  Selector: '<S79>/Selector'
                 *  Sum: '<S73>/Sum4'
                 *  Sum: '<S79>/Sum'
                 */
                c_c = rtDW.Sum2_m[((s108_iter + 1) - 1) * 13] *
                        rtDW.OutportBufferForcp13[(qY_2 + 1) - 1];

                /* Gain: '<S79>/Gain2' incorporates:
                 *  Constant: '<S73>/Constant1'
                 *  Constant: '<S79>/Constant'
                 *  Product: '<S79>/Product'
                 *  Selector: '<S73>/sp[m+1]'
                 *  Selector: '<S79>/Selector'
                 *  Sum: '<S73>/Sum4'
                 *  Sum: '<S79>/Sum'
                 */
                oblq_rad = rtDW.Sum2_m[((s108_iter + 1) - 1) * 13] *
                        rtDW.OutportBufferForsp13[(qY_2 + 1) - 1];

                /* End of Outputs for SubSystem: '<S73>/If Action Subsystem' */
            } else {
                /* Outputs for IfAction SubSystem: '<S73>/If Action Subsystem1' incorporates:
                 *  ActionPort: '<S80>/Action Port'
                 */
                /* Sum: '<S80>/Sum' incorporates:
                 *  Constant: '<S73>/Constant1'
                 *  Constant: '<S81>/Constant'
                 *  Product: '<S80>/Product'
                 *  Product: '<S80>/Product1'
                 *  Selector: '<S73>/cp[m+1]'
                 *  Selector: '<S73>/sp[m+1]'
                 *  Selector: '<S80>/Selector'
                 *  Selector: '<S80>/Selector1'
                 *  Sum: '<S73>/Sum4'
                 *  Sum: '<S81>/Sum'
                 */
                c_c = rtDW.Sum2_m[(((s108_iter + 1) - 1) * 13 + (qY_2 + 1)) - 1] *
                        rtDW.OutportBufferForcp13[(qY_2 + 1) - 1] + rtDW.Sum2_m[(qY_2 - 1) *
                                                                                13 + s108_iter] * rtDW.OutportBufferForsp13[(qY_2 + 1) - 1];

                /* Sum: '<S80>/Sum1' incorporates:
                 *  Constant: '<S73>/Constant1'
                 *  Constant: '<S81>/Constant'
                 *  Product: '<S80>/Product'
                 *  Product: '<S80>/Product1'
                 *  Selector: '<S73>/cp[m+1]'
                 *  Selector: '<S73>/sp[m+1]'
                 *  Selector: '<S80>/Selector'
                 *  Selector: '<S80>/Selector1'
                 *  Sum: '<S73>/Sum4'
                 *  Sum: '<S81>/Sum'
                 */
                oblq_rad = rtDW.Sum2_m[(((s108_iter + 1) - 1) * 13 + (qY_2 + 1)) - 1] *
                        rtDW.OutportBufferForsp13[(qY_2 + 1) - 1] - rtDW.Sum2_m[(qY_2 - 1) *
                                                                                13 + s108_iter] * rtDW.OutportBufferForcp13[(qY_2 + 1) - 1];

                /* End of Outputs for SubSystem: '<S73>/If Action Subsystem1' */
            }

            /* End of If: '<S73>/If' */

            /* Outputs for Enabled SubSystem: '<S67>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' incorporates:
             *  EnablePort: '<S69>/Enable'
             */
            if (rtb_LogicalOperator_h) {
                /* If: '<S69>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' incorporates:
                 *  Sum: '<S67>/Sum1'
                 */
                if (s108_iter == qY_2) {
                    /* Outputs for IfAction SubSystem: '<S69>/If Action Subsystem' incorporates:
                     *  ActionPort: '<S83>/Action Port'
                     */
                    /* Gain: '<S87>/Gain' incorporates:
                     *  Sum: '<S87>/Sum2'
                     */
                    rtb_Sum1_jr = mul_s32_sat(13L, qY_2 - 1);

                    /* Selector: '<S83>/Selector' incorporates:
                     *  Sum: '<S87>/Sum1'
                     */
                    if (rtb_Sum1_jr > MAX_int32_T - s108_iter) {
                        rtb_Sum1_b = MAX_int32_T;
                        rtb_Sum1_jr = MAX_int32_T;
                    } else {
                        rtb_Sum1_b = s108_iter + rtb_Sum1_jr;
                        rtb_Sum1_jr += s108_iter;
                    }

                    /* Product: '<S83>/Product1' incorporates:
                     *  Selector: '<S83>/Selector'
                     *  UnitDelay: '<S69>/Unit Delay1'
                     */
                    rtDW.Merge_n = rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_b - 1] *
                            rtDW.sqrt_n;

                    /* Sum: '<S83>/Sum' incorporates:
                     *  Product: '<S83>/Product'
                     *  Product: '<S83>/Product2'
                     *  Selector: '<S83>/Selector'
                     *  Selector: '<S83>/Selector1'
                     *  UnitDelay: '<S69>/Unit Delay'
                     *  UnitDelay: '<S69>/Unit Delay1'
                     */
                    rtDW.Merge1_b = rtDW.UnitDelay_DSTATE_a[((s108_iter - 1) * 13 + qY_2)
                                                            - 1] * rtDW.sqrt_n + rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_jr -
                                                                                                          1] * rtDW.Product4;

                    /* End of Outputs for SubSystem: '<S69>/If Action Subsystem' */
                } else if ((s108_iter == 1) && (qY_2 == 0)) {
                    /* Outputs for IfAction SubSystem: '<S69>/If Action Subsystem1' incorporates:
                     *  ActionPort: '<S84>/Action Port'
                     */
                    /* Selector: '<S84>/Selector' incorporates:
                     *  Gain: '<S89>/Gain'
                     *  Sum: '<S89>/Sum1'
                     */
                    rtb_Sum1_jr = mul_s32_sat(13L, 0L);
                    if (rtb_Sum1_jr > 2147483646L) {
                        rtb_Sum1_jr = MAX_int32_T;
                    } else {
                        rtb_Sum1_jr++;
                    }

                    /* Product: '<S84>/Product3' incorporates:
                     *  Selector: '<S84>/Selector'
                     *  UnitDelay: '<S69>/Unit Delay1'
                     */
                    rtDW.Merge_n = rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_jr - 1] *
                            rtDW.Product4;

                    /* Selector: '<S84>/Selector' incorporates:
                     *  Gain: '<S89>/Gain'
                     *  Sum: '<S89>/Sum1'
                     */
                    rtb_Sum1_jr = mul_s32_sat(13L, 0L);
                    if (rtb_Sum1_jr > 2147483646L) {
                        rtb_Sum1_jr = MAX_int32_T;
                    } else {
                        rtb_Sum1_jr++;
                    }

                    /* Sum: '<S84>/Sum' incorporates:
                     *  Product: '<S84>/Product'
                     *  Product: '<S84>/Product2'
                     *  Selector: '<S84>/Selector'
                     *  Selector: '<S84>/Selector1'
                     *  UnitDelay: '<S69>/Unit Delay'
                     *  UnitDelay: '<S69>/Unit Delay1'
                     */
                    rtDW.Merge1_b = rtDW.Product4 * rtDW.UnitDelay_DSTATE_a[0] -
                            rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_jr - 1] * rtDW.sqrt_n;

                    /* End of Outputs for SubSystem: '<S69>/If Action Subsystem1' */
                } else {
                    if ((s108_iter > 1) && (s108_iter != qY_2)) {
                        /* Outputs for IfAction SubSystem: '<S69>/If Action Subsystem2' incorporates:
                         *  ActionPort: '<S85>/Action Port'
                         */
                        /* Sum: '<S93>/Sum' */
                        i_1 = qY_2 + 1;

                        /* Sum: '<S92>/Sum' */
                        qY_1 = s108_iter + 1;
                        qY_0 = qY_2 + 1;

                        /* Gain: '<S91>/Gain' */
                        rtb_Sum1_jr = mul_s32_sat(13L, qY_2);

                        /* Selector: '<S85>/Selector' incorporates:
                         *  Sum: '<S91>/Sum1'
                         */
                        if (rtb_Sum1_jr > MAX_int32_T - s108_iter) {
                            rtb_Sum1_b = MAX_int32_T;
                        } else {
                            rtb_Sum1_b = s108_iter + rtb_Sum1_jr;
                        }

                        /* Switch: '<S85>/Switch' incorporates:
                         *  Constant: '<S85>/Constant'
                         *  DataTypeConversion: '<S94>/Data Type Conversion'
                         *  RelationalOperator: '<S94>/Relational Operator'
                         *  Selector: '<S85>/Selector1'
                         *  Sum: '<S93>/Sum'
                         *  Sum: '<S94>/Sum2'
                         *  UnitDelay: '<S69>/Unit Delay'
                         */
                        if ((s108_iter - 2 >= qY_2) > 0.5) {
                            i = rtDW.UnitDelay_DSTATE_a[((s108_iter - 2) * 13 + i_1) - 1];
                        } else {
                            i = 0.0;
                        }

                        /* End of Switch: '<S85>/Switch' */

                        /* Sum: '<S85>/Sum' incorporates:
                         *  Constant: '<S85>/k[13][13]'
                         *  Product: '<S85>/Product'
                         *  Product: '<S85>/Product1'
                         *  Product: '<S85>/Product4'
                         *  Selector: '<S85>/Selector'
                         *  Selector: '<S85>/Selector1'
                         *  Selector: '<S85>/Selector2'
                         *  Sum: '<S92>/Sum'
                         *  Sum: '<S93>/Sum'
                         *  UnitDelay: '<S69>/Unit Delay'
                         *  UnitDelay: '<S69>/Unit Delay1'
                         */
                        rtDW.Merge1_b = (rtDW.UnitDelay_DSTATE_a[((s108_iter - 1) * 13 + i_1)
                                                                 - 1] * rtDW.Product4 - rtDW.UnitDelay1_DSTATE_m
                                [(int16_T)rtb_Sum1_b - 1] * rtDW.sqrt_n) -
                                        rtConstP.pooled5[((qY_1 - 1) * 13 + qY_0) - 1] * i;

                        /* Switch: '<S85>/Switch1' incorporates:
                         *  Constant: '<S85>/Constant1'
                         *  DataTypeConversion: '<S95>/Data Type Conversion'
                         *  RelationalOperator: '<S95>/Relational Operator'
                         *  Selector: '<S85>/Selector'
                         *  Sum: '<S95>/Sum2'
                         *  UnitDelay: '<S69>/Unit Delay1'
                         */
                        if ((s108_iter - 2 >= qY_2) > 0.5) {
                            /* Selector: '<S85>/Selector' incorporates:
                             *  Sum: '<S91>/Sum1'
                             *  Sum: '<S91>/Sum2'
                             */
                            i_1 = s108_iter - 1;
                            if (rtb_Sum1_jr > MAX_int32_T - i_1) {
                                rtb_Sum1_b = MAX_int32_T;
                            } else {
                                rtb_Sum1_b = i_1 + rtb_Sum1_jr;
                            }

                            i = rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_b - 1];
                        } else {
                            i = 0.0;
                        }

                        /* End of Switch: '<S85>/Switch1' */

                        /* Selector: '<S85>/Selector' incorporates:
                         *  Sum: '<S91>/Sum1'
                         */
                        if (rtb_Sum1_jr > MAX_int32_T - s108_iter) {
                            rtb_Sum1_jr = MAX_int32_T;
                        } else {
                            rtb_Sum1_jr += s108_iter;
                        }

                        /* Sum: '<S85>/Sum1' incorporates:
                         *  Constant: '<S85>/k[13][13]'
                         *  Product: '<S85>/Product2'
                         *  Product: '<S85>/Product3'
                         *  Selector: '<S85>/Selector'
                         *  Selector: '<S85>/Selector2'
                         *  Sum: '<S92>/Sum'
                         *  UnitDelay: '<S69>/Unit Delay1'
                         */
                        rtDW.Merge_n = rtDW.UnitDelay1_DSTATE_m[(int16_T)rtb_Sum1_jr - 1] *
                                rtDW.Product4 - rtConstP.pooled5[((qY_1 - 1) * 13 + qY_0) - 1] * i;

                        /* End of Outputs for SubSystem: '<S69>/If Action Subsystem2' */
                    }
                }

                /* End of If: '<S69>/if n == m elseif (n==1&m==0) elseif (n>1&m~=n)' */

                /* Sum: '<S69>/Sum' incorporates:
                 *  Sum: '<S67>/Sum1'
                 */
                i_1 = s108_iter + 1;
                qY_1 = qY_2 + 1;

                /* Assignment: '<S69>/Assignment' incorporates:
                 *  Sum: '<S69>/Sum'
                 *  UnitDelay: '<S69>/Unit Delay'
                 */
                if (rtDW.ForIterator_IterationMarker[2] < 2) {
                    rtDW.ForIterator_IterationMarker[2L] = 2U;
                    memcpy(&rtDW.Assignment_o[0], &rtDW.UnitDelay_DSTATE_a[0], 169U *
                           sizeof(real_T));
                }

                rtDW.Assignment_o[(qY_1 + 13 * (i_1 - 1)) - 1] = rtDW.Merge1_b;

                /* End of Assignment: '<S69>/Assignment' */

                /* Assignment: '<S69>/Assignment_snorm' incorporates:
                 *  Gain: '<S86>/Gain'
                 *  Sum: '<S69>/Sum'
                 *  Sum: '<S86>/Sum1'
                 *  Sum: '<S86>/Sum2'
                 *  UnitDelay: '<S69>/Unit Delay1'
                 */
                if (rtDW.ForIterator_IterationMarker[3] < 2) {
                    rtDW.ForIterator_IterationMarker[3L] = 2U;
                    memcpy(&rtDW.Assignment_snorm[0], &rtDW.UnitDelay1_DSTATE_m[0], 169U *
                           sizeof(real_T));
                }

                rtDW.Assignment_snorm[(int16_T)((real_T)i_1 + (real_T)mul_s32_sat(13L,
                                                                                  qY_1 - 1)) - 1] = rtDW.Merge_n;

                /* End of Assignment: '<S69>/Assignment_snorm' */

                /* Update for UnitDelay: '<S69>/Unit Delay' */
                memcpy(&rtDW.UnitDelay_DSTATE_a[0], &rtDW.Assignment_o[0], 169U * sizeof
                       (real_T));

                /* Update for UnitDelay: '<S69>/Unit Delay1' */
                memcpy(&rtDW.UnitDelay1_DSTATE_m[0], &rtDW.Assignment_snorm[0], 169U *
                       sizeof(real_T));
            }

            /* End of Outputs for SubSystem: '<S67>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

            /* Selector: '<S68>/snorm[n+m*13]' incorporates:
             *  Gain: '<S72>/Gain'
             *  Sum: '<S67>/Sum1'
             *  Sum: '<S72>/Sum1'
             */
            i_1 = 1 + s108_iter;
            rtb_Sum1_jr = mul_s32_sat(13L, qY_2);
            if (rtb_Sum1_jr > MAX_int32_T - i_1) {
                rtb_Sum1_jr = MAX_int32_T;
            } else {
                rtb_Sum1_jr += i_1;
            }

            /* Product: '<S68>/par' incorporates:
             *  Selector: '<S68>/snorm[n+m*13]'
             */
            s = rtDW.Assignment_snorm[(int16_T)rtb_Sum1_jr - 1] * day_num;

            /* Outputs for Enabled SubSystem: '<S68>/Special case - North//South Geographic Pole' incorporates:
             *  EnablePort: '<S71>/Enable'
             */
            if ((rtDW.sqrt_n == 0.0) && (1 == qY_2)) {
                if (!rtDW.SpecialcaseNorthSouthGeographic) {
                    rtDW.SpecialcaseNorthSouthGeographic = true;
                }

                /* If: '<S71>/n ==1' incorporates:
                 *  Assignment: '<S76>/Assignment2'
                 */
                if (s108_iter == 1) {
                    /* Outputs for IfAction SubSystem: '<S71>/If Action Subsystem1' incorporates:
                     *  ActionPort: '<S75>/Action Port'
                     */
                    /* Assignment: '<S75>/Assignment2' incorporates:
                     *  Selector: '<S75>/pp[n-1]'
                     *  UnitDelay: '<S71>/Unit Delay1'
                     */
                    if (rtDW.ForIterator_IterationMarker[0] < 2) {
                        rtDW.ForIterator_IterationMarker[0L] = 2U;
                        memcpy(&rtDW.Assignment2_j[0], &rtDW.UnitDelay1_DSTATE_i[0], 13U *
                               sizeof(real_T));
                    }

                    rtDW.Assignment2_j[1] = rtDW.UnitDelay1_DSTATE_i[0];

                    /* End of Assignment: '<S75>/Assignment2' */
                    /* End of Outputs for SubSystem: '<S71>/If Action Subsystem1' */
                } else {
                    /* Outputs for IfAction SubSystem: '<S71>/If Action Subsystem2' incorporates:
                     *  ActionPort: '<S76>/Action Port'
                     */
                    if (rtDW.ForIterator_IterationMarker[1] < 2) {
                        /* Assignment: '<S76>/Assignment2' incorporates:
                         *  UnitDelay: '<S71>/Unit Delay1'
                         */
                        rtDW.ForIterator_IterationMarker[1L] = 2U;
                        memcpy(&rtDW.Assignment2_k[0], &rtDW.UnitDelay1_DSTATE_i[0], 13U *
                               sizeof(real_T));
                    }

                    /* Assignment: '<S76>/Assignment2' incorporates:
                     *  Constant: '<S76>/k[13][13]'
                     *  Product: '<S76>/Product1'
                     *  Product: '<S76>/Product2'
                     *  Selector: '<S76>/Selector2'
                     *  Selector: '<S76>/pp[n-2] pp[n-1]'
                     *  Sum: '<S76>/Sum1'
                     *  UnitDelay: '<S71>/Unit Delay1'
                     */
                    rtDW.Assignment2_k[s108_iter] = rtDW.UnitDelay1_DSTATE_i[s108_iter - 1]
                                                                             * rtDW.Product4 - rtConstP.pooled5[s108_iter * 13 + 1] *
                                                                             rtDW.UnitDelay1_DSTATE_i[s108_iter - 2];

                    /* End of Outputs for SubSystem: '<S71>/If Action Subsystem2' */
                }

                /* End of If: '<S71>/n ==1' */

                /* SignalConversion: '<S71>/TmpSignal ConversionAtpp[n]Inport1' incorporates:
                 *  UnitDelay: '<S71>/Unit Delay1'
                 */
                rtb_TmpSignalConversionAtppnInp[0] = rtDW.UnitDelay1_DSTATE_i[0];
                rtb_TmpSignalConversionAtppnInp[1] = rtDW.Assignment2_j[1];
                memcpy(&rtb_TmpSignalConversionAtppnInp[2], &rtDW.Assignment2_k[2], 11U *
                       sizeof(real_T));

                /* Product: '<S71>/Product2' incorporates:
                 *  Selector: '<S71>/pp[n]'
                 */
                rtDW.Product2 = day_num * rtb_TmpSignalConversionAtppnInp[s108_iter] *
                        oblq_rad;

                /* Update for UnitDelay: '<S71>/Unit Delay1' */
                memcpy(&rtDW.UnitDelay1_DSTATE_i[0], &rtb_TmpSignalConversionAtppnInp[0],
                       13U * sizeof(real_T));
            } else {
                if (rtDW.SpecialcaseNorthSouthGeographic) {
                    /* Disable for Outport: '<S71>/bpp' */
                    rtDW.Product2 = 0.0;
                    rtDW.SpecialcaseNorthSouthGeographic = false;
                }
            }

            /* End of Outputs for SubSystem: '<S68>/Special case - North//South Geographic Pole' */

            /* Sum: '<S68>/Sum1' incorporates:
             *  Constant: '<S74>/Constant'
             *  Constant: '<S74>/Constant1'
             *  Logic: '<S74>/Logical Operator'
             *  Product: '<S68>/Product'
             *  RelationalOperator: '<S74>/Relational Operator'
             *  RelationalOperator: '<S74>/Relational Operator1'
             *  Selector: '<S68>/dp[n][m]'
             *  Sum: '<S67>/Sum1'
             *  UnitDelay: '<S68>/Unit Delay1'
             */
            rtDW.Sum1 = rtDW.UnitDelay1_DSTATE_k - rtDW.Assignment_o[s108_iter * 13 +
                                                                     qY_2] * c_c * day_num;

            /* Sum: '<S68>/Sum2' incorporates:
             *  Constant: '<S68>/fm'
             *  Product: '<S68>/Product1'
             *  Selector: '<S68>/fm[m]'
             *  Sum: '<S67>/Sum1'
             *  UnitDelay: '<S68>/Unit Delay3'
             */
            rtDW.Sum2 = rtConstP.fm_Value[qY_2] * s * oblq_rad +
                    rtDW.UnitDelay3_DSTATE;

            /* Sum: '<S68>/Sum3' incorporates:
             *  Constant: '<S68>/fn'
             *  Product: '<S68>/Product2'
             *  Selector: '<S68>/fn[m]'
             *  UnitDelay: '<S68>/Unit Delay2'
             */
            rtDW.Sum3 = rtConstP.fn_Value[s108_iter] * s * c_c +
                    rtDW.UnitDelay2_DSTATE_h;

            /* Sum: '<S68>/Sum5' incorporates:
             *  UnitDelay: '<S68>/Unit Delay4'
             */
            rtDW.Sum5 = rtDW.UnitDelay4_DSTATE + rtDW.Product2;

            /* Update for UnitDelay: '<S68>/Unit Delay1' */
            rtDW.UnitDelay1_DSTATE_k = rtDW.Sum1;

            /* Update for UnitDelay: '<S68>/Unit Delay3' */
            rtDW.UnitDelay3_DSTATE = rtDW.Sum2;

            /* Update for UnitDelay: '<S68>/Unit Delay2' */
            rtDW.UnitDelay2_DSTATE_h = rtDW.Sum3;

            /* Update for UnitDelay: '<S68>/Unit Delay4' */
            rtDW.UnitDelay4_DSTATE = rtDW.Sum5;
        }

        /* End of Outputs for SubSystem: '<S59>/For Iterator Subsystem' */

        /* Sum: '<S59>/Sum1' incorporates:
         *  UnitDelay: '<S59>/Unit Delay2'
         */
        rtb_Sum1_dv[0] = UnitDelay2_DSTATE_n[0] + rtDW.Sum1;
        rtb_Sum1_dv[1] = UnitDelay2_DSTATE_n[1] + rtDW.Sum2;
        rtb_Sum1_dv[2] = UnitDelay2_DSTATE_n[2] + rtDW.Sum3;
        rtb_Sum1_dv[3] = UnitDelay2_DSTATE_n[3] + rtDW.Sum5;

        /* Update for UnitDelay: '<S59>/Unit Delay2' */
        UnitDelay2_DSTATE_n[0] = rtb_Sum1_dv[0];
        UnitDelay2_DSTATE_n[1] = rtb_Sum1_dv[1];
        UnitDelay2_DSTATE_n[2] = rtb_Sum1_dv[2];
        UnitDelay2_DSTATE_n[3] = rtb_Sum1_dv[3];
    }

    /* End of Outputs for SubSystem: '<S57>/Compute magnetic vector in spherical coordinates' */

    /* Switch: '<S111>/Switch' incorporates:
     *  Product: '<S111>/Product'
     */
    if (rtDW.sqrt_n != 0.0) {
        xi = rtb_Sum1_dv[1] / rtDW.sqrt_n;
    } else {
        xi = rtb_Sum1_dv[3];
    }

    /* End of Switch: '<S111>/Switch' */

    /* Sum: '<S110>/Sum1' incorporates:
     *  Product: '<S110>/Product1'
     *  Product: '<S110>/Product4'
     */
    oblq_rad = (0.0 - rtDW.Product11 * rtb_Sum1_dv[0]) - rtb_Sum1_dv[2] *
            rtDW.Product12;

    /* UnitConversion: '<S115>/Unit Conversion' incorporates:
     *  Trigonometry: '<S113>/Trigonometric Function1'
     */
    /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
    c_c = 57.295779513082323 * rt_atan2d_snf(xi, oblq_rad);

    /* Sum: '<S112>/Sum1' incorporates:
     *  Product: '<S112>/Product1'
     *  Product: '<S112>/Product4'
     */
    i = rtDW.Product12 * rtb_Sum1_dv[0] - rtb_Sum1_dv[2] * rtDW.Product11;

    /* Sum: '<S113>/Sum' incorporates:
     *  Product: '<S113>/Product'
     *  Product: '<S113>/Product1'
     */
    xi = xi * xi + oblq_rad * oblq_rad;

    /* UnitConversion: '<S58>/Unit Conversion' incorporates:
     *  Sqrt: '<S113>/sqrt1'
     *  Trigonometry: '<S113>/Trigonometric Function'
     *  UnitConversion: '<S114>/Unit Conversion'
     */
    /* Unit Conversion - from: rad to: deg
     Expression: output = (57.2958*input) + (0) */
    /* Unit Conversion - from: deg to: rad
     Expression: output = (0.0174533*input) + (0) */
    s = 0.017453292519943295 * c_c;
    c_c = 57.295779513082323 * rt_atan2d_snf(i, sqrt(xi)) * 0.017453292519943295;

    /* Trigonometry: '<S52>/sincos' */
    b_o = cos(s);
    s = sin(s);

    /* Sum: '<S113>/Sum1' incorporates:
     *  Product: '<S113>/Product2'
     */
    xi += i * i;

    /* Sqrt: '<S113>/sqrt' */
    xi = sqrt(xi);

    /* Product: '<S52>/h1' incorporates:
     *  Trigonometry: '<S52>/sincos'
     */
    i = cos(c_c) * xi;

    /* Product: '<S52>/x1' */
    oblq_rad = b_o * i;

    /* Product: '<S52>/y1' */
    i *= s;

    /* Product: '<S52>/z1' incorporates:
     *  Trigonometry: '<S52>/sincos'
     */
    xi *= sin(c_c);

    /* Gain: '<S19>/nT2T' */
    dayofyear = 1.0E-9 * oblq_rad;
    day_num = 1.0E-9 * i;
    c_c = 1.0E-9 * xi;

    /* Product: '<S19>/Matrix Multiply1' incorporates:
     *  Math: '<S19>/Math Function'
     *  Product: '<S19>/Matrix Multiply2'
     */
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        dayofyear_0[s108_iter] = rtb_VectorConcatenate[3 * s108_iter + 2] * c_c +
                (rtb_VectorConcatenate[3 * s108_iter + 1] * day_num +
                        rtb_VectorConcatenate[3 * s108_iter] * dayofyear);
    }

    /* End of Product: '<S19>/Matrix Multiply1' */
    for (i_1 = 0; i_1 < 3; i_1++) {
        /* Product: '<S19>/Matrix Multiply2' */
        dayofyear = rtb_itrf_to_gcrf[i_1 + 6] * dayofyear_0[2] +
                (rtb_itrf_to_gcrf[i_1 + 3] * dayofyear_0[1] + rtb_itrf_to_gcrf[i_1] *
                        dayofyear_0[0]);

        /* Math: '<S25>/Math Function' */
        sc_pos_eci_km[i_1] = dayofyear * dayofyear;

        /* Product: '<S19>/Matrix Multiply2' */
        rtb_Gain[i_1] = dayofyear;
    }

    /* Sum: '<S25>/Sum of Elements' */
    i = (sc_pos_eci_km[0] + sc_pos_eci_km[1]) + sc_pos_eci_km[2];

    /* Math: '<S25>/Math Function1'
     *
     * About '<S25>/Math Function1':
     *  Operator: sqrt
     */
    if (i < 0.0) {
        i = -sqrt(fabs(i));
    } else {
        i = sqrt(i);
    }

    /* End of Math: '<S25>/Math Function1' */

    /* Switch: '<S25>/Switch' incorporates:
     *  Constant: '<S25>/Constant'
     *  Product: '<S25>/Product'
     */
    if (i > 0.0) {
        UnitDelay2_DSTATE_n[0] = rtb_Gain[0];
        UnitDelay2_DSTATE_n[1] = rtb_Gain[1];
        UnitDelay2_DSTATE_n[2] = rtb_Gain[2];
        UnitDelay2_DSTATE_n[3] = i;
    } else {
        UnitDelay2_DSTATE_n[0] = rtb_Gain[0] * 0.0;
        UnitDelay2_DSTATE_n[1] = rtb_Gain[1] * 0.0;
        UnitDelay2_DSTATE_n[2] = rtb_Gain[2] * 0.0;
        UnitDelay2_DSTATE_n[3] = 1.0;
    }

    /* End of Switch: '<S25>/Switch' */

    /* If: '<S6>/If' incorporates:
     *  Inport: '<Root>/valid_mag'
     *  Inport: '<Root>/valid_sun'
     *  Inport: '<S13>/bias_min'
     *  Inport: '<S13>/cov_min'
     *  Inport: '<S13>/q_min'
     *  Logic: '<S1>/Logical Operator'
     *  Logic: '<S2>/Logical Operator'
     *  MATLAB Function: '<S12>/update_state '
     *  MATLAB Function: '<S21>/MATLAB Function1'
     *  SignalConversion: '<S12>/OutportBufferForbias_plus'
     *  UnitDelay: '<S2>/Unit Delay'
     *  UnitDelay: '<S2>/Unit Delay1'
     *  UnitDelay: '<S2>/Unit Delay2'
     */
    if (rtU.valid_mag && ((lowAlt != 0) && (rtU.valid_sun != 0.0))) {
        /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem' incorporates:
         *  ActionPort: '<S12>/Action Port'
         */
        /* MATLAB Function: '<S12>/convert_inertial_body' incorporates:
         *  SignalConversion: '<S14>/TmpSignal ConversionAt SFunction Inport2'
         *  UnitDelay: '<S2>/Unit Delay'
         */
        /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body': '<S14>:1' */
        /* '<S14>:1:3' rotation_matrix = att(q_min); */
        /* '<S14>:1:9' dcm = [q(1)^2 + q(2)^2 - q(3)^2 - q(4)^2,   2*(q(2)*q(3)+q(1)*q(4)),    2*(q(2)*q(4) - q(1)*q(3)); */
        /* '<S14>:1:10'         2*(q(2)*q(3) - q(1)*q(4)),          q(1)^2-q(2)^2+q(3)^2-q(4)^2,    2*(q(3)*q(4) + q(1)*q(2)); */
        /* '<S14>:1:11'         2*(q(2)*q(4)+q(1)*q(3)),            2*(q(3)*q(4)-q(1)*q(2)),        q(1)^2-q(2)^2-q(3)^2+q(4)^2]; */
        tod_to_mod[0] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] +
                rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) -
                rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) -
                rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];
        tod_to_mod[3] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[2] +
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[3]) * 2.0;
        tod_to_mod[6] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[3] -
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[2]) * 2.0;
        tod_to_mod[1] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[2] -
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[3]) * 2.0;
        tod_to_mod[4] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] -
                rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) +
                rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) -
                rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];
        tod_to_mod[7] = (rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[3] +
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[1]) * 2.0;
        tod_to_mod[2] = (rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[3] +
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[2]) * 2.0;
        tod_to_mod[5] = (rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[3] -
                rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[1]) * 2.0;
        tod_to_mod[8] = ((rtDW.UnitDelay_DSTATE[0] * rtDW.UnitDelay_DSTATE[0] -
                rtDW.UnitDelay_DSTATE[1] * rtDW.UnitDelay_DSTATE[1]) -
                rtDW.UnitDelay_DSTATE[2] * rtDW.UnitDelay_DSTATE[2]) +
                rtDW.UnitDelay_DSTATE[3] * rtDW.UnitDelay_DSTATE[3];

        /* '<S14>:1:4' body_est = blkdiag(rotation_matrix,rotation_matrix)*meas_eci; */
        memset(&KHI[0], 0, 36U * sizeof(real_T));
        for (s108_iter = 0; s108_iter < 3; s108_iter++) {
            KHI[6 * s108_iter] = tod_to_mod[3 * s108_iter];
            KHI[3 + 6 * (3 + s108_iter)] = tod_to_mod[3 * s108_iter];
            KHI[1 + 6 * s108_iter] = tod_to_mod[3 * s108_iter + 1];
            KHI[4 + 6 * (3 + s108_iter)] = tod_to_mod[3 * s108_iter + 1];
            KHI[2 + 6 * s108_iter] = tod_to_mod[3 * s108_iter + 2];
            KHI[5 + 6 * (3 + s108_iter)] = tod_to_mod[3 * s108_iter + 2];

            /* SignalConversion: '<S14>/TmpSignal ConversionAt SFunction Inport2' incorporates:
             *  Product: '<S25>/Divide'
             */
            UnitDelay2_DSTATE_n_0[s108_iter] = UnitDelay2_DSTATE_n[s108_iter] /
                    UnitDelay2_DSTATE_n[3];
            UnitDelay2_DSTATE_n_0[s108_iter + 3] = rtb_sun_vector_eci_km[s108_iter];
        }

        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            rtb_time_vec_ut1[s108_iter] = 0.0;
            for (qY = 0; qY < 6; qY++) {
                rtb_time_vec_ut1[s108_iter] += KHI[6 * qY + s108_iter] *
                        UnitDelay2_DSTATE_n_0[qY];
            }
        }

        /* End of MATLAB Function: '<S12>/convert_inertial_body' */

        /* MATLAB Function: '<S12>/observation_matrix' */
        /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/observation_matrix': '<S17>:1' */
        /* '<S17>:1:3' H = [  skew(body_est(1:3))     zeros(3); */
        /* '<S17>:1:4'        skew(body_est(4:6))     zeros(3)]; */
        /* '<S17>:1:9' S = [0  -v(3)   v(2); */
        /* '<S17>:1:10'     v(3)    0   -v(1); */
        /* '<S17>:1:11'     -v(2)   v(1)    0]; */
        /* '<S17>:1:9' S = [0  -v(3)   v(2); */
        /* '<S17>:1:10'     v(3)    0   -v(1); */
        /* '<S17>:1:11'     -v(2)   v(1)    0]; */
        rtb_Merge2[0] = 0.0;
        rtb_Merge2[6] = -rtb_time_vec_ut1[2];
        rtb_Merge2[12] = rtb_time_vec_ut1[1];
        rtb_Merge2[1] = rtb_time_vec_ut1[2];
        rtb_Merge2[7] = 0.0;
        rtb_Merge2[13] = -rtb_time_vec_ut1[0];
        rtb_Merge2[2] = -rtb_time_vec_ut1[1];
        rtb_Merge2[8] = rtb_time_vec_ut1[0];
        rtb_Merge2[14] = 0.0;
        rtb_Merge2[3] = 0.0;
        rtb_Merge2[9] = -rtb_time_vec_ut1[5];
        rtb_Merge2[15] = rtb_time_vec_ut1[4];
        rtb_Merge2[4] = rtb_time_vec_ut1[5];
        rtb_Merge2[10] = 0.0;
        rtb_Merge2[16] = -rtb_time_vec_ut1[3];
        rtb_Merge2[5] = -rtb_time_vec_ut1[4];
        rtb_Merge2[11] = rtb_time_vec_ut1[3];
        rtb_Merge2[17] = 0.0;
        for (s108_iter = 0; s108_iter < 3; s108_iter++) {
            rtb_Merge2[6 * (s108_iter + 3)] = 0.0;
            rtb_Merge2[3 + 6 * (s108_iter + 3)] = 0.0;
            rtb_Merge2[1 + 6 * (s108_iter + 3)] = 0.0;
            rtb_Merge2[4 + 6 * (s108_iter + 3)] = 0.0;
            rtb_Merge2[2 + 6 * (s108_iter + 3)] = 0.0;
            rtb_Merge2[5 + 6 * (s108_iter + 3)] = 0.0;
        }

        /* End of MATLAB Function: '<S12>/observation_matrix' */

        /* MATLAB Function: '<S12>/kalman_gain' incorporates:
         *  Inport: '<Root>/meas_cov'
         *  Inport: '<Root>/valid_gyro'
         *  UnitDelay: '<S2>/Unit Delay2'
         */
        /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/kalman_gain': '<S16>:1' */
        /* '<S16>:1:3' K = cov_min*H'/(H*cov_min*H' + meas_cov); */
        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            for (qY = 0; qY < 6; qY++) {
                rtb_K[s108_iter + 6 * qY] = 0.0;
                rtb_Merge2_1[s108_iter + 6 * qY] = 0.0;
                for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                    rtb_K[s108_iter + 6 * qY] += rtDW.UnitDelay2_DSTATE[6 * s67_iter +
                                                                        s108_iter] * rtb_Merge2[6 * s67_iter + qY];
                    rtb_Merge2_1[s108_iter + 6 * qY] += rtb_Merge2[6 * s67_iter +
                                                                   s108_iter] * rtDW.UnitDelay2_DSTATE[6 * qY + s67_iter];
                }
            }

            for (qY = 0; qY < 6; qY++) {
                i = 0.0;
                for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                    i += rtb_Merge2_1[6 * s67_iter + s108_iter] * rtb_Merge2[6 * s67_iter
                                                                             + qY];
                }

                rtb_Merge2_0[s108_iter + 6 * qY] = rtU.meas_cov[6 * qY + s108_iter] + i;
            }
        }

        mrdivide(rtb_K, rtb_Merge2_0);

        /*  zero off part of gain if gyro reading is invalid */
        /* '<S16>:1:7' if valid_gyro == 0 */
        if (rtU.valid_gyro == 0.0) {
            /* '<S16>:1:8' K(4:6,:) = zeros(3,6); */
            for (s108_iter = 0; s108_iter < 6; s108_iter++) {
                rtb_K[3 + 6 * s108_iter] = 0.0;
                rtb_K[4 + 6 * s108_iter] = 0.0;
                rtb_K[5 + 6 * s108_iter] = 0.0;
            }
        }

        /* End of MATLAB Function: '<S12>/kalman_gain' */

        /* Sum: '<S12>/Sum' incorporates:
         *  Inport: '<Root>/mag_body'
         *  Inport: '<Root>/sun_body'
         *  Product: '<S12>/Matrix Multiply'
         */
        tmp[0] = rtU.mag_body[0];
        tmp[3] = rtU.sun_body[0];
        tmp[1] = rtU.mag_body[1];
        tmp[4] = rtU.sun_body[1];
        tmp[2] = rtU.mag_body[2];
        tmp[5] = rtU.sun_body[2];
        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            UnitDelay2_DSTATE_n_0[s108_iter] = tmp[s108_iter] -
                    rtb_time_vec_ut1[s108_iter];
        }

        /* End of Sum: '<S12>/Sum' */

        /* Product: '<S12>/Matrix Multiply' */
        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            rtb_MatrixMultiply[s108_iter] = 0.0;
            for (qY = 0; qY < 6; qY++) {
                rtb_MatrixMultiply[s108_iter] += rtb_K[6 * qY + s108_iter] *
                        UnitDelay2_DSTATE_n_0[qY];
            }
        }

        /* MATLAB Function: '<S12>/update_state ' */
        /*  select out bias */
        /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/update_state ': '<S18>:1' */
        /* '<S18>:1:5' bias_plu = bias_min + del_x_plu(4:6); */
        /* '<S18>:1:7' q_plu = quat_update(del_x_plu(1:3,1),q_min); */
        /* '<S18>:1:15' q_hat_pk = quat_multiply([sqrt(1 - norm(del_apk/2)^2); del_apk/2], q_hat_mk); */
        i = 2.2250738585072014E-308;
        dayofyear = fabs(rtb_MatrixMultiply[0] / 2.0);
        if (dayofyear > 2.2250738585072014E-308) {
            xi = 1.0;
            i = dayofyear;
        } else {
            day_num = dayofyear / 2.2250738585072014E-308;
            xi = day_num * day_num;
        }

        c_c = rtb_MatrixMultiply[0] / 2.0;
        rtb_sun_vector_eci_km[0] = rtDW.UnitDelay1_DSTATE[0] + rtb_MatrixMultiply[3];

        /* MATLAB Function: '<S12>/update_state ' incorporates:
         *  SignalConversion: '<S12>/OutportBufferForbias_plus'
         *  UnitDelay: '<S2>/Unit Delay1'
         */
        dayofyear = fabs(rtb_MatrixMultiply[1] / 2.0);
        if (dayofyear > i) {
            day_num = i / dayofyear;
            xi = xi * day_num * day_num + 1.0;
            i = dayofyear;
        } else {
            day_num = dayofyear / i;
            xi += day_num * day_num;
        }

        s = rtb_MatrixMultiply[1] / 2.0;
        rtb_sun_vector_eci_km[1] = rtDW.UnitDelay1_DSTATE[1] + rtb_MatrixMultiply[4];

        /* MATLAB Function: '<S12>/update_state ' incorporates:
         *  SignalConversion: '<S12>/OutportBufferForbias_plus'
         *  UnitDelay: '<S2>/Unit Delay1'
         */
        dayofyear = fabs(rtb_MatrixMultiply[2] / 2.0);
        if (dayofyear > i) {
            day_num = i / dayofyear;
            xi = xi * day_num * day_num + 1.0;
            i = dayofyear;
        } else {
            day_num = dayofyear / i;
            xi += day_num * day_num;
        }

        dayofyear = rtb_MatrixMultiply[2] / 2.0;
        rtb_sun_vector_eci_km[2] = rtDW.UnitDelay1_DSTATE[2] + rtb_MatrixMultiply[5];

        /* MATLAB Function: '<S12>/update_state ' incorporates:
         *  SignalConversion: '<S12>/OutportBufferForbias_plus'
         *  UnitDelay: '<S2>/Unit Delay1'
         */
        xi = i * sqrt(xi);

        /* '<S18>:1:22' q2 = quatmultiply(q1',dq')'; */
        day_num = sqrt(1.0 - xi * xi);

        /* MATLAB Function: '<S12>/covariance_update' incorporates:
         *  Inport: '<Root>/meas_cov'
         *  UnitDelay: '<S2>/Unit Delay2'
         */
        /* MATLAB Function 'Estimation_EKF /Update Step /If Action Subsystem/covariance_update': '<S15>:1' */
        /* '<S15>:1:4' KHI = (K*H - eye(6)); */
        for (s108_iter = 0; s108_iter < 36; s108_iter++) {
            I_0[s108_iter] = 0;
        }

        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            I_0[s108_iter + 6 * s108_iter] = 1;
        }

        /* '<S15>:1:5' cov_plus = KHI*cov_min*KHI' + K*meas_cov*K'; */
        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            for (qY = 0; qY < 6; qY++) {
                i = 0.0;
                for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                    i += rtb_K[6 * s67_iter + s108_iter] * rtb_Merge2[6 * qY + s67_iter];
                }

                KHI[s108_iter + 6 * qY] = i - (real_T)I_0[6 * qY + s108_iter];
            }

            for (qY = 0; qY < 6; qY++) {
                rtb_Merge2_0[s108_iter + 6 * qY] = 0.0;
                rtb_Merge2_1[s108_iter + 6 * qY] = 0.0;
                for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                    rtb_Merge2_0[s108_iter + 6 * qY] += KHI[6 * s67_iter + s108_iter] *
                            rtDW.UnitDelay2_DSTATE[6 * qY + s67_iter];
                    rtb_Merge2_1[s108_iter + 6 * qY] += rtb_K[6 * s67_iter + s108_iter] *
                            rtU.meas_cov[6 * qY + s67_iter];
                }
            }
        }

        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            for (qY = 0; qY < 6; qY++) {
                KHI_0[s108_iter + 6 * qY] = 0.0;
                rtb_K_0[s108_iter + 6 * qY] = 0.0;
                for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                    KHI_0[s108_iter + 6 * qY] += rtb_Merge2_0[6 * s67_iter + s108_iter] *
                            KHI[6 * s67_iter + qY];
                    rtb_K_0[s108_iter + 6 * qY] += rtb_Merge2_1[6 * s67_iter + s108_iter] *
                            rtb_K[6 * s67_iter + qY];
                }
            }
        }

        /* SignalConversion: '<S12>/OutportBufferForcov_plus' incorporates:
         *  MATLAB Function: '<S12>/covariance_update'
         */
        for (s108_iter = 0; s108_iter < 6; s108_iter++) {
            for (qY = 0; qY < 6; qY++) {
                rtb_Merge2[qY + 6 * s108_iter] = KHI_0[6 * s108_iter + qY] + rtb_K_0[6 *
                                                                                     s108_iter + qY];
            }
        }

        /* End of SignalConversion: '<S12>/OutportBufferForcov_plus' */

        /* SignalConversion: '<S12>/OutportBufferForq_plus' incorporates:
         *  MATLAB Function: '<S12>/update_state '
         *  UnitDelay: '<S2>/Unit Delay'
         */
        UnitDelay2_DSTATE_n[0] = ((rtDW.UnitDelay_DSTATE[0] * day_num -
                rtDW.UnitDelay_DSTATE[1] * c_c) - rtDW.UnitDelay_DSTATE[2] * s) -
                rtDW.UnitDelay_DSTATE[3] * dayofyear;
        UnitDelay2_DSTATE_n[1] = (rtDW.UnitDelay_DSTATE[0] * c_c + day_num *
                rtDW.UnitDelay_DSTATE[1]) + (rtDW.UnitDelay_DSTATE[2] * dayofyear -
                        rtDW.UnitDelay_DSTATE[3] * s);
        UnitDelay2_DSTATE_n[2] = (rtDW.UnitDelay_DSTATE[0] * s + day_num *
                rtDW.UnitDelay_DSTATE[2]) + (rtDW.UnitDelay_DSTATE[3] * c_c -
                        rtDW.UnitDelay_DSTATE[1] * dayofyear);
        UnitDelay2_DSTATE_n[3] = (rtDW.UnitDelay_DSTATE[0] * dayofyear + day_num *
                rtDW.UnitDelay_DSTATE[3]) + (rtDW.UnitDelay_DSTATE[1] * s -
                        rtDW.UnitDelay_DSTATE[2] * c_c);

        /* End of Outputs for SubSystem: '<S6>/If Action Subsystem' */
    } else {
        /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S13>/Action Port'
         */
        UnitDelay2_DSTATE_n[0] = rtDW.UnitDelay_DSTATE[0];
        UnitDelay2_DSTATE_n[1] = rtDW.UnitDelay_DSTATE[1];
        UnitDelay2_DSTATE_n[2] = rtDW.UnitDelay_DSTATE[2];
        UnitDelay2_DSTATE_n[3] = rtDW.UnitDelay_DSTATE[3];
        rtb_sun_vector_eci_km[0] = rtDW.UnitDelay1_DSTATE[0];
        rtb_sun_vector_eci_km[1] = rtDW.UnitDelay1_DSTATE[1];
        rtb_sun_vector_eci_km[2] = rtDW.UnitDelay1_DSTATE[2];
        memcpy(&rtb_Merge2[0], &rtDW.UnitDelay2_DSTATE[0], 36U * sizeof(real_T));

        /* End of Outputs for SubSystem: '<S6>/If Action Subsystem1' */
    }

    /* End of If: '<S6>/If' */

    /* Switch: '<S5>/Switch' incorporates:
     *  Inport: '<Root>/valid_gyro'
     *  Inport: '<Root>/w_body_radps'
     *  Sum: '<S5>/Sum'
     */
    if (rtU.valid_gyro >= 0.0) {
        rtb_Gain[0] = rtU.w_body_radps[0] - rtb_sun_vector_eci_km[0];
        rtb_Gain[1] = rtU.w_body_radps[1] - rtb_sun_vector_eci_km[1];
        rtb_Gain[2] = rtU.w_body_radps[2] - rtb_sun_vector_eci_km[2];
    } else {
        rtb_Gain[0] = 0.0;
        rtb_Gain[1] = 0.0;
        rtb_Gain[2] = 0.0;
    }

    /* End of Switch: '<S5>/Switch' */

    /* Outport: '<Root>/sc_in_sun' incorporates:
     *  MATLAB Function: '<S21>/MATLAB Function1'
     */
    rtY.sc_in_sun = lowAlt;

    /* MATLAB Function: '<S5>/state_transition' */
    /* MATLAB Function 'Estimation_EKF /Propagate Step /state_transition': '<S9>:1' */
    /* '<S9>:1:3' Phi = state_transition_matrix(w_plu,dt); */
    /* '<S9>:1:10' w_n = norm(w); */
    i = 2.2250738585072014E-308;

    /* Outport: '<Root>/w_body_hat_radps' */
    rtY.w_body_hat_radps[0] = rtb_Gain[0];

    /* MATLAB Function: '<S5>/state_transition' */
    dayofyear = fabs(rtb_Gain[0]);
    if (dayofyear > 2.2250738585072014E-308) {
        xi = 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / 2.2250738585072014E-308;
        xi = day_num * day_num;
    }

    /* Outport: '<Root>/w_body_hat_radps' */
    rtY.w_body_hat_radps[1] = rtb_Gain[1];

    /* MATLAB Function: '<S5>/state_transition' */
    dayofyear = fabs(rtb_Gain[1]);
    if (dayofyear > i) {
        day_num = i / dayofyear;
        xi = xi * day_num * day_num + 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / i;
        xi += day_num * day_num;
    }

    /* Outport: '<Root>/w_body_hat_radps' */
    rtY.w_body_hat_radps[2] = rtb_Gain[2];

    /* MATLAB Function: '<S5>/state_transition' incorporates:
     *  Constant: '<S5>/Constant'
     */
    dayofyear = fabs(rtb_Gain[2]);
    if (dayofyear > i) {
        day_num = i / dayofyear;
        xi = xi * day_num * day_num + 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / i;
        xi += day_num * day_num;
    }

    xi = i * sqrt(xi);

    /* '<S9>:1:11' w_skew = skew(w); */
    /* '<S9>:1:31' S = [0  -v(3)   v(2); */
    /* '<S9>:1:32'     v(3)    0   -v(1); */
    /* '<S9>:1:33'     -v(2)   v(1)    0]; */
    tod_to_mod[0] = 0.0;
    tod_to_mod[3] = -rtb_Gain[2];
    tod_to_mod[6] = rtb_Gain[1];
    tod_to_mod[1] = rtb_Gain[2];
    tod_to_mod[4] = 0.0;
    tod_to_mod[7] = -rtb_Gain[0];
    tod_to_mod[2] = -rtb_Gain[1];
    tod_to_mod[5] = rtb_Gain[0];
    tod_to_mod[8] = 0.0;

    /* '<S9>:1:13' if w_n < 1e-14 */
    if (xi < 1.0E-14) {
        /* '<S9>:1:14' phi = [ eye(3) -eye(3)*dt; */
        /* '<S9>:1:15'             zeros(3,3) eye(3)]; */
        memset(&tod_to_mod[0], 0, 9U * sizeof(real_T));
        for (s108_iter = 0; s108_iter < 9; s108_iter++) {
            I[s108_iter] = 0;
        }

        I[0] = 1;
        I[4] = 1;
        I[8] = 1;
        for (lowAlt = 0; lowAlt < 3; lowAlt++) {
            tod_to_mod[lowAlt + 3 * lowAlt] = 1.0;
            rtb_K[6 * lowAlt] = I[3 * lowAlt];
            rtb_K[6 * (lowAlt + 3)] = (real_T)a_0[3 * lowAlt] * 0.1;
            rtb_K[3 + 6 * lowAlt] = 0.0;
            rtb_K[1 + 6 * lowAlt] = I[3 * lowAlt + 1];
            rtb_K[1 + 6 * (lowAlt + 3)] = (real_T)a_0[3 * lowAlt + 1] * 0.1;
            rtb_K[4 + 6 * lowAlt] = 0.0;
            rtb_K[2 + 6 * lowAlt] = I[3 * lowAlt + 2];
            rtb_K[2 + 6 * (lowAlt + 3)] = (real_T)a_0[3 * lowAlt + 2] * 0.1;
            rtb_K[5 + 6 * lowAlt] = 0.0;
        }

        for (s108_iter = 0; s108_iter < 3; s108_iter++) {
            rtb_K[3 + 6 * (s108_iter + 3)] = tod_to_mod[3 * s108_iter];
            rtb_K[4 + 6 * (s108_iter + 3)] = tod_to_mod[3 * s108_iter + 1];
            rtb_K[5 + 6 * (s108_iter + 3)] = tod_to_mod[3 * s108_iter + 2];
        }
    } else {
        /* '<S9>:1:16' else */
        /* '<S9>:1:17' Phi_11 = eye(3) - w_skew*sin(w_n*dt)/w_n + w_skew^2*(1 - cos(w_n*dt))/w_n^2; */
        for (s108_iter = 0; s108_iter < 9; s108_iter++) {
            I[s108_iter] = 0;
            Phi_22[s108_iter] = 0;
        }

        I[0] = 1;
        I[4] = 1;
        I[8] = 1;
        s = sin(xi * 0.1);
        b_s = cos(xi * 0.1);
        i = xi * xi;

        /* '<S9>:1:19' Phi_12 = w_skew*(1 - cos(w_n*dt))/w_n^2 - eye(3)*dt - w_skew^2*(w_n*dt - sin(w_n*dt))/w_n^3; */
        oblq_rad = cos(xi * 0.1);
        day_num = xi * xi;
        dayofyear = xi * 0.1 - sin(xi * 0.1);
        c_c = rt_powd_snf(xi, 3.0);

        /* '<S9>:1:21' Phi_21 = zeros(3,3); */
        /* '<S9>:1:23' Phi_22 = eye(3); */
        /* '<S9>:1:25' phi = [ Phi_11 Phi_12; */
        /* '<S9>:1:26'             Phi_21 Phi_22]; */
        for (lowAlt = 0; lowAlt < 3; lowAlt++) {
            Phi_22[lowAlt + 3 * lowAlt] = 1;
            for (s108_iter = 0; s108_iter < 3; s108_iter++) {
                tod_to_mod_0[lowAlt + 3 * s108_iter] = 0.0;
                rtb_VectorConcatenate[lowAlt + 3 * s108_iter] = 0.0;
                tod_to_mod_0[lowAlt + 3 * s108_iter] += tod_to_mod[3 * s108_iter] *
                        tod_to_mod[lowAlt];
                rtb_VectorConcatenate[lowAlt + 3 * s108_iter] += tod_to_mod[3 *
                                                                            s108_iter] * tod_to_mod[lowAlt];
                tod_to_mod_0[lowAlt + 3 * s108_iter] += tod_to_mod[3 * s108_iter + 1] *
                        tod_to_mod[lowAlt + 3];
                rtb_VectorConcatenate[lowAlt + 3 * s108_iter] += tod_to_mod[3 *
                                                                            s108_iter + 1] * tod_to_mod[lowAlt + 3];
                tod_to_mod_0[lowAlt + 3 * s108_iter] += tod_to_mod[3 * s108_iter + 2] *
                        tod_to_mod[lowAlt + 6];
                rtb_VectorConcatenate[lowAlt + 3 * s108_iter] += tod_to_mod[3 *
                                                                            s108_iter + 2] * tod_to_mod[lowAlt + 6];
            }
        }

        for (s108_iter = 0; s108_iter < 3; s108_iter++) {
            rtb_K[6 * s108_iter] = ((real_T)I[3 * s108_iter] - tod_to_mod[3 *
                                                                          s108_iter] * s / xi) + tod_to_mod_0[3 * s108_iter] * (1.0 - b_s) / i;
            rtb_K[6 * (s108_iter + 3)] = (tod_to_mod[3 * s108_iter] * (1.0 - oblq_rad)
                    / day_num - (real_T)b_a[3 * s108_iter] * 0.1) - rtb_VectorConcatenate[3 *
                                                                                          s108_iter] * dayofyear / c_c;
            rtb_K[3 + 6 * s108_iter] = 0.0;
            rtb_K[3 + 6 * (s108_iter + 3)] = Phi_22[3 * s108_iter];
            rtb_K[1 + 6 * s108_iter] = ((real_T)I[3 * s108_iter + 1] - tod_to_mod[3 *
                                                                                  s108_iter + 1] * s / xi) + tod_to_mod_0[3 * s108_iter + 1] * (1.0 - b_s)
                                                                                  / i;
            rtb_K[1 + 6 * (s108_iter + 3)] = (tod_to_mod[3 * s108_iter + 1] * (1.0 -
                    oblq_rad) / day_num - (real_T)b_a[3 * s108_iter + 1] * 0.1) -
                    rtb_VectorConcatenate[3 * s108_iter + 1] * dayofyear / c_c;
            rtb_K[4 + 6 * s108_iter] = 0.0;
            rtb_K[4 + 6 * (s108_iter + 3)] = Phi_22[3 * s108_iter + 1];
            rtb_K[2 + 6 * s108_iter] = ((real_T)I[3 * s108_iter + 2] - tod_to_mod[3 *
                                                                                  s108_iter + 2] * s / xi) + tod_to_mod_0[3 * s108_iter + 2] * (1.0 - b_s)
                                                                                  / i;
            rtb_K[2 + 6 * (s108_iter + 3)] = (tod_to_mod[3 * s108_iter + 2] * (1.0 -
                    oblq_rad) / day_num - (real_T)b_a[3 * s108_iter + 2] * 0.1) -
                    rtb_VectorConcatenate[3 * s108_iter + 2] * dayofyear / c_c;
            rtb_K[5 + 6 * s108_iter] = 0.0;
            rtb_K[5 + 6 * (s108_iter + 3)] = Phi_22[3 * s108_iter + 2];
        }
    }

    /* MATLAB Function: '<S5>/propagate_quat' incorporates:
     *  Constant: '<S5>/Constant1'
     */
    /* MATLAB Function 'Estimation_EKF /Propagate Step /propagate_quat': '<S8>:1' */
    /* '<S8>:1:3' q_min_k1 = omega_matrix(w_plu,dt)*q_plu; */
    /* '<S8>:1:8' w_n = norm(w); */
    i = 2.2250738585072014E-308;
    dayofyear = fabs(rtb_Gain[0]);
    if (dayofyear > 2.2250738585072014E-308) {
        xi = 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / 2.2250738585072014E-308;
        xi = day_num * day_num;
    }

    dayofyear = fabs(rtb_Gain[1]);
    if (dayofyear > i) {
        day_num = i / dayofyear;
        xi = xi * day_num * day_num + 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / i;
        xi += day_num * day_num;
    }

    dayofyear = fabs(rtb_Gain[2]);
    if (dayofyear > i) {
        day_num = i / dayofyear;
        xi = xi * day_num * day_num + 1.0;
        i = dayofyear;
    } else {
        day_num = dayofyear / i;
        xi += day_num * day_num;
    }

    xi = i * sqrt(xi);

    /* '<S8>:1:10' if w_n < 1e-14 */
    if (xi < 1.0E-14) {
        /* '<S8>:1:11' Psi = 0*w; */
        rtb_Gain[0] *= 0.0;
        rtb_Gain[1] *= 0.0;
        rtb_Gain[2] *= 0.0;
    } else {
        /* '<S8>:1:12' else */
        /* '<S8>:1:13' Psi = sin(1/2*w_n*dt)/w_n*w; */
        i = sin(0.5 * xi * 0.1) / xi;
        rtb_Gain[0] *= i;
        rtb_Gain[1] *= i;
        rtb_Gain[2] *= i;
    }

    /* '<S8>:1:16' Psi_skew = skew(Psi); */
    /* '<S8>:1:28' S = [0  -v(3)   v(2); */
    /* '<S8>:1:29'     v(3)    0   -v(1); */
    /* '<S8>:1:30'     -v(2)   v(1)    0]; */
    /* '<S8>:1:18' omega = [cos(1/2*w_n*dt)*eye(3) - Psi_skew      Psi; */
    /* '<S8>:1:19'         -Psi'                                   cos(1/2*w_n*dt)]; */
    s = cos(0.5 * xi * 0.1);

    /* '<S8>:1:21' T = [zeros(3,1) eye(3); */
    /* '<S8>:1:22'     1       zeros(1,3)]; */
    /* '<S8>:1:24' omega = T'*omega*T; */
    tmp_0[0] = 0.0;
    tmp_0[3] = -rtb_Gain[2];
    tmp_0[6] = rtb_Gain[1];
    tmp_0[1] = rtb_Gain[2];
    tmp_0[4] = 0.0;
    tmp_0[7] = -rtb_Gain[0];
    tmp_0[2] = -rtb_Gain[1];
    tmp_0[5] = rtb_Gain[0];
    tmp_0[8] = 0.0;
    for (s108_iter = 0; s108_iter < 3; s108_iter++) {
        s_0[s108_iter << 2] = (real_T)b_b[3 * s108_iter] * s - tmp_0[3 * s108_iter];
        s_0[1 + (s108_iter << 2)] = (real_T)b_b[3 * s108_iter + 1] * s - tmp_0[3 *
                                                                               s108_iter + 1];
        s_0[2 + (s108_iter << 2)] = (real_T)b_b[3 * s108_iter + 2] * s - tmp_0[3 *
                                                                               s108_iter + 2];
        s_0[12 + s108_iter] = rtb_Gain[s108_iter];
        s_0[3 + (s108_iter << 2)] = -rtb_Gain[s108_iter];
    }

    s_0[15] = cos(0.5 * xi * 0.1);
    for (s108_iter = 0; s108_iter < 4; s108_iter++) {
        for (qY = 0; qY < 4; qY++) {
            a_3[s108_iter + (qY << 2)] = 0.0;
            a_3[s108_iter + (qY << 2)] += s_0[qY << 2] * (real_T)a_1[s108_iter];
            a_3[s108_iter + (qY << 2)] += s_0[(qY << 2) + 1] * (real_T)a_1[s108_iter +
                                                                           4];
            a_3[s108_iter + (qY << 2)] += s_0[(qY << 2) + 2] * (real_T)a_1[s108_iter +
                                                                           8];
            a_3[s108_iter + (qY << 2)] += s_0[(qY << 2) + 3] * (real_T)a_1[s108_iter +
                                                                           12];
        }

        rtb_Sum1_dv[s108_iter] = 0.0;
        for (qY = 0; qY < 4; qY++) {
            a_4[s108_iter + (qY << 2)] = 0.0;
            a_4[s108_iter + (qY << 2)] += (real_T)b_2[qY << 2] * a_3[s108_iter];
            a_4[s108_iter + (qY << 2)] += (real_T)b_2[(qY << 2) + 1] * a_3[s108_iter +
                                                                           4];
            a_4[s108_iter + (qY << 2)] += (real_T)b_2[(qY << 2) + 2] * a_3[s108_iter +
                                                                           8];
            a_4[s108_iter + (qY << 2)] += (real_T)b_2[(qY << 2) + 3] * a_3[s108_iter +
                                                                           12];
            rtb_Sum1_dv[s108_iter] += a_4[(qY << 2) + s108_iter] *
                    UnitDelay2_DSTATE_n[qY];
        }
    }

    /* End of MATLAB Function: '<S5>/propagate_quat' */

    /* Sqrt: '<S10>/sqrt' incorporates:
     *  Product: '<S11>/Product'
     *  Product: '<S11>/Product1'
     *  Product: '<S11>/Product2'
     *  Product: '<S11>/Product3'
     *  Sum: '<S11>/Sum'
     */
    i = sqrt(((rtb_Sum1_dv[0] * rtb_Sum1_dv[0] + rtb_Sum1_dv[1] * rtb_Sum1_dv[1])
            + rtb_Sum1_dv[2] * rtb_Sum1_dv[2]) + rtb_Sum1_dv[3] * rtb_Sum1_dv[3]);

    /* Product: '<S7>/Product' */
    xi = rtb_Sum1_dv[0] / i;

    /* Product: '<S7>/Product1' */
    dayofyear = rtb_Sum1_dv[1] / i;

    /* Product: '<S7>/Product2' */
    day_num = rtb_Sum1_dv[2] / i;

    /* Product: '<S7>/Product3' */
    i = rtb_Sum1_dv[3] / i;

    /* Outport: '<Root>/quat_hat' */
    rtY.quat_hat[0] = xi;
    rtY.quat_hat[1] = dayofyear;
    rtY.quat_hat[2] = day_num;
    rtY.quat_hat[3] = i;

    /* Update for UnitDelay: '<S2>/Unit Delay' */
    /* MATLAB Function 'Estimation_EKF /3_sig_bnd ': '<S4>:1' */
    /* '<S4>:1:3' bnd = cov; */
    rtDW.UnitDelay_DSTATE[0] = xi;
    rtDW.UnitDelay_DSTATE[1] = dayofyear;
    rtDW.UnitDelay_DSTATE[2] = day_num;
    rtDW.UnitDelay_DSTATE[3] = i;

    /* Update for UnitDelay: '<S2>/Unit Delay1' */
    rtDW.UnitDelay1_DSTATE[0] = rtb_sun_vector_eci_km[0];
    rtDW.UnitDelay1_DSTATE[1] = rtb_sun_vector_eci_km[1];
    rtDW.UnitDelay1_DSTATE[2] = rtb_sun_vector_eci_km[2];

    /* Product: '<S5>/Product' incorporates:
     *  Math: '<S5>/Math Function'
     */
    for (s108_iter = 0; s108_iter < 6; s108_iter++) {
        for (qY = 0; qY < 6; qY++) {
            rtb_Merge2_0[s108_iter + 6 * qY] = 0.0;
            for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                rtb_Merge2_0[s108_iter + 6 * qY] += rtb_Merge2[6 * s67_iter + s108_iter]
                                                               * rtb_K[6 * s67_iter + qY];
            }
        }
    }

    /* Update for UnitDelay: '<S2>/Unit Delay2' incorporates:
     *  Product: '<S5>/Product'
     *  Sum: '<S5>/Add'
     */
    for (s108_iter = 0; s108_iter < 6; s108_iter++) {
        for (qY = 0; qY < 6; qY++) {
            i = 0.0;
            for (s67_iter = 0; s67_iter < 6; s67_iter++) {
                i += rtb_K[6 * s67_iter + qY] * rtb_Merge2_0[6 * s108_iter + s67_iter];
            }

            rtDW.UnitDelay2_DSTATE[qY + 6 * s108_iter] = rtConstB.Product1_k[6 *
                                                                             s108_iter + qY] + i;
        }
    }

    /* End of Update for UnitDelay: '<S2>/Unit Delay2' */

    /* Update for Memory: '<S65>/otime' */
    rtDW.otime_PreviousInput = T1900;

    /* Update for Memory: '<S64>/olon' */
    rtDW.olon_PreviousInput = days;

    /* Update for Memory: '<S63>/olat' */
    rtDW.olat_PreviousInput = JD_days_gps;

    /* Update for Memory: '<S63>/oalt' */
    rtDW.oalt_PreviousInput = c_s;
}

/* Model initialize function */
void CDR_autocode_initialize(void)
{
    /* Registration code */

    /* initialize non-finites */
    rt_InitInfAndNaN(sizeof(real_T));

    {
        int16_T i;

        /* InitializeConditions for UnitDelay: '<S2>/Unit Delay' */
        rtDW.UnitDelay_DSTATE[0] = 1.0;
        rtDW.UnitDelay_DSTATE[1] = 0.0;
        rtDW.UnitDelay_DSTATE[2] = 0.0;
        rtDW.UnitDelay_DSTATE[3] = 0.0;

        /* InitializeConditions for UnitDelay: '<S2>/Unit Delay2' */
        memcpy(&rtDW.UnitDelay2_DSTATE[0], &rtConstP.UnitDelay2_InitialCondition[0],
               36U * sizeof(real_T));

        /* InitializeConditions for Memory: '<S65>/otime' */
        rtDW.otime_PreviousInput = -1000.0;

        /* InitializeConditions for Memory: '<S64>/olon' */
        rtDW.olon_PreviousInput = -1000.0;

        /* InitializeConditions for Memory: '<S63>/olat' */
        rtDW.olat_PreviousInput = -1000.0;

        /* InitializeConditions for Memory: '<S63>/oalt' */
        rtDW.oalt_PreviousInput = -1000.0;

        /* SystemInitialize for Enabled SubSystem: '<S57>/Convert from geodetic to  spherical coordinates ' */
        /* SystemInitialize for Outport: '<S61>/sp[13]' */
        memcpy(&rtDW.OutportBufferForsp13[0], &rtConstP.sp13_Y0[0], 13U * sizeof
               (real_T));

        /* SystemInitialize for Outport: '<S61>/cp[13]' */
        memcpy(&rtDW.OutportBufferForcp13[0], &rtConstP.cp13_Y0[0], 13U * sizeof
               (real_T));

        /* End of SystemInitialize for SubSystem: '<S57>/Convert from geodetic to  spherical coordinates ' */

        /* SystemInitialize for Enabled SubSystem: '<S57>/Convert from geodetic to  spherical coordinates' */
        /* SystemInitialize for Outport: '<S60>/r' */
        rtDW.sqrt_a = 6378.137;

        /* SystemInitialize for Outport: '<S60>/ct' */
        rtDW.Product4 = 1.0;

        /* End of SystemInitialize for SubSystem: '<S57>/Convert from geodetic to  spherical coordinates' */

        /* SystemInitialize for Iterator SubSystem: '<S57>/Compute magnetic vector in spherical coordinates' */
        /* SystemInitialize for Iterator SubSystem: '<S59>/For Iterator Subsystem' */
        /* SystemInitialize for Enabled SubSystem: '<S67>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */
        /* InitializeConditions for UnitDelay: '<S69>/Unit Delay1' */
        memcpy(&rtDW.UnitDelay1_DSTATE_m[0], &rtConstP.pooled8[0], 169U * sizeof
               (real_T));

        /* SystemInitialize for Outport: '<S69>/snorm[169]' */
        memcpy(&rtDW.Assignment_snorm[0], &rtConstP.pooled8[0], 169U * sizeof(real_T));

        /* End of SystemInitialize for SubSystem: '<S67>/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations' */

        /* SystemInitialize for Enabled SubSystem: '<S68>/Special case - North//South Geographic Pole' */
        /* SystemInitialize for IfAction SubSystem: '<S71>/If Action Subsystem2' */
        /* SystemInitialize for IfAction SubSystem: '<S71>/If Action Subsystem1' */
        for (i = 0; i < 13; i++) {
            /* InitializeConditions for UnitDelay: '<S71>/Unit Delay1' */
            rtDW.UnitDelay1_DSTATE_i[i] = 1.0;

            /* SystemInitialize for Outport: '<S75>/pp[13]' */
            rtDW.Assignment2_j[i] = 1.0;

            /* SystemInitialize for Outport: '<S76>/pp[13]' */
            rtDW.Assignment2_k[i] = 1.0;
        }

        /* End of SystemInitialize for SubSystem: '<S71>/If Action Subsystem1' */
        /* End of SystemInitialize for SubSystem: '<S71>/If Action Subsystem2' */
        /* End of SystemInitialize for SubSystem: '<S68>/Special case - North//South Geographic Pole' */
        /* End of SystemInitialize for SubSystem: '<S59>/For Iterator Subsystem' */
        /* End of SystemInitialize for SubSystem: '<S57>/Compute magnetic vector in spherical coordinates' */
    }
}

/* Model terminate function */
void CDR_autocode_terminate(void)
{
    /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
