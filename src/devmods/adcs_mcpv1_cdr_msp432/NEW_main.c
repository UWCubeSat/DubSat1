/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 02-Nov-2017 18:25:03
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include Files */
#include <NEW_main.h>
#include "rt_nonfinite.h"
#include "mpcmoveCodeGeneration.h"
#include "mpcmoveCodeGeneration_terminate.h"
#include "mpcmoveCodeGeneration_initialize.h"

#include <msp.h>
#include <stdint.h>

/* Function Declarations */
static void argInit_1x7_real_T(double result[7]);
static void argInit_3x1_real_T(double result[3]);
static void argInit_7x1_real_T(double result[7]);
static double argInit_real_T(void);
static void argInit_struct1_T(struct1_T *result);
static void argInit_struct2_T(struct2_T *result);
static void argInit_struct3_T(struct3_T *result);
static void main_mpcmoveCodeGeneration(void);

int _system_pre_init()
{
    WDT_A->CTL = WDT_A_CTL_PW |
                WDT_A_CTL_HOLD;            // Stop WDT
    return 1;

}

/* Function Definitions */

/*
 * Arguments    : double result[7]
 * Return Type  : void
 */
static void argInit_1x7_real_T(double result[7])
{
  int idx1;

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 7; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[3]
 * Return Type  : void
 */
static void argInit_3x1_real_T(double result[3])
{
  int idx0;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 3; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[7]
 * Return Type  : void
 */
static void argInit_7x1_real_T(double result[7])
{
  int idx0;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 7; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : struct1_T *result
 * Return Type  : void
 */
static void argInit_struct1_T(struct1_T *result)
{
  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  argInit_7x1_real_T(result->Plant);
  argInit_3x1_real_T(result->LastMove);
}

/*
 * Arguments    : struct2_T *result
 * Return Type  : void
 */
static void argInit_struct2_T(struct2_T *result)
{
  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  argInit_struct3_T(&result->signals);
}

/*
 * Arguments    : struct3_T *result
 * Return Type  : void
 */
static void argInit_struct3_T(struct3_T *result)
{
  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  argInit_7x1_real_T(result->ym);
  argInit_1x7_real_T(result->ref);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_mpcmoveCodeGeneration(void)
{
  struct1_T mpcmovestate;
  struct2_T r0;
  double u[3];
  struct7_T Info;

  /* Initialize function 'mpcmoveCodeGeneration' input arguments. */
  /* Initialize function input argument 'mpcmovestate'. */
  /* Initialize function input argument 'mpcmovedata'. */
  /* Call the entry-point 'mpcmoveCodeGeneration'. */
  argInit_struct1_T(&mpcmovestate);
  argInit_struct2_T(&r0);
  mpcmoveCodeGeneration(&mpcmovestate, &r0, u, &Info);
}

void error(void)
{
    volatile uint32_t i;

    while (1)
    {
        P1->OUT ^= BIT0;
        for(i = 20000; i > 0; i--);           // Blink LED forever
    }
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
    volatile uint32_t i;
    uint32_t currentPowerState;

    WDT_A->CTL = WDT_A_CTL_PW |
            WDT_A_CTL_HOLD;            // Stop WDT

    P1->DIR |= BIT0;                        // P1.0 set as output

    /* NOTE: This example assumes the default power state is AM0_LDO.
     * Refer to  msp432p401x_pcm_0x code examples for more complete PCM
     * operations to exercise various power state transitions between active
     * modes.
     */

    /* Step 1: Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
//
//    /* Get current power state, if it's not AM0_LDO, error out */
//    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;
//    if (currentPowerState != PCM_CTL0_CPM_0)
//        error();
//
//    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
//    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
//    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
//    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
//        error();                            // Error if transition was not successful
//    if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1)
//        error();                            // Error if device is not in AM1_LDO mode
//
//    /* Step 2: Configure Flash wait-state to 1 for both banks 0 & 1 */
//    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
//            FLCTL_BANK0_RDCTL_WAIT_1;
//    FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
//            FLCTL_BANK1_RDCTL_WAIT_1;
//
//    /* Step 3: Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
//    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
//    CS->CTL0 = 0;                           // Reset tuning parameters
//    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
//    /* Select MCLK = DCO, no divider */
//    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
//            CS_CTL1_SELM_3;
//    CS->KEY = 0;                            // Lock CS module from unintended accesses
//
//    /* Step 4: Output MCLK to port pin to demonstrate 48MHz operation */
//    P4->DIR |= BIT3;
//    P4->SEL0 |=BIT3;                        // Output MCLK
//    P4->SEL1 &= ~(BIT3);

    (void)argc;
    (void)argv;

//    /* Initialize the application.
//     You do not need to do this more than one time. */
//    mpcmoveCodeGeneration_initialize();
//
//    /* Invoke the entry-point functions.
//     You can call entry-point functions multiple times. */
//    main_mpcmoveCodeGeneration();
//    P1->OUT ^= BIT0;
//
    while (1)
    {
        //main_mpcmoveCodeGeneration();
        P1->OUT ^= BIT0;
    }
    //
    //    /* Terminate the application.
//     You do not need to do this more than one time. */
//    mpcmoveCodeGeneration_terminate();
    return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
