/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.h
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.359
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Fri Apr 27 17:45:09 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_MSP_SP_h_
#define RTW_HEADER_MSP_SP_h_
#include <math.h>
#ifndef MSP_SP_COMMON_INCLUDES_
# define MSP_SP_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* MSP_SP_COMMON_INCLUDES_ */

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
  real32_T RateTransition2[4];         /* '<S3>/Rate Transition2' */
  real32_T RateTransition[4];          /* '<S3>/Rate Transition' */
  real32_T RateTransition1_m[3];       /* '<S5>/Rate Transition1' */
  real32_T DiscreteTransferFcn_states[3];/* '<S3>/Discrete Transfer Fcn' */
  real32_T DiscreteTransferFcn1_states[3];/* '<S2>/Discrete Transfer Fcn1' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S2>/Constant1'
   *   '<S3>/process_matrix'
   *   '<S3>/sensor2body'
   *   '<S5>/process_matrix'
   *   '<S5>/sensor2body'
   */
  real32_T pooled1[9];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T mag1_vec_body_T[4];         /* '<Root>/mag1_body_T' */
  real32_T mag2_vec_body_T[4];         /* '<Root>/mag2_body_T' */
  real32_T omega_body_radps_gyro[4];   /* '<Root>/omega_radps_gyro' */
  real32_T sun_vec_body_sunsensor[3];  /* '<Root>/sun_vec_body_angles' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T mag_body_processed_T[4];    /* '<Root>/mag_body_processed_T' */
  real32_T omega_radps_processed[4];   /* '<Root>/omega_radps_processed' */
  real32_T sun_vec_body[4];            /* '<Root>/sun_vec_body' */
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
      uint8_T TID0_2;
      uint8_T TID1_2;
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
extern void MSP_SP_initialize(void);
extern void MSP_SP_step0(void);
extern void MSP_SP_step1(void);
extern void MSP_SP_step2(void);
extern void MSP_SP_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition5' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition6' : Eliminated since input and output rates are identical
 * Block '<S7>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion2' : Eliminate redundant data type conversion
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
 * hilite_system('adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP')    - opens subsystem adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP
 * hilite_system('adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/sensor_processing'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/gyro_processing_lib'
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/magnetometer_processing_lib'
 * '<S4>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/rate_transition'
 * '<S5>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/sunsensor_processing_lib'
 * '<S6>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/sunsensor_processing_lib/MATLAB Function'
 * '<S7>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/sunsensor_processing_lib/angles_to_vec'
 */
#endif                                 /* RTW_HEADER_MSP_SP_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
