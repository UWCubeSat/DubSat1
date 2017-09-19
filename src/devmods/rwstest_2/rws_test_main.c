#include <msp430.h>
#include <rws_test.h>

#include "bsp/bsp.h"
#include "core/timers.h"
#include "actuators/rw.h"

// TIMER MACROS:  experiment to see if this cleans up the code
// PWM timer
#define PWM_TIMER(reg)          TB0##reg
#define PWM_ROOT_TIMER(bits)    TB##bits

#define INITIAL_TARGET_RPM   200.0        // In RPM
FILE_STATIC double setpoint_cmd = INITIAL_TARGET_RPM;
FILE_STATIC double output_cmd = 0;

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_ADCS_RWX);
    rwsInit();

    // PWM timer configuration
    PWM_TIMER(CCR0) = MAX_PWM_OUT + 1;
    PWM_TIMER(CCTL4) = OUTMOD_7;
    PWM_TIMER(CCR4) = 0;
    PWM_TIMER(CTL) = PWM_ROOT_TIMER(SSEL__SMCLK) | MC__UP | PWM_ROOT_TIMER(CLR);  // SMCLK, up mode, clear TAR

    debugPrintF("Timers initialized for RW wheel drive and frequency capture.\r\n");

    __bis_SR_register(GIE);

    rwsSetTuningParams(2.0, 4.4, 0.0);
    rwsRunAuto();

    uint16_t sequencecounter = 0;
    while (1)
    {
        __delay_cycles(.05 * SEC);

        // Work through a simple sequence
        //sequencecounter++;
        if (sequencecounter < 200)
            setpoint_cmd = 600;
        else if (sequencecounter < 500)
            setpoint_cmd = 3000;
        else if (sequencecounter < 900)
            setpoint_cmd = 150;
        else if (sequencecounter < 1500)
            setpoint_cmd = 8000;
        else if (sequencecounter < 2000)
            setpoint_cmd = 1800;
        else
            sequencecounter = 0;


        output_cmd = rwsPIDStep(setpoint_cmd);

        // Assign output to PWM output signal for motor driver
        PWM_TIMER(CCR4) = output_cmd;

        //debugPrintF("%f,%f\r\n", setpoint_cmd, output_cmd);
    }

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

	return 0;
}



