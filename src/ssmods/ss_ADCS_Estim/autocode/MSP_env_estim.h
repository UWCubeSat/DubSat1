/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_env_estim.h
 *
 * Code generated for Simulink model 'MSP_env_estim'.
 *
 * Model version                  : 1.361
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Apr 30 18:18:31 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_MSP_env_estim_h_
#define RTW_HEADER_MSP_env_estim_h_
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef MSP_env_estim_COMMON_INCLUDES_
# define MSP_env_estim_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* MSP_env_estim_COMMON_INCLUDES_ */

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
  real_T RateTransition1[9];           /* '<S1>/Rate Transition1' */
  real_T OutportBufferForcp13[13];
  real_T OutportBufferForsp13[13];
  real_T Sum2_n[169];                  /* '<S99>/Sum2' */
  real_T Assignment_o[169];            /* '<S98>/Assignment' */
  real_T Assignment_snorm[169];        /* '<S98>/Assignment_snorm' */
  real_T Assignment2_e[13];            /* '<S105>/Assignment2' */
  real_T Assignment2_en[13];           /* '<S104>/Assignment2' */
  real_T UnitDelay1_DSTATE[2];         /* '<S137>/Unit Delay1' */
  real_T UnitDelay_DSTATE_p[169];      /* '<S99>/Unit Delay' */
  real_T UnitDelay_DSTATE_n[169];      /* '<S126>/Unit Delay' */
  real_T UnitDelay_DSTATE_f[169];      /* '<S98>/Unit Delay' */
  real_T UnitDelay1_DSTATE_k[169];     /* '<S98>/Unit Delay1' */
  real_T UnitDelay1_DSTATE_m[13];      /* '<S100>/Unit Delay1' */
  real_T RateTransition1_Buffer0[9];   /* '<S1>/Rate Transition1' */
  real_T RateTransition;               /* '<S1>/Rate Transition' */
  real_T RateTransition4;              /* '<S1>/Rate Transition4' */
  real_T sqrt_e;                       /* '<S89>/sqrt' */
  real_T Product11;                    /* '<S129>/Product11' */
  real_T Product4;                     /* '<S130>/Product4' */
  real_T Product12;                    /* '<S135>/Product12' */
  real_T sqrt_j;                       /* '<S136>/sqrt' */
  real_T Sum1;                         /* '<S97>/Sum1' */
  real_T Sum2;                         /* '<S97>/Sum2' */
  real_T Sum3;                         /* '<S97>/Sum3' */
  real_T Sum5;                         /* '<S97>/Sum5' */
  real_T Merge1_b;                     /* '<S98>/Merge1' */
  real_T Merge_p;                      /* '<S98>/Merge' */
  real_T Product2;                     /* '<S100>/Product2' */
  real_T UnitDelay1_DSTATE_a;          /* '<S97>/Unit Delay1' */
  real_T UnitDelay3_DSTATE;            /* '<S97>/Unit Delay3' */
  real_T UnitDelay2_DSTATE_e;          /* '<S97>/Unit Delay2' */
  real_T UnitDelay4_DSTATE;            /* '<S97>/Unit Delay4' */
  real_T RateTransition_Buffer0;       /* '<S1>/Rate Transition' */
  real_T RateTransition4_Buffer0;      /* '<S1>/Rate Transition4' */
  real_T otime_PreviousInput;          /* '<S94>/otime' */
  real_T olon_PreviousInput;           /* '<S93>/olon' */
  real_T olat_PreviousInput;           /* '<S92>/olat' */
  real_T oalt_PreviousInput;           /* '<S92>/oalt' */
  real_T Memory_PreviousInput;         /* '<S77>/Memory' */
  uint8_T ForIterator_IterationMarker[7];/* '<S96>/For Iterator' */
  boolean_T SpecialcaseNorthSouthGeographic;/* '<S97>/Special case - North//South Geographic Pole' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Expression: k)
   * Referenced by:
   *   '<S114>/k[13][13]'
   *   '<S105>/k[13][13]'
   */
  real_T pooled9[169];

  /* Pooled Parameter (Expression: snorm)
   * Referenced by:
   *   '<S98>/snorm[169]'
   *   '<S98>/Unit Delay1'
   */
  real_T pooled12[169];

  /* Expression: c
   * Referenced by: '<S99>/c[maxdef][maxdef]'
   */
  real_T cmaxdefmaxdef_Value[169];

  /* Expression: cd
   * Referenced by: '<S99>/cd[maxdef][maxdef]'
   */
  real_T cdmaxdefmaxdef_Value[169];

  /* Expression: fm
   * Referenced by: '<S97>/fm'
   */
  real_T fm_Value[13];

  /* Expression: fn
   * Referenced by: '<S97>/fn'
   */
  real_T fn_Value[13];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S137>/sp[11]'
   *   '<S137>/cp[11]'
   *   '<S137>/Constant'
   *   '<S137>/Constant1'
   */
  real_T pooled15[11];

  /* Expression: [0 0 (1:(maxdef-1))]
   * Referenced by: '<S90>/sp[13]'
   */
  real_T sp13_Y0[13];

  /* Expression: [1 1 (1:(maxdef-1))]
   * Referenced by: '<S90>/cp[13]'
   */
  real_T cp13_Y0[13];

  /* Expression: fsw_params.env_estimation.ic.orbit_tle
   * Referenced by: '<S1>/Rate Transition1'
   */
  real_T RateTransition1_X0[9];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T orbit_TLE[9];                 /* '<Root>/orbit_tle' */
  real_T MET;                          /* '<Root>/MET' */
  real_T MET_epoch;                    /* '<Root>/MET_epoch' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  boolean_T sc_in_sun;                 /* '<Root>/sc_in_sun' */
  real_T sc2sun_unit[3];               /* '<Root>/sc2sun_unit' */
  real_T mag_eci_unit[3];              /* '<Root>/mag_eci_unit' */
  boolean_T sc_above_gs;               /* '<Root>/sc_above_gs' */
  real_T pos_eci_m[3];                 /* '<Root>/pos_eci_m' */
  real_T vel_eci_mps[3];               /* '<Root>/vel_eci_mps' */
  int8_T SGP4_flag;                    /* '<Root>/SGP4_flag' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID0_1;
    } RateInteraction;
  } Timing;
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
extern void MSP_env_estim_initialize(void);
extern void MSP_env_estim_step0(void);
extern void MSP_env_estim_step1(void);
extern void MSP_env_estim_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S10>/Data Type Conversion1' : Unused code path elimination
 * Block '<S84>/Unit Conversion' : Unused code path elimination
 * Block '<S85>/Unit Conversion' : Unused code path elimination
 * Block '<S1>/Rate Transition12' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition13' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition6' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition7' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition9' : Eliminated since input and output rates are identical
 * Block '<S28>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S39>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S67>/Reshape (9) to [3x3] column-major' : Reshape block reduction
 * Block '<S82>/maxtype' : Eliminate redundant data type conversion
 * Block '<S82>/mintype' : Eliminate redundant data type conversion
 * Block '<S83>/Unit Conversion' : Eliminated nontunable gain of 1
 * Block '<S55>/Unit Conversion2' : Eliminated nontunable gain of 1
 * Block '<S105>/Reshape' : Reshape block reduction
 * Block '<S112>/Reshape' : Reshape block reduction
 * Block '<S113>/Reshape' : Reshape block reduction
 * Block '<S114>/Reshape' : Reshape block reduction
 * Block '<S114>/Reshape1' : Reshape block reduction
 * Block '<S90>/Gain' : Eliminated nontunable gain of 1
 * Block '<S90>/Gain1' : Eliminated nontunable gain of 1
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
 * hilite_system('adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim')    - opens subsystem adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim
 * hilite_system('adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/adcs_main'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/MET_2_GPS_lib'
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction'
 * '<S4>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_LS_lib'
 * '<S5>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib'
 * '<S6>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/sgp4_lib_fsw'
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/sun_vector_lib'
 * '<S8>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/time_coord_rotation_lib'
 * '<S9>'   : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/MET_2_GPS_lib/MATLAB Function'
 * '<S10>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib'
 * '<S11>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position'
 * '<S12>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/MATLAB Function1'
 * '<S13>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED'
 * '<S14>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1'
 * '<S15>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude'
 * '<S16>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap'
 * '<S17>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Radius at Geocentric Latitude'
 * '<S18>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A11'
 * '<S19>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A12'
 * '<S20>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A13'
 * '<S21>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A21'
 * '<S22>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A22'
 * '<S23>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A23'
 * '<S24>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A31'
 * '<S25>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A32'
 * '<S26>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/A33'
 * '<S27>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/Angle Conversion'
 * '<S28>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED/Create Transformation Matrix'
 * '<S29>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A11'
 * '<S30>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A12'
 * '<S31>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A13'
 * '<S32>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A21'
 * '<S33>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A22'
 * '<S34>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A23'
 * '<S35>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A31'
 * '<S36>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A32'
 * '<S37>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/A33'
 * '<S38>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/Angle Conversion'
 * '<S39>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Direction Cosine Matrix ECI to NED1/Create Transformation Matrix'
 * '<S40>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion'
 * '<S41>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion1'
 * '<S42>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Conversion2'
 * '<S43>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Latitude Wrap 90'
 * '<S44>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Radius at Geocentric Latitude'
 * '<S45>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Geodetic to  Geocentric Latitude/Radius at Geocentric Latitude/Conversion'
 * '<S46>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90'
 * '<S47>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Wrap Longitude'
 * '<S48>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Compare To Constant'
 * '<S49>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Wrap Angle 180'
 * '<S50>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Latitude Wrap 90/Wrap Angle 180/Compare To Constant'
 * '<S51>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/LatLong wrap/Wrap Longitude/Compare To Constant'
 * '<S52>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/gs_prediction/gs_prediction_lib/LLA to ECEF Position/Radius at Geocentric Latitude/Conversion'
 * '<S53>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED'
 * '<S54>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA'
 * '<S55>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015'
 * '<S56>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/YMDHMS_2_dec_year_lib'
 * '<S57>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A11'
 * '<S58>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A12'
 * '<S59>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A13'
 * '<S60>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A21'
 * '<S61>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A22'
 * '<S62>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A23'
 * '<S63>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A31'
 * '<S64>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A32'
 * '<S65>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/A33'
 * '<S66>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/Angle Conversion'
 * '<S67>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/Direction Cosine Matrix ECEF to NED/Create Transformation Matrix'
 * '<S68>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/Conversion'
 * '<S69>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/Subsystem2'
 * '<S70>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/Subsystem3'
 * '<S71>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem'
 * '<S72>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/e^1'
 * '<S73>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/e^2'
 * '<S74>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/e^3'
 * '<S75>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem2'
 * '<S76>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem3'
 * '<S77>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/ECEF Position to LLA/While Iterator Subsystem/Subsystem4'
 * '<S78>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Check Altitude'
 * '<S79>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Check Latitude'
 * '<S80>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Check Longitude'
 * '<S81>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field'
 * '<S82>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Is time within model limits'
 * '<S83>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Length Conversion'
 * '<S84>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/MagField Conversion'
 * '<S85>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/MagField Conversion1'
 * '<S86>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag'
 * '<S87>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field/Angle Conversion'
 * '<S88>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates'
 * '<S89>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates'
 * '<S90>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates '
 * '<S91>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude'
 * '<S92>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Has altitude or latitude changed'
 * '<S93>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Has longitude changed '
 * '<S94>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Has time changed'
 * '<S95>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity'
 * '<S96>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem'
 * '<S97>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion'
 * '<S98>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations'
 * '<S99>'  : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients'
 * '<S100>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole'
 * '<S101>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate  index'
 * '<S102>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values'
 * '<S103>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/special case'
 * '<S104>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem1'
 * '<S105>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2'
 * '<S106>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  indices'
 * '<S107>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  row and column'
 * '<S108>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem'
 * '<S109>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1'
 * '<S110>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/m,n'
 * '<S111>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/n,m-1'
 * '<S112>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem'
 * '<S113>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1'
 * '<S114>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2'
 * '<S115>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/calculate  index'
 * '<S116>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  index'
 * '<S117>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  row and column'
 * '<S118>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  index'
 * '<S119>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  row and column'
 * '<S120>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  indices'
 * '<S121>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column1'
 * '<S122>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column2'
 * '<S123>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2'
 * '<S124>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2 '
 * '<S125>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/If Action Subsystem'
 * '<S126>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)'
 * '<S127>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem1'
 * '<S128>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem2'
 * '<S129>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ca'
 * '<S130>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ct'
 * '<S131>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate d'
 * '<S132>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q'
 * '<S133>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q2'
 * '<S134>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate r2'
 * '<S135>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate sa'
 * '<S136>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate st'
 * '<S137>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates /For Iterator Subsystem'
 * '<S138>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude/Angle Conversion2'
 * '<S139>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bx'
 * '<S140>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate by'
 * '<S141>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bz'
 * '<S142>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity'
 * '<S143>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion'
 * '<S144>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion1'
 * '<S145>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/mag_vec_lib/YMDHMS_2_dec_year_lib/MATLAB Function'
 * '<S146>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/sgp4_lib_fsw/MATLAB Function'
 * '<S147>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/sun_vector_lib/MATLAB Function'
 * '<S148>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/sun_vector_lib/MATLAB Function1'
 * '<S149>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/time_coord_rotation_lib/coord_rotations_lib'
 * '<S150>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/time_coord_rotation_lib/time_conversion_lib'
 * '<S151>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/time_coord_rotation_lib/coord_rotations_lib/MATLAB Function'
 * '<S152>' : 'adcs_sim_main/Flightsoftware/adcs_main/MSP_env_estim/time_coord_rotation_lib/time_conversion_lib/time-conversion-lib'
 */
#endif                                 /* RTW_HEADER_MSP_env_estim_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
