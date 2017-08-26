#include <msp430.h>

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/timers.h"

uint32_t tempTime = 0;
uint32_t currentPeriod = 0;
uint32_t overflowCount = 0;

#define OUTPUT_INCREMENT  26
#define CLKFACTOR_8MHZ  240000000
#define CLK_PERIOD_8MHZ  0.000000125

uint16_t outputCounter = 0;
float currentRPM = 0.0;

// PWM output stuff
#define TIMER(reg)          T##B0##reg
#define ROOT_TIMER(bits)    T##B##bits

// FOR PID
#define TARGET_RPM   2000.0        // In RPM
#define PID_LOOP_FREQ   5       // In Hz
#define MAX_PWM_OUT     999    // Assume 1000 CCR counter on PWM out
#define MIN_PWM_OUT     0

unsigned long lastTime;
double Input, rawOutput, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;

void rwsCompute()
{
    // Calc time delta
    unsigned long now = TA2R;
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

    // Set pin P1.2 for input, to monitor frequency (and thus RPM) of motor
    // This is using TA1.CCI1A - set to '0 0 1'
    P1DIR &= ~BIT2;
    P1SEL1 &= ~BIT2;
    P1SEL0 |= BIT2;

    // For LED
    P1DIR |= BIT0;

    // Configure GPIO - Timer B - for PWM
    P1DIR |= BIT4 | BIT5;                   // P1.4 and P1.5 output
    P1SEL0 |= BIT4 | BIT5;                  // P1.4 and P1.5 options select
    P1SEL1 &= ~(BIT4 | BIT5);

    // Timer1_A1 Setup:  rising edge, CCIA input, sync mode, capture mode and interrupts enabled
    // SMCLK as clk source, continuous mode
    TA1CCTL1 = CM__RISING | CCIS__CCIA | SCS | CAP | CCIE;
    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | TACLR | TAIE;

    // This timer strictly drives compute run frequency for the PID controller
    TA2CTL = TASSEL__ACLK | MC__CONTINOUS | TACLR;

    debugPrintF("Timer initialized for capture.\r\n");

    //__bis_SR_register(LPM0_bits | GIE);
    __bis_SR_register(GIE);

    rwsSetTuningParams(2.9, 0.0, 0.0);
    Setpoint = TARGET_RPM;

    TIMER(CCR0) = 1000;
    TIMER(CCTL1) = OUTMOD_7;
    TIMER(CCR1) = 0;
    TIMER(CTL) = ROOT_TIMER(SSEL__SMCLK) | MC__UP | ROOT_TIMER(CLR);  // SMCLK, up mode, clear TAR

    while (1)
    {
        P1OUT ^= 0x01;
        __delay_cycles(2000000);
        Input = currentRPM;
        rwsCompute();
        TIMER(CCR1) = Output;
        debugPrintF("%f,%f\r\n", Input, Output);
    }

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

	return 0;
}

uint8_t initialTAIFG = 1;

// Timer1_A1 CC1-4, TA Interrupt Handler
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
{
    switch (__even_in_range(TA1IV, TAIV__TAIFG))
    {
        case TAIV__TACCR1:
            tempTime = (overflowCount << 16);
            currentPeriod = tempTime + TA1CCR1;
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
            TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | TACLR | TAIE;

            __no_operation();
            break;
        case TAIV__TACCR2: break;           // not used
        case TAIV__TACCR3: break;           // not used
        case TAIV__TACCR4: break;           // not used
        case TAIV__TACCR5: break;           // not used
        case TAIV__TACCR6: break;           // not used
        case TAIV__TAIFG:                   // TimerA1 overflow
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
