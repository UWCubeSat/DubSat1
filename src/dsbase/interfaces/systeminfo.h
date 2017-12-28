/*
 * systeminfo.h
 *
 *  Created on: Jun 29, 2017
 *      Author: jeffc
 */
#ifndef INTERFACES_SYSTEMINFO_H_
#define INTERFACES_SYSTEMINFO_H_

#include "../core/timers.h"

typedef volatile uint8_t flag_t;

// NOTE:  KEEP ENUM IN SYNC WITH PATH STRINGS (in systeminfo.c)
typedef enum _subsystemModule {
    Module_Test = 0,
    Module_ADCS_BDot = 1,
    Module_ADCS_RWX = 2,
    Module_ADCS_RWY = 3,
    Module_ADCS_RWZ = 4,
    Module_PPT = 5,
    Module_EPS_Dist = 6,
} SubsystemModule;

// REMOTE INFO STRUCTURES
// The following data structure are for storing a subsystem module's "knowledge" of the state of other
// relevant systems on the spacecraft.

// PPT remote info:  for the PPT, the correct language is:
//     period (or firing period):  the ~1-2 min span in which the PPT will either operate
//                                 throughout (at ~2 sec intervals) or not operate at all
//     cycle (or firing cycle):    the ~2 sec span in which the PPT charges and then fires once
//     phase (or firing phase):    the specific portion of the PPT charge/fire cycle
typedef enum _ppt_phase_info {
    PPT_Suspended,
    PPT_Standby,
    PPT_MainCharging,
    PPT_IgniterCharging,
    PPT_FiringPulse,
} PPTPhaseInfo;

typedef struct _ppt_firing_info {
    met_time_ms periodstart;
    met_time_ms lastlevelchange;
    PPTPhaseInfo currentphase;
} PPTFiringInfo;

typedef enum _startup_type {
    Startup_Unknown,   // Used while SS determining restart reason, or can't decide
    Startup_FirstBoot,
    Startup_RequestedReboot,
    Startup_ForcedReboot,
} StartupType;

// SpacecraftMode reflects the overall mode of the spacecraft, rather than individual
// subsystems (though the PPT's "subsystem" mode dictates many things about all other
// subsystems, hence it's inclusion here.
// TODO:  Might break out statuses into different fields/enums (one for PPT, one for ADCS, one general etc.)
typedef enum _spacecraft_mode {
    SCMode_Unknown,   // Used before SS has heard over CAN what the overall mode currently is
    SCMode_Comissioning,
    SCMode_InitialDetumbling,
    SCMode_ThresholdDetumbling,
    SCMode_PPTOperationsActive,
    SCMode_PPTOperationsSuspended,
} SpacecraftMode;

typedef struct _spacecraft_info {
    SpacecraftMode scmode;
    PPTFiringInfo pptactionstatus;
} SpacecraftInfo;

typedef void (*sync_pulse_handler)();

typedef enum _activity_type {
    Activity_I2C,
    Activity_SPI,
    Activity_UART,
    Activity_CAN,
    Activity_Magtom,
    Activity_IMU,
} ActivityType;

uint8_t *getSubsystemModulePath();
StartupType coreStartup(sync_pulse_handler sync1, sync_pulse_handler sync2);
uint8_t coreGetPrecautionLevel();

#endif /* INTERFACES_SYSTEMINFO_H_ */
