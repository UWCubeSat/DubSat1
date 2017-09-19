/*
 * rw.c
 *
 *  Created on: Sep 18, 2017
 *      Author: jeffc
 */

#include "actuators/rw.h"
#include "core/utils.h"
#include "core/debugtools.h"
#include "core/timers.h"



FILE_STATIC unsigned long lastTime_cycles;
FILE_STATIC double input, rawOutput, lastInput, output, setpoint;
FILE_STATIC double errSum, lastErr;
FILE_STATIC double kp, ki, kd;

FILE_STATIC uint16_t outputCounter = 0;
FILE_STATIC float currentRPM = 0.0;
FILE_STATIC uint32_t tempTime_cycles = 0;
FILE_STATIC uint32_t currentPeriod_cycles = 0;
FILE_STATIC uint32_t overflowCount = 0;

FILE_STATIC uint8_t active = 0;

void rwsInit()
{
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
}

void rwsRunAuto()
{
    active = 1;
}

void rwsRunManual()
{
    active = 0;
}

void rwsSetTuningParams(double Kp, double Ki, double Kd)
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

double rwsPIDStep(double cmd)
{
    if (active != 1)
        return cmd;

    setpoint = cmd;
    input = currentRPM;

    // Calc time delta
    uint32_t now_cycles = TIMESTAMP_TIMER(R);
    double timeChange_s  = (double)(CLK_PERIOD_8MHZ * timerCycleDiff16(lastTime_cycles, now_cycles));

    // Compute vars
    double error = setpoint - input;
    errSum += (error * timeChange_s);
    double dErr = (error - lastErr) / timeChange_s;


    // Compute PID output
    rawOutput = (kp * error) + (ki * errSum) + (kd * dErr);

    if (rawOutput < MIN_PWM_OUT)
        output = MIN_PWM_OUT;
    else if (rawOutput > MAX_PWM_OUT)
        output = MAX_PWM_OUT;
    else
        output = rawOutput;

    // Store values for subsequent comparisons
    lastErr = error;
    lastTime_cycles = now_cycles;

    debugPrintF("%f,%f,%f,%f,%f,%f\r\n", timeChange_s, setpoint, input, error, errSum, output);
    return output;

}

// Timer4_A0 (CCR=0) TA Interrupt Handler
#define AVG_WINDOW_SIZE  3
uint32_t avg_window[AVG_WINDOW_SIZE] = { 0, 0, 0 };
uint8_t window_index = 0;

// TODO:  Figure out why this is happening so I can remove this hack!
uint8_t initialTAIFG = 1;

#pragma vector = TIMER4_A0_VECTOR
__interrupt void Timer4_A0_ISR(void)
{
    uint8_t i;
    uint32_t sumPeriods_cycles, avgPeriod_cycles;

    tempTime_cycles = (overflowCount << 16);
    currentPeriod_cycles = tempTime_cycles + TA4CCR0;

    // Manage circular averaging buffer
    avg_window[window_index] = currentPeriod_cycles;
    if (window_index >= AVG_WINDOW_SIZE-1)
        window_index = 0;
    else
        window_index++;

    // Build the sum, then use the average period to calculate RPM
    sumPeriods_cycles = 0;
    for (i=0; i<AVG_WINDOW_SIZE; i++)
        sumPeriods_cycles += avg_window[i];
    avgPeriod_cycles = sumPeriods_cycles/(float)AVG_WINDOW_SIZE;
    currentRPM = (CLK_RPM_PERIOD_CONVERSION_8MHZ)/avgPeriod_cycles;

    //currentRPM = (240000000.0)/currentPeriod;

    if (outputCounter++ > OUTPUT_INCREMENT)
    {
        //currentRPM = (240000000.0)/currentPeriod;
        outputCounter = 0;
        //debugPrintF("%lu, %f\r\n", currentPeriod, currentRPM);
        //debugPrintF("%f\r\n", currentRPM);
    }

    // Start time over, to simplify roll-over handling
    overflowCount = 0;
    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__SMCLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | FREQ_ROOT_TIMER(IE);

}

double getCurrentRPM()
{
    return currentRPM;
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




