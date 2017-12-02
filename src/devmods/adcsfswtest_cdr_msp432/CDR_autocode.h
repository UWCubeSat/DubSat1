/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CDR_autocode.h
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

#ifndef RTW_HEADER_CDR_autocode_h_
#define RTW_HEADER_CDR_autocode_h_
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef CDR_autocode_COMMON_INCLUDES_
# define CDR_autocode_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* CDR_autocode_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T OutportBufferForcp13[13];
  real_T OutportBufferForsp13[13];
  real_T Sum2_m[169];                  /* '<S70>/Sum2' */
  real_T Assignment_o[169];            /* '<S69>/Assignment' */
  real_T Assignment_snorm[169];        /* '<S69>/Assignment_snorm' */
  real_T Assignment2_k[13];            /* '<S76>/Assignment2' */
  real_T Assignment2_j[13];            /* '<S75>/Assignment2' */
  real_T UnitDelay_DSTATE[4];          /* '<S2>/Unit Delay' */
  real_T UnitDelay1_DSTATE[3];         /* '<S2>/Unit Delay1' */
  real_T UnitDelay2_DSTATE[36];        /* '<S2>/Unit Delay2' */
  real_T UnitDelay1_DSTATE_l[2];       /* '<S108>/Unit Delay1' */
  real_T UnitDelay_DSTATE_b[169];      /* '<S70>/Unit Delay' */
  real_T UnitDelay_DSTATE_g[169];      /* '<S97>/Unit Delay' */
  real_T UnitDelay_DSTATE_a[169];      /* '<S69>/Unit Delay' */
  real_T UnitDelay1_DSTATE_m[169];     /* '<S69>/Unit Delay1' */
  real_T UnitDelay1_DSTATE_i[13];      /* '<S71>/Unit Delay1' */
  real_T sqrt_a;                       /* '<S60>/sqrt' */
  real_T Product11;                    /* '<S100>/Product11' */
  real_T Product4;                     /* '<S101>/Product4' */
  real_T Product12;                    /* '<S106>/Product12' */
  real_T sqrt_n;                       /* '<S107>/sqrt' */
  real_T Sum1;                         /* '<S68>/Sum1' */
  real_T Sum2;                         /* '<S68>/Sum2' */
  real_T Sum3;                         /* '<S68>/Sum3' */
  real_T Sum5;                         /* '<S68>/Sum5' */
  real_T Merge1_b;                     /* '<S69>/Merge1' */
  real_T Merge_n;                      /* '<S69>/Merge' */
  real_T Product2;                     /* '<S71>/Product2' */
  real_T UnitDelay1_DSTATE_k;          /* '<S68>/Unit Delay1' */
  real_T UnitDelay3_DSTATE;            /* '<S68>/Unit Delay3' */
  real_T UnitDelay2_DSTATE_h;          /* '<S68>/Unit Delay2' */
  real_T UnitDelay4_DSTATE;            /* '<S68>/Unit Delay4' */
  real_T otime_PreviousInput;          /* '<S65>/otime' */
  real_T olon_PreviousInput;           /* '<S64>/olon' */
  real_T olat_PreviousInput;           /* '<S63>/olat' */
  real_T oalt_PreviousInput;           /* '<S63>/oalt' */
  real_T Memory_PreviousInput;         /* '<S37>/Memory' */
  uint8_T ForIterator_IterationMarker[7];/* '<S67>/For Iterator' */
  boolean_T SpecialcaseNorthSouthGeographic;/* '<S68>/Special case - North//South Geographic Pole' */
} DW;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T Product1_k[36];         /* '<S5>/Product1' */
} ConstB;

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Expression: k)
   * Referenced by:
   *   '<S85>/k[13][13]'
   *   '<S76>/k[13][13]'
   */
  real_T pooled5[169];

  /* Pooled Parameter (Expression: snorm)
   * Referenced by:
   *   '<S69>/snorm[169]'
   *   '<S69>/Unit Delay1'
   */
  real_T pooled8[169];

  /* Expression: c
   * Referenced by: '<S70>/c[maxdef][maxdef]'
   */
  real_T cmaxdefmaxdef_Value[169];

  /* Expression: cd
   * Referenced by: '<S70>/cd[maxdef][maxdef]'
   */
  real_T cdmaxdefmaxdef_Value[169];

  /* Expression: fm
   * Referenced by: '<S68>/fm'
   */
  real_T fm_Value[13];

  /* Expression: fn
   * Referenced by: '<S68>/fn'
   */
  real_T fn_Value[13];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S108>/sp[11]'
   *   '<S108>/cp[11]'
   *   '<S108>/Constant'
   *   '<S108>/Constant1'
   */
  real_T pooled11[11];

  /* Expression: [0 0 (1:(maxdef-1))]
   * Referenced by: '<S61>/sp[13]'
   */
  real_T sp13_Y0[13];

  /* Expression: [1 1 (1:(maxdef-1))]
   * Referenced by: '<S61>/cp[13]'
   */
  real_T cp13_Y0[13];

  /* Expression: fsw_params.estimation.ic.error_cov
   * Referenced by: '<S2>/Unit Delay2'
   */
  real_T UnitDelay2_InitialCondition[36];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T orbit_tle[9];                 /* '<Root>/orbit_tle' */
  real_T gps_time[2];                  /* '<Root>/gps_time' */
  real_T valid_gyro;                   /* '<Root>/valid_gyro' */
  boolean_T valid_mag;                 /* '<Root>/valid_mag' */
  real_T valid_sun;                    /* '<Root>/valid_sun' */
  real_T w_body_radps[3];              /* '<Root>/w_body_radps' */
  real_T meas_cov[36];                 /* '<Root>/meas_cov' */
  real_T mag_body[3];                  /* '<Root>/mag_body' */
  real_T sun_body[3];                  /* '<Root>/sun_body' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T quat_hat[4];                  /* '<Root>/quat_hat' */
  real_T w_body_hat_radps[3];          /* '<Root>/w_body_hat_radps' */
  real_T sc_in_sun;                    /* '<Root>/sc_in_sun' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (auto storage) */
extern DW rtDW;

/* External inputs (root inport signals with auto storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY rtY;
extern const ConstB rtConstB;          /* constant block i/o */

/* Constant parameters (auto storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void CDR_autocode_initialize(void);
extern void CDR_autocode_step(void);
extern void CDR_autocode_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Rate Transition10' : Unused code path elimination
 * Block '<S2>/Reshape2' : Unused code path elimination
 * Block '<S2>/Reshape3' : Unused code path elimination
 * Block '<S3>/Gain1' : Unused code path elimination
 * Block '<S3>/Product2' : Unused code path elimination
 * Block '<S3>/Rate Transition11' : Unused code path elimination
 * Block '<S49>/Assertion' : Unused code path elimination
 * Block '<S49>/conjunction' : Unused code path elimination
 * Block '<S49>/max_relop' : Unused code path elimination
 * Block '<S49>/max_val' : Unused code path elimination
 * Block '<S49>/min_relop' : Unused code path elimination
 * Block '<S49>/min_val' : Unused code path elimination
 * Block '<S50>/Assertion' : Unused code path elimination
 * Block '<S50>/conjunction' : Unused code path elimination
 * Block '<S50>/max_relop' : Unused code path elimination
 * Block '<S50>/max_val' : Unused code path elimination
 * Block '<S50>/min_relop' : Unused code path elimination
 * Block '<S50>/min_val' : Unused code path elimination
 * Block '<S51>/Assertion' : Unused code path elimination
 * Block '<S51>/conjunction' : Unused code path elimination
 * Block '<S51>/max_relop' : Unused code path elimination
 * Block '<S51>/max_val' : Unused code path elimination
 * Block '<S51>/min_relop' : Unused code path elimination
 * Block '<S51>/min_val' : Unused code path elimination
 * Block '<S53>/Assertion' : Unused code path elimination
 * Block '<S53>/conjunction' : Unused code path elimination
 * Block '<S53>/max_relop' : Unused code path elimination
 * Block '<S53>/max_val' : Unused code path elimination
 * Block '<S53>/maxtype' : Unused code path elimination
 * Block '<S53>/min_relop' : Unused code path elimination
 * Block '<S53>/min_val' : Unused code path elimination
 * Block '<S53>/mintype' : Unused code path elimination
 * Block '<S55>/Unit Conversion' : Unused code path elimination
 * Block '<S56>/Unit Conversion' : Unused code path elimination
 * Block '<S2>/ ' : Eliminated since input and output rates are identical
 * Block '<S2>/ 1' : Eliminated since input and output rates are identical
 * Block '<S2>/ 10' : Eliminated since input and output rates are identical
 * Block '<S2>/ 11' : Eliminated since input and output rates are identical
 * Block '<S2>/ 2' : Eliminated since input and output rates are identical
 * Block '<S2>/ 3' : Eliminated since input and output rates are identical
 * Block '<S2>/ 4' : Eliminated since input and output rates are identical
 * Block '<S2>/ 5' : Eliminated since input and output rates are identical
 * Block '<S2>/ 6' : Eliminated since input and output rates are identical
 * Block '<S2>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S2>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S2>/Reshape1' : Reshape block reduction
 * Block '<S2>/Reshape4' : Reshape block reduction
 * Block '<S3>/Rate Transition10' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition12' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition13' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition14' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition15' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition16' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition8' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition9' : Eliminated since input and output rates are identical
 * Block '<S48>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S19>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S54>/Unit Conversion' : Eliminated nontunable gain of 1
 * Block '<S26>/Unit Conversion2' : Eliminated nontunable gain of 1
 * Block '<S76>/Reshape' : Reshape block reduction
 * Block '<S83>/Reshape' : Reshape block reduction
 * Block '<S84>/Reshape' : Reshape block reduction
 * Block '<S85>/Reshape' : Reshape block reduction
 * Block '<S85>/Reshape1' : Reshape block reduction
 * Block '<S61>/Gain' : Eliminated nontunable gain of 1
 * Block '<S61>/Gain1' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode')    - opens subsystem adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/MSP_FSW'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF '
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors'
 * '<S4>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /3_sig_bnd '
 * '<S5>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step '
 * '<S6>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step '
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step /Quaternion Normalize'
 * '<S8>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step /propagate_quat'
 * '<S9>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step /state_transition'
 * '<S10>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus'
 * '<S11>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Propagate Step /Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S12>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem'
 * '<S13>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem1'
 * '<S14>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem/convert_inertial_body'
 * '<S15>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem/covariance_update'
 * '<S16>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem/kalman_gain'
 * '<S17>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem/observation_matrix'
 * '<S18>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/Estimation_EKF /Update Step /If Action Subsystem/update_state '
 * '<S19>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector'
 * '<S20>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/sgp4_lib_fsw'
 * '<S21>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/sun_vector_lib'
 * '<S22>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries'
 * '<S23>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA'
 * '<S24>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED'
 * '<S25>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/Normalize Vector'
 * '<S26>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015'
 * '<S27>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/YMDHMS_2_dec_year_lib'
 * '<S28>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/Conversion'
 * '<S29>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/Subsystem2'
 * '<S30>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/Subsystem3'
 * '<S31>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/While Iterator Subsystem'
 * '<S32>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/e^1'
 * '<S33>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/e^2'
 * '<S34>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/e^3'
 * '<S35>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem2'
 * '<S36>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem3'
 * '<S37>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem4'
 * '<S38>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A11'
 * '<S39>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A12'
 * '<S40>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A13'
 * '<S41>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A21'
 * '<S42>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A22'
 * '<S43>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A23'
 * '<S44>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A31'
 * '<S45>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A32'
 * '<S46>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/A33'
 * '<S47>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/Angle Conversion'
 * '<S48>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/ECEFtoNED/Create Transformation Matrix'
 * '<S49>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Check Altitude'
 * '<S50>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Check Latitude'
 * '<S51>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Check Longitude'
 * '<S52>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field'
 * '<S53>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Is time within model limits'
 * '<S54>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Length Conversion'
 * '<S55>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/MagField Conversion'
 * '<S56>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/MagField Conversion1'
 * '<S57>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag'
 * '<S58>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field/Angle Conversion'
 * '<S59>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates'
 * '<S60>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates'
 * '<S61>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates '
 * '<S62>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude'
 * '<S63>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Has altitude or latitude changed'
 * '<S64>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Has longitude changed '
 * '<S65>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Has time changed'
 * '<S66>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity'
 * '<S67>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem'
 * '<S68>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion'
 * '<S69>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations'
 * '<S70>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients'
 * '<S71>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole'
 * '<S72>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate  index'
 * '<S73>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values'
 * '<S74>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/special case'
 * '<S75>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem1'
 * '<S76>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2'
 * '<S77>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  indices'
 * '<S78>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  row and column'
 * '<S79>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem'
 * '<S80>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1'
 * '<S81>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/m,n'
 * '<S82>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/n,m-1'
 * '<S83>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem'
 * '<S84>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1'
 * '<S85>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2'
 * '<S86>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/calculate  index'
 * '<S87>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  index'
 * '<S88>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  row and column'
 * '<S89>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  index'
 * '<S90>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  row and column'
 * '<S91>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  indices'
 * '<S92>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column1'
 * '<S93>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column2'
 * '<S94>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2'
 * '<S95>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2 '
 * '<S96>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/If Action Subsystem'
 * '<S97>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)'
 * '<S98>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem1'
 * '<S99>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem2'
 * '<S100>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ca'
 * '<S101>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ct'
 * '<S102>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate d'
 * '<S103>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q'
 * '<S104>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q2'
 * '<S105>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate r2'
 * '<S106>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate sa'
 * '<S107>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate st'
 * '<S108>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates /For Iterator Subsystem'
 * '<S109>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude/Angle Conversion2'
 * '<S110>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bx'
 * '<S111>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate by'
 * '<S112>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bz'
 * '<S113>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity'
 * '<S114>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion'
 * '<S115>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion1'
 * '<S116>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/mag_vector/YMDHMS_2_dec_year_lib/MATLAB Function'
 * '<S117>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/sgp4_lib_fsw/MATLAB Function'
 * '<S118>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/sun_vector_lib/MATLAB Function'
 * '<S119>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/sun_vector_lib/MATLAB Function1'
 * '<S120>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/coord_rotations_lib'
 * '<S121>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/gps_to_tt'
 * '<S122>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/gps_to_ut1'
 * '<S123>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/coord_rotations_lib/MATLAB Function'
 * '<S124>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/gps_to_tt/Translate to TT'
 * '<S125>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/CDR_autocode/estimate_eci_vectors/time and coord rotation libraries/gps_to_ut1/Translate to UT1'
 */
#endif                                 /* RTW_HEADER_CDR_autocode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
