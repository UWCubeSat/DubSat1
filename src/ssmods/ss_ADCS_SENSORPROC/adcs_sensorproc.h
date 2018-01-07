/*
 * adcs_sensorproc.h
 */

#ifndef ADCS_SENSORPROC_H_
#define ADCS_SENSORPROC_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

#include "sensors/gps/GPSPackage.h"

// COSMOS telem and cmd packets

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t error;
    uint32_t status;
    uint32_t aux1;
    uint32_t aux2;
    uint32_t aux3;
} rxstatus_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    gps_enum posStatus;
    GPSVectorD pos;
    GPSVectorF posStdDev;
    gps_enum velStatus;
    GPSVectorD vel;
    GPSVectorF velStdDev;
    uint16_t week;
    int32_t ms;
} bestxyz_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    double offset;
    int32_t ms;
    uint16_t week;
    gps_enum clockStatus;
    gps_enum utcStatus;
} time_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float temp;
    uint8_t tempStatus;
} hwmonitor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t numGPS;
    uint32_t numGLONASS;
    uint32_t numSBAS;
} satvis2_segment;

CMD_SEGMENT {

} sendascii_segment;

#define OPCODE_SENDASCII 1

// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    State_GPSOff,
    State_GPSPoweringOn,
    State_GPSOn,
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

void handleSyncPulse1();
void handleSyncPulse2();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* ADCS_SENSORPROC_H_ */
