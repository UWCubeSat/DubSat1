/*
 * eps_dist.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef EPS_DIST_H_
#define EPS_DIST_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "sensors/pcvsensor.h"
#include "bsp/bsp.h"
#include "core/debugtools.h"

// Configure power domains
#define NUM_POWER_DOMAINS  8

#define SHUNT_HIGH_DRAW_DEVICE  0.01f
#define SHUNT_LOW_DRAW_DEVICE   0.1f

#define OCP_THRESH_LOW_DRAW_DEVICE  0.300f
#define OCP_THRESH_MED_DRAW_DEVICE  0.500f
#define OCP_THRESH_HIGH_DRAW_DEVICE 0.700f

// Configure power domain control pins
#define DOMAIN_ENABLE_COM2_DIR  P7DIR
#define DOMAIN_ENABLE_COM2_OUT  P7OUT
#define DOMAIN_ENABLE_COM2_BIT  BIT4

#define DOMAIN_ENABLE_PPT_DIR  P2DIR
#define DOMAIN_ENABLE_PPT_OUT  P2OUT
#define DOMAIN_ENABLE_PPT_BIT  BIT5

#define DOMAIN_ENABLE_BDOT_DIR  P4DIR
#define DOMAIN_ENABLE_BDOT_OUT  P4OUT
#define DOMAIN_ENABLE_BDOT_BIT  BIT1

#define DOMAIN_ENABLE_COM1_DIR  P7DIR
#define DOMAIN_ENABLE_COM1_OUT  P7OUT
#define DOMAIN_ENABLE_COM1_BIT  BIT3

#define DOMAIN_ENABLE_RAHS_DIR  P4DIR
#define DOMAIN_ENABLE_RAHS_OUT  P4OUT
#define DOMAIN_ENABLE_RAHS_BIT  BIT0

#define DOMAIN_ENABLE_ESTIM_DIR  P4DIR
#define DOMAIN_ENABLE_ESTIM_OUT  P4OUT
#define DOMAIN_ENABLE_ESTIM_BIT  BIT2

#define DOMAIN_ENABLE_EPS_DIR  P1DIR
#define DOMAIN_ENABLE_EPS_OUT  P1OUT
#define DOMAIN_ENABLE_EPS_BIT  BIT6

#define DOMAIN_ENABLE_WHEELS_DIR  P4DIR
#define DOMAIN_ENABLE_WHEELS_OUT  P4OUT
#define DOMAIN_ENABLE_WHEELS_BIT  BIT3


#define DOMAIN_ENABLE_CURRENT_SENSORS_DIR   P3DIR
#define DOMAIN_ENABLE_CURRENT_SENSORS_OUT   P3OUT
#define DOMAIN_ENABLE_CURRENT_SENSORS_BIT   BIT4

#define DEPLOY_ENABLE_DIR P1DIR
#define DEPLOY_ENABLE_OUT P1OUT
#define DEPLOY_ENABLE_BIT BIT7

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

// Capture power domain info, IDs, addresses, and enable pins
// DO NOT REORDER
typedef enum {
    PD_COM1,
    PD_COM2,
    PD_RAHS,
    PD_BDOT,
    PD_ESTIM,
    PD_WHEELS,
    PD_EPS,
    PD_PPT,
} PowerDomainID;

typedef enum {
    PD_CMD_NoChange,
    PD_CMD_Enable,
    PD_CMD_Disable,
    PD_CMD_Toggle,
    PD_CMD_OCLatch,
    PD_CMD_BattVLow,
    PD_CMD_AutoStart,
} PowerDomainCmd;

typedef enum {
    Switch_Disabled,
    Switch_Enabled,
} PowerDomainSwitchState;

typedef enum {
    UV_InRange,
    UV_PartialShutdown,
    UV_FullShutdown,
} UndervoltageHandlingMode;


typedef struct _power_domain_info {
    PowerDomainID id;
    uint8_t i2caddr;
    hDev hpcvsensor;

    // eventually add other stuff, like handle to the averaging queues, etc.
} PowerDomainInfo;

// COSMOS telem and cmd packets
TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float battV;
    uint8_t uvmode;   // Battery under voltage mode
    float undervoltagethreshold;

    uint8_t powerdomainlastcmds[NUM_POWER_DOMAINS];
    uint8_t powerdomainswitchstate[NUM_POWER_DOMAINS];

    uint8_t powerdomaincurrentlimitedcount[NUM_POWER_DOMAINS];
    float powerdomainocpthreshold[NUM_POWER_DOMAINS];
} general_segment;

// SensorDat packet:  high-frequency sends that capture state of sensors (including overcurrent flags)
TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float currents[NUM_POWER_DOMAINS];
    uint8_t powerdomaincurrentlimited[NUM_POWER_DOMAINS];
    float busV[NUM_POWER_DOMAINS];
} sensordat_segment;

CMD_SEGMENT {
    uint8_t pd_cmds[NUM_POWER_DOMAINS];
} domaincmd_segment;

#define BATTV_CONV_FACTOR    2.8867925f

//#define BATT_DEFAULT_PARTIAL_THRESH   (5.8f)
#define BATT_DEFAULT_FULL_THRESH      (5.2f)
#define BATT_HYSTER   (0.25f)


CMD_SEGMENT {
    float newCurrentThreshold[NUM_POWER_DOMAINS];
    float newBattVThreshold;
} ocpthresh_segment;

#define DEPLOYMENT_SYSTEM_KEY  126
CMD_SEGMENT {
    uint8_t key;
} firedeploy_segment;

#define OPCODE_DOMAINSWITCH        0x64  // Dec '100', ASCII 'd'
#define OPCODE_OCPTHRESH           0x74  // Dec '116', ASCII 't'
#define OPCODE_FIREDEPLOY          0x66  // Dec '102', ASCII 'f'

#define TLM_ID_EPS_DIST_GENERAL    TLM_ID_SHARED_SSGENERAL  // == 0x02  <--- standard message ID
#define TLM_ID_EPS_DIST_SENSORDAT  0x03

// General enums/structs
// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    State_Normal,
    State_UnderVoltage,
    State_FirstState,
    State_SecondState,
    State_ThirdState,
} SubsystemState;

// Additional, it can be helpful if states are grouped into higher level
// "modes" in a hierarchical way for certain kinds of decision making and
// reporting.  These are not mandatory, however.  State transitions will need
// to explicitly transition the mode as well
typedef enum _subsystem_mode {
    Mode_FirstMode,
    Mode_SecondMode,
    Mode_ThirdMode,
} SubsystemMode;

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;

    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

void distInitializeOCPThresholds();

#endif /* EPS_DIST_H_ */
