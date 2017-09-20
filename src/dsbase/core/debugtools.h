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

#ifndef __DEBUG__
#warning  Header debugtools.h included, but __DEBUG__ flag not set.
#endif  /* __DEBUG__ */

#define MAX_TRACE_LEVEL  4
#if !defined(__INITIAL_TRACE_LEVEL__)
#define __INITIAL_TRACE_LEVEL__   0
#endif

void debugInit();

void debugPrint(uint8_t * buff, uint8_t szBuff);
void debugPrintF(const char *_format, ...);
void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff);
void debugTraceF(uint8_t level, const char *_format, ...);

void debugReadCallback(uint8_t rcvdbyte);
void processCommand(uint8_t * cmdbuff, uint8_t cmdlength);
uint8_t normalizeTraceLevel(uint8_t lvl);
void displayPrompt();

typedef enum _debugmode {
    InteractiveMode,
    HeadlessMode,
} DebugMode;

typedef struct _svc_status_debug {
    uint8_t initialized;
    uint8_t registration_errors;
    uint8_t trace_level;
    DebugMode debug_mode;

} svc_status_debug;

// KEEP THESE ENUMS IN SYNC WITH SRC FILE
typedef enum _entityID {
    Entity_Test,
    Entity_DebugService,
    Entity_I2CBus,
    Entity_SPIBus,
    Entity_CANBus,
    Entity_BSP,
    Entity_UART,
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

void debugRegisterEntity(entityID id, uint8_t pathchar,
                         simple_debug_handler infohandler, simple_debug_handler statushandler,
                         param_debug_handler actionhandler);

#endif /* DEBUGTOOLS_H_ */
