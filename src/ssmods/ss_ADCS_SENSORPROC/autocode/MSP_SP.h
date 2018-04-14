/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MSP_SP.h
 *
 * Code generated for Simulink model 'MSP_SP'.
 *
 * Model version                  : 1.330
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Sat Apr 14 10:28:14 2018
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

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S2>/process_matrix'
   *   '<S2>/sensor2body'
   */
  real_T pooled1[9];
} ConstP;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T sun_vec_body_sunsensor[4];    /* '<Root>/sun_vec_body_sunsensor' */
} ExtU;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T sun_vec_body[4];              /* '<Root>/sun_vec_body' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with auto storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY rtY;

/* Constant parameters (auto storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void MSP_SP_initialize(void);
extern void MSP_SP_step(void);
extern void MSP_SP_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S2>/Rate Transition6' : Eliminated since input and output rates are identical
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
 * '<S2>'   : 'adcs_sim_main/Flightsoftware/sensor_processing/MSP_SP/sunsensor_processing_lib'
 */
#endif                                 /* RTW_HEADER_MSP_SP_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
