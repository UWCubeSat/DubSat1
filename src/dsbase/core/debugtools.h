/*
 * debugtools.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef DEBUGTOOLS_H_
#define DEBUGTOOLS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils.h"
#include "../interfaces/systeminfo.h"
#include "../sensors/analogsensor.h"

#ifndef __DEBUG__
#warning  Header debugtools.h included, but __DEBUG__ flag not set.
#endif  /* __DEBUG__ */

#define MAX_TRACE_LEVEL  4
#if !defined(__INITIAL_TRACE_LEVEL__)
#define __INITIAL_TRACE_LEVEL__   0
#endif

#if !defined(__INITIAL_DEBUG_MODE__)
#define __INITIAL_DEBUG_MODE__  0
#endif

#if defined(__DEBUG_UART_SPEED_9600__)
#define DEBUG_UART_SPEED Speed_9600
#elif defined(__DEBUG_UART_SPEED_38400__)
#define DEBUG_UART_SPEED Speed_38400
#else
#define DEBUG_UART_SPEED Speed_115200
#endif


typedef enum _debugmode {
    Mode_ASCIIInteractive,
    Mode_ASCIIHeadless,
    Mode_BinaryStreaming,
} DebugMode;

typedef struct _svc_status_debug {
    uint8_t initialized;
    uint8_t registration_errors;
    uint8_t unknown_cmd_ids;
    uint8_t unknown_entity_ids;
    uint8_t trace_level;
    DebugMode debug_mode;

} svc_status_debug;

typedef enum {
    CommMatch_Unknown,
    CommMatch_Matched,
    CommMatch_Violation,
} comm_match_state;

void debugInit();

void debugPrint(uint8_t * buff, uint8_t szBuff);
void debugPrintF(const char *_format, ...);
void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff);
void debugTraceF(uint8_t level, const char *_format, ...);

void debugReadCallback(uint8_t rcvdbyte);
void processCommand(uint8_t * cmdbuff, uint8_t cmdlength);
uint8_t normalizeTraceLevel(uint8_t lvl);
void displayPrompt();
void debugSetMode(DebugMode newmode);
DebugMode debugGetMode();
comm_match_state debugGetCommMatchState();

// KEEP THESE ENUMS IN SYNC WITH SRC FILE
typedef enum _entityID {
    Entity_NONE,
    Entity_Test,
    Entity_DebugService,
    Entity_I2CBus,
    Entity_SPIBus,
    Entity_CANBus,
    Entity_BSP,
    Entity_UART,
    Entity_RWS,
    Entity_AnalogSensors,
    Entity_SUBSYSTEM,
} entityID;


typedef enum _simple_handler_type {
    Handler_Info,
    Handler_Status,
} simple_handler_type;

// For dynamically allowing entities to register debug console functionality
typedef uint8_t (*simple_debug_handler)(DebugMode mode);
typedef uint8_t (*param_debug_handler)(DebugMode mode, uint8_t * cmdstr);
typedef struct _debug_context {
    entityID id;
    uint8_t pathchar;
    simple_debug_handler info_handler;
    simple_debug_handler status_handler;
    param_debug_handler action_handler;
} debug_context;


void debugRegisterEntity(entityID id,
                         simple_debug_handler infohandler, simple_debug_handler statushandler,
                         param_debug_handler actionhandler);

void debugInvokeInfoHandler(entityID id);
void debugInvokeStatusHandler(entityID id);
void debugInvokeActionHandler(entityID id, uint8_t * params);

void debugInvokeInfoHandlers();
void debugInvokeStatusHandlers();
void debugInvokeActionHandlers(uint8_t * cmdstr);

// COSMOS-specific backchannel binary telemetery/command stuff

#define BCBIN_SYNCPATTERN           0xFC

typedef enum _bccmd_state {
    STATE_START,
    STATE_LEN_WAIT,
    STATE_ENTID_WAIT,
    STATE_OPCODE_WAIT,
    STATE_GATHERING_PARAMS,

} BcCmdState;

typedef uint8_t oms_status;

#define OMS_Nominal        0x00
#define OMS_MinorFaults    0x01
#define OMS_MajorFaults    0x02
#define OMS_Failures       0x03
#define OMS_FatalErrors    0x04
#define OMS_Unknown        0x05

// Standard packet definitions
#define TLM_ID_SHARED_META       0x00
#define TLM_ID_SHARED_HEALTH     0x01   // All MSP's will implement - fixed structure defined here

// Standard bus identifiers (centralized so we don't collide) - start at decimal 128/hex 0x80
#define TLM_ID_SHARED_BUS_UART   0x80

#define MAX_META_FIELD_LEN   16
TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    uint8_t module;

    uint64_t chipid;
    uint8_t hwswmatchstate;
    uint8_t commmatchstate;

    uint16_t compver;
    uint16_t stdcver;

    uint8_t compdate[MAX_META_FIELD_LEN];
    uint8_t comptime[MAX_META_FIELD_LEN];

} meta_segment;

TLM_SEGMENT {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    oms_status oms;  // Overall health

    float inttemp;   // Internal MSP43x temperature, from built-in sensor

    uint32_t reset_count;

} health_segment;

typedef enum {
    Clear_All,
    Clear_OnlyErrorFlags,
    Clear_OnlyErrorCounts,
    Clear_OnlyCounts,
} clear_state_commands;

#define OPCODE_COMMONCMD  0x00
CMD_SEGMENT {
    uint8_t expectedmodule;
    // TODO:  Add other common commands for all subsystems (like restart and reset/set counters and timers)
} commoncmd_segment;

#define TLM_ID_SHARED_SSGENERAL  0x02   // Most MSP's will implement - SS-specific stuff, so defined with SS module

void bcbinPopulateHeader( BcTlmHeader *header, uint8_t opcode, uint8_t fulllen);
void bcbinSendPacket(uint8_t * buff, uint8_t szBuff);
void bcbinPopulateMeta(meta_segment *mpkt, size_t sz);


#endif /* DEBUGTOOLS_H_ */
