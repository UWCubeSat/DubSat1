/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mpcmoveCodeGeneration_types.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 02-Nov-2017 18:25:03
 */

#ifndef MPCMOVECODEGENERATION_TYPES_H
#define MPCMOVECODEGENERATION_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_struct1_T
#define typedef_struct1_T

typedef struct {
  double Plant[7];
  double LastMove[3];
} struct1_T;

#endif                                 /*typedef_struct1_T*/

#ifndef typedef_struct3_T
#define typedef_struct3_T

typedef struct {
  double ym[7];
  double ref[7];
} struct3_T;

#endif                                 /*typedef_struct3_T*/

#ifndef typedef_struct2_T
#define typedef_struct2_T

typedef struct {
  struct3_T signals;
} struct2_T;

#endif                                 /*typedef_struct2_T*/

#ifndef typedef_struct7_T
#define typedef_struct7_T

typedef struct {
  double Uopt[63];
  double Yopt[147];
  double Xopt[147];
  double Topt[21];
  double Slack;
  double Iterations;
  char QPCode[8];
  double Cost;
} struct7_T;

#endif                                 /*typedef_struct7_T*/
#endif

/*
 * File trailer for mpcmoveCodeGeneration_types.h
 *
 * [EOF]
 */
