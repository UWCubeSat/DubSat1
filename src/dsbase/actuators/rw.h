/*
 * rw.h
 *
 *  Created on: Sep 18, 2017
 *      Author: jeffc
 */

#ifndef DSBASE_ACTUATORS_RW_H_
#define DSBASE_ACTUATORS_RW_H_

#include <msp430.h>
#include <stdint.h>

#include "core/utils.h"
#include "core/debugtools.h"

#define OUTPUT_INCREMENT  5
#define CLK_RPM_PERIOD_CONVERSION_8MHZ  240000000.0
#define CLK_PERIOD_8MHZ  0.000000125

// Configuration for PID
#define MAX_PWM_OUT     999    // Assume 1000 CCR counter on PWM out
#define MIN_PWM_OUT     0
// TODO:  Add support for negative speeds (with special handling to flip the direction bit)

// Pin definitions
#define RW_FREQ_DIR     P7DIR
#define RW_FREQ_SEL1    P7SEL1
#define RW_FREQ_SEL0    P7SEL0
#define RW_FREQ_PIN     BIT4

#define RW_MOTORDIR_OUT     P7OUT
#define RW_MOTORDIR_DIR     P7DIR
#define RW_MOTORDIR_SEL1    P7SEL1
#define RW_MOTORDIR_SEL0    P7SEL0
#define RW_MOTORDIR_PIN     BIT3

#define RW_PWM_DIR      P1DIR
#define RW_PWM_SEL1     P1SEL1
#define RW_PWM_SEL0     P1SEL0
#define RW_PWM_PIN      BIT7

// Frequency capture timer
#define FREQ_TIMER(reg)         TA4##reg
#define FREQ_ROOT_TIMER(bits)   TA##bits

// Timestamp timer
#define TIMESTAMP_TIMER(reg)    TA2##reg
#define TIMESTAMP_ROOT_TIMER(bits)  TA##bits

void rwsInit();
void rwsSetTuningParams(double Kp, double Ki, double Kd);
double rwsPIDStep(double setpoint);
void rwsRunAuto();
void rwsRunManual();

// COSMOS Stuff

#define OPCODE_DIRCHANGE            0x64
#define OPCODE_SETPOINTCHANGE       0x73
typedef struct PACKED_STRUCT _cmd_pidctrl {
    uint16_t newsetpoint;
    BOOL resetwindup;
} CmdPidCtrl;

typedef struct PACKED_STRUCT _pid_step_info {
    BcTlmHeader header;
    double timeChange_s;
    double setpoint;
    double input;
    double error;
    double errSum;
    double dErr;
    double output;

    uint8_t resetwindupcnt;
    CmdPidCtrl lastcmd;

} PidStepInfo;


#define TLM_ID_RWS_PIDMOT     0x07

#endif /* DSBASE_ACTUATORS_RW_H_ */
