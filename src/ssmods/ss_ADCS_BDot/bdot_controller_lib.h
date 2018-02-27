/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bdot_controller_lib.h
 *
 * Code generated for Simulink model 'bdot_controller_lib'.
 *
 * Model version                  : 1.28
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Mon Aug 28 19:59:51 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_bdot_controller_lib_h_
#define RTW_HEADER_bdot_controller_lib_h_
#ifndef bdot_controller_lib_COMMON_INCLUDES_
# define bdot_controller_lib_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* bdot_controller_lib_COMMON_INCLUDES_ */

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
  real_T UnitDelay_DSTATE[3];          /* '<S1>/Unit Delay' */
  real_T DiscreteTransferFcn_states[3];/* '<S1>/Discrete Transfer Fcn' */
  real_T UD_DSTATE[3];                 /* '<S2>/UD' */
  real_T RateTransition1_Buffer0[3];   /* '<S1>/Rate Transition1' */
  real_T RateTransition4_Buffer0[3];   /* '<S1>/Rate Transition4' */
  real_T RateTransition3_Buffer0;      /* '<S1>/Rate Transition3' */
  real_T RateTransition2_Buffer0;      /* '<S1>/Rate Transition2' */
  int8_T RateTransition3_semaphoreTaken;/* '<S1>/Rate Transition3' */
  int8_T RateTransition2_semaphoreTaken;/* '<S1>/Rate Transition2' */
  int8_T RateTransition4_semaphoreTaken;/* '<S1>/Rate Transition4' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: fsw_params.sensor_processing.magnetometer.process_matrix
   * Referenced by: '<S3>/Constant1'
   */
  real_T Constant1_Value[9];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T B_body_in_T[3];               /* '<Root>/B_body_in_T' */
  real_T B_meas_valid;                 /* '<Root>/B_meas_valid' */
  real_T MT_on;                        /* '<Root>/MT_on' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Dig_val[3];                   /* '<Root>/Dig_val' */
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
extern void bdot_controller_lib_initialize(void);
extern void bdot_controller_lib_step0(void);
extern void bdot_controller_lib_step1(void);
extern void bdot_controller_lib_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Data Type Duplicate' : Unused code path elimination
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
 * hilite_system('bdot_simple_sim/bdot_controller_lib')    - opens subsystem bdot_simple_sim/bdot_controller_lib
 * hilite_system('bdot_simple_sim/bdot_controller_lib/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'bdot_simple_sim'
 * '<S1>'   : 'bdot_simple_sim/bdot_controller_lib'
 * '<S2>'   : 'bdot_simple_sim/bdot_controller_lib/Discrete Derivative'
 * '<S3>'   : 'bdot_simple_sim/bdot_controller_lib/Signal Processing'
 */
#endif                                 /* RTW_HEADER_bdot_controller_lib_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
