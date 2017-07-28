/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: adcs_fsw_main_skeleton.c
 *
 * Code generated for Simulink model 'adcs_fsw_main_skeleton'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
 * C/C++ source code generated on : Tue Jul 25 18:49:09 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->MSP430
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "adcs_fsw_main_skeleton.h"

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

int i = 0;

/* Model step function */
void adcs_fsw_main_skeleton_step(void)
{
  i += 1;
}

/* Model initialize function */
void adcs_fsw_main_skeleton_initialize(void)
{
  i += 1;
}

/* Model terminate function */
void adcs_fsw_main_skeleton_terminate(void)
{
  i += 1;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
