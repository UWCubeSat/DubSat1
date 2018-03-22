/*
 * rw.c
 *
 *  Created on: Sep 18, 2017
 *      Author: jeffc
 *      DRV HI
 *      COM LO
 *      BRK LO
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "actuators/rw.h"
#include "core/utils.h"
#include "core/debugtools.h"
#include "core/timers.h"

FILE_STATIC unsigned long lastTime_cycles;
FILE_STATIC double rawOutput, lastInput;
FILE_STATIC double lastErr;
FILE_STATIC double kp, ki, kd;
FILE_STATIC uint16_t outputCounter = 0;
FILE_STATIC float currentRPM = 0.0;
FILE_STATIC uint32_t tempTime_cycles = 0;
FILE_STATIC uint32_t currentPeriod_cycles = 0;
FILE_STATIC uint32_t overflowCount = 0;
FILE_STATIC uint8_t rpmUpdated = 0;

FILE_STATIC uint8_t active = 0;
FILE_STATIC uint8_t setpoint_override = 0;
FILE_STATIC double overridden_setpoint;
FILE_STATIC uint8_t direction =0;

FILE_STATIC PidStepInfo pid;
int rwsRPMUpdated()
{
    return rpmUpdated;
}
void rwsSetRPMUpdated(uint8_t f)
{
    rpmUpdated = f;
}

void rwsShowUsage()
{
    debugPrintF("Usage:\r\n");
    debugPrintF("\t!rs<rpm>  --  where <rpm> = desired setpoint\r\n");
    debugPrintF("\t!rsd  --  switch direction of motor\r\n");
}

void rwSendTlm()
{
    bcbinSendPacket((uint8_t *) &pid, sizeof(pid));
}
void rwsSwitchDirection(){
    direction = (direction + 1)%2;
    RW_MOTORDIR_OUT ^= RW_MOTORDIR_PIN;
}

uint8_t rwsStatusCallback(DebugMode mode)
{
    if (mode == Mode_BinaryStreaming)
        rwSendTlm();
    return 1;
}

int speedPercentage = 150;
uint8_t rwsActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    uint8_t len = strlen((const char *)cmdstr);
    uint16_t inputnum = 0;

    CmdPidCtrl *cmdpidctrl;

    if (mode == Mode_ASCIIInteractive)
    {
        if (len == 0)
        {
            rwsShowUsage();
            return 0;
        }
        else
        {
            if (cmdstr[0] == 's')
            {
                // Just '!rs' reverts back to "programmed" speed
                if (len == 1)
                {
                    debugPrintF("Reverting back to programmed control.\r\n");
                    setpoint_override = 0;
                }
                else
                {
                    inputnum = atoi((const char*)&cmdstr[1]);
                    debugPrintF("Overriding PID setpoint to %d rpm.", inputnum);
                    setpoint_override = 1;
                    overridden_setpoint = inputnum;
                    speedPercentage = inputnum;
                }
                return 1;
            }
            else if (cmdstr[0] == 'd')
            {
                debugPrintF("Switching motor direction.\r\n");
                RW_MOTORDIR_OUT ^= RW_MOTORDIR_PIN;
            }
            else
            {
                rwsShowUsage();
                return 0;
            }
        }
    }
    else
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_DIRCHANGE:
                rwsSwitchDirection();
                pid.lastcmd.newsetpoint = 666;
                pid.lastcmd.resetwindup = 0;
                break;
//            case OPCODE_MANUAL:
//                rwsSetMotorSpeed(60000);
//                break;
            case OPCODE_SETPOINTCHANGE:
                cmdpidctrl = (CmdPidCtrl *) &cmdstr[1];
                if (cmdpidctrl->resetwindup == TRUE)
                {
                    pid.resetwindupcnt++;
                    pid.errSum = 0;
                    pid.lastcmd.resetwindup = TRUE;
                }
                else
                    pid.lastcmd.resetwindup = FALSE;

                // TODO:  Remove this janky override stuff and just have one commanding mode
                if (cmdpidctrl->newsetpoint == 0)
                {
                    setpoint_override = 0;
                    pid.lastcmd.newsetpoint = 0;
                }
                else
                {
                    kp = (((double)(cmdpidctrl->p))/2500.0);
                    ki = (((double)(cmdpidctrl->i))/2500.0);
                    kd = (((double)(cmdpidctrl->d))/2500.0);
                    setpoint_override = 1;
                    overridden_setpoint = cmdpidctrl->newsetpoint;

                    pid.lastcmd.newsetpoint = overridden_setpoint;
                    pid.setpoint = overridden_setpoint;
                    if(kp == 2 && ki == 2 && kd == 2){
                        rwsSetMotorSpeed((pid.setpoint/10000.0)*600);
                        rwsRunManual();
                    }
                    else{
                        rwsRunAuto();
                    }
                }
                break;
        }
        return 1;
    }
    return 1;
}

void rwsInit()
{
    //  FR (direction):  P7.3 to control direction, P7.4 to read FG pin ('1 0 0')
    RW_MOTORDIR_DIR |= RW_MOTORDIR_PIN;
    RW_MOTORDIR_OUT &= ~RW_MOTORDIR_PIN;
    RW_MOTORDIR_SEL1 &= ~RW_MOTORDIR_PIN;
    RW_MOTORDIR_SEL0 &= ~RW_MOTORDIR_PIN;
//
//    // Configure free-running timer to generate timestamps
//    // TODO:  eventually move this to a clock library function
    TIMESTAMP_TIMER(CTL) = TIMESTAMP_ROOT_TIMER(SSEL__SMCLK) | TIMESTAMP_ROOT_TIMER(CLR);
//
//    // All pins configured to work with RW/CANMSP block board circa September 2017
//    // FG:  Pin P7.4 for input, to monitor frequency (and thus RPM) of motor
//    // This is using TA4.CCI0B - set to '0 1 0'
    RW_FREQ_DIR &= ~RW_FREQ_PIN;
    RW_FREQ_SEL1 |= RW_FREQ_PIN;
    RW_FREQ_SEL0 &= ~RW_FREQ_PIN;

    // Timer1_A4 setup for capturing FG pin frequency:
    // Rising edge, CCIA input, sync mode, capture mode and interrupts enabled
    // SMCLK as clk source, continuous mode
//    FREQ_TIMER(CCTL0) = CM__RISING | CCIS__CCIB | SCS | CAP | CCIE;
//    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__ACLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | TAIE;
//    TA4CCTL1 = CCIE;                   // CCR0 interrupt enabled
    // PWM output:  pin P1.7 ('1 0 1') - as TB0.4
    RW_PWM_DIR |= RW_PWM_PIN;
    RW_PWM_SEL1 &= ~RW_PWM_PIN;
    RW_PWM_SEL0 |= RW_PWM_PIN;

//    TA4CCR1 = 3;
    TA0CTL = TASSEL__ACLK + MC_1  + ID_1 + TACLR;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 65535;
    P1DIR |= BIT0;
    NVIC_EnableIRQ(TA0_0_IRQn);
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
    __enable_interrupt();
//    TB0CCR0 = 60000;
//    TB0CCR4 = 50;
//    TB0CCTL4 = OUTMOD_7 ;
//    TB0CTL = TASSEL_2 + MC_1;

//    // Setup binary telemetry header
    bcbinPopulateHeader(&(pid.header), TLM_ID_RWS_PIDMOT, sizeof(pid));
    debugRegisterEntity(Entity_RWS, NULL, rwsStatusCallback, rwsActionCallback);
    rwSendTlm();
    rwsSetMinMaxOutput(1,600);
}



//takes a percentage between 0 and 100 to run motor speed
void rwsSetMotorSpeed(double percentage) {
    if(percentage < 1){
        percentage = 1;
    }
    else if(percentage > 600) {
        percentage = 600;
    }
//    TB0CCR4 = (uint16_t)(percentage*100);
}

//takes a percentage between 0 and 100 to run motor speed
void rwsSetTargetRPM(double rpm) {
    if(rpm < 0){
        rpm=0;
    }
    if(rpm > 10000){
        rpm=10000;
    }
    pid.setpoint = rpm;
}

void rwsRunAuto()
{
    active = 1;
}

void rwsRunManual()
{
    active = 0;
}
int rwsMode()
{
    return active;
}

void rwsSetMinMaxOutput(int min, int max)
{
    pid.minOutput = min;
    pid.maxOutput = max;
}

void rwsSetTuningParams(double Kp, double Ki, double Kd)
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

double rwsPIDStep(double cmd)
{
    if (active != 1){
        pid.input = currentRPM;
        rwSendTlm();
        return cmd;
    }

//    if (setpoint_override == 1)
//        pid.setpoint = overridden_setpoint;
//    else
//        pid.setpoint = cmd;

    pid.input = currentRPM;

    // Calc time delta
    uint32_t now_cycles = TIMESTAMP_TIMER(R);
    pid.timeChange_s  = (double)(CLK_PERIOD_32KHZ * timerCycleDiff16(lastTime_cycles, now_cycles));

    // Compute vars
    pid.error = pid.setpoint - pid.input;
    // Multiplying by Ki here to avoid change of curve when integral factor changed
    pid.errSum += (ki * pid.error * pid.timeChange_s);

    if (pid.errSum > pid.maxOutput )
        pid.errSum = pid.maxOutput;
    else if (pid.errSum < pid.minOutput )
        pid.errSum = pid.minOutput;

    pid.dErr = (pid.error - lastErr) / pid.timeChange_s;


    // Compute PID output
    //derivative on measurement in order to remove derivative kick
    // NOT MULTIPLYING BY Ki not a mistake. done up above
    rawOutput = (kp * pid.error) + (pid.errSum) - (kd * pid.dErr);

    if (rawOutput < pid.minOutput)
        pid.output = pid.minOutput;
    else if (rawOutput > pid.maxOutput)
        pid.output = pid.maxOutput;
    else
        pid.output = rawOutput;
    if(direction){
        pid.input = pid.input * -1;
    }
    // Store values for subsequent comparisons
    lastErr = pid.error;
    lastTime_cycles = now_cycles;
    if (debugGetMode() == Mode_ASCIIInteractive)
    {
        debugTraceF(2,"%f,%f,%f,%f,%f,%f\r\n", pid.timeChange_s, pid.setpoint, pid.input, pid.error, pid.errSum, pid.output);
//        debugTraceF(1,"%f,%f,%f\r\n", kp, ki, kd);
    }
    else if (debugGetMode() == Mode_BinaryStreaming)
    {
        rwSendTlm();
    }


    return pid.output;

}

// Timer4_A0 (CCR=0) TA Interrupt Handler
#define AVG_WINDOW_SIZE  3
uint32_t avg_window[AVG_WINDOW_SIZE] = { 0, 0, 0 };
uint8_t window_index = 0;
int ignore = 0;

// TODO:  Figure out why this is happening so I can remove this hack!
uint8_t initialTAIFG = 1;
//This interrupt reads the hall effect sensor of the motor and calculates rpm
//averages over 3 readings
//#pragma vector = TIMER4_A0_VECTOR
__interrupt void Timer4_A0_ISR(void)
{
    ignore=(ignore+1)%2;
    if(ignore)
        return;
    uint8_t i;
    uint32_t sumPeriods_cycles, avgPeriod;

    tempTime_cycles = (overflowCount << 16);
//    currentPeriod_cycles = tempTime_cycles + TA4CCR0;
//    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__ACLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | FREQ_ROOT_TIMER(IE);
    if (outputCounter++ > OUTPUT_INCREMENT)
    {
        //currentRPM = (240000000.0)/currentPeriod;
        outputCounter = 0;
        //debugPrintF("%lu, %f\r\n", currentPeriod, currentRPM);
        //debugPrintF("%f\r\n", currentRPM);
    }

    // Start time over, to simplify roll-over handling
    overflowCount = 0;


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
    avgPeriod= sumPeriods_cycles/(float)AVG_WINDOW_SIZE;
    currentRPM = (CLK_RPM_PERIOD_CONVERSION_32KHZ)/avgPeriod;
//    rwsSetRPMUpdated(1);
    //currentRPM = (240000000.0)/currentPeriod;
}

double getCurrentRPM()
{
    return currentRPM;
}


// Timer4_A1 (CCR=1..n) TA Interrupt Handler
int booped = 0;
void TA0_0_IRQHandler (void)
{
    TA0CCTL0 &= ~CCIFG;
    debugTraceF(0,"hey dawg hey whats up");
//    bcbinSendPacket((uint8_t *) &pid, sizeof(pid));
    P1OUT ^= BIT0;
//    booped=booped%2;
//    switch (__even_in_range(TA4IV, TAIV__TAIFG))
//    {
//        case TAIV__TACCR1:
//            rwsSetRPMUpdated(1);
//            break;           // not used
//        case TAIV__TACCR2: break;           // not used
//        case TAIV__TACCR3: break;           // not used
//        case TAIV__TACCR4: break;           // not used
//        case TAIV__TACCR5: break;           // not used
//        case TAIV__TACCR6: break;           // not used
//        case TAIV__TAIFG:                   // TA4.0 overflow
            // Deal with bogus initial overflow flag that pops on startup
//            if (initialTAIFG == 1)
//                initialTAIFG = 0;
//            else
//                overflowCount++;
//            break;
//        default:
//            break;
//    }
}
