/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef EPSDISTTEST_H_
#define EPSDISTTEST_H_

#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"
#include "core/utils.h"
#include "core/debugtools.h"

#define OPCODE_DOMAINSWITCH        0x64  // Dec '100', ASCII 'd'

#define TLM_ID_EPS_DIST_GENERAL    TLM_ID_SHARED_SSGENERAL  // == 0x01  <--- standard message ID
#define TLM_ID_EPS_DIST_SENSORDAT  0x02

typedef enum {
    PD_CMD_NoChange,
    PD_CMD_Enable,
    PD_CMD_Disable,
    PD_CMD_Toggle,
} PowerDomainCmd;

typedef enum {
    Switch_Disabled,
    Switch_Enabled,
} PowerDomainSwitchState;


#endif /* EPSDISTTEST_H_ */
