//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: rtwtypes.h
//
// Code generated for Simulink model 'MainSimulation'.
//
// Model version                  : 1.39
// Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
// C/C++ source code generated on : Fri Jul  7 12:44:20 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Texas Instruments->MSP430
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//

#ifndef RTWTYPES_H
#define RTWTYPES_H

// Logical type definitions
#if (!defined(__cplusplus))
#  ifndef false
#   define false                       (0U)
#  endif

#  ifndef true
#   define true                        (1U)
#  endif
#endif

//=======================================================================*
//  Target hardware information
//    Device type: Texas Instruments->MSP430
//    Number of bits:     char:   8    short:   16    int:  16
//                        long:  32
//                        native word size:  16
//    Byte ordering: LittleEndian
//    Signed integer division rounds to: Zero
//    Shift right on a signed integer as arithmetic shift: on
// =======================================================================

//=======================================================================*
//  Fixed width word size data types:                                     *
//    int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
//    uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
//    real32_T, real64_T           - 32 and 64 bit floating point numbers *
// =======================================================================
typedef signed char int8_T;
typedef unsigned char uint8_T;
typedef int int16_T;
typedef unsigned int uint16_T;
typedef long int32_T;
typedef unsigned long uint32_T;
typedef float real32_T;
typedef double real64_T;

//===========================================================================*
//  Generic type definitions: boolean_T, char_T, byte_T, int_T, uint_T,       *
//                            real_T, time_T, ulong_T.                        *
// ===========================================================================
typedef double real_T;
typedef double time_T;
typedef unsigned char boolean_T;
typedef int int_T;
typedef unsigned int uint_T;
typedef unsigned long ulong_T;
typedef char char_T;
typedef unsigned char uchar_T;
typedef char_T byte_T;

//=======================================================================*
//  Min and Max:                                                          *
//    int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
//    uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
// =======================================================================
#define MAX_int8_T                     ((int8_T)(127))
#define MIN_int8_T                     ((int8_T)(-128))
#define MAX_uint8_T                    ((uint8_T)(255U))
#define MAX_int16_T                    ((int16_T)(32767))
#define MIN_int16_T                    ((int16_T)(-32768))
#define MAX_uint16_T                   ((uint16_T)(65535U))
#define MAX_int32_T                    ((int32_T)(2147483647))
#define MIN_int32_T                    ((int32_T)(-2147483647-1))
#define MAX_uint32_T                   ((uint32_T)(0xFFFFFFFFU))

// Block D-Work pointer type
typedef void * pointer_T;

#endif                                 // RTWTYPES_H

//
// File trailer for generated code.
//
// [EOF]
//
