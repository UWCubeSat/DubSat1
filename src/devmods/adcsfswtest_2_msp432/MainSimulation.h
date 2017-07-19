//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MainSimulation.h
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
#ifndef RTW_HEADER_MainSimulation_h_
#define RTW_HEADER_MainSimulation_h_
#include <stddef.h>
#include "rtwtypes.h"
#include <cmath>
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef MainSimulation_COMMON_INCLUDES_
# define MainSimulation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 // MainSimulation_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_commands_
#define DEFINED_TYPEDEF_FOR_commands_

typedef struct {
  real_T RW_RPM[3];
  real_T magnetorquer_volt[3];
} commands;

#endif

#ifndef DEFINED_TYPEDEF_FOR_act_meas_
#define DEFINED_TYPEDEF_FOR_act_meas_

typedef struct {
  boolean_T MT_power_OK;
  boolean_T MT_on;
  real_T RW_pwrstate[3];
  real_T magnetorquer_pwrstate[3];
  real_T current_RW_RPM[3];
} act_meas;

#endif

#ifndef DEFINED_TYPEDEF_FOR_control_
#define DEFINED_TYPEDEF_FOR_control_

// Outputs of actuation models.
typedef struct {
  real_T torque[3];
  real_T magnetic_dipole[3];
} control;

#endif

#ifndef DEFINED_TYPEDEF_FOR_disturbances_
#define DEFINED_TYPEDEF_FOR_disturbances_

typedef struct {
  real_T DT_aero[3];
  real_T DT_grav[3];
  real_T DT_solar[3];
  real_T MD_SC_resdipole[3];
  real_T env_acc_eci[3];
} disturbances;

#endif

#ifndef DEFINED_TYPEDEF_FOR_orbit_data_
#define DEFINED_TYPEDEF_FOR_orbit_data_

typedef struct {
  real_T env_acc_eci[3];
  real_T sun_vec_eci[3];
  real_T mag_vector_eci[3];
  real_T init_pos_eci[3];
  real_T init_vel_eci[3];
} orbit_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_states_
#define DEFINED_TYPEDEF_FOR_states_

typedef struct {
  real_T quaternion[4];
  real_T body_rates[3];
  real_T position[3];
  real_T velocity[3];
} states;

#endif

#ifndef DEFINED_TYPEDEF_FOR_sens_meas_
#define DEFINED_TYPEDEF_FOR_sens_meas_

typedef struct {
  real_T measurements_deg_err[6];
  real_T measurements_valid[3];
  real_T sun_eci_unit_photodiode[3];
  real_T sun_eci_unit_sunsensor[3];
  real_T mag_eci_T[3];
  real_T GPS[8];
  real_T omega_radps_gyro[3];
} sens_meas;

#endif

// Block signals and states (auto storage) for system '<S80>/While Iterator Subsystem' 
typedef struct {
  real_T Memory_PreviousInput;         // '<S114>/Memory'
} DW_WhileIteratorSubsystem;

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  DW_WhileIteratorSubsystem WhileIteratorSubsystem_o;// '<S125>/While Iterator Subsystem' 
  DW_WhileIteratorSubsystem WhileIteratorSubsystem_c;// '<S80>/While Iterator Subsystem' 
  real_T TmpRTBAtMatrixMultiply2Inport1[9];// '<S82>/Math Function1'
  real_T TmpRTBAtMatrixMultiply2Inport_h[9];
  real_T TmpRTBAtIntegrator1Inport2[3];
  real_T SFunction_o1[2];              // '<S77>/S-Function'
  real_T SFunction_o2[9];              // '<S77>/S-Function'
  real_T TmpRTBAtMatrixMultiply1Inport1[9];
  real_T TmpRTBAtIntegratorInport2[3];
  real_T Integrator_j[3];              // '<S41>/Integrator'
  real_T TmpRTBAtMatrixMultiplyInport1[9];// '<S115>/Math Function'
  real_T TmpRTBAtMATLABFunction1Inport1[3];// '<S217>/MATLAB Function'
  real_T TmpRTBAtMatrixMultiplyInport1_g[9];// '<S83>/Math Function'
  real_T Product1[3];                  // '<S51>/Product1'
  real_T Sum[3];                       // '<S41>/Sum'
  real_T Product[4];                   // '<S42>/Product'
  real_T OutportBufferForcp13[13];
  real_T OutportBufferForsp13[13];
  real_T Assignment[11];               // '<S209>/Assignment'
  real_T Assignment1[11];              // '<S209>/Assignment1'
  real_T Sum1[4];                      // '<S160>/Sum1'
  real_T Sum2_i[169];                  // '<S171>/Sum2'
  real_T Assignment_h[169];            // '<S170>/Assignment'
  real_T Assignment_snorm[169];        // '<S170>/Assignment_snorm'
  real_T Assignment2_g[13];            // '<S177>/Assignment2'
  real_T Assignment2_n[13];            // '<S176>/Assignment2'
  real_T UnitDelay1_DSTATE[2];         // '<S209>/Unit Delay1'
  real_T UnitDelay_DSTATE_m[169];      // '<S171>/Unit Delay'
  real_T UnitDelay_DSTATE_d[169];      // '<S198>/Unit Delay'
  real_T UnitDelay_DSTATE_ft[169];     // '<S170>/Unit Delay'
  real_T UnitDelay1_DSTATE_o[169];     // '<S170>/Unit Delay1'
  real_T UnitDelay1_DSTATE_h[13];      // '<S172>/Unit Delay1'
  real_T TmpRTBAtMatrixMultiply2Inport1_[9];// synthesized block
  real_T TmpRTBAtMatrixMultiply2Inport_p[9];// synthesized block
  real_T TmpRTBAtIntegrator1Inport2_Buff[3];// synthesized block
  real_T Product_DWORK4[9];            // '<S53>/Product'
  real_T TmpRTBAtMatrixMultiply1Inport1_[9];// synthesized block
  real_T TmpRTBAtIntegratorInport2_Buffe[3];// synthesized block
  real_T TmpRTBAtMatrixMultiplyInport1_B[9];// synthesized block
  real_T TmpRTBAtMATLABFunction1Inport1_[3];// synthesized block
  real_T TmpRTBAtMatrixMultiplyInport1_o[9];// synthesized block
  real_T SetpointWeightingDerivative;  // '<S19>/Setpoint Weighting (Derivative)' 
  real_T omega;                        // '<S16>/Integrator1'
  real_T Filter;                       // '<S19>/Filter'
  real_T FilterCoefficient;            // '<S19>/Filter Coefficient'
  real_T IntegralGain;                 // '<S19>/Integral Gain'
  real_T Integrator;                   // '<S19>/Integrator'
  real_T SetpointWeightingProportional;// '<S19>/Setpoint Weighting (Proportional)' 
  real_T SetpointWeightingDerivative_m;// '<S20>/Setpoint Weighting (Derivative)' 
  real_T omega_m;                      // '<S18>/Integrator1'
  real_T Filter_p;                     // '<S20>/Filter'
  real_T FilterCoefficient_a;          // '<S20>/Filter Coefficient'
  real_T IntegralGain_g;               // '<S20>/Integral Gain'
  real_T Integrator_e;                 // '<S20>/Integrator'
  real_T SetpointWeightingProportional_o;// '<S20>/Setpoint Weighting (Proportional)' 
  real_T SetpointWeightingDerivative_mx;// '<S21>/Setpoint Weighting (Derivative)' 
  real_T omega_h;                      // '<S17>/Integrator1'
  real_T Filter_g;                     // '<S21>/Filter'
  real_T FilterCoefficient_o;          // '<S21>/Filter Coefficient'
  real_T IntegralGain_n;               // '<S21>/Integral Gain'
  real_T Integrator_n;                 // '<S21>/Integrator'
  real_T SetpointWeightingProportional_p;// '<S21>/Setpoint Weighting (Proportional)' 
  real_T didt;                         // '<S16>/Gain'
  real_T omega_dot;                    // '<S16>/Gain1'
  real_T didt_f;                       // '<S17>/Gain'
  real_T omega_dot_p;                  // '<S17>/Gain1'
  real_T didt_k;                       // '<S18>/Gain'
  real_T omega_dot_a;                  // '<S18>/Gain1'
  real_T Gain;                         // '<S28>/Gain'
  real_T Gain_b;                       // '<S29>/Gain'
  real_T Gain_m;                       // '<S30>/Gain'
  real_T TmpRTBAtComputemagneticvectorin;// '<S158>/Sum'
  real_T TmpRTBAtRelationalOperatorInpor;
  real_T u80deg;                       // '<S128>/+//- 180 deg'
  real_T u0deg;                        // '<S128>/+//- 90 deg'
  real_T olat;                         // '<S164>/olat'
  real_T sincos_o1;                    // '<S131>/sincos'
  real_T sincos_o2;                    // '<S131>/sincos'
  real_T Product3;                     // '<S131>/Product3'
  real_T Product4;                     // '<S131>/Product4'
  real_T Gain_a;                       // '<S128>/Gain'
  real_T oalt;                         // '<S164>/oalt'
  real_T sincos_o1_m;                  // '<S107>/sincos'
  real_T sincos_o2_c;                  // '<S107>/sincos'
  real_T Product3_h;                   // '<S107>/Product3'
  real_T Product4_h;                   // '<S107>/Product4'
  real_T TmpRTBAtSFunctionInport4;
  real_T TmpRTBAtSFunctionInport5;
  real_T TmpRTBAtSFunctionInport6;
  real_T TmpRTBAtotimeInport1;
  real_T TmpRTBAtIntegratorInport1;    // '<S228>/RRW'
  real_T sqrt_l;                       // '<S161>/sqrt'
  real_T Product11;                    // '<S201>/Product11'
  real_T Product4_m;                   // '<S202>/Product4'
  real_T Product12;                    // '<S207>/Product12'
  real_T sqrt_kw;                      // '<S208>/sqrt'
  real_T Sum1_h;                       // '<S169>/Sum1'
  real_T Sum2;                         // '<S169>/Sum2'
  real_T Sum3;                         // '<S169>/Sum3'
  real_T Sum5;                         // '<S169>/Sum5'
  real_T Merge1_j;                     // '<S170>/Merge1'
  real_T Merge_b;                      // '<S170>/Merge'
  real_T Product2;                     // '<S172>/Product2'
  real_T TrigonometricFunction4;       // '<S136>/Trigonometric Function4'
  real_T TrigonometricFunction4_e;     // '<S112>/Trigonometric Function4'
  real_T Filter_DSTATE;                // '<S19>/Filter'
  real_T Integrator_DSTATE;            // '<S19>/Integrator'
  real_T Filter_DSTATE_j;              // '<S20>/Filter'
  real_T Integrator_DSTATE_e;          // '<S20>/Integrator'
  real_T Filter_DSTATE_k;              // '<S21>/Filter'
  real_T Integrator_DSTATE_g;          // '<S21>/Integrator'
  real_T UnitDelay_DSTATE;             // '<S70>/Unit Delay'
  real_T UnitDelay1_DSTATE_k;          // '<S169>/Unit Delay1'
  real_T UnitDelay3_DSTATE;            // '<S169>/Unit Delay3'
  real_T UnitDelay2_DSTATE_p;          // '<S169>/Unit Delay2'
  real_T UnitDelay4_DSTATE;            // '<S169>/Unit Delay4'
  real_T TmpRTBAtComputemagneticvector_m;// synthesized block
  real_T TmpRTBAtRelationalOperatorInp_c;// synthesized block
  real_T otime_PreviousInput;          // '<S166>/otime'
  real_T olon_PreviousInput;           // '<S165>/olon'
  real_T olat_PreviousInput;           // '<S164>/olat'
  real_T oalt_PreviousInput;           // '<S164>/oalt'
  real_T TmpRTBAtSFunctionInport4_Buffer;// synthesized block
  real_T TmpRTBAtSFunctionInport5_Buffer;// synthesized block
  real_T TmpRTBAtSFunctionInport6_Buffer;// synthesized block
  real_T SFunction_phi;                // '<S77>/S-Function'
  real_T SFunction_lambda;             // '<S77>/S-Function'
  real_T SFunction_h;                  // '<S77>/S-Function'
  real_T SFunction_ap;                 // '<S77>/S-Function'
  real_T TmpRTBAtotimeInport1_Buffer0; // synthesized block
  real_T NextOutput;                   // '<S228>/Random Number1'
  real_T TmpRTBAtIntegratorInport1_Buffe;// synthesized block
  int32_T SFunction_doy;               // '<S77>/S-Function'
  uint32_T RandSeed;                   // '<S228>/Random Number1'
  uint32_T RandSeed_g;                 // '<S231>/Random Number2'
  uint32_T RandSeed_f;                 // '<S231>/Random Number3'
  uint32_T RandSeed_e;                 // '<S231>/Random Number4'
  int_T Integrator1_IWORK;             // '<S41>/Integrator1'
  int_T Integrator_IWORK;              // '<S41>/Integrator'
  uint8_T ForIterator_IterationMarker[7];// '<S168>/For Iterator'
  boolean_T Convertfromgeodetictosphericalc;// '<S158>/Convert from geodetic to  spherical coordinates ' 
  boolean_T Convertfromgeodetictospherica_d;// '<S158>/Convert from geodetic to  spherical coordinates' 
  boolean_T SpecialcaseNorthSouthGeographic;// '<S169>/Special case - North//South Geographic Pole' 
} DW;

// Continuous states (auto storage)
typedef struct {
  real_T Integrator1_CSTATE;           // '<S16>/Integrator1'
  real_T Integrator1_CSTATE_k;         // '<S18>/Integrator1'
  real_T Integrator1_CSTATE_b;         // '<S17>/Integrator1'
  real_T Integrator_CSTATE;            // '<S16>/Integrator'
  real_T Integrator2_CSTATE;           // '<S16>/Integrator2'
  real_T Integrator_CSTATE_i;          // '<S17>/Integrator'
  real_T Integrator2_CSTATE_j;         // '<S17>/Integrator2'
  real_T Integrator_CSTATE_b;          // '<S18>/Integrator'
  real_T Integrator2_CSTATE_l;         // '<S18>/Integrator2'
  real_T Integrator_CSTATE_k;          // '<S28>/Integrator'
  real_T Integrator_CSTATE_g;          // '<S29>/Integrator'
  real_T Integrator_CSTATE_n;          // '<S30>/Integrator'
  real_T Integrator1_CSTATE_g[3];      // '<S41>/Integrator1'
  real_T Integrator1_CSTATE_a[4];      // '<S42>/Integrator1'
  real_T Integrator_CSTATE_o[3];       // '<S39>/Integrator'
  real_T Integrator_CSTATE_e[3];       // '<S41>/Integrator'
  real_T Integrator_CSTATE_d[3];       // '<S228>/Integrator'
} X;

// State derivatives (auto storage)
typedef struct {
  real_T Integrator1_CSTATE;           // '<S16>/Integrator1'
  real_T Integrator1_CSTATE_k;         // '<S18>/Integrator1'
  real_T Integrator1_CSTATE_b;         // '<S17>/Integrator1'
  real_T Integrator_CSTATE;            // '<S16>/Integrator'
  real_T Integrator2_CSTATE;           // '<S16>/Integrator2'
  real_T Integrator_CSTATE_i;          // '<S17>/Integrator'
  real_T Integrator2_CSTATE_j;         // '<S17>/Integrator2'
  real_T Integrator_CSTATE_b;          // '<S18>/Integrator'
  real_T Integrator2_CSTATE_l;         // '<S18>/Integrator2'
  real_T Integrator_CSTATE_k;          // '<S28>/Integrator'
  real_T Integrator_CSTATE_g;          // '<S29>/Integrator'
  real_T Integrator_CSTATE_n;          // '<S30>/Integrator'
  real_T Integrator1_CSTATE_g[3];      // '<S41>/Integrator1'
  real_T Integrator1_CSTATE_a[4];      // '<S42>/Integrator1'
  real_T Integrator_CSTATE_o[3];       // '<S39>/Integrator'
  real_T Integrator_CSTATE_e[3];       // '<S41>/Integrator'
  real_T Integrator_CSTATE_d[3];       // '<S228>/Integrator'
} XDot;

// State disabled
typedef struct {
  boolean_T Integrator1_CSTATE;        // '<S16>/Integrator1'
  boolean_T Integrator1_CSTATE_k;      // '<S18>/Integrator1'
  boolean_T Integrator1_CSTATE_b;      // '<S17>/Integrator1'
  boolean_T Integrator_CSTATE;         // '<S16>/Integrator'
  boolean_T Integrator2_CSTATE;        // '<S16>/Integrator2'
  boolean_T Integrator_CSTATE_i;       // '<S17>/Integrator'
  boolean_T Integrator2_CSTATE_j;      // '<S17>/Integrator2'
  boolean_T Integrator_CSTATE_b;       // '<S18>/Integrator'
  boolean_T Integrator2_CSTATE_l;      // '<S18>/Integrator2'
  boolean_T Integrator_CSTATE_k;       // '<S28>/Integrator'
  boolean_T Integrator_CSTATE_g;       // '<S29>/Integrator'
  boolean_T Integrator_CSTATE_n;       // '<S30>/Integrator'
  boolean_T Integrator1_CSTATE_g[3];   // '<S41>/Integrator1'
  boolean_T Integrator1_CSTATE_a[4];   // '<S42>/Integrator1'
  boolean_T Integrator_CSTATE_o[3];    // '<S39>/Integrator'
  boolean_T Integrator_CSTATE_e[3];    // '<S41>/Integrator'
  boolean_T Integrator_CSTATE_d[3];    // '<S228>/Integrator'
} XDis;

// Invariant block signals (auto storage)
typedef const struct tag_ConstB {
  real_T Product1;                     // '<S125>/Product1'
  real_T Product2_n;                   // '<S133>/Product2'
  real_T Product_a[9];                 // '<S53>/Product'
  real_T Product1_b;                   // '<S80>/Product1'
  real_T Product2_f;                   // '<S109>/Product2'
  real_T umass[3];                     // '<S41>/1//mass'
} ConstB;

#ifndef ODE3_INTG
#define ODE3_INTG

// ODE3 Integration Data
typedef struct {
  real_T *y;                           // output
  real_T *f[3];                        // derivatives
} ODE3_IntgData;

#endif

// Constant parameters (auto storage)
typedef struct {
  // Pooled Parameter (Expression: k)
  //  Referenced by:
  //    '<S186>/k[13][13]'
  //    '<S177>/k[13][13]'

  real_T pooled5[169];

  // Pooled Parameter (Expression: snorm)
  //  Referenced by:
  //    '<S170>/snorm[169]'
  //    '<S170>/Unit Delay1'

  real_T pooled8[169];

  // Expression: c
  //  Referenced by: '<S171>/c[maxdef][maxdef]'

  real_T cmaxdefmaxdef_Value[169];

  // Expression: cd
  //  Referenced by: '<S171>/cd[maxdef][maxdef]'

  real_T cdmaxdefmaxdef_Value[169];

  // Expression: fm
  //  Referenced by: '<S169>/fm'

  real_T fm_Value[13];

  // Expression: fn
  //  Referenced by: '<S169>/fn'

  real_T fn_Value[13];

  // Pooled Parameter (Mixed Expressions)
  //  Referenced by:
  //    '<S209>/sp[11]'
  //    '<S209>/cp[11]'
  //    '<S209>/Constant'
  //    '<S209>/Constant1'

  real_T pooled12[11];

  // Expression: [0 0 (1:(maxdef-1))]
  //  Referenced by: '<S162>/sp[13]'

  real_T sp13_Y0[13];

  // Expression: [1 1 (1:(maxdef-1))]
  //  Referenced by: '<S162>/cp[13]'

  real_T cp13_Y0[13];

  // Pooled Parameter (Expression: sim_params.bus.inertia)
  //  Referenced by:
  //    '<S51>/inertia_tensor'
  //    '<S51>/inertia_tensor1'
  //    '<S81>/Inertia'

  real_T pooled41[9];

  // Expression: 16
  //  Referenced by: '<S77>/lst'

  real_T lst_Value;

  // Pooled Parameter (Expression: 150)
  //  Referenced by:
  //    '<S77>/f107'
  //    '<S77>/f107a'

  real_T pooled42;

  // Expression: [4 0 0 0 0 0 0]
  //  Referenced by: '<S77>/aph'

  real_T aph_Value[7];

  // Expression: flags
  //  Referenced by: '<S77>/flags'

  real_T flags_Value[23];
} ConstP;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T blkStateChange;
  real_T odeY[28];
  real_T odeF[3][28];
  ODE3_IntgData intgData;

  //
  //  Sizes:
  //  The following substructure contains sizes information
  //  for many of the model attributes such as inputs, outputs,
  //  dwork, sample times, etc.

  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTick6;
    boolean_T firstInitCondFlag;
    struct {
      uint16_T TID1_2;
      uint16_T TID1_6;
    } RateInteraction;

    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[7];
  } Timing;
};

// Continuous states (auto storage)
extern X rtX;

// Block signals and states (auto storage)
extern DW rtDW;

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

extern const ConstB rtConstB;          // constant block i/o

// Constant parameters (auto storage)
extern const ConstP rtConstP;

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void MainSimulation_initialize(void);
  extern void MainSimulation_step0(void);
  extern void MainSimulation_step2(void);
  extern void MainSimulation_step3(void);
  extern void MainSimulation_step4(void);
  extern void MainSimulation_step5(void);
  extern void MainSimulation_step6(void);
  extern void MainSimulation_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL *const rtM;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S13>/Data Type Duplicate' : Unused code path elimination
//  Block '<S13>/Diff' : Unused code path elimination
//  Block '<S13>/TSamp' : Unused code path elimination
//  Block '<S13>/UD' : Unused code path elimination
//  Block '<S14>/Data Type Duplicate' : Unused code path elimination
//  Block '<S14>/Diff' : Unused code path elimination
//  Block '<S14>/TSamp' : Unused code path elimination
//  Block '<S14>/UD' : Unused code path elimination
//  Block '<S15>/Data Type Duplicate' : Unused code path elimination
//  Block '<S15>/Diff' : Unused code path elimination
//  Block '<S15>/TSamp' : Unused code path elimination
//  Block '<S15>/UD' : Unused code path elimination
//  Block '<S6>/wheel Inertia' : Unused code path elimination
//  Block '<S6>/wheel Inertia1' : Unused code path elimination
//  Block '<S6>/wheel Inertia2' : Unused code path elimination
//  Block '<S7>/Constant1' : Unused code path elimination
//  Block '<S7>/Constant2' : Unused code path elimination
//  Block '<S7>/Constant3' : Unused code path elimination
//  Block '<S7>/Constant4' : Unused code path elimination
//  Block '<S7>/Constant5' : Unused code path elimination
//  Block '<S7>/Data Type Conversion' : Unused code path elimination
//  Block '<S7>/Data Type Conversion1' : Unused code path elimination
//  Block '<S8>/Constant' : Unused code path elimination
//  Block '<S8>/Constant1' : Unused code path elimination
//  Block '<S9>/Logical Operator' : Unused code path elimination
//  Block '<S9>/Logical Operator1' : Unused code path elimination
//  Block '<S28>/Abs' : Unused code path elimination
//  Block '<S31>/Compare' : Unused code path elimination
//  Block '<S31>/Constant' : Unused code path elimination
//  Block '<S32>/Compare' : Unused code path elimination
//  Block '<S32>/Constant' : Unused code path elimination
//  Block '<S28>/Product' : Unused code path elimination
//  Block '<S28>/W_2_mW' : Unused code path elimination
//  Block '<S29>/Abs' : Unused code path elimination
//  Block '<S33>/Compare' : Unused code path elimination
//  Block '<S33>/Constant' : Unused code path elimination
//  Block '<S34>/Compare' : Unused code path elimination
//  Block '<S34>/Constant' : Unused code path elimination
//  Block '<S29>/Product' : Unused code path elimination
//  Block '<S29>/W_2_mW' : Unused code path elimination
//  Block '<S30>/Abs' : Unused code path elimination
//  Block '<S35>/Compare' : Unused code path elimination
//  Block '<S35>/Constant' : Unused code path elimination
//  Block '<S36>/Compare' : Unused code path elimination
//  Block '<S36>/Constant' : Unused code path elimination
//  Block '<S30>/Product' : Unused code path elimination
//  Block '<S30>/W_2_mW' : Unused code path elimination
//  Block '<S40>/Constant1' : Unused code path elimination
//  Block '<S40>/Constant2' : Unused code path elimination
//  Block '<S40>/Constant3' : Unused code path elimination
//  Block '<S40>/Constant4' : Unused code path elimination
//  Block '<S69>/1//mass' : Unused code path elimination
//  Block '<S87>/Unit Conversion' : Unused code path elimination
//  Block '<S69>/Sum1' : Unused code path elimination
//  Block '<S81>/Math Function' : Unused code path elimination
//  Block '<S81>/Matrix Multiply' : Unused code path elimination
//  Block '<S127>/Constant' : Unused code path elimination
//  Block '<S127>/Divide' : Unused code path elimination
//  Block '<S127>/Math Function' : Unused code path elimination
//  Block '<S127>/Math Function1' : Unused code path elimination
//  Block '<S127>/Product' : Unused code path elimination
//  Block '<S127>/Sum of Elements' : Unused code path elimination
//  Block '<S127>/Switch' : Unused code path elimination
//  Block '<S156>/Unit Conversion' : Unused code path elimination
//  Block '<S157>/Unit Conversion' : Unused code path elimination
//  Block '<S72>/Constant1' : Unused code path elimination
//  Block '<S72>/Constant2' : Unused code path elimination
//  Block '<S72>/Constant3' : Unused code path elimination
//  Block '<S72>/Constant4' : Unused code path elimination
//  Block '<S72>/Constant5' : Unused code path elimination
//  Block '<S73>/Constant1' : Unused code path elimination
//  Block '<S73>/Constant2' : Unused code path elimination
//  Block '<S73>/Constant3' : Unused code path elimination
//  Block '<S73>/Constant5' : Unused code path elimination
//  Block '<S225>/Constant26' : Unused code path elimination
//  Block '<S225>/Constant6' : Unused code path elimination
//  Block '<S226>/   ' : Unused code path elimination
//  Block '<S226>/       ' : Unused code path elimination
//  Block '<S226>/1 Hz' : Unused code path elimination
//  Block '<S226>/1 Hz ' : Unused code path elimination
//  Block '<S226>/Constant2' : Unused code path elimination
//  Block '<S226>/Math Function' : Unused code path elimination
//  Block '<S226>/Pulse Generator' : Unused code path elimination
//  Block '<S227>/Constant1' : Unused code path elimination
//  Block '<S227>/Constant2' : Unused code path elimination
//  Block '<S227>/Constant3' : Unused code path elimination
//  Block '<S227>/Constant4' : Unused code path elimination
//  Block '<S227>/Constant5' : Unused code path elimination
//  Block '<S227>/Constant6' : Unused code path elimination
//  Block '<S227>/Constant7' : Unused code path elimination
//  Block '<S228>/ARW' : Unused code path elimination
//  Block '<S228>/Pulse Generator' : Unused code path elimination
//  Block '<S228>/Quantizer' : Unused code path elimination
//  Block '<S228>/Quantizer1' : Unused code path elimination
//  Block '<S228>/Quantizer2' : Unused code path elimination
//  Block '<S228>/Random Number' : Unused code path elimination
//  Block '<S228>/Sum' : Unused code path elimination
//  Block '<S228>/Sum1' : Unused code path elimination
//  Block '<S229>/Constant' : Unused code path elimination
//  Block '<S229>/Constant1' : Unused code path elimination
//  Block '<S229>/Pulse Generator' : Unused code path elimination
//  Block '<S229>/Quantizer' : Unused code path elimination
//  Block '<S229>/Sum' : Unused code path elimination
//  Block '<S229>/Switch' : Unused code path elimination
//  Block '<S233>/Divide' : Unused code path elimination
//  Block '<S233>/Divide1' : Unused code path elimination
//  Block '<S233>/Dot Product' : Unused code path elimination
//  Block '<S233>/Math Function' : Unused code path elimination
//  Block '<S233>/Math Function1' : Unused code path elimination
//  Block '<S233>/Math Function2' : Unused code path elimination
//  Block '<S233>/Math Function3' : Unused code path elimination
//  Block '<S233>/Trigonometric Function' : Unused code path elimination
//  Block '<S234>/Random Number2' : Unused code path elimination
//  Block '<S234>/Random Number3' : Unused code path elimination
//  Block '<S234>/Random Number4' : Unused code path elimination
//  Block '<S234>/sigma_mag_x' : Unused code path elimination
//  Block '<S234>/sigma_mag_y' : Unused code path elimination
//  Block '<S234>/sigma_mag_z' : Unused code path elimination
//  Block '<S230>/Constant' : Unused code path elimination
//  Block '<S230>/Constant1' : Unused code path elimination
//  Block '<S230>/Quantizer' : Unused code path elimination
//  Block '<S230>/Sum' : Unused code path elimination
//  Block '<S230>/Switch' : Unused code path elimination
//  Block '<S235>/Dot Product' : Unused code path elimination
//  Block '<S235>/Normalization' : Unused code path elimination
//  Block '<S235>/Normalization1' : Unused code path elimination
//  Block '<S235>/Trigonometric Function' : Unused code path elimination
//  Block '<S236>/Random Number2' : Unused code path elimination
//  Block '<S236>/Random Number3' : Unused code path elimination
//  Block '<S236>/Random Number4' : Unused code path elimination
//  Block '<S236>/sigma_mag_x' : Unused code path elimination
//  Block '<S236>/sigma_mag_y' : Unused code path elimination
//  Block '<S236>/sigma_mag_z' : Unused code path elimination
//  Block '<S231>/Constant2' : Unused code path elimination
//  Block '<S231>/Constant3' : Unused code path elimination
//  Block '<S231>/Product' : Unused code path elimination
//  Block '<S231>/Pulse Generator' : Unused code path elimination
//  Block '<S231>/Quantizer' : Unused code path elimination
//  Block '<S240>/Reshape (9) to [3x3] column-major' : Unused code path elimination
//  Block '<S231>/Switch1' : Unused code path elimination
//  Block '<S238>/Divide' : Unused code path elimination
//  Block '<S238>/Divide1' : Unused code path elimination
//  Block '<S238>/Dot Product' : Unused code path elimination
//  Block '<S238>/Math Function' : Unused code path elimination
//  Block '<S238>/Math Function1' : Unused code path elimination
//  Block '<S238>/Math Function2' : Unused code path elimination
//  Block '<S238>/Math Function3' : Unused code path elimination
//  Block '<S238>/Trigonometric Function' : Unused code path elimination
//  Block '<S6>/Rate Transition1' : Eliminated since input and output rates are identical
//  Block '<S6>/Rate Transition2' : Eliminated since input and output rates are identical
//  Block '<S6>/Rate Transition3' : Eliminated since input and output rates are identical
//  Block '<S29>/mu' : Eliminated nontunable gain of 1
//  Block '<S54>/Reshape' : Reshape block reduction
//  Block '<S84>/Unit Conversion' : Eliminated nontunable gain of 1
//  Block '<S86>/Unit Conversion' : Eliminated nontunable gain of 1
//  Block '<S115>/Reshape' : Reshape block reduction
//  Block '<S149>/Reshape (9) to [3x3] column-major' : Reshape block reduction
//  Block '<S154>/maxtype' : Eliminate redundant data type conversion
//  Block '<S154>/mintype' : Eliminate redundant data type conversion
//  Block '<S155>/Unit Conversion' : Eliminated nontunable gain of 1
//  Block '<S128>/Unit Conversion2' : Eliminated nontunable gain of 1
//  Block '<S177>/Reshape' : Reshape block reduction
//  Block '<S184>/Reshape' : Reshape block reduction
//  Block '<S185>/Reshape' : Reshape block reduction
//  Block '<S186>/Reshape' : Reshape block reduction
//  Block '<S186>/Reshape1' : Reshape block reduction
//  Block '<S162>/Gain' : Eliminated nontunable gain of 1
//  Block '<S162>/Gain1' : Eliminated nontunable gain of 1
//  Block '<S83>/Reshape' : Reshape block reduction


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'MainSimulation'
//  '<S1>'   : 'MainSimulation/Actuators'
//  '<S2>'   : 'MainSimulation/Dynamics'
//  '<S3>'   : 'MainSimulation/Environment'
//  '<S4>'   : 'MainSimulation/Flightsoftware'
//  '<S5>'   : 'MainSimulation/Sensors'
//  '<S6>'   : 'MainSimulation/Actuators/Reaction wheel Assembly'
//  '<S7>'   : 'MainSimulation/Actuators/bus_stub_act_meas'
//  '<S8>'   : 'MainSimulation/Actuators/bus_stub_control'
//  '<S9>'   : 'MainSimulation/Actuators/magnetorquer_assembly_lib'
//  '<S10>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller'
//  '<S11>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller1'
//  '<S12>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller2'
//  '<S13>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Discrete Derivative'
//  '<S14>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Discrete Derivative1'
//  '<S15>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Discrete Derivative2'
//  '<S16>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib'
//  '<S17>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib1'
//  '<S18>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib2'
//  '<S19>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller/Discrete PID Controller (2DOF)'
//  '<S20>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller1/Discrete PID Controller (2DOF)'
//  '<S21>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/Closed-loop controller2/Discrete PID Controller (2DOF)'
//  '<S22>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib/map w,i to V'
//  '<S23>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib/map w_cmd to V_cmd'
//  '<S24>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib1/map w,i to V'
//  '<S25>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib1/map w_cmd to V_cmd'
//  '<S26>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib2/map w,i to V'
//  '<S27>'  : 'MainSimulation/Actuators/Reaction wheel Assembly/dc_motor_lib2/map w_cmd to V_cmd'
//  '<S28>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib'
//  '<S29>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib1'
//  '<S30>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib3'
//  '<S31>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib/Compare To Constant'
//  '<S32>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib/Compare To Constant1'
//  '<S33>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib1/Compare To Constant'
//  '<S34>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib1/Compare To Constant1'
//  '<S35>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib3/Compare To Constant'
//  '<S36>'  : 'MainSimulation/Actuators/magnetorquer_assembly_lib/simple_magnetorquer_lib3/Compare To Constant1'
//  '<S37>'  : 'MainSimulation/Dynamics/3x3 Cross Product'
//  '<S38>'  : 'MainSimulation/Dynamics/Quaternion Rotation'
//  '<S39>'  : 'MainSimulation/Dynamics/RigidBodyDynamics'
//  '<S40>'  : 'MainSimulation/Dynamics/bus_stub_states'
//  '<S41>'  : 'MainSimulation/Dynamics/orbital_dynamics'
//  '<S42>'  : 'MainSimulation/Dynamics/quat_propagation'
//  '<S43>'  : 'MainSimulation/Dynamics/3x3 Cross Product/Subsystem'
//  '<S44>'  : 'MainSimulation/Dynamics/3x3 Cross Product/Subsystem1'
//  '<S45>'  : 'MainSimulation/Dynamics/Quaternion Rotation/Quaternion Normalize'
//  '<S46>'  : 'MainSimulation/Dynamics/Quaternion Rotation/V1'
//  '<S47>'  : 'MainSimulation/Dynamics/Quaternion Rotation/V2'
//  '<S48>'  : 'MainSimulation/Dynamics/Quaternion Rotation/V3'
//  '<S49>'  : 'MainSimulation/Dynamics/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
//  '<S50>'  : 'MainSimulation/Dynamics/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S51>'  : 'MainSimulation/Dynamics/RigidBodyDynamics/dynamics'
//  '<S52>'  : 'MainSimulation/Dynamics/RigidBodyDynamics/dynamics/Cross Product'
//  '<S53>'  : 'MainSimulation/Dynamics/RigidBodyDynamics/dynamics/Invert  3x3 Matrix'
//  '<S54>'  : 'MainSimulation/Dynamics/RigidBodyDynamics/dynamics/Invert  3x3 Matrix/Determinant of 3x3 Matrix'
//  '<S55>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Inverse'
//  '<S56>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation'
//  '<S57>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Inverse/Quaternion Conjugate'
//  '<S58>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Inverse/Quaternion Norm'
//  '<S59>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/Quaternion Normalize'
//  '<S60>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/V1'
//  '<S61>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/V2'
//  '<S62>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/V3'
//  '<S63>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
//  '<S64>'  : 'MainSimulation/Dynamics/orbital_dynamics/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S65>'  : 'MainSimulation/Dynamics/quat_propagation/MATLAB Function'
//  '<S66>'  : 'MainSimulation/Dynamics/quat_propagation/Quaternion Normalize'
//  '<S67>'  : 'MainSimulation/Dynamics/quat_propagation/Quaternion Normalize/Quaternion Modulus'
//  '<S68>'  : 'MainSimulation/Dynamics/quat_propagation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S69>'  : 'MainSimulation/Environment/Disturbances'
//  '<S70>'  : 'MainSimulation/Environment/GPS model'
//  '<S71>'  : 'MainSimulation/Environment/Translate to UT1'
//  '<S72>'  : 'MainSimulation/Environment/bus_stub_disturbances'
//  '<S73>'  : 'MainSimulation/Environment/bus_stub_orbit_data'
//  '<S74>'  : 'MainSimulation/Environment/eci_2_ecef_lib'
//  '<S75>'  : 'MainSimulation/Environment/sgp4_lib'
//  '<S76>'  : 'MainSimulation/Environment/teme_2_eci_lib'
//  '<S77>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model'
//  '<S78>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque'
//  '<S79>'  : 'MainSimulation/Environment/Disturbances/aero_drag'
//  '<S80>'  : 'MainSimulation/Environment/Disturbances/ecef_to_lla'
//  '<S81>'  : 'MainSimulation/Environment/Disturbances/grav_dist'
//  '<S82>'  : 'MainSimulation/Environment/Disturbances/mag_vector'
//  '<S83>'  : 'MainSimulation/Environment/Disturbances/sun_vector'
//  '<S84>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model/Density Conversion'
//  '<S85>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model/Length Conversion'
//  '<S86>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model/Mass Conversion'
//  '<S87>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model/Temperature Conversion'
//  '<S88>'  : 'MainSimulation/Environment/Disturbances/NRLMSISE-00 Atmosphere Model/recombine outputs'
//  '<S89>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/MATLAB Function'
//  '<S90>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation'
//  '<S91>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/Quaternion Normalize'
//  '<S92>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/V1'
//  '<S93>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/V2'
//  '<S94>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/V3'
//  '<S95>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
//  '<S96>'  : 'MainSimulation/Environment/Disturbances/Solar Disturbance Torque/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S97>'  : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation'
//  '<S98>'  : 'MainSimulation/Environment/Disturbances/aero_drag/drag'
//  '<S99>'  : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/Quaternion Normalize'
//  '<S100>' : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/V1'
//  '<S101>' : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/V2'
//  '<S102>' : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/V3'
//  '<S103>' : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
//  '<S104>' : 'MainSimulation/Environment/Disturbances/aero_drag/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S105>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/Conversion'
//  '<S106>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/Subsystem2'
//  '<S107>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/Subsystem3'
//  '<S108>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/While Iterator Subsystem'
//  '<S109>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/e^1'
//  '<S110>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/e^2'
//  '<S111>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/e^3'
//  '<S112>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/While Iterator Subsystem/Subsystem2'
//  '<S113>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/While Iterator Subsystem/Subsystem3'
//  '<S114>' : 'MainSimulation/Environment/Disturbances/ecef_to_lla/While Iterator Subsystem/Subsystem4'
//  '<S115>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body'
//  '<S116>' : 'MainSimulation/Environment/Disturbances/grav_dist/grav_grad'
//  '<S117>' : 'MainSimulation/Environment/Disturbances/grav_dist/gravity'
//  '<S118>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation'
//  '<S119>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/Quaternion Normalize'
//  '<S120>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/V1'
//  '<S121>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/V2'
//  '<S122>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/V3'
//  '<S123>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
//  '<S124>' : 'MainSimulation/Environment/Disturbances/grav_dist/eci_to_body/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S125>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA'
//  '<S126>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED'
//  '<S127>' : 'MainSimulation/Environment/Disturbances/mag_vector/Normalize Vector'
//  '<S128>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015'
//  '<S129>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/Conversion'
//  '<S130>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/Subsystem2'
//  '<S131>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/Subsystem3'
//  '<S132>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/While Iterator Subsystem'
//  '<S133>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/e^1'
//  '<S134>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/e^2'
//  '<S135>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/e^3'
//  '<S136>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem2'
//  '<S137>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem3'
//  '<S138>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoLLA/While Iterator Subsystem/Subsystem4'
//  '<S139>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A11'
//  '<S140>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A12'
//  '<S141>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A13'
//  '<S142>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A21'
//  '<S143>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A22'
//  '<S144>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A23'
//  '<S145>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A31'
//  '<S146>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A32'
//  '<S147>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/A33'
//  '<S148>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/Angle Conversion'
//  '<S149>' : 'MainSimulation/Environment/Disturbances/mag_vector/ECEFtoNED/Create Transformation Matrix'
//  '<S150>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Check Altitude'
//  '<S151>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Check Latitude'
//  '<S152>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Check Longitude'
//  '<S153>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field'
//  '<S154>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Is time within model limits'
//  '<S155>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Length Conversion'
//  '<S156>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/MagField Conversion'
//  '<S157>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/MagField Conversion1'
//  '<S158>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag'
//  '<S159>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/Compute x,y,z, and h components of magnetic field/Angle Conversion'
//  '<S160>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates'
//  '<S161>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates'
//  '<S162>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates '
//  '<S163>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude'
//  '<S164>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Has altitude or latitude changed'
//  '<S165>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Has longitude changed '
//  '<S166>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Has time changed'
//  '<S167>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity'
//  '<S168>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem'
//  '<S169>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion'
//  '<S170>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations'
//  '<S171>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients'
//  '<S172>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole'
//  '<S173>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate  index'
//  '<S174>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values'
//  '<S175>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/special case'
//  '<S176>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem1'
//  '<S177>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2'
//  '<S178>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  indices'
//  '<S179>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/Special case - North//South Geographic Pole/If Action Subsystem2/calculate  row and column'
//  '<S180>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem'
//  '<S181>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1'
//  '<S182>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/m,n'
//  '<S183>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Accumulate terms of the  spherical harmonic expansion/calculate temp values/If Action Subsystem1/n,m-1'
//  '<S184>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem'
//  '<S185>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1'
//  '<S186>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2'
//  '<S187>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/calculate  index'
//  '<S188>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  index'
//  '<S189>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem/calculate  row and column'
//  '<S190>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  index'
//  '<S191>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem1/calculate  row and column'
//  '<S192>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  indices'
//  '<S193>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column1'
//  '<S194>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/calculate  row and column2'
//  '<S195>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2'
//  '<S196>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Compute unnormalized associated  legendre polynomials and  derivatives via recursion relations/If Action Subsystem2/m<n-2 '
//  '<S197>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/If Action Subsystem'
//  '<S198>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)'
//  '<S199>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem1'
//  '<S200>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Compute magnetic vector in spherical coordinates/For Iterator Subsystem/Time adjust the gauss coefficients/if (m~=0)/If Action Subsystem2'
//  '<S201>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ca'
//  '<S202>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate ct'
//  '<S203>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate d'
//  '<S204>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q'
//  '<S205>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate q2'
//  '<S206>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate r2'
//  '<S207>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate sa'
//  '<S208>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates/calculate st'
//  '<S209>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Convert from geodetic to  spherical coordinates /For Iterator Subsystem'
//  '<S210>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Get Cosine and Sine  of Latitude and Longitude/Angle Conversion2'
//  '<S211>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bx'
//  '<S212>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate by'
//  '<S213>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Calculate bz'
//  '<S214>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity'
//  '<S215>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion'
//  '<S216>' : 'MainSimulation/Environment/Disturbances/mag_vector/World Magnetic Model 2015/geomag/Rotate magnetic vector components  to geodetic from spherical and  compute declination, inclination  and total intensity/Compute declination, inclination,  and total intensity/Angle Conversion1'
//  '<S217>' : 'MainSimulation/Environment/Disturbances/sun_vector/sun_vector_lib'
//  '<S218>' : 'MainSimulation/Environment/Disturbances/sun_vector/sun_vector_lib/MATLAB Function'
//  '<S219>' : 'MainSimulation/Environment/Disturbances/sun_vector/sun_vector_lib/MATLAB Function1'
//  '<S220>' : 'MainSimulation/Environment/GPS model/MATLAB Function'
//  '<S221>' : 'MainSimulation/Environment/Translate to UT1/Translate to UT1'
//  '<S222>' : 'MainSimulation/Environment/eci_2_ecef_lib/eci_2_ecef_lib'
//  '<S223>' : 'MainSimulation/Environment/sgp4_lib/MATLAB Function'
//  '<S224>' : 'MainSimulation/Environment/teme_2_eci_lib/TEME to ECI'
//  '<S225>' : 'MainSimulation/Flightsoftware/bus_stub_commands'
//  '<S226>' : 'MainSimulation/Sensors/GPS'
//  '<S227>' : 'MainSimulation/Sensors/bus_stub_sens_meas'
//  '<S228>' : 'MainSimulation/Sensors/gyro_sim_lib'
//  '<S229>' : 'MainSimulation/Sensors/magnetometer_sim_lib'
//  '<S230>' : 'MainSimulation/Sensors/photodiodes_lib'
//  '<S231>' : 'MainSimulation/Sensors/sun_sensor_lib '
//  '<S232>' : 'MainSimulation/Sensors/GPS/MATLAB Function'
//  '<S233>' : 'MainSimulation/Sensors/magnetometer_sim_lib/deg_err'
//  '<S234>' : 'MainSimulation/Sensors/magnetometer_sim_lib/gaussian_error_vector'
//  '<S235>' : 'MainSimulation/Sensors/photodiodes_lib/deg_err'
//  '<S236>' : 'MainSimulation/Sensors/photodiodes_lib/gaussian_error_vector'
//  '<S237>' : 'MainSimulation/Sensors/sun_sensor_lib /Rotation Angles to Direction Cosine Matrix'
//  '<S238>' : 'MainSimulation/Sensors/sun_sensor_lib /deg_err1'
//  '<S239>' : 'MainSimulation/Sensors/sun_sensor_lib /gaussian_error_vector'
//  '<S240>' : 'MainSimulation/Sensors/sun_sensor_lib /Rotation Angles to Direction Cosine Matrix/Create 3x3 Matrix'

#endif                                 // RTW_HEADER_MainSimulation_h_

//
// File trailer for generated code.
//
// [EOF]
//
