#include <msp430.h>

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/timers.h"

uint32_t tempTime = 0;
uint32_t currentPeriod = 0;
uint32_t overflowCount = 0;

#define OUTPUT_INCREMENT  5
#define CLKFACTOR_8MHZ  240000000
#define CLK_PERIOD_8MHZ  0.000000125

uint16_t outputCounter = 0;
float currentRPM = 0.0;

// TIMER MACROS:  experiment to see if this cleans up the code
// PWM timer
#define PWM_TIMER(reg)          TB0##reg
#define PWM_ROOT_TIMER(bits)    TB##bits

// Frequency capture timer
#define FREQ_TIMER(reg)         TA4##reg
#define FREQ_ROOT_TIMER(bits)   TA##bits

// Timestamp timer
#define TIMESTAMP_TIMER(reg)    TA2##reg
#define TIMESTAMP_ROOT_TIMER(bits)  TA##bits

// Pin definitions
// TODO:  Move these into BSP when it makes sense
#define RW_FREQ_DIR     P7DIR
#define RW_FREQ_SEL1    P7SEL1
#define RW_FREQ_SEL0    P7SEL0
#define RW_FREQ_PIN     BIT4

#define RW_MOTORDIR_DIR     P7DIR
#define RW_MOTORDIR_SEL1    P7SEL1
#define RW_MOTORDIR_SEL0    P7SEL0
#define RW_MOTORDIR_PIN     BIT3

#define RW_PWM_DIR      P1DIR
#define RW_PWM_SEL1     P1SEL1
#define RW_PWM_SEL0     P1SEL0
#define RW_PWM_PIN      BIT7

// Configuration for PID
#define INITIAL_TARGET_RPM   200.0        // In RPM
#define PID_LOOP_FREQ   5       // In Hz
#define MAX_PWM_OUT     999    // Assume 1000 CCR counter on PWM out
#define MIN_PWM_OUT     0
// TODO:  Add support for negative speeds (with special handling to flip the direction bit)

unsigned long lastTime;
double Input, rawOutput, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;

void rwsCompute()
{
    // Calc time delta
    unsigned long now = TIMESTAMP_TIMER(R);
    double timeChange  = (double)(CLK_PERIOD_8MHZ * timerCycleDiff16(lastTime, now));

    // Compute vars
    double error = Setpoint - Input;
    errSum += (error * timeChange);
    double dErr = (error - lastErr) / timeChange;

    // Compute PID output
    rawOutput = (kp * error) + (ki * errSum) + (kd * dErr);

    if (rawOutput < MIN_PWM_OUT)
        Output = MIN_PWM_OUT;
    else if (rawOutput > MAX_PWM_OUT)
        Output = MAX_PWM_OUT;
    else
        Output = rawOutput;

    // Store values for subsequent comparisons
    lastErr = error;
    lastTime = now;

    //debugPrintF("%f,%f\r\n", error, Output);
}

void rwsSetTuningParams(double Kp, double Ki, double Kd)
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

/*
 * main.c
 */


int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    //  FR (direction):  P7.3 to control direction, P7.4 to read FG pin ('1 0 0')
    RW_MOTORDIR_DIR |= RW_MOTORDIR_PIN;
    RW_MOTORDIR_SEL1 &= ~RW_MOTORDIR_PIN;
    RW_MOTORDIR_SEL0 &= ~RW_MOTORDIR_PIN;

    // Configure free-running timer to generate timestamps
    // TODO:  eventually move this to a clock library function
    TIMESTAMP_TIMER(CTL) = TIMESTAMP_ROOT_TIMER(SSEL__ACLK) | MC__CONTINOUS | TIMESTAMP_ROOT_TIMER(CLR);

    // All pins configured to work with RW/CANMSP block board circa September 2017
    // FG:  Pin P7.4 for input, to monitor frequency (and thus RPM) of motor
    // This is using TA4.CCI0B - set to '0 1 0'
    RW_FREQ_DIR &= ~RW_FREQ_PIN;
    RW_FREQ_SEL1 |= RW_FREQ_PIN;
    RW_FREQ_SEL0 &= ~RW_FREQ_PIN;

    // Timer1_A4 setup for capturing FG pin frequency:
    // Rising edge, CCIA input, sync mode, capture mode and interrupts enabled
    // SMCLK as clk source, continuous mode
    FREQ_TIMER(CCTL0) = CM__RISING | CCIS__CCIB | SCS | CAP | CCIE;
    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__SMCLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | TAIE;

    // PWM output:  pin P1.7 ('1 0 1') - as TB0.4
    RW_PWM_DIR |= RW_PWM_PIN;
    RW_PWM_SEL1 &= ~RW_PWM_PIN;
    RW_PWM_SEL0 |= RW_PWM_PIN;

    // PWM timer configuration
    PWM_TIMER(CCR0) = MAX_PWM_OUT + 1;
    PWM_TIMER(CCTL4) = OUTMOD_7;
    PWM_TIMER(CCR4) = 0;
    PWM_TIMER(CTL) = PWM_ROOT_TIMER(SSEL__SMCLK) | MC__UP | PWM_ROOT_TIMER(CLR);  // SMCLK, up mode, clear TAR

    debugPrintF("Timers initialized for RW wheel drive and frequency capture.\r\n");

    //__bis_SR_register(LPM0_bits | GIE);
    __bis_SR_register(GIE);

    rwsSetTuningParams(2.9, 0.0, 0.0);
    Setpoint = INITIAL_TARGET_RPM;

    PWM_TIMER(CCR4) = 100;

    uint16_t sequencecounter = 0;
    while (1)
    {
        sequencecounter++;
        if (sequencecounter < 200)
            Setpoint = 300;
        else if (sequencecounter < 500)
            Setpoint = 3000;
        else if (sequencecounter < 900)
            Setpoint = 150;
        else if (sequencecounter < 1500)
            Setpoint = 8000;
        else if (sequencecounter < 2000)
            Setpoint = 1800;
        else
            Setpoint = 0;


        __delay_cycles(.1 * SEC);
        Input = currentRPM;
        rwsCompute();
        PWM_TIMER(CCR4) = Output;
        //debugPrintF("%f,%f\r\n", Input, Output);
        debugPrintF("%f,%f\r\n", Setpoint, currentRPM);
    }

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

	return 0;
}

uint8_t initialTAIFG = 1;

// Timer4_A0 (CCR=0) TA Interrupt Handler
#pragma vector = TIMER4_A0_VECTOR
__interrupt void Timer4_A0_ISR(void)
{
    tempTime = (overflowCount << 16);
    currentPeriod = tempTime + TA4CCR0;
    currentRPM = (240000000.0)/currentPeriod;

    if (outputCounter++ > OUTPUT_INCREMENT)
    {
        currentRPM = (240000000.0)/currentPeriod;
        outputCounter = 0;
        //debugPrintF("%lu, %f\r\n", currentPeriod, currentRPM);
        //debugPrintF("%f\r\n", currentRPM);
    }

    // Start time over, to simplify roll-over handling
    overflowCount = 0;
    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__SMCLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | FREQ_ROOT_TIMER(IE);

}

// Timer4_A1 (CCR=1..n) TA Interrupt Handler
#pragma vector = TIMER4_A1_VECTOR
__interrupt void Timer4_A1_ISR(void)
{
    switch (__even_in_range(TA4IV, TAIV__TAIFG))
    {
        case TAIV__TACCR1: break;           // not used
        case TAIV__TACCR2: break;           // not used
        case TAIV__TACCR3: break;           // not used
        case TAIV__TACCR4: break;           // not used
        case TAIV__TACCR5: break;           // not used
        case TAIV__TACCR6: break;           // not used
        case TAIV__TAIFG:                   // TA4.0 overflow
            // Deal with bogus initial overflow flag that pops on startup
            if (initialTAIFG == 1)
                initialTAIFG = 0;
            else
                overflowCount++;
            break;
        default:
            break;
    }
}

