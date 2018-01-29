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
#include "sensors/gps/gps.h"

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

// photodiode addresses
//both addr lines wired to vcc
const uint8_t PD_ADDR_LH = 0x14;
const uint8_t PD_ADDR_LF = 0x15;
const uint8_t PD_ADDR_FH = 0x17;
const uint8_t PD_ADDR_FF = 0x24;
const uint8_t PD_ADDR_HH = 0x26;
const uint8_t PD_ADDR_HF = 0x27;

// COSMOS telemetry IDs
#define TLM_ID_SUNSENSOR  121
#define TLM_ID_PHOTODIODE 118
#define TLM_ID_GPSHEALTH  120
#define TLM_ID_GPSPOWER   122
#define TLM_ID_RXSTATUS   123
#define TLM_ID_BESTXYZ    124
#define TLM_ID_TIME       125
#define TLM_ID_HWMONITOR  126
#define TLM_ID_SATVIS2    127
#define TLM_ID_RANGE      119

// --- COSMOS telem and cmd packets ---

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float alpha;
    float beta;
    uint8_t error;
} sunsensor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t unknownMsg;
    uint8_t unknownEvt;
    gps_health health;
} gpshealth_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint8_t gpsEnabled;
    uint8_t buckEnabled;
    uint8_t state;
} gpspower_segment;

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
    float antCurrent;
    float supVolt;
    float antVolt;
    float digCoreVolt;
    float secTemp;
} hwmonitor_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t numGPS;
    uint32_t numGLONASS;
    uint32_t numSBAS;
} satvis2_segment;

typedef struct PACKED_STRUCT
{
    uint16_t prn;
    float cToNo;
    float locktime;
} range_segMember;

#define NUM_RANGE_SEGMENT_MEMBERS 6

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    range_segMember obs[NUM_RANGE_SEGMENT_MEMBERS];
} range_segment;

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t center;
    uint32_t right;
    uint32_t left;
} photodiode_segment;

/*
 * Union of telemetry segments that may share memory
 */
typedef union {
    bestxyz_segment bestxyz;
    hwmonitor_segment hwmonitor;
    satvis2_segment satvis2;
    range_segment range;
} shared_segment;

CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

#define OPCODE_SENDASCII 0x01
#define OPCODE_ENABLE 0x02

// Most subsystem modules should be implemented at least in part
// as a state machine (specifically, a FSM).  Here the available states are
// defined.
typedef enum _subsystem_state {
    State_GPSOff = 0,
    State_BuckWaitOn = 1,
    State_GPSWait = 2,
    State_GPSOn = 3,
    State_BuckWaitOff = 4,
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

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

// general telemetry functions
void sendHealthSegment();
void sendMetaSegment();

// gps functions
void gpsConfigure();
void sendGPSPowerStatus();
void sendGPSStatus();
bool handleGPSPackage(GPSPackage *p);

// gps message handlers
void handleRxStatus(const GPSPackage *package);
void handleTime(const GPSPackage *package);
void handleBestXYZ(const GPSPackage *package);
void handleHwMonitor(const GPSPackage *package);
void handleSatvis2(const GPSPackage *package);
void handleRange(const GPSPackage *package);
void handleRxStatusEvent(const GPSPackage *package);

// gps status event handlers
void handlePositionStatusEvent(const GPSRXStatusEvent *e);
void handleClockStatusEvent(const GPSRXStatusEvent *e);

// gps util functions
void toUtc(uint16_t *week, gps_ec *ms, double offset);

// sun sensor functions
void sunSensorUpdate();
void sendSunSensorData();

// photodiode functions
void photodiodeInitAll();
void photodiodeUpdate();
void sendPhotodiodeData();

#endif /* ADCS_SENSORPROC_H_ */
