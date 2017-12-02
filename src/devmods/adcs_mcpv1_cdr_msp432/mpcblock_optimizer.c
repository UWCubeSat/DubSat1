/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mpcblock_optimizer.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 02-Nov-2017 18:25:03
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "mpcmoveCodeGeneration.h"
#include "mpcblock_optimizer.h"

/* Function Definitions */

/*
 * Arguments    : const double rseq[140]
 *                const double vseq[21]
 *                const double x[7]
 *                const double old_u[3]
 *                const double Hinv[3600]
 *                const double Kx[420]
 *                const double Ku1[180]
 *                const double Kut[3600]
 *                const double Kr[8400]
 *                const double Kv[1260]
 *                const double utarget[60]
 *                const double uoff[3]
 *                const double H[3600]
 *                const double Wy[7]
 *                const double Jm[3600]
 *                const double Su1[420]
 *                const double Sx[980]
 *                const double Hv[2940]
 *                const double I1[180]
 *                double u[3]
 *                double *cost
 *                double useq[60]
 *                double *status
 * Return Type  : void
 */
//void mpcblock_optimizer(const double rseq[140], const double vseq[21], const
//  double x[7], const double old_u[3], const double Hinv[3600], const double Kx
//  [420], const double Ku1[180], const double Kut[3600], const double Kr[8400],
//  const double Kv[1260], const double utarget[60], const double uoff[3], const
//  double H[3600], const double Wy[7], const double Jm[3600], const double Su1
//  [420], const double Sx[980], const double Hv[2940], const double I1[180],
//  double u[3], double *cost, double useq[60], double *status)
//{
//  int i;
//  double d0;
//  int i2;
//  int kidx;
//  double d1;
//  double zopt[60];
//  double f[60];
//  double d2;
//  double b_Sx[140];
//  double d3;
//  double b_Su1[140];
//  double aux3[60];
//  double d4;
//  double aux[60];
//  double aux2[140];
//  signed char I[9];
//  int b_j1;
//  int j2;
//  double b_old_u[3];
//  double K[3600];
//  signed char b_K[3600];
//  static const signed char A[400] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//    1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
//    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//    1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
//    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
//    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
//    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
//
//  for (i = 0; i < 60; i++) {
//    d0 = 0.0;
//    for (i2 = 0; i2 < 7; i2++) {
//      d0 += Kx[i2 + 7 * i] * x[i2];
//    }
//
//    d1 = 0.0;
//    for (i2 = 0; i2 < 140; i2++) {
//      d1 += Kr[i2 + 140 * i] * rseq[i2];
//    }
//
//    d2 = 0.0;
//    for (i2 = 0; i2 < 3; i2++) {
//      d2 += Ku1[i2 + 3 * i] * old_u[i2];
//    }
//
//    d3 = 0.0;
//    for (i2 = 0; i2 < 21; i2++) {
//      d3 += Kv[i2 + 21 * i] * vseq[i2];
//    }
//
//    d4 = 0.0;
//    for (i2 = 0; i2 < 60; i2++) {
//      d4 += Kut[i2 + 60 * i] * utarget[i2];
//    }
//
//    f[i] = (((d0 + d1) + d2) + d3) + d4;
//    zopt[i] = 0.0;
//  }
//
//  for (i = 0; i < 60; i++) {
//    for (kidx = 0; kidx < 60; kidx++) {
//      zopt[i] -= Hinv[i + 60 * kidx] * f[kidx];
//    }
//  }
//
//  *status = 1.0;
//  for (i = 0; i < 3; i++) {
//    u[i] = old_u[i] + zopt[i];
//  }
//
//  for (i2 = 0; i2 < 140; i2++) {
//    b_Sx[i2] = 0.0;
//    for (kidx = 0; kidx < 7; kidx++) {
//      b_Sx[i2] += Sx[i2 + 140 * kidx] * x[kidx];
//    }
//
//    b_Su1[i2] = 0.0;
//    for (kidx = 0; kidx < 3; kidx++) {
//      b_Su1[i2] += Su1[i2 + 140 * kidx] * old_u[kidx];
//    }
//
//    d0 = 0.0;
//    for (kidx = 0; kidx < 21; kidx++) {
//      d0 += Hv[i2 + 140 * kidx] * vseq[kidx];
//    }
//
//    aux2[i2] = ((b_Sx[i2] + b_Su1[i2]) + d0) - rseq[i2];
//  }
//
//  for (i2 = 0; i2 < 60; i2++) {
//    d0 = 0.0;
//    for (kidx = 0; kidx < 3; kidx++) {
//      d0 += I1[i2 + 60 * kidx] * old_u[kidx];
//    }
//
//    aux3[i2] = d0 - utarget[i2];
//  }
//
//  kidx = -1;
//  for (i = 0; i < 20; i++) {
//    for (i2 = 0; i2 < 7; i2++) {
//      kidx++;
//      b_Sx[kidx] = Wy[i2];
//    }
//  }
//
//  d0 = 0.0;
//  for (i = 0; i < 60; i++) {
//    aux[i] = zopt[i];
//    d0 += aux3[i] * aux3[i];
//  }
//
//  d1 = 0.0;
//  for (i2 = 0; i2 < 140; i2++) {
//    d1 += aux2[i2] * (b_Sx[i2] * aux2[i2]);
//  }
//
//  d2 = 0.0;
//  for (i2 = 0; i2 < 60; i2++) {
//    d3 = 0.0;
//    for (kidx = 0; kidx < 60; kidx++) {
//      d3 += H[i2 + 60 * kidx] * aux[kidx];
//    }
//
//    d2 += aux[i2] * (d3 + 2.0 * f[i2]);
//  }
//
//  *cost = (d0 + d1) + d2;
//  for (i2 = 0; i2 < 9; i2++) {
//    I[i2] = 0;
//  }
//
//  for (kidx = 0; kidx < 3; kidx++) {
//    I[kidx + 3 * kidx] = 1;
//  }
//
//  kidx = -1;
//  for (b_j1 = 0; b_j1 < 20; b_j1++) {
//    for (j2 = 0; j2 < 3; j2++) {
//      for (i = 0; i < 20; i++) {
//        for (i2 = 0; i2 < 3; i2++) {
//          kidx++;
//          b_K[kidx] = (signed char)(A[i + 20 * b_j1] * I[i2 + 3 * j2]);
//        }
//      }
//    }
//  }
//
//  for (i2 = 0; i2 < 60; i2++) {
//    for (kidx = 0; kidx < 60; kidx++) {
//      K[i2 + 60 * kidx] = 0.0;
//      for (i = 0; i < 60; i++) {
//        K[i2 + 60 * kidx] += (double)b_K[i2 + 60 * i] * Jm[i + 60 * kidx];
//      }
//    }
//  }
//
//  for (i2 = 0; i2 < 3; i2++) {
//    b_old_u[i2] = old_u[i2] + uoff[i2];
//  }
//
//  for (i2 = 0; i2 < 60; i2++) {
//    f[i2] = 0.0;
//    for (kidx = 0; kidx < 60; kidx++) {
//      f[i2] += K[i2 + 60 * kidx] * zopt[kidx];
//    }
//
//    aux3[i2] = 0.0;
//    for (kidx = 0; kidx < 3; kidx++) {
//      aux3[i2] += I1[i2 + 60 * kidx] * b_old_u[kidx];
//    }
//
//    aux[i2] = f[i2] + aux3[i2];
//  }
//
//  for (i2 = 0; i2 < 3; i2++) {
//    for (kidx = 0; kidx < 20; kidx++) {
//      useq[kidx + 20 * i2] = aux[i2 + 3 * kidx];
//    }
//  }
//}

/*
 * File trailer for mpcblock_optimizer.c
 *
 * [EOF]
 */
