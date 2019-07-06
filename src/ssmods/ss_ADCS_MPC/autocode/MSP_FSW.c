/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_FSW.c
 *
 * Code generated for Simulink model 'MSP_FSW'.
 *
 * Model version                  : 1.384
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Jun 25 21:15:08 2018
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
extern void sun_point_lib_Init(DW_sun_point_lib *localDW);
extern void sun_point_lib(const real32_T rtu_solar_panel_power[3], const real_T
  rtu_meas_ss_body_unit[3], const real_T rtu_meas_body_rate_radps[3], const
  real_T rtu_B_body_T[3], real_T rty_cmd_dipole_Am2[3], DW_sun_point_lib
  *localDW);

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

/* System initialize for atomic system: '<S6>/sun_point_lib' */
void sun_point_lib_Init(DW_sun_point_lib *localDW)
{
  /* InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
  localDW->UnitDelay_DSTATE[0] = 0.0;
  localDW->UnitDelay_DSTATE[1] = 0.0;
  localDW->UnitDelay_DSTATE[2] = 1.0;
}

/* Output and update for atomic system: '<S6>/sun_point_lib' */
void sun_point_lib(const real32_T rtu_solar_panel_power[3], const real_T
                   rtu_meas_ss_body_unit[3], const real_T
                   rtu_meas_body_rate_radps[3], const real_T rtu_B_body_T[3],
                   real_T rty_cmd_dipole_Am2[3], DW_sun_point_lib *localDW)
{
  real32_T mtmp;
  int16_T itmp;
  int16_T ixstart;
  int16_T ix;
  int32_T idxStart;
  real_T rtb_sun_body_unit[3];
  real_T rtb_drv_gain[3];
  real_T rtb_Divide_c[3];
  real_T rtb_TrigonometricFunction2;
  int32_T idx1;
  int32_T idx2;
  real_T rtb_Divide_e;
  real_T rtb_Divide_e_0;
  real_T rtb_Divide_e_1;
  boolean_T exitg1;

  /* MATLAB Function: '<S38>/MATLAB Function' incorporates:
   *  UnitDelay: '<S38>/Unit Delay'
   */
  /*  ----------------------------------------------------------------------- % */
  /* GET_SUN_BODY   Estimates the unit vector pointing to the sun in the body */
  /* frame based on the last valid sun sensor angles and the known solar panel */
  /* power draws.  */
  /*  */
  /*  solar_panel_power is a 3x1 vector with +x in entry 1, -x in entry 2 and */
  /*  +y in entry 3. */
  /*  */
  /* LOGIC: If any solar panels are generating sufficient power to claim the */
  /* sun is hitting them, point that way. If none are, look at the last valid */
  /* angle \alpha (about the x-axis) to decide between +/- z axis. Library */
  /* never outputs +/- y axis, since the cross product with solar panel normal */
  /* would return null. */
  /*  */
  /*  UW HuskySat-1, ADCS System */
  /*  */
  /*  T. Reynolds */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'sun_point_lib/MATLAB Function': '<S52>:1' */
  /*  Constants */
  /* '<S52>:1:22' tol         = 1e-3; */
  /*  Lower power thresholds [W]. Computed from P_max * 0.3 * (0.18/0.3), where */
  /*  first fraction is Earth's albedo, and second is the difference in */
  /*  efficiency for generating power from albedo vs. raw sunlight. From */
  /*  discussions with Sean Poulter, Power lead. */
  /* '<S52>:1:28' SP_Ptol_F   = 1.00; */
  /*  Power TOLerance on Front face */
  /* '<S52>:1:29' SP_Ptol_S   = 1.30; */
  /*  Power TOLerance on Side face */
  /* '<S52>:1:30' sun_body_unit   = sun_body_last; */
  rtb_sun_body_unit[0] = localDW->UnitDelay_DSTATE[0];
  rtb_sun_body_unit[1] = localDW->UnitDelay_DSTATE[1];
  rtb_sun_body_unit[2] = localDW->UnitDelay_DSTATE[2];

  /*  retain last used vector */
  /* '<S52>:1:32' [P,i] = max(solar_panel_power); */
  ixstart = 1;
  mtmp = rtu_solar_panel_power[0];
  itmp = 1;
  if (rtIsNaNF(rtu_solar_panel_power[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 4)) {
      ixstart = ix;
      if (!rtIsNaNF(rtu_solar_panel_power[ix - 1])) {
        mtmp = rtu_solar_panel_power[ix - 1];
        itmp = ix;
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 3) {
    while (ixstart + 1 < 4) {
      if (rtu_solar_panel_power[ixstart] > mtmp) {
        mtmp = rtu_solar_panel_power[ixstart];
        itmp = ixstart + 1;
      }

      ixstart++;
    }
  }

  /* '<S52>:1:33' if( P - SP_Ptol_F > tol ) */
  if (mtmp - 1.0F > 0.001) {
    /*  +y face has highest priority. If it reads sufficient power, go there */
    /* '<S52>:1:35' if( solar_panel_power(3) >= SP_Ptol_F ) */
    if (rtu_solar_panel_power[2] >= 1.0F) {
      /* '<S52>:1:36' sun_body_unit   = meas_ss_body; */
      rtb_sun_body_unit[0] = rtu_meas_ss_body_unit[0];
      rtb_sun_body_unit[1] = rtu_meas_ss_body_unit[1];
      rtb_sun_body_unit[2] = rtu_meas_ss_body_unit[2];
    } else {
      /* '<S52>:1:38' else */
      /* '<S52>:1:39' if( (abs(i-1) < tol) && (abs(P - SP_Ptol_S) >= tol) ) */
      if (((real_T)itmp - 1.0 < 0.001) && ((real32_T)fabs(mtmp - 1.3F) >= 0.001))
      {
        /* '<S52>:1:40' sun_body_unit  = [ 1; 0; 0 ]; */
        rtb_sun_body_unit[0] = 1.0;
        rtb_sun_body_unit[1] = 0.0;
        rtb_sun_body_unit[2] = 0.0;
      } else {
        if ((fabs((real_T)itmp - 2.0) < 0.001) && ((real32_T)fabs(mtmp - 1.3F) >=
             0.001)) {
          /* '<S52>:1:41' elseif( (abs(i-2) < tol) && (abs(P - SP_Ptol_S) >= tol) ) */
          /* '<S52>:1:42' sun_body_unit  = [ -1; 0; 0 ]; */
          rtb_sun_body_unit[0] = -1.0;
          rtb_sun_body_unit[1] = 0.0;
          rtb_sun_body_unit[2] = 0.0;
        }
      }
    }
  }

  /* End of MATLAB Function: '<S38>/MATLAB Function' */

  /* S-Function (sdsp2norm2): '<S53>/Normalization1' */
  idxStart = 0L;
  ixstart = 0;
  while (ixstart < 1) {
    idx1 = idxStart;
    idx2 = idxStart;
    ixstart = 0;
    while (ixstart < 1) {
      rtb_Divide_e = 0.0;
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_Divide_e += rtb_sun_body_unit[idx1] * rtb_sun_body_unit[idx1];
        idx1++;
      }

      rtb_Divide_e = 1.0 / (sqrt(rtb_Divide_e) + 1.0E-10);
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_drv_gain[idx2] = rtb_sun_body_unit[idx2] * rtb_Divide_e;
        idx2++;
      }

      ixstart = 1;
    }

    idxStart++;
    ixstart = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S53>/Normalization1' */

  /* S-Function (sdsp2norm2): '<S53>/Normalization2' incorporates:
   *  Constant: '<S38>/Constant'
   */
  idxStart = 0L;
  ixstart = 0;
  while (ixstart < 1) {
    idx1 = idxStart;
    idx2 = idxStart;
    ixstart = 0;
    while (ixstart < 1) {
      rtb_Divide_e = 0.0;
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_Divide_e += rtConstP.Constant_Value[idx1] *
          rtConstP.Constant_Value[idx1];
        idx1++;
      }

      rtb_Divide_e = 1.0 / (sqrt(rtb_Divide_e) + 1.0E-10);
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_Divide_c[idx2] = rtConstP.Constant_Value[idx2] * rtb_Divide_e;
        idx2++;
      }

      ixstart = 1;
    }

    idxStart++;
    ixstart = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S53>/Normalization2' */

  /* DotProduct: '<S53>/Dot Product' */
  rtb_TrigonometricFunction2 = (rtb_drv_gain[0] * rtb_Divide_c[0] +
    rtb_drv_gain[1] * rtb_Divide_c[1]) + rtb_drv_gain[2] * rtb_Divide_c[2];

  /* Saturate: '<S53>/Saturation' incorporates:
   *  DotProduct: '<S53>/Dot Product'
   */
  if (rtb_TrigonometricFunction2 > 1.0) {
    rtb_TrigonometricFunction2 = 1.0;
  } else {
    if (rtb_TrigonometricFunction2 < -1.0) {
      rtb_TrigonometricFunction2 = -1.0;
    }
  }

  /* End of Saturate: '<S53>/Saturation' */

  /* Trigonometry: '<S53>/Trigonometric Function2' incorporates:
   *  Gain: '<S53>/Gain1'
   *  Trigonometry: '<S53>/Trigonometric Function'
   */
  rtb_TrigonometricFunction2 = sin(0.5 * acos(rtb_TrigonometricFunction2));

  /* Product: '<S55>/k x i' */
  rtb_Divide_e = rtb_Divide_c[0];

  /* Product: '<S55>/i x j' */
  rtb_Divide_e_0 = rtb_Divide_c[1];

  /* Product: '<S56>/j x i' */
  rtb_Divide_e_1 = rtb_Divide_c[0];

  /* Sum: '<S54>/Sum' incorporates:
   *  Product: '<S55>/i x j'
   *  Product: '<S55>/j x k'
   *  Product: '<S55>/k x i'
   *  Product: '<S56>/i x k'
   *  Product: '<S56>/j x i'
   *  Product: '<S56>/k x j'
   */
  rtb_Divide_c[0] = rtb_drv_gain[1] * rtb_Divide_c[2] - rtb_drv_gain[2] *
    rtb_Divide_c[1];
  rtb_Divide_c[1] = rtb_drv_gain[2] * rtb_Divide_e - rtb_drv_gain[0] *
    rtb_Divide_c[2];
  rtb_Divide_c[2] = rtb_drv_gain[0] * rtb_Divide_e_0 - rtb_drv_gain[1] *
    rtb_Divide_e_1;

  /* S-Function (sdsp2norm2): '<S53>/Normalization' */
  idxStart = 0L;
  ixstart = 0;
  while (ixstart < 1) {
    idx1 = idxStart;
    idx2 = idxStart;
    ixstart = 0;
    while (ixstart < 1) {
      rtb_Divide_e = 0.0;
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_Divide_e += rtb_Divide_c[idx1] * rtb_Divide_c[idx1];
        idx1++;
      }

      rtb_Divide_e = 1.0 / (sqrt(rtb_Divide_e) + 1.0E-10);
      for (ixstart = 0; ixstart < 3; ixstart++) {
        rtb_drv_gain[idx2] = rtb_Divide_c[idx2] * rtb_Divide_e;
        idx2++;
      }

      ixstart = 1;
    }

    idxStart++;
    ixstart = 1;
  }

  /* End of S-Function (sdsp2norm2): '<S53>/Normalization' */

  /* Switch: '<S53>/Switch' incorporates:
   *  DotProduct: '<S53>/Dot Product1'
   *  Product: '<S53>/Product'
   *  Sqrt: '<S53>/Sqrt'
   *  UnitDelay: '<S53>/Unit Delay'
   */
  if (sqrt((rtb_Divide_c[0] * rtb_Divide_c[0] + rtb_Divide_c[1] * rtb_Divide_c[1])
           + rtb_Divide_c[2] * rtb_Divide_c[2]) != 0.0) {
    rtb_drv_gain[0] *= rtb_TrigonometricFunction2;
    rtb_drv_gain[1] *= rtb_TrigonometricFunction2;
    rtb_TrigonometricFunction2 *= rtb_drv_gain[2];
  } else {
    rtb_drv_gain[0] = localDW->UnitDelay_DSTATE_b[0];
    rtb_drv_gain[1] = localDW->UnitDelay_DSTATE_b[1];
    rtb_TrigonometricFunction2 = localDW->UnitDelay_DSTATE_b[2];
  }

  /* End of Switch: '<S53>/Switch' */
  for (ixstart = 0; ixstart < 3; ixstart++) {
    /* UnaryMinus: '<S38>/Unary Minus' incorporates:
     *  Gain: '<S38>/drv_gain'
     *  Gain: '<S38>/prop_gain'
     *  Sum: '<S38>/Sum'
     */
    rtb_Divide_c[ixstart] = -((rtConstP.prop_gain_Gain[ixstart + 6] *
      rtb_TrigonometricFunction2 + (rtConstP.prop_gain_Gain[ixstart + 3] *
      rtb_drv_gain[1] + rtConstP.prop_gain_Gain[ixstart] * rtb_drv_gain[0])) +
      (rtConstP.drv_gain_Gain[ixstart + 6] * rtu_meas_body_rate_radps[2] +
       (rtConstP.drv_gain_Gain[ixstart + 3] * rtu_meas_body_rate_radps[1] +
        rtConstP.drv_gain_Gain[ixstart] * rtu_meas_body_rate_radps[0])));
  }

  /* DotProduct: '<S38>/Dot Product' */
  rtb_Divide_e = (rtu_B_body_T[0] * rtu_B_body_T[0] + rtu_B_body_T[1] *
                  rtu_B_body_T[1]) + rtu_B_body_T[2] * rtu_B_body_T[2];

  /* Update for UnitDelay: '<S38>/Unit Delay' */
  localDW->UnitDelay_DSTATE[0] = rtb_sun_body_unit[0];

  /* Update for UnitDelay: '<S53>/Unit Delay' */
  localDW->UnitDelay_DSTATE_b[0] = rtb_drv_gain[0];

  /* Update for UnitDelay: '<S38>/Unit Delay' */
  localDW->UnitDelay_DSTATE[1] = rtb_sun_body_unit[1];

  /* Update for UnitDelay: '<S53>/Unit Delay' */
  localDW->UnitDelay_DSTATE_b[1] = rtb_drv_gain[1];

  /* Update for UnitDelay: '<S38>/Unit Delay' */
  localDW->UnitDelay_DSTATE[2] = rtb_sun_body_unit[2];

  /* Update for UnitDelay: '<S53>/Unit Delay' */
  localDW->UnitDelay_DSTATE_b[2] = rtb_TrigonometricFunction2;

  /* Saturate: '<S38>/Saturation1' incorporates:
   *  DotProduct: '<S38>/Dot Product'
   *  Product: '<S38>/Divide'
   *  Product: '<S51>/Element product'
   *  Sum: '<S51>/Add3'
   */
  rtb_TrigonometricFunction2 = (rtu_B_body_T[1] * rtb_Divide_c[2] -
    rtu_B_body_T[2] * rtb_Divide_c[1]) / rtb_Divide_e;
  if (rtb_TrigonometricFunction2 > 0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[0] = 0.15;
  } else if (rtb_TrigonometricFunction2 < -0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[0] = -0.15;
  } else {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[0] = rtb_TrigonometricFunction2;
  }

  /* End of Saturate: '<S38>/Saturation1' */

  /* Saturate: '<S38>/Saturation2' incorporates:
   *  DotProduct: '<S38>/Dot Product'
   *  Product: '<S38>/Divide'
   *  Product: '<S51>/Element product'
   *  Sum: '<S51>/Add3'
   */
  rtb_TrigonometricFunction2 = (rtu_B_body_T[2] * rtb_Divide_c[0] -
    rtu_B_body_T[0] * rtb_Divide_c[2]) / rtb_Divide_e;
  if (rtb_TrigonometricFunction2 > 0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[1] = 0.15;
  } else if (rtb_TrigonometricFunction2 < -0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[1] = -0.15;
  } else {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[1] = rtb_TrigonometricFunction2;
  }

  /* End of Saturate: '<S38>/Saturation2' */

  /* Saturate: '<S38>/Saturation3' incorporates:
   *  DotProduct: '<S38>/Dot Product'
   *  Product: '<S38>/Divide'
   *  Product: '<S51>/Element product'
   *  Sum: '<S51>/Add3'
   */
  rtb_TrigonometricFunction2 = (rtu_B_body_T[0] * rtb_Divide_c[1] -
    rtu_B_body_T[1] * rtb_Divide_c[0]) / rtb_Divide_e;
  if (rtb_TrigonometricFunction2 > 0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[2] = 0.15;
  } else if (rtb_TrigonometricFunction2 < -0.15) {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[2] = -0.15;
  } else {
    /* RateTransition: '<S38>/Rate Transition4' */
    rty_cmd_dipole_Am2[2] = rtb_TrigonometricFunction2;
  }

  /* End of Saturate: '<S38>/Saturation3' */
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

/* Function for MATLAB Function: '<S10>/TARG_GEN' */
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

/* Function for MATLAB Function: '<S10>/TARG_GEN' */
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
 * Function for MATLAB Function: '<S10>/TARG_GEN'
 * function q = myDCM2quat( DCM )
 */
static void myDCM2quat(const real32_T DCM[9], real_T q[4])
{
  real32_T tr;
  real_T eta;
  real32_T y;

  /* '<S59>:1:76' q   = zeros(4,1); */
  /* '<S59>:1:78' tr  = trace(DCM); */
  /* '<S59>:1:80' q(1)    = 0.5*sqrt(tr + 1); */
  tr = (real32_T)sqrt(((DCM[0] + DCM[4]) + DCM[8]) + 1.0F);
  y = 0.5F * tr;
  q[0] = 0.5F * tr;

  /* '<S59>:1:82' if( q(1) ~= 0 ) */
  if (y != 0.0F) {
    /* '<S59>:1:83' eta     = 1/(4*q(1)); */
    eta = 1.0 / (4.0 * y);

    /* '<S59>:1:84' q(2)    = eta*(DCM(2,3) - DCM(3,2)); */
    q[1] = (DCM[7] - DCM[5]) * (real32_T)eta;

    /* '<S59>:1:85' q(3)    = eta*(DCM(3,1) - DCM(1,3)); */
    q[2] = (DCM[2] - DCM[6]) * (real32_T)eta;

    /* '<S59>:1:86' q(4)    = eta*(DCM(1,2) - DCM(2,1)); */
    q[3] = (DCM[3] - DCM[1]) * (real32_T)eta;
  } else {
    /* '<S59>:1:87' else */
    /* '<S59>:1:88' q(2)  = sqrt(0.5*(DCM(1,1) + 1)); */
    q[1] = (real32_T)sqrt((DCM[0] + 1.0F) * 0.5F);

    /* '<S59>:1:89' q(3)  = sign(DCM(1,2))*sqrt(0.5*(DCM(2,2) + 1)); */
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

    /* '<S59>:1:90' q(4)  = sign(DCM(1,3))*sqrt(0.5*(DCM(3,3) + 1)); */
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
  real_T w_n;
  real_T Psi[3];
  static const int8_T b[16] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };

  static const int8_T a[16] = { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 };

  static const int8_T b_b[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real32_T C_cmd[9];
  real32_T S1[3];
  real32_T Yti[3];
  int8_T I[9];
  static const real32_T b_0[9] = { 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,
    0.0F, 1.0F };

  static const int8_T d[9] = { 0, -1, 0, 0, 0, -1, 1, 0, 0 };

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
  int16_T k;
  real_T KHI[36];
  int8_T I_0[36];
  real_T rtb_DataTypeConversion2[3];
  real32_T rtb_Divide3;
  real_T rtb_DataTypeConversion1[3];
  real_T rtb_q_min_k1[4];
  real_T rtb_Product2_g;
  real_T rtb_Product_am;
  real_T rtb_Product1_h;
  real_T rtb_Product2_d;
  real_T rtb_Switch[3];
  real32_T rtb_Divide;
  real32_T rtb_Divide1;
  real32_T rtb_Divide2;
  boolean_T rtb_Switch_ms;
  real_T rtb_body_est[6];
  real_T rtb_MatrixMultiply[6];
  real_T rtb_Merge[4];
  int8_T rtb_sc_mode;
  real_T rtb_Merge2[36];
  real_T rtb_K[36];
  real_T RateTransition4[3];
  real32_T S1_0[3];
  real32_T S1_1[3];
  real_T rtb_Merge2_0[36];
  int16_T i;
  real_T rtb_Merge2_1[36];
  real_T rtb_DataTypeConversion1_0[6];
  real_T rtb_DataTypeConversion1_1[6];
  real_T KHI_0[36];
  real_T rtb_K_0[36];
  real_T scale_0[16];
  real_T a_1[16];
  real_T a_2[16];
  real32_T S1_2[3];
  real32_T S1_3[9];
  real_T rotation_matrix_0[9];
  real_T rotation_matrix_1[9];
  int16_T i_0;
  real_T rtb_Merge1_idx_0;
  real_T rtb_Merge1_idx_1;
  real_T rtb_Merge1_idx_2;
  boolean_T guard1 = false;

  /* Switch: '<S7>/Switch2' incorporates:
   *  Constant: '<S7>/1'
   *  Constant: '<S7>/2'
   *  Inport: '<Root>/mag1_vec_body_T'
   *  Inport: '<Root>/mag2_vec_body_T'
   *  Logic: '<S7>/Logical Operator1'
   */
  if ((rtU.mag2_vec_body_T[3] != 0.0F) && (rtU.mag1_vec_body_T[3] != 0.0F)) {
    rtb_Divide3 = 2.0F;
  } else {
    rtb_Divide3 = 1.0F;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* DataTypeConversion: '<S1>/Data Type Conversion2' incorporates:
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   */
  rtb_DataTypeConversion2[0] = rtU.sun_vec_body_sunsensor[0];

  /* Switch: '<S7>/Switch1' incorporates:
   *  Inport: '<Root>/mag1_vec_body_T'
   */
  if (rtU.mag1_vec_body_T[3] != 0.0F) {
    rtb_Divide = rtU.mag1_vec_body_T[0];
  } else {
    rtb_Divide = 0.0F;
  }

  /* Switch: '<S7>/Switch' incorporates:
   *  Inport: '<Root>/mag2_vec_body_T'
   */
  if (rtU.mag2_vec_body_T[3] != 0.0F) {
    rtb_Divide1 = rtU.mag2_vec_body_T[0];
  } else {
    rtb_Divide1 = 0.0F;
  }

  /* DataTypeConversion: '<S7>/Data Type Conversion1' incorporates:
   *  Product: '<S7>/Divide'
   *  Sum: '<S7>/Sum1'
   */
  rtb_DataTypeConversion1[0] = (rtb_Divide + rtb_Divide1) / rtb_Divide3;

  /* DataTypeConversion: '<S1>/Data Type Conversion2' incorporates:
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   */
  rtb_DataTypeConversion2[1] = rtU.sun_vec_body_sunsensor[1];

  /* Switch: '<S7>/Switch1' incorporates:
   *  Inport: '<Root>/mag1_vec_body_T'
   */
  if (rtU.mag1_vec_body_T[3] != 0.0F) {
    rtb_Divide = rtU.mag1_vec_body_T[1];
  } else {
    rtb_Divide = 0.0F;
  }

  /* Switch: '<S7>/Switch' incorporates:
   *  Inport: '<Root>/mag2_vec_body_T'
   */
  if (rtU.mag2_vec_body_T[3] != 0.0F) {
    rtb_Divide1 = rtU.mag2_vec_body_T[1];
  } else {
    rtb_Divide1 = 0.0F;
  }

  /* DataTypeConversion: '<S7>/Data Type Conversion1' incorporates:
   *  Product: '<S7>/Divide'
   *  Sum: '<S7>/Sum1'
   */
  rtb_DataTypeConversion1[1] = (rtb_Divide + rtb_Divide1) / rtb_Divide3;

  /* DataTypeConversion: '<S1>/Data Type Conversion2' incorporates:
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   */
  rtb_DataTypeConversion2[2] = rtU.sun_vec_body_sunsensor[2];

  /* Switch: '<S7>/Switch1' incorporates:
   *  Inport: '<Root>/mag1_vec_body_T'
   */
  if (rtU.mag1_vec_body_T[3] != 0.0F) {
    rtb_Divide = rtU.mag1_vec_body_T[2];
  } else {
    rtb_Divide = 0.0F;
  }

  /* Switch: '<S7>/Switch' incorporates:
   *  Inport: '<Root>/mag2_vec_body_T'
   */
  if (rtU.mag2_vec_body_T[3] != 0.0F) {
    rtb_Divide1 = rtU.mag2_vec_body_T[2];
  } else {
    rtb_Divide1 = 0.0F;
  }

  /* DataTypeConversion: '<S7>/Data Type Conversion1' incorporates:
   *  Product: '<S7>/Divide'
   *  Sum: '<S7>/Sum1'
   */
  rtb_DataTypeConversion1[2] = (rtb_Divide + rtb_Divide1) / rtb_Divide3;

  /* Switch: '<S9>/Switch' incorporates:
   *  Inport: '<Root>/gyro_omega_body_radps'
   */
  rtb_Switch_ms = (rtU.gyro_omega_body_radps[3] > 0.5F);

  /* If: '<S21>/If' incorporates:
   *  Inport: '<Root>/MT_valid'
   *  Inport: '<Root>/mag1_vec_body_T'
   *  Inport: '<Root>/mag2_vec_body_T'
   *  Inport: '<Root>/sc_in_sun'
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   *  Inport: '<S29>/bias_min'
   *  Inport: '<S29>/cov_min'
   *  Inport: '<S29>/q_min'
   *  Logic: '<S3>/Logical Operator'
   *  Logic: '<S3>/Logical Operator1'
   *  Logic: '<S3>/Logical Operator2'
   *  Logic: '<S4>/Logical Operator'
   *  Logic: '<S7>/Logical Operator'
   *  Switch: '<S22>/Switch'
   *  UnitDelay: '<S3>/Unit Delay'
   *  UnitDelay: '<S3>/Unit Delay1'
   *  UnitDelay: '<S3>/Unit Delay2'
   */
  if (((rtU.mag2_vec_body_T[3] != 0.0F) || (rtU.mag1_vec_body_T[3] != 0.0F)) &&
      (rtU.MT_valid[0] && rtU.MT_valid[1] && rtU.MT_valid[2]) &&
      ((rtU.sun_vec_body_sunsensor[3] > 0.5F) && rtU.sc_in_sun)) {
    /* Outputs for IfAction SubSystem: '<S21>/If Action Subsystem' incorporates:
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
     *  DataTypeConversion: '<S1>/Data Type Conversion2'
     *  Product: '<S28>/Matrix Multiply'
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
    rtb_Merge1_idx_0 = rtb_MatrixMultiply[0] / 2.0;
    rtb_Merge1_idx_1 = rtb_MatrixMultiply[1] / 2.0;
    rtb_Merge1_idx_2 = rtb_MatrixMultiply[2] / 2.0;
    rtb_Product2_g = ((rtDW.UnitDelay_DSTATE[0] - rtDW.UnitDelay_DSTATE[1] *
                       rtb_Merge1_idx_0) - rtDW.UnitDelay_DSTATE[2] *
                      rtb_Merge1_idx_1) - rtDW.UnitDelay_DSTATE[3] *
      rtb_Merge1_idx_2;
    rtb_Product1_h = (rtDW.UnitDelay_DSTATE[0] * rtb_Merge1_idx_0 +
                      rtDW.UnitDelay_DSTATE[1]) + (rtDW.UnitDelay_DSTATE[2] *
      rtb_Merge1_idx_2 - rtDW.UnitDelay_DSTATE[3] * rtb_Merge1_idx_1);
    rtb_Product2_d = (rtDW.UnitDelay_DSTATE[0] * rtb_Merge1_idx_1 +
                      rtDW.UnitDelay_DSTATE[2]) + (rtDW.UnitDelay_DSTATE[3] *
      rtb_Merge1_idx_0 - rtDW.UnitDelay_DSTATE[1] * rtb_Merge1_idx_2);
    w_n = (rtDW.UnitDelay_DSTATE[0] * rtb_Merge1_idx_2 + rtDW.UnitDelay_DSTATE[3])
      + (rtDW.UnitDelay_DSTATE[1] * rtb_Merge1_idx_1 - rtDW.UnitDelay_DSTATE[2] *
         rtb_Merge1_idx_0);

    /* q_hat_pk = q_hat_mk + 0.5*xi_matrix(q_hat_mk)*del_apk; */
    /* '<S34>:1:8' q_plu = q_plu/norm(q_plu); */
    scale = 2.2250738585072014E-308;
    absxk = fabs(rtb_Product2_g);
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
    rtb_Merge1_idx_0 = rtDW.UnitDelay1_DSTATE[0] + rtb_MatrixMultiply[3];
    rtb_Merge1_idx_1 = rtDW.UnitDelay1_DSTATE[1] + rtb_MatrixMultiply[4];
    rtb_Merge1_idx_2 = rtDW.UnitDelay1_DSTATE[2] + rtb_MatrixMultiply[5];

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
    rtb_Merge[0] = rtb_Product2_g / rtb_Product_am;
    rtb_Merge[1] = rtb_Product1_h / rtb_Product_am;
    rtb_Merge[2] = rtb_Product2_d / rtb_Product_am;
    rtb_Merge[3] = w_n / rtb_Product_am;

    /* End of Outputs for SubSystem: '<S21>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S21>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S29>/Action Port'
     */
    rtb_Merge[0] = rtDW.UnitDelay_DSTATE[0];
    rtb_Merge[1] = rtDW.UnitDelay_DSTATE[1];
    rtb_Merge[2] = rtDW.UnitDelay_DSTATE[2];
    rtb_Merge[3] = rtDW.UnitDelay_DSTATE[3];
    rtb_Merge1_idx_0 = rtDW.UnitDelay1_DSTATE[0];
    rtb_Merge1_idx_1 = rtDW.UnitDelay1_DSTATE[1];
    rtb_Merge1_idx_2 = rtDW.UnitDelay1_DSTATE[2];
    memcpy(&rtb_Merge2[0], &rtDW.UnitDelay2_DSTATE[0], 36U * sizeof(real_T));

    /* End of Outputs for SubSystem: '<S21>/If Action Subsystem1' */
  }

  /* End of If: '<S21>/If' */

  /* Switch: '<S20>/Switch' incorporates:
   *  Inport: '<Root>/gyro_omega_body_radps'
   *  Sum: '<S20>/Sum'
   */
  if (rtb_Switch_ms) {
    rtb_Switch[0] = rtU.gyro_omega_body_radps[0] - rtb_Merge1_idx_0;
    rtb_Switch[1] = rtU.gyro_omega_body_radps[1] - rtb_Merge1_idx_1;
    rtb_Switch[2] = rtU.gyro_omega_body_radps[2] - rtb_Merge1_idx_2;
  } else {
    rtb_Switch[0] = 0.0;
    rtb_Switch[1] = 0.0;
    rtb_Switch[2] = 0.0;
  }

  /* End of Switch: '<S20>/Switch' */

  /* MATLAB Function: '<S20>/propagate_quat' incorporates:
   *  Constant: '<S20>/Constant1'
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
  scale = cos(0.5 * w_n * 0.1);

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
    scale_0[i << 2] = (real_T)b_b[3 * i] * scale - rotation_matrix_0[3 * i];
    scale_0[1 + (i << 2)] = (real_T)b_b[3 * i + 1] * scale - rotation_matrix_0[3
      * i + 1];
    scale_0[2 + (i << 2)] = (real_T)b_b[3 * i + 2] * scale - rotation_matrix_0[3
      * i + 2];
    scale_0[12 + i] = Psi[i];
    scale_0[3 + (i << 2)] = -Psi[i];
  }

  scale_0[15] = cos(0.5 * w_n * 0.1);
  for (i = 0; i < 4; i++) {
    for (k = 0; k < 4; k++) {
      a_1[i + (k << 2)] = 0.0;
      a_1[i + (k << 2)] += scale_0[k << 2] * (real_T)a[i];
      a_1[i + (k << 2)] += scale_0[(k << 2) + 1] * (real_T)a[i + 4];
      a_1[i + (k << 2)] += scale_0[(k << 2) + 2] * (real_T)a[i + 8];
      a_1[i + (k << 2)] += scale_0[(k << 2) + 3] * (real_T)a[i + 12];
    }

    rtb_q_min_k1[i] = 0.0;
    for (k = 0; k < 4; k++) {
      a_2[i + (k << 2)] = 0.0;
      a_2[i + (k << 2)] += (real_T)b[k << 2] * a_1[i];
      a_2[i + (k << 2)] += (real_T)b[(k << 2) + 1] * a_1[i + 4];
      a_2[i + (k << 2)] += (real_T)b[(k << 2) + 2] * a_1[i + 8];
      a_2[i + (k << 2)] += (real_T)b[(k << 2) + 3] * a_1[i + 12];
      rtb_q_min_k1[i] += a_2[(k << 2) + i] * rtb_Merge[k];
    }
  }

  /* End of MATLAB Function: '<S20>/propagate_quat' */

  /* Sqrt: '<S26>/sqrt' incorporates:
   *  Product: '<S27>/Product'
   *  Product: '<S27>/Product1'
   *  Product: '<S27>/Product2'
   *  Product: '<S27>/Product3'
   *  Sum: '<S27>/Sum'
   */
  rtb_Product2_g = sqrt(((rtb_q_min_k1[0] * rtb_q_min_k1[0] + rtb_q_min_k1[1] *
    rtb_q_min_k1[1]) + rtb_q_min_k1[2] * rtb_q_min_k1[2]) + rtb_q_min_k1[3] *
                        rtb_q_min_k1[3]);

  /* Product: '<S23>/Product' */
  rtb_Product_am = rtb_q_min_k1[0] / rtb_Product2_g;

  /* Product: '<S23>/Product1' */
  rtb_Product1_h = rtb_q_min_k1[1] / rtb_Product2_g;

  /* Product: '<S23>/Product2' */
  rtb_Product2_d = rtb_q_min_k1[2] / rtb_Product2_g;

  /* Product: '<S23>/Product3' */
  rtb_Product2_g = rtb_q_min_k1[3] / rtb_Product2_g;

  /* Outport: '<Root>/sc_quat' */
  rtY.sc_quat[0] = rtb_Product_am;
  rtY.sc_quat[1] = rtb_Product1_h;
  rtY.sc_quat[2] = rtb_Product2_d;
  rtY.sc_quat[3] = rtb_Product2_g;

  /* Outport: '<Root>/body_rates' incorporates:
   *  Reshape: '<S3>/Reshape1'
   */
  rtY.body_rates[0] = rtb_Switch[0];
  rtY.body_rates[1] = rtb_Switch[1];
  rtY.body_rates[2] = rtb_Switch[2];

  /* Outport: '<Root>/sc_mode' incorporates:
   *  UnitDelay: '<S8>/Unit Delay'
   */
  rtY.sc_mode = rtDW.UnitDelay_DSTATE_a;

  /* MATLAB Function: '<S10>/TARG_GEN' incorporates:
   *  Constant: '<S10>/r_SEA'
   *  Inport: '<Root>/sc2sun_unit'
   *  UnitDelay: '<S8>/Unit Delay'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  FSW Target Generation Library */
  /*    UW HuskySat-1, ADCS Team */
  /*    T. Reynolds 4.23.18 */
  /*  */
  /*  Computes desired quaternion and angular velocity depending on the sc_mode */
  /*    mode_logic.bdot             = 1; */
  /*    mode_logic.seek_sun         = 2; */
  /*    mode_logic.point            = 3; */
  /*        point.velocity          = 31; */
  /*        point.ground            = 32; */
  /*    mode_logic.low_power        = 4; */
  /* MATLAB Function 'target_generation_lib/TARG_GEN': '<S59>:1' */
  /*  ----------------------------------------------------------------------- % */
  /* '<S59>:1:17' COM2    = [0; 0; 1]; */
  /* '<S59>:1:18' SP      = [0; 1; 0]; */
  /* '<S59>:1:19' flag    = int8(0.0); */
  /* '<S59>:1:20' tol     = 1e-3; */
  /*  PPT     = [0;1;2];    % these are for testing only */
  /*  SP      = [1;3;0]; */
  /*  Check for zero vectors */
  /* '<S59>:1:26' if( norm(sc2sun_eci) < tol ) */
  if (norm(rtU.sc2sun_unit) < 0.001) {
    /* '<S59>:1:27' flag    = int8(-1); */
    /* '<S59>:1:28' C_cmd   = single(eye(3)); */
    /* '<S59>:1:29' q_cmd   = myDCM2quat(C_cmd); */
    myDCM2quat(b_0, rtb_Merge);

    /* '<S59>:1:30' w_cmd   = single(zeros(3,1)); */
  } else {
    /* '<S59>:1:34' if( (sc_mode == 31 || sc_mode == 32) && flag == 0 ) */
    guard1 = false;
    if ((rtDW.UnitDelay_DSTATE_a == 31) || (rtDW.UnitDelay_DSTATE_a == 32)) {
      /*  Body */
      /* '<S59>:1:37' B1  = COM2./norm(COM2); */
      /* '<S59>:1:38' B2  = SP./norm(SP); */
      /* '<S59>:1:39' Xtb     = B1; */
      /* '<S59>:1:40' Ytb     = cross(B1,B2)/norm(cross(B1,B2)); */
      /* '<S59>:1:41' Ztb     = cross(Xtb,Ytb)/norm(cross(Xtb,Ytb)); */
      /*  Inertial */
      /* '<S59>:1:44' S1  = -r_eci_SEA./norm(r_eci_SEA); */
      rtb_Divide3 = norm_i(rtConstP.r_SEA_Value);

      /* Pointing direction is towards SEA */
      /* '<S59>:1:45' S2  = sc2sun_eci./norm(sc2sun_eci); */
      w_n = norm(rtU.sc2sun_unit);
      S1[0] = -0.409930646F / rtb_Divide3;
      Psi[0] = rtU.sc2sun_unit[0] / w_n;
      S1[1] = 0.538629532F / rtb_Divide3;
      Psi[1] = rtU.sc2sun_unit[1] / w_n;
      S1[2] = -0.736094475F / rtb_Divide3;
      Psi[2] = rtU.sc2sun_unit[2] / w_n;

      /* '<S59>:1:46' Xti     = S1; */
      /* '<S59>:1:47' Yti     = cross(S1,S2)/norm(cross(S1,S2)); */
      S1_1[0] = S1[1] * (real32_T)Psi[2] - S1[2] * (real32_T)Psi[1];
      S1_1[1] = S1[2] * (real32_T)Psi[0] - S1[0] * (real32_T)Psi[2];
      S1_1[2] = S1[0] * (real32_T)Psi[1] - S1[1] * (real32_T)Psi[0];
      rtb_Divide3 = norm_i(S1_1);
      Yti[0] = (S1[1] * (real32_T)Psi[2] - S1[2] * (real32_T)Psi[1]) /
        rtb_Divide3;
      Yti[1] = (S1[2] * (real32_T)Psi[0] - S1[0] * (real32_T)Psi[2]) /
        rtb_Divide3;
      Yti[2] = (S1[0] * (real32_T)Psi[1] - S1[1] * (real32_T)Psi[0]) /
        rtb_Divide3;

      /* '<S59>:1:48' Zti     = cross(Xti,Yti)/norm(cross(Xti,Yti)); */
      S1_0[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
      S1_0[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
      S1_0[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
      rtb_Divide3 = norm_i(S1_0);

      /*  Checks */
      /* '<S59>:1:51' if ( abs(dot(B1,S2)) > 1-tol ) */
      if (fabs((0.0 * Psi[0] + 0.0 * Psi[1]) + Psi[2]) > 0.999) {
        /* '<S59>:1:52' flag    = int8(-1); */
        /* '<S59>:1:53' C_cmd   = single(eye(3)); */
        /* '<S59>:1:54' q_cmd   = myDCM2quat(C_cmd); */
        myDCM2quat(b_0, rtb_Merge);

        /* '<S59>:1:55' w_cmd   = single(zeros(3,1)); */
      } else {
        /*  Stack vectors */
        /* '<S59>:1:60' Cb  = [Xtb, Ytb, Ztb]; */
        /* '<S59>:1:61' Ci  = [Xti, Yti, Zti]; */
        /*  Commanded DCM ( body to inertial ) */
        /* '<S59>:1:64' C_cmd   = single(Ci*Cb'); */
        S1_2[0] = S1[1] * Yti[2] - S1[2] * Yti[1];
        S1_2[1] = S1[2] * Yti[0] - S1[0] * Yti[2];
        S1_2[2] = S1[0] * Yti[1] - S1[1] * Yti[0];
        for (i = 0; i < 3; i++) {
          S1_3[i] = S1[i];
          S1_3[3 + i] = Yti[i];
          S1_3[6 + i] = S1_2[i] / rtb_Divide3;
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
      /* '<S59>:1:65' else */
      /* '<S59>:1:66' C_cmd   = single(eye(3)); */
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
      /* '<S59>:1:70' q_cmd   = myDCM2quat( C_cmd ); */
      myDCM2quat(C_cmd, rtb_Merge);

      /* '<S59>:1:71' w_cmd   = single(zeros(3,1)); */
    }
  }

  /* End of MATLAB Function: '<S10>/TARG_GEN' */

  /* Sum: '<S15>/Sum' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   *  Product: '<S15>/Product'
   *  Product: '<S15>/Product1'
   *  Product: '<S15>/Product2'
   *  Product: '<S15>/Product3'
   *  UnaryMinus: '<S13>/Unary Minus'
   *  UnaryMinus: '<S13>/Unary Minus1'
   *  UnaryMinus: '<S13>/Unary Minus2'
   */
  scale = ((rtb_Product_am * (real32_T)rtb_Merge[0] - -rtb_Product1_h *
            (real32_T)rtb_Merge[1]) - -rtb_Product2_d * (real32_T)rtb_Merge[2])
    - -rtb_Product2_g * (real32_T)rtb_Merge[3];

  /* Saturate: '<S12>/Saturation' */
  if (scale > 1.0) {
    scale = 1.0;
  } else {
    if (scale < -1.0) {
      scale = -1.0;
    }
  }

  /* End of Saturate: '<S12>/Saturation' */

  /* Outport: '<Root>/point_true' incorporates:
   *  Gain: '<S12>/Gain'
   *  Gain: '<S12>/rad2deg'
   *  Switch: '<S12>/Switch'
   *  Trigonometry: '<S12>/Trigonometric Function'
   */
  rtY.point_true = !(2.0 * acos(scale) * 57.295779513082323 >= 20.0);

  /* Outport: '<Root>/sc_above_gsb' incorporates:
   *  Inport: '<Root>/sc_above_gs'
   */
  rtY.sc_above_gsb = rtU.sc_above_gs;

  /* Outport: '<Root>/sc_modeb' incorporates:
   *  UnitDelay: '<S8>/Unit Delay'
   */
  rtY.sc_modeb = rtDW.UnitDelay_DSTATE_a;

  /* Sum: '<S46>/Sum' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   *  Product: '<S46>/Product'
   *  Product: '<S46>/Product1'
   *  Product: '<S46>/Product2'
   *  Product: '<S46>/Product3'
   */
  rtb_Divide3 = (((real32_T)rtb_Merge[0] * (real32_T)rtb_Merge[0] + (real32_T)
                  rtb_Merge[1] * (real32_T)rtb_Merge[1]) + (real32_T)rtb_Merge[2]
                 * (real32_T)rtb_Merge[2]) + (real32_T)rtb_Merge[3] * (real32_T)
    rtb_Merge[3];

  /* Product: '<S43>/Divide' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   */
  rtb_Divide = (real32_T)rtb_Merge[0] / rtb_Divide3;

  /* Product: '<S43>/Divide1' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   *  UnaryMinus: '<S45>/Unary Minus'
   */
  rtb_Divide1 = -(real32_T)rtb_Merge[1] / rtb_Divide3;

  /* Product: '<S43>/Divide2' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   *  UnaryMinus: '<S45>/Unary Minus1'
   */
  rtb_Divide2 = -(real32_T)rtb_Merge[2] / rtb_Divide3;

  /* Product: '<S43>/Divide3' incorporates:
   *  DataTypeConversion: '<S10>/Data Type Conversion'
   *  UnaryMinus: '<S45>/Unary Minus2'
   */
  rtb_Divide3 = -(real32_T)rtb_Merge[3] / rtb_Divide3;

  /* Sum: '<S47>/Sum' incorporates:
   *  Product: '<S47>/Product'
   *  Product: '<S47>/Product1'
   *  Product: '<S47>/Product2'
   *  Product: '<S47>/Product3'
   */
  w_n = ((rtb_Divide * rtb_Product_am - rtb_Divide1 * rtb_Product1_h) -
         rtb_Divide2 * rtb_Product2_d) - rtb_Divide3 * rtb_Product2_g;

  /* Signum: '<S37>/Sign' */
  if (w_n < 0.0) {
    w_n = -1.0;
  } else if (w_n > 0.0) {
    w_n = 1.0;
  } else {
    if (w_n == 0.0) {
      w_n = 0.0;
    }
  }

  /* End of Signum: '<S37>/Sign' */

  /* Product: '<S37>/Product1' incorporates:
   *  Product: '<S48>/Product'
   *  Product: '<S48>/Product1'
   *  Product: '<S48>/Product2'
   *  Product: '<S48>/Product3'
   *  Product: '<S49>/Product'
   *  Product: '<S49>/Product1'
   *  Product: '<S49>/Product2'
   *  Product: '<S49>/Product3'
   *  Product: '<S50>/Product'
   *  Product: '<S50>/Product1'
   *  Product: '<S50>/Product2'
   *  Product: '<S50>/Product3'
   *  Sum: '<S48>/Sum'
   *  Sum: '<S49>/Sum'
   *  Sum: '<S50>/Sum'
   */
  scale = (((rtb_Divide * rtb_Product1_h + rtb_Divide1 * rtb_Product_am) +
            rtb_Divide2 * rtb_Product2_g) - rtb_Divide3 * rtb_Product2_d) * w_n;
  absxk = (((rtb_Divide * rtb_Product2_d - rtb_Divide1 * rtb_Product2_g) +
            rtb_Divide2 * rtb_Product_am) + rtb_Divide3 * rtb_Product1_h) * w_n;
  t = (((rtb_Divide * rtb_Product2_g + rtb_Divide1 * rtb_Product2_d) -
        rtb_Divide2 * rtb_Product1_h) + rtb_Divide3 * rtb_Product_am) * w_n;
  for (i = 0; i < 3; i++) {
    /* Sum: '<S37>/Sum1' incorporates:
     *  Constant: '<S37>/d-gain'
     *  Constant: '<S37>/p-gain'
     *  Product: '<S37>/Matrix Multiply'
     *  Product: '<S37>/Matrix Multiply1'
     *  Reshape: '<S3>/Reshape1'
     *  Sum: '<S37>/Sum'
     */
    Psi[i] = (rtConstP.pgain_Value[i + 6] * t + (rtConstP.pgain_Value[i + 3] *
               absxk + rtConstP.pgain_Value[i] * scale)) +
      (rtConstP.dgain_Value[i + 6] * rtb_Switch[2] + (rtConstP.dgain_Value[i + 3]
        * rtb_Switch[1] + rtConstP.dgain_Value[i] * rtb_Switch[0]));
  }

  /* Sqrt: '<S37>/Sqrt' incorporates:
   *  DotProduct: '<S37>/Dot Product'
   */
  w_n = sqrt((rtb_DataTypeConversion1[0] * rtb_DataTypeConversion1[0] +
              rtb_DataTypeConversion1[1] * rtb_DataTypeConversion1[1]) +
             rtb_DataTypeConversion1[2] * rtb_DataTypeConversion1[2]);

  /* Outputs for Atomic SubSystem: '<S6>/sun_point_lib' */

  /* Inport: '<Root>/solar_panel_power_W' incorporates:
   *  Reshape: '<S3>/Reshape1'
   */
  sun_point_lib(rtU.solar_panel_power_W, rtb_DataTypeConversion2, rtb_Switch,
                rtb_DataTypeConversion1, RateTransition4, &rtDW.sun_point_lib_j);

  /* End of Outputs for SubSystem: '<S6>/sun_point_lib' */

  /* MATLAB Function: '<S35>/control_selection' incorporates:
   *  UnitDelay: '<S8>/Unit Delay'
   */
  /* CONTROL SELECTION LOGIC */
  /*    HuskySat-1, ADCS Team */
  /*    Last Update: T. Reynolds, 11.21.17 */
  /* MATLAB Function 'command_generation/Control Selection/control_selection': '<S39>:1' */
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
  /* '<S39>:1:21' if( mode == 1 ) */
  switch (rtDW.UnitDelay_DSTATE_a) {
   case 1:
    /* '<S39>:1:22' cmd_MT_out      = zeros(3,1); */
    RateTransition4[0] = 0.0;
    RateTransition4[1] = 0.0;
    RateTransition4[2] = 0.0;

    /* '<S39>:1:23' FLAG            = int8(1.0); */
    break;

   case 2:
    /* '<S39>:1:24' elseif( mode == 2 ) */
    /* '<S39>:1:25' cmd_MT_out      = cmd_sun_point; */
    /* '<S39>:1:26' FLAG            = int8(2.0); */
    break;

   default:
    if ((rtDW.UnitDelay_DSTATE_a == 31) || (rtDW.UnitDelay_DSTATE_a == 32) ||
        (rtDW.UnitDelay_DSTATE_a == 33)) {
      /* Saturate: '<S37>/Saturation1' incorporates:
       *  Product: '<S37>/Divide'
       *  Product: '<S41>/Element product'
       *  Sum: '<S41>/Add3'
       */
      /* '<S39>:1:27' elseif( mode == 31 || mode == 32 || mode == 33 ) */
      /* '<S39>:1:28' cmd_MT_out      = cmd_MT; */
      scale = (Psi[1] * rtb_DataTypeConversion1[2] - Psi[2] *
               rtb_DataTypeConversion1[1]) / w_n;
      if (scale > 0.15) {
        RateTransition4[0] = 0.15;
      } else if (scale < -0.15) {
        RateTransition4[0] = -0.15;
      } else {
        RateTransition4[0] = scale;
      }

      /* End of Saturate: '<S37>/Saturation1' */

      /* Saturate: '<S37>/Saturation2' incorporates:
       *  Product: '<S37>/Divide'
       *  Product: '<S41>/Element product'
       *  Sum: '<S41>/Add3'
       */
      scale = (Psi[2] * rtb_DataTypeConversion1[0] - Psi[0] *
               rtb_DataTypeConversion1[2]) / w_n;
      if (scale > 0.15) {
        RateTransition4[1] = 0.15;
      } else if (scale < -0.15) {
        RateTransition4[1] = -0.15;
      } else {
        RateTransition4[1] = scale;
      }

      /* End of Saturate: '<S37>/Saturation2' */

      /* Saturate: '<S37>/Saturation3' incorporates:
       *  Product: '<S37>/Divide'
       *  Product: '<S41>/Element product'
       *  Sum: '<S41>/Add3'
       */
      scale = (Psi[0] * rtb_DataTypeConversion1[1] - Psi[1] *
               rtb_DataTypeConversion1[0]) / w_n;
      if (scale > 0.15) {
        RateTransition4[2] = 0.15;
      } else if (scale < -0.15) {
        RateTransition4[2] = -0.15;
      } else {
        RateTransition4[2] = scale;
      }

      /* End of Saturate: '<S37>/Saturation3' */
      /* '<S39>:1:29' FLAG            = int8(mode); */
    } else if (rtDW.UnitDelay_DSTATE_a == 4) {
      /* '<S39>:1:30' elseif( mode == 4 ) */
      /* '<S39>:1:31' cmd_MT_out      = zeros(3,1); */
      RateTransition4[0] = 0.0;
      RateTransition4[1] = 0.0;
      RateTransition4[2] = 0.0;

      /* '<S39>:1:32' FLAG            = int8(4.0); */
    } else {
      /* '<S39>:1:33' else */
      /* '<S39>:1:34' FLAG            = int8(-1); */
      /* '<S39>:1:35' cmd_MT_out      = zeros(3,1); */
      RateTransition4[0] = 0.0;
      RateTransition4[1] = 0.0;
      RateTransition4[2] = 0.0;
    }
    break;
  }

  /* End of MATLAB Function: '<S35>/control_selection' */

  /* Gain: '<S40>/To DigVal1' */
  scale = 666.66666666666663 * RateTransition4[0];

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
  scale = 666.66666666666663 * RateTransition4[1];

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
  scale = 666.66666666666663 * RateTransition4[2];

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

  /* MATLAB Function: '<S20>/state_transition' incorporates:
   *  Constant: '<S20>/Constant'
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
    scale = sin(w_n * 0.1);
    t = cos(w_n * 0.1);
    absxk = w_n * w_n;

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
      rtb_K[6 * i] = ((real_T)I[3 * i] - rotation_matrix[3 * i] * scale / w_n) +
        rotation_matrix_0[3 * i] * (1.0 - t) / absxk;
      rtb_K[6 * (i + 3)] = (rotation_matrix[3 * i] * (1.0 - c_x) / b_c - (real_T)
                            b_a[3 * i] * 0.1) - rotation_matrix_1[3 * i] * b_1 /
        c_c;
      rtb_K[3 + 6 * i] = 0.0;
      rtb_K[3 + 6 * (i + 3)] = Phi_22[3 * i];
      rtb_K[1 + 6 * i] = ((real_T)I[3 * i + 1] - rotation_matrix[3 * i + 1] *
                          scale / w_n) + rotation_matrix_0[3 * i + 1] * (1.0 - t)
        / absxk;
      rtb_K[1 + 6 * (i + 3)] = (rotation_matrix[3 * i + 1] * (1.0 - c_x) / b_c -
        (real_T)b_a[3 * i + 1] * 0.1) - rotation_matrix_1[3 * i + 1] * b_1 / c_c;
      rtb_K[4 + 6 * i] = 0.0;
      rtb_K[4 + 6 * (i + 3)] = Phi_22[3 * i + 1];
      rtb_K[2 + 6 * i] = ((real_T)I[3 * i + 2] - rotation_matrix[3 * i + 2] *
                          scale / w_n) + rotation_matrix_0[3 * i + 2] * (1.0 - t)
        / absxk;
      rtb_K[2 + 6 * (i + 3)] = (rotation_matrix[3 * i + 2] * (1.0 - c_x) / b_c -
        (real_T)b_a[3 * i + 2] * 0.1) - rotation_matrix_1[3 * i + 2] * b_1 / c_c;
      rtb_K[5 + 6 * i] = 0.0;
      rtb_K[5 + 6 * (i + 3)] = Phi_22[3 * i + 2];
    }
  }

  /* End of MATLAB Function: '<S20>/state_transition' */

  /* Abs: '<S8>/Abs' incorporates:
   *  Reshape: '<S3>/Reshape1'
   */
  /* MATLAB Function 'Estimation_EKF /3_sig_bnd ': '<S19>:1' */
  /* '<S19>:1:3' bnd = cov; */
  Psi[0] = fabs(rtb_Switch[0]);
  Psi[1] = fabs(rtb_Switch[1]);
  Psi[2] = fabs(rtb_Switch[2]);

  /* MinMax: '<S8>/MinMax' */
  if ((Psi[0] >= Psi[1L]) || rtIsNaN(Psi[1L])) {
    w_n = Psi[0];
  } else {
    w_n = Psi[1L];
  }

  if (!((w_n >= Psi[2L]) || rtIsNaN(Psi[2L]))) {
    w_n = Psi[2L];
  }

  /* Relay: '<S8>/Relay' incorporates:
   *  MinMax: '<S8>/MinMax'
   */
  if (w_n >= 0.12) {
    rtDW.Relay_Mode = true;
  } else {
    if (w_n <= 0.00436) {
      rtDW.Relay_Mode = false;
    }
  }

  /* MATLAB Function: '<S8>/MATLAB Function' incorporates:
   *  Inport: '<Root>/CAN_IN'
   *  Inport: '<Root>/sc_above_gs'
   *  Inport: '<Root>/sc_in_sun'
   *  Inport: '<Root>/sun_vec_body_sunsensor'
   *  Relay: '<S8>/Relay'
   *  UnitDelay: '<S8>/Unit Delay'
   */
  /*  ----------------------------------------------------------------------- % */
  /*  Main FSW Mode Selector */
  /*    UW HuskySat-1, ADCS Team */
  /*    Last Updated: T. Reynolds 2.15.18 */
  /*  */
  /*  Chooses control mode based on sensor and actuator data. */
  /*    mode_logic.bdot             = 1; */
  /*    mode_logic.seek_sun         = 2; */
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
  /*  ss_valid: - Valid bit from the sun sensor indicating whether or not the */
  /*  sun is in our FoV. Type: 1x1 boolean. */
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
  /*  sc_mode - Spacecraft state determined. Type: int8. */
  /*  */
  /*  #codegen */
  /*  ----------------------------------------------------------------------- % */
  /* MATLAB Function 'mode_selecction/MATLAB Function': '<S58>:1' */
  /* '<S58>:1:56' tol     = 1e-3; */
  /*  Default to last known condition and no experimental control */
  /* '<S58>:1:58' sc_mode     = sc_mode_last; */
  rtb_sc_mode = rtDW.UnitDelay_DSTATE_a;

  /* '<S58>:1:60' if( CAN(1) == 0 ) */
  if (rtU.CAN_IN[0] == 0) {
    /*  Check for bdot condition */
    /* '<S58>:1:63' if( bdot_flag == 1 ) */
    if (rtDW.Relay_Mode) {
      /* '<S58>:1:64' sc_mode     = int8(1.0); */
      rtb_sc_mode = 1;

      /*  If Bdot is triggered, return immediately since control has high  */
      /*  priority and control will be handed off. */
    } else {
      /*  Bdot flag will go low once omega_radps drops below the lower */
      /*  threshold. Accounted for in the relay block. */
      /*  Check for low power condition or recent boot up */
      /* '<S58>:1:74' if( CAN(2) == 1 || (sc_mode == 1 && bdot_flag == 0) ) */
      if ((rtU.CAN_IN[1] == 1) || (rtDW.UnitDelay_DSTATE_a == 1)) {
        /*  If reboot/bdot just occured and we are not tumbling, enter low */
        /*  power/estimate mode. */
        /* '<S58>:1:77' sc_mode     = int8(4.0); */
        rtb_sc_mode = 4;
      }

      /*  Check if we have the sun in our FoV. If not, enter reorientation  */
      /*  mode to acquire sun. This has high priority if we are not tumbling. */
      /* '<S58>:1:82' if( sc_in_sun && ( abs(ss_valid-1) > tol ) ) */
      if (rtU.sc_in_sun && ((real32_T)fabs(rtU.sun_vec_body_sunsensor[3] - 1.0F)
                            > 0.001)) {
        /* '<S58>:1:83' sc_mode     = int8(2.0); */
        rtb_sc_mode = 2;
      } else {
        /*  If all cases upstream have been passed, then enter pointing mode if */
        /*  allowed to do so. Check for pointing condition from CAN */
        /* '<S58>:1:89' if( CAN(3) == 1 ) */
        if (rtU.CAN_IN[2] == 1) {
          /*  Default to velocity pointing mode */
          /* '<S58>:1:91' sc_mode     = int8(31.0); */
          rtb_sc_mode = 31;

          /*  Check to see if entering GS range, not in eclipse and */
          /*  COM2 system will be activated. */
          /* '<S58>:1:95' if( GS_approach && sc_in_sun && (CAN(4)==1) ) */
          if (rtU.sc_above_gs && rtU.sc_in_sun && (rtU.CAN_IN[3] == 1)) {
            /* '<S58>:1:96' sc_mode  = int8(32.0); */
            rtb_sc_mode = 32;
          }
        }

        /*  If signal received to override the autonomous mode selection, set mode  */
        /*  based on CAN(1) value. */
      }
    }
  } else {
    if (rtU.CAN_IN[0] != 0) {
      /* '<S58>:1:103' elseif( CAN(1) ~= 0 ) */
      /* '<S58>:1:105' sc_mode = CAN(1); */
      rtb_sc_mode = rtU.CAN_IN[0];
    }
  }

  /* End of MATLAB Function: '<S8>/MATLAB Function' */

  /* Update for UnitDelay: '<S3>/Unit Delay' */
  rtDW.UnitDelay_DSTATE[0] = rtb_Product_am;
  rtDW.UnitDelay_DSTATE[1] = rtb_Product1_h;
  rtDW.UnitDelay_DSTATE[2] = rtb_Product2_d;
  rtDW.UnitDelay_DSTATE[3] = rtb_Product2_g;

  /* Update for UnitDelay: '<S3>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE[0] = rtb_Merge1_idx_0;
  rtDW.UnitDelay1_DSTATE[1] = rtb_Merge1_idx_1;
  rtDW.UnitDelay1_DSTATE[2] = rtb_Merge1_idx_2;
  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      /* Product: '<S20>/Product' incorporates:
       *  Math: '<S20>/Math Function'
       */
      rtb_Merge2_0[i + 6 * k] = 0.0;

      /* Product: '<S20>/Product1' incorporates:
       *  Constant: '<S20>/Constant3'
       *  Constant: '<S20>/G'
       *  Math: '<S20>/Math Function1'
       */
      KHI[i + 6 * k] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        /* Product: '<S20>/Product' incorporates:
         *  Math: '<S20>/Math Function'
         */
        rtb_Merge2_0[i + 6 * k] += rtb_Merge2[6 * i_0 + i] * rtb_K[6 * i_0 + k];

        /* Product: '<S20>/Product1' incorporates:
         *  Constant: '<S20>/Constant3'
         *  Constant: '<S20>/G'
         *  Math: '<S20>/Math Function1'
         */
        KHI[i + 6 * k] += rtConstP.Constant3_Value[6 * i_0 + i] *
          rtConstP.G_Value[6 * i_0 + k];
      }
    }
  }

  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      /* Product: '<S20>/Product' incorporates:
       *  Sum: '<S20>/Add'
       */
      rtb_Merge2_1[k + 6 * i] = 0.0;

      /* Product: '<S20>/Product1' incorporates:
       *  Constant: '<S20>/G'
       *  Sum: '<S20>/Add'
       */
      rtb_Merge2[k + 6 * i] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        /* Product: '<S20>/Product' incorporates:
         *  Sum: '<S20>/Add'
         */
        rtb_Merge2_1[k + 6 * i] += rtb_K[6 * i_0 + k] * rtb_Merge2_0[6 * i + i_0];

        /* Product: '<S20>/Product1' incorporates:
         *  Constant: '<S20>/G'
         *  Sum: '<S20>/Add'
         */
        rtb_Merge2[k + 6 * i] += rtConstP.G_Value[6 * i_0 + k] * KHI[6 * i + i_0];
      }
    }
  }

  /* Update for UnitDelay: '<S3>/Unit Delay2' incorporates:
   *  Sum: '<S20>/Add'
   */
  for (i = 0; i < 6; i++) {
    for (k = 0; k < 6; k++) {
      rtDW.UnitDelay2_DSTATE[k + 6 * i] = rtb_Merge2_1[6 * i + k] + rtb_Merge2[6
        * i + k];
    }
  }

  /* End of Update for UnitDelay: '<S3>/Unit Delay2' */

  /* Update for UnitDelay: '<S8>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_a = rtb_sc_mode;
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

  /* InitializeConditions for UnitDelay: '<S8>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_a = 1;

  /* SystemInitialize for Atomic SubSystem: '<S6>/sun_point_lib' */
  sun_point_lib_Init(&rtDW.sun_point_lib_j);

  /* End of SystemInitialize for SubSystem: '<S6>/sun_point_lib' */
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
