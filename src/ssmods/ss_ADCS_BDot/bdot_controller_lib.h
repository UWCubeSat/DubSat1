/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bdot_controller_lib.h
 *
 * Code generated for Simulink model 'bdot_controller_lib'.
 *
 * Model version                  : 1.350
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Wed Apr 18 10:34:19 2018
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
#include <math.h>
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
  real_T DiscreteTransferFcn_states[3];/* '<S1>/Discrete Transfer Fcn' */
  real_T UD_DSTATE[3];                 /* '<S2>/UD' */
  boolean_T Relay_Mode;                /* '<S1>/Relay' */
} DW;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: fsw_params.bdot.gain_matrix
   * Referenced by: '<S1>/gain matrix'
   */
  real_T gainmatrix_Value[9];

  /* Expression: fsw_params.sensor_processing.magnetometer.process_matrix
   * Referenced by: '<S3>/Constant1'
   */
  real_T Constant1_Value[9];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T B_body_in_T[3];             /* '<Root>/B_body_in_T' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int8_T Dig_val[3];                   /* '<Root>/Dig_val' */
  boolean_T tumble;                    /* '<Root>/tumble' */
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
extern void bdot_controller_lib_initialize(void);
extern void bdot_controller_lib_step(void);
extern void bdot_controller_lib_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Data Type Duplicate' : Unused code path elimination
 * Block '<S1>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S1>/Rate Transition4' : Eliminated since input and output rates are identical
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
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib')    - opens subsystem adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib
 * hilite_system('adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'adcs_sim_main/Flightsoftware/MSP_BDOT'
 * '<S1>'   : 'adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib'
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib/Discrete Derivative'
 * '<S3>'   : 'adcs_sim_main/Flightsoftware/MSP_BDOT/bdot_controller_lib/Signal Processing'
 */
#endif                                 /* RTW_HEADER_bdot_controller_lib_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
