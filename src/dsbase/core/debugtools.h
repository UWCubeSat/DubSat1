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

// Backchannel binary telemetery/command stuff

#define BCBIN_SYNCPATTERN           0xFC

// THIS HEADER MUST BE THE FIRST FIELD IN OUTGOING BINARY TELEMETRY STRUCTS
typedef struct PACKED_STRUCT _bctlm_header {
    uint8_t syncpattern;
    uint8_t length;
    uint8_t id;
} BcTlmHeader;

typedef struct PACKED_STRUCT _bccmd_header {
    uint8_t syncpattern;
    uint8_t length;
    uint8_t entityid;
    uint8_t opcode;
} BcCmdHeader;

void bcbinPopulateHeader( BcTlmHeader *header, uint8_t opcode, uint8_t fulllen);
void bcbinSendPacket(uint8_t * buff, uint8_t szBuff);

#define BINTLM_OPCODE_RWS_PIDMOT     0x07

typedef enum _bccmd_state {
    STATE_START,
    STATE_LEN_WAIT,
    STATE_ENTID_WAIT,
    STATE_OPCODE_WAIT,
    STATE_GATHERING_PARAMS,

} BcCmdState;

#endif /* DEBUGTOOLS_H_ */
