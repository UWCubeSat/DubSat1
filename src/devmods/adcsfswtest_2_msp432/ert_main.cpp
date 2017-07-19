//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
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
#include <stddef.h>
#include <stdio.h>                     // This ert_main.c example uses printf/fflush 
#include "MainSimulation.h"            // Model's header file
#include "rtwtypes.h"

#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
    static boolean_T OverrunFlags[7] = { 0, 0, 0, 0, 0, 0, 0 };

    static boolean_T eventFlags[7] = { 0, 0, 0, 0, 0, 0, 0 };// Model has 7 rates

    static int_T taskCounter[7] = { 0, 0, 0, 0, 0, 0, 0 };

    int_T i;

    // Disable interrupts here

    // Check base rate for overrun
    if (OverrunFlags[0]) {
        rtmSetErrorStatus(rtM, "Overrun");
        return;
    }

    OverrunFlags[0] = true;

    // Save FPU context here (if necessary)
    // Re-enable timer or interrupt here

    //
    //  For a bare-board target (i.e., no operating system), the
    //  following code checks whether any subrate overruns,
    //  and also sets the rates that need to run this time step.

    for (i = 2; i < 7; i++) {
        if (taskCounter[i] == 0) {
            if (eventFlags[i]) {
                OverrunFlags[0] = false;
                OverrunFlags[i] = true;

                // Sampling too fast
                rtmSetErrorStatus(rtM, "Overrun");
                return;
            }

            eventFlags[i] = true;
        }
    }

    taskCounter[1]++;
    if (taskCounter[1] == 1) {
        taskCounter[1]= 0;
    }

    taskCounter[2]++;
    if (taskCounter[2] == 2) {
        taskCounter[2]= 0;
    }

    taskCounter[3]++;
    if (taskCounter[3] == 5) {
        taskCounter[3]= 0;
    }

    taskCounter[4]++;
    if (taskCounter[4] == 20) {
        taskCounter[4]= 0;
    }

    taskCounter[5]++;
    if (taskCounter[5] == 40) {
        taskCounter[5]= 0;
    }

    taskCounter[6]++;
    if (taskCounter[6] == 200) {
        taskCounter[6]= 0;
    }

    // Set model inputs associated with base rate here

    // Step the model for base rate
    MainSimulation_step0();

    // Get model outputs here

    // Indicate task for base rate complete
    OverrunFlags[0] = false;

    // Step the model for any subrate
    for (i = 2; i < 7; i++) {
        // If task "i" is running, don't run any lower priority task
        if (OverrunFlags[i]) {
            return;
        }

        if (eventFlags[i]) {
            OverrunFlags[i] = true;

            // Set model inputs associated with subrates here

            // Step the model for subrate "i"
            switch (i) {
            case 2 :
                MainSimulation_step2();

                // Get model outputs here
                break;

            case 3 :
                MainSimulation_step3();

                // Get model outputs here
                break;

            case 4 :
                MainSimulation_step4();

                // Get model outputs here
                break;

            case 5 :
                MainSimulation_step5();

                // Get model outputs here
                break;

            case 6 :
                MainSimulation_step6();

                // Get model outputs here
                break;

            default :
                break;
            }

            // Indicate task complete for sample time "i"
            OverrunFlags[i] = false;
            eventFlags[i] = false;
        }
    }

    // Disable interrupts here
    // Restore FPU context here (if necessary)
    // Enable interrupts here
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

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
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

    /* Get current power state, if it's not AM0_LDO, error out */
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;
    if (currentPowerState != PCM_CTL0_CPM_0)
        error();

    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
        error();                            // Error if transition was not successful
    if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1)
        error();                            // Error if device is not in AM1_LDO mode

    /* Step 2: Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_BANK1_RDCTL_WAIT_1;

    /* Step 3: Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    /* Select MCLK = DCO, no divider */
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    /* Step 4: Output MCLK to port pin to demonstrate 48MHz operation */
    P4->DIR |= BIT3;
    P4->SEL0 |=BIT3;                        // Output MCLK
    P4->SEL1 &= ~(BIT3);


    // Unused arguments
    (void)(argc);
    (void)(argv);

    // Initialize model
    MainSimulation_initialize();

    // Simulating the model step behavior (in non real-time) to
    //   simulate model behavior at stop time.

    while ((rtmGetErrorStatus(rtM) == (NULL)) && !rtmGetStopRequested(rtM)) {
        P1->OUT ^= BIT0;
        rt_OneStep();
    }

    // Disable rt_OneStep() here

    // Terminate model
    MainSimulation_terminate();
    return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
