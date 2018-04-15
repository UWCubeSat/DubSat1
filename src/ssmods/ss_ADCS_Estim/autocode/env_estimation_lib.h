/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: env_estimation_lib.h
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

#ifndef RTW_HEADER_env_estimation_lib_h_
#define RTW_HEADER_env_estimation_lib_h_
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef env_estimation_lib_COMMON_INCLUDES_
# define env_estimation_lib_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* env_estimation_lib_COMMON_INCLUDES_ */

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
  real_T Sum2_o[169];                  /* '<S98>/Sum2' */
  real_T Assignment_k[169];            /* '<S97>/Assignment' */
  real_T Assignment_snorm[169];        /* '<S97>/Assignment_snorm' */
  real_T Assignment2_j[13];            /* '<S104>/Assignment2' */
  real_T Assignment2_p[13];            /* '<S103>/Assignment2' */
  real_T UnitDelay1_DSTATE[2];         /* '<S136>/Unit Delay1' */
  real_T UnitDelay_DSTATE_p[169];      /* '<S98>/Unit Delay' */
  real_T UnitDelay_DSTATE_g[169];      /* '<S125>/Unit Delay' */
  real_T UnitDelay_DSTATE_k[169];      /* '<S97>/Unit Delay' */
  real_T UnitDelay1_DSTATE_g[169];     /* '<S97>/Unit Delay1' */
  real_T UnitDelay1_DSTATE_j[13];      /* '<S99>/Unit Delay1' */
  real_T sqrt_o;                       /* '<S88>/sqrt' */
  real_T Product11;                    /* '<S128>/Product11' */
  real_T Product4;                     /* '<S129>/Product4' */
  real_T Product12;                    /* '<S134>/Product12' */
  real_T sqrt_od;                      /* '<S135>/sqrt' */
  real_T Sum1;                         /* '<S96>/Sum1' */
  real_T Sum2;                         /* '<S96>/Sum2' */
  real_T Sum3;                         /* '<S96>/Sum3' */
  real_T Sum5;                         /* '<S96>/Sum5' */
  real_T Merge1_f;                     /* '<S97>/Merge1' */
  real_T Merge_d;                      /* '<S97>/Merge' */
  real_T Product2;                     /* '<S99>/Product2' */
  real_T UnitDelay1_DSTATE_k;          /* '<S96>/Unit Delay1' */
  real_T UnitDelay3_DSTATE;            /* '<S96>/Unit Delay3' */
  real_T UnitDelay2_DSTATE_d;          /* '<S96>/Unit Delay2' */
  real_T UnitDelay4_DSTATE;            /* '<S96>/Unit Delay4' */
  real_T otime_PreviousInput;          /* '<S93>/otime' */
  real_T olon_PreviousInput;           /* '<S92>/olon' */
  real_T olat_PreviousInput;           /* '<S91>/olat' */
  real_T oalt_PreviousInput;           /* '<S91>/oalt' */
  real_T Memory_PreviousInput;         /* '<S76>/Memory' */
  uint8_T ForIterator_IterationMarker[7];/* '<S95>/For Iterator' */
  boolean_T SpecialcaseNorthSouthGeographic;/* '<S96>/Special case - North//South Geographic Pole' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Expression: k)
   * Referenced by:
   *   '<S113>/k[13][13]'
   *   '<S104>/k[13][13]'
   */
  real_T pooled9[169];

  /* Pooled Parameter (Expression: snorm)
   * Referenced by:
   *   '<S97>/snorm[169]'
   *   '<S97>/Unit Delay1'
   */
  real_T pooled12[169];

  /* Expression: c
   * Referenced by: '<S98>/c[maxdef][maxdef]'
   */
  real_T cmaxdefmaxdef_Value[169];

  /* Expression: cd
   * Referenced by: '<S98>/cd[maxdef][maxdef]'
   */
  real_T cdmaxdefmaxdef_Value[169];

  /* Expression: fm
   * Referenced by: '<S96>/fm'
   */
  real_T fm_Value[13];

  /* Expression: fn
   * Referenced by: '<S96>/fn'
   */
  real_T fn_Value[13];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S136>/sp[11]'
   *   '<S136>/cp[11]'
   *   '<S136>/Constant'
   *   '<S136>/Constant1'
   */
  real_T pooled15[11];

  /* Expression: [0 0 (1:(maxdef-1))]
   * Referenced by: '<S89>/sp[13]'
   */
  real_T sp13_Y0[13];

  /* Expression: [1 1 (1:(maxdef-1))]
   * Referenced by: '<S89>/cp[13]'
   */
  real_T cp13_Y0[13];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T orbit_tle[9];                 /* '<Root>/orbit_tle' */
  real_T MET;                          /* '<Root>/MET' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T sc_above_gs;                  /* '<Root>/sc_above_gs' */
  real_T sc_in_fov;                    /* '<Root>/sc_in_fov' */
  real_T sc2gs_unit[3];                /* '<Root>/sc2gs_unit' */
  real_T sc_in_sun;                    /* '<Root>/sc_in_sun' */
  real_T sc2sun_unit[3];               /* '<Root>/sc2sun_unit' */
  real_T mag_unit_vector_eci[3];       /* '<Root>/mag_unit_vector_eci' */
  real_T mag_vector_eci[3];            /* '<Root>/mag_vector_eci' */
  real_T vel_eci_mps[3];               /* '<Root>/vel_eci_mps' */
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

/* Constant parameters (auto storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void env_estimation_lib_initialize(void);
extern void env_estimation_lib_step(void);
extern void env_estimation_lib_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Rate Transition10' : Unused code path elimination
 * Block '<S83>/Unit Conversion' : Unused code path elimination
 * Block '<S84>/Unit Conversion' : Unused code path elimination
 * Block '<S1>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition5' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition6' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition7' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition8' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition9' : Eliminated since input and output rates are identical
 * Block '<S27>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S38>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S66>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S81>/maxtype' : Eliminate redundant data type conversion
 * Block '<S81>/mintype' : Eliminate redundant data type conversion
 * Block '<S82>/Unit Conversion' : Eliminated nontunable gain of 1
 * Block '<S54>/Unit Conversion2' : Eliminated nontunable gain of 1
 * Block '<S104>/Reshape' : Reshape block reduction
 * Block '<S111>/Reshape' : Reshape block reduction
 * Block '<S112>/Reshape' : Reshape block reduction
 * Block '<S113>/Reshape' : Reshape block reduction
 * Block '<S113>/Reshape1' : Reshape block reduction
 * Block '<S89>/Gain' : Eliminated nontunable gain of 1
 * Block '<S89>/Gain1' : Eliminated nontunable gain of 1
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
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib')    - opens subsystem adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/MSP_FSW'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/MET_2_GPS_lib'
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction'
 * '<S4>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib'
 * '<S5>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/sgp4_lib_fsw'
 * '<S6>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/sun_vector_lib'
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/time_coord_rotation_lib'
 * '<S8>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/MET_2_GPS_lib/MATLAB Function'
 * '<S9>'   : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib'
 * '<S10>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position'
 * '<S11>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/MATLAB Function1'
 * '<S12>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED'
 * '<S13>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1'
 * '<S14>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude'
 * '<S15>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap'
 * '<S16>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Radius at Geocentric Latitude'
 * '<S17>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A11'
 * '<S18>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A12'
 * '<S19>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A13'
 * '<S20>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A21'
 * '<S21>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A22'
 * '<S22>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A23'
 * '<S23>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A31'
 * '<S24>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A32'
 * '<S25>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A33'
 * '<S26>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/Angle Conversion'
 * '<S27>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/Create Transformation Matrix'
 * '<S28>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A11'
 * '<S29>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A12'
 * '<S30>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A13'
 * '<S31>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A21'
 * '<S32>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A22'
 * '<S33>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A23'
 * '<S34>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A31'
 * '<S35>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A32'
 * '<S36>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A33'
 * '<S37>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/Angle Conversion'
 * '<S38>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/Create Transformation Matrix'
 * '<S39>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion'
 * '<S40>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion1'
 * '<S41>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion2'
 * '<S42>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Latitude Wrap 90'
 * '<S43>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Radius at Geocentric Latitude'
 * '<S44>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Radius at Geocentric Latitude/Conversion'
 * '<S45>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90'
 * '<S46>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Wrap Longitude'
 * '<S47>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Compare To Constant'
 * '<S48>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Wrap Angle 180'
 * '<S49>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Wrap Angle 180/Compare To Constant'
 * '<S50>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Wrap Longitude/Compare To Constant'
 * '<S51>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Radius at Geocentric Latitude/Conversion'
 * '<S52>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED'
 * '<S53>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA'
 * '<S54>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015'
 * '<S55>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/YMDHMS_2_dec_year_lib'
 * '<S56>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A11'
 * '<S57>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A12'
 * '<S58>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A13'
 * '<S59>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A21'
 * '<S60>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A22'
 * '<S61>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A23'
 * '<S62>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A31'
 * '<S63>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A32'
 * '<S64>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A33'
 * '<S65>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/Angle Conversion'
 * '<S66>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/Direction Cosine Matrix ECEF to NED/Create Transformation Matrix'
 * '<S67>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/Conversion'
 * '<S68>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/Subsystem2'
 * '<S69>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/Subsystem3'
 * '<S70>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem'
 * '<S71>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/e^1'
 * '<S72>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/e^2'
 * '<S73>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/e^3'
 * '<S74>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem2'
 * '<S75>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem3'
 * '<S76>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem4'
 * '<S77>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Check Altitude'
 * '<S78>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Check Latitude'
 * '<S79>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Check Longitude'
 * '<S80>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field'
 * '<S81>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Is time within model limits'
 * '<S82>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Length Conversion'
 * '<S83>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/MagField Conversion'
 * '<S84>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/MagField Conversion1'
 * '<S85>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag'
 * '<S86>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field/Angle Conversion'
 * '<S87>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates'
 * '<S88>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates'
 * '<S89>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates '
 * '<S90>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude'
 * '<S91>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Has altitude or latitude changed'
 * '<S92>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Has longitude changed '
 * '<S93>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Has time changed'
 * '<S94>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity'
 * '<S95>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem'
 * '<S96>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion'
 * '<S97>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations'
 * '<S98>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients'
 * '<S99>'  : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole'
 * '<S100>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate  index'
 * '<S101>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values'
 * '<S102>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/special case'
 * '<S103>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem1'
 * '<S104>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2'
 * '<S105>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  indices'
 * '<S106>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  row and column'
 * '<S107>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem'
 * '<S108>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1'
 * '<S109>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/m,n'
 * '<S110>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/n,m-1'
 * '<S111>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem'
 * '<S112>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1'
 * '<S113>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2'
 * '<S114>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/calculate  index'
 * '<S115>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  index'
 * '<S116>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  row and column'
 * '<S117>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  index'
 * '<S118>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  row and column'
 * '<S119>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  indices'
 * '<S120>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column1'
 * '<S121>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column2'
 * '<S122>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2'
 * '<S123>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2 '
 * '<S124>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/If Action Subsystem'
 * '<S125>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)'
 * '<S126>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem1'
 * '<S127>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem2'
 * '<S128>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ca'
 * '<S129>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ct'
 * '<S130>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate d'
 * '<S131>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q'
 * '<S132>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q2'
 * '<S133>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate r2'
 * '<S134>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate sa'
 * '<S135>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate st'
 * '<S136>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates /For Iterator Subsystem'
 * '<S137>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude/Angle Conversion2'
 * '<S138>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bx'
 * '<S139>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate by'
 * '<S140>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bz'
 * '<S141>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity'
 * '<S142>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion'
 * '<S143>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion1'
 * '<S144>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/mag_vec_lib/YMDHMS_2_dec_year_lib/MATLAB Function'
 * '<S145>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/sgp4_lib_fsw/MATLAB Function'
 * '<S146>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/sun_vector_lib/MATLAB Function'
 * '<S147>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/sun_vector_lib/MATLAB Function1'
 * '<S148>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/time_coord_rotation_lib/coord_rotations_lib'
 * '<S149>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/time_coord_rotation_lib/time_conversion_lib'
 * '<S150>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/time_coord_rotation_lib/coord_rotations_lib/MATLAB Function'
 * '<S151>' : 'adcs_sim_main/Flightsoftware/MSP_FSW/env_estimation_lib/time_coord_rotation_lib/time_conversion_lib/time-conversion-lib'
 */
#endif                                 /* RTW_HEADER_env_estimation_lib_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
