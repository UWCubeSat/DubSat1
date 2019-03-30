//////////////////////////////////////////////////////////////////////
//DO NOT MODIFY THIS FILE DIRECTLY
//Instead, change DubSat1/teams/cdh/CAN/Experiments/CANModelTemplate.*
//////////////////////////////////////////////////////////////////////

/*
* CANModel
*
*  Created on: Mar 11, 2019
*      Author: Nathan Wacker
*/

#ifndef CAN_MODEL_H_
#define CAN_MODEL_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"

typedef enum {
	PD_DIST = 0,
    PD_COM2 = 1,
    PD_RAHS = 2,
    PD_BDOT = 3,
    PD_ESTIM = 4,
    PD_EPS = 5,
    PD_PPT = 6
};

#define LED_DIR PJDIR
#define LED_OUT PJOUT
#define LED0_BIT BIT0
#define LED1_BIT BIT1
#define LED2_BIT BIT2

#define PD_DIR_COM2 P3DIR
#define PD_IN_COM2 P3IN
#define PD_OUT_COM2 P3OUT
#define PD_BIT_COM2 BIT1

#define PD_DIR_RAHS P3DIR
#define PD_IN_RAHS P3IN
#define PD_OUT_RAHS P3OUT
#define PD_BIT_RAHS BIT2

#define PD_DIR_BDOT P3DIR
#define PD_IN_BDOT P3IN
#define PD_OUT_BDOT P3OUT
#define PD_BIT_BDOT BIT3

#define PD_DIR_ESTIM P1DIR
#define PD_IN_ESTIM P1IN
#define PD_OUT_ESTIM P1OUT
#define PD_BIT_ESTIM BIT3

#define PD_DIR_EPS P1DIR
#define PD_IN_EPS P1IN
#define PD_OUT_EPS P1OUT
#define PD_BIT_EPS BIT4

#define PD_DIR_PPT P1DIR
#define PD_IN_PPT P1IN
#define PD_OUT_PPT P1OUT
#define PD_BIT_PPT BIT5


#define TIMER_LENGTH uint16_t

#define LED_0_FREQ 100
TIMER_LENGTH lastLED0Time = 0;

TIMER_LENGTH LED_1_FREQ = 0x9f6;
TIMER_LENGTH lastLED1Time = 0;

TIMER_LENGTH last_rahs_camera_time = 0;
TIMER_LENGTH last_estim_sun_unit_z_time = 0;
TIMER_LENGTH last_estim_sun_unit_y_time = 0;
TIMER_LENGTH last_estim_sun_unit_x_time = 0;
TIMER_LENGTH last_estim_mag_unit_z_time = 0;
TIMER_LENGTH last_estim_mag_unit_y_time = 0;
TIMER_LENGTH last_estim_mag_unit_x_time = 0;
TIMER_LENGTH last_estim_state_time = 0;
TIMER_LENGTH last_com2_state_time = 0;
TIMER_LENGTH last_cmd_mtq_fsw_time = 0;
TIMER_LENGTH last_mtq_ack_time = 0;
TIMER_LENGTH last_cmd_mtq_bdot_time = 0;


#endif /* CAN_MODEL_H_ */