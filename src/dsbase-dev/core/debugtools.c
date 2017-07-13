/*
 * debugtools.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include "debugtools.h"
#include "uart.h"
#include "utils.h"
#include "config/config.h"
#include "bsp/bsp.h"

// NOTE:  Virtually everything in this file should be inside of the
// __DEBUG__ #ifdef, as the implementation of all these things
// should go away when compiled for release
#ifdef __DEBUG__

FILE_STATIC char debugWriteOutputBuff[CONFIGM_debug_outputbuffsize];
FILE_STATIC char debugConsoleInputBuff[CONFIGM_debug_consoleinputbuffsize];

FILE_STATIC uint8_t consoleBytesRead = 0;
FILE_STATIC uint8_t consoleBuildingCommand = 0;

FILE_STATIC debug_handler registeredInfoHandlers[CONFIGM_debug_maxinfohandlers];
FILE_STATIC debug_handler registeredStatusHandlers[CONFIGM_debug_maxinfohandlers];
FILE_STATIC debug_handler registeredActionHandlers[CONFIGM_debug_maxinfohandlers];

FILE_STATIC svc_status_debug debug_status;

uint8_t debugReportStatusCallback(DebugMode mode)
{
    if (mode == InteractiveMode)
        {
            debugPrintF("**Debug Service Status:\r\n");
            debugPrintF("Trace level: %d\r\n\r\n", debug_status.trace_level);
            debugPrintF("*Handlers:\r\nInfo: %d\r\nStatus: %d\r\nAction: %d\r\nRegistration errors: %d",
                        debug_status.num_info_handlers, debug_status.num_status_handlers, debug_status.num_action_handlers,
                        debug_status.registration_errors);
        }
        else
        {
            debugPrintF("Stuff without as many words (e.g. just CSV)");
        }
        return 1;
}

void debugInit()
{
    if (debug_status.initialized != 0)
        return;

    debug_status.initialized = 1;
    debug_status.trace_level = __INITIAL_TRACE_LEVEL__;
    debug_status.debug_mode = InteractiveMode;

    uartInit();
    uartRegisterRxCallback(debugReadCallback);

    debugRegisterStatusHandler(debugReportStatusCallback);
}

void debugPrint(uint8_t * buff, uint8_t szBuff)
{
    uartTransmit(buff, szBuff);
}

void debugPrintF(const char *_format, ...)
{
    int numBytes;

    va_list argptr;
    va_start(argptr, _format);
    numBytes = vsnprintf(debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
    uartTransmit((uint8_t *)debugWriteOutputBuff, numBytes + 1);
}

void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff)
{
    if (normalizeTraceLevel(level) <= debug_status.trace_level)
        debugPrint(buff, szBuff);
}

void debugTraceF(uint8_t level, const char *_format, ...)
{
    int numBytes;

    if (normalizeTraceLevel(level) <= debug_status.trace_level)
    {
        va_list argptr;
        va_start(argptr, _format);
        numBytes = vsnprintf(debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
        uartTransmit((uint8_t *)debugWriteOutputBuff, numBytes + 1);
    }
}

void debugReadCallback(uint8_t rcvdbyte)
{
    // New command starting
    if (consoleBuildingCommand == 0)
    {
        consoleBuildingCommand = 1;
        consoleBytesRead = 0;
    }

    // Keep building command string until enter is hit
    if (rcvdbyte == '\r')
    {
        consoleBytesRead++;
        consoleBuildingCommand = 0;
        processCommand((uint8_t *)debugConsoleInputBuff, consoleBytesRead);
    }
    else
    {
        debugConsoleInputBuff[consoleBytesRead] = rcvdbyte;
        consoleBytesRead++;
    }
}

void coreCallDisplayHandlers(debug_handler *handlers, uint8_t numhandlers)
{
    int i;
    for (i = 0; i < numhandlers; i++)
    {
        (*handlers[i])(debug_status.debug_mode);
        debugPrintF("\r\n");
    }
}

void cmdInfo()
{
    debugPrintF("Subsystem information (static):\r\n--------------------------------\r\n");
    coreCallDisplayHandlers(registeredInfoHandlers, debug_status.num_info_handlers);
    displayPrompt();
}

void cmdStatus()
{
    debugPrintF("Subsystem status (dynamic):\r\n----------------------------\r\n");
    coreCallDisplayHandlers(registeredStatusHandlers, debug_status.num_status_handlers);
    displayPrompt();
}

void processCommand(uint8_t * cmdbuff, uint8_t cmdlength)
{
    // Someone hits just enter - turn "off" anything noisy going on
    if (cmdlength == 1)
    {
        debug_status.trace_level = 0;
        displayPrompt();
        return;
    }
    switch(cmdbuff[0])
    {
        case 't':    // Change TRACE level
            // Poor-man's atoi for single digits
            debug_status.trace_level = normalizeTraceLevel(((uint8_t)cmdbuff[1]) - 48);
            break;
        case 'i':   // Call info functions on all entities
            debug_status.trace_level = 0;
            cmdInfo();
            break;
        case 's':   // Call status functions on all entities
            debug_status.trace_level = 0;
            cmdStatus();
            break;
    }

    displayPrompt();
}

uint8_t normalizeTraceLevel(uint8_t lvl)
{
    uint8_t vallvl = lvl;

    // Normalize to proper range (note:  it's unsigned, so don't need to check for < 0)
    if (vallvl > MAX_TRACE_LEVEL)
        vallvl = MAX_TRACE_LEVEL;
    return vallvl;
}

void displayPrompt()
{
    if (debug_status.debug_mode == InteractiveMode)
    {
        debugPrintF("\r\n[%s]>>", getSubsystemModulePath());
    }
    else
        debugPrintF(">");

}

uint8_t coreHandlerAdd(debug_handler *handlers, debug_handler handler, uint8_t currentcount, uint8_t maxcount )
{
    if (currentcount >= maxcount)
    {
        debug_status.registration_errors++;
        return 0;
    }
    handlers[currentcount] = handler;
    return 1;
}

void debugRegisterInfoHandler(debug_handler handler)
{
    if (coreHandlerAdd(registeredInfoHandlers, handler, debug_status.num_info_handlers, CONFIGM_debug_maxinfohandlers) == 1)
        debug_status.num_info_handlers++;
}

void debugRegisterStatusHandler(debug_handler handler)
{
    if (coreHandlerAdd(registeredStatusHandlers, handler, debug_status.num_status_handlers, CONFIGM_debug_maxstatushandlers) == 1)
            debug_status.num_status_handlers++;
}

void debugRegisterActionHandler(debug_handler handler)
{
    if (coreHandlerAdd(registeredActionHandlers, handler, debug_status.num_action_handlers, CONFIGM_debug_maxactionhandlers) == 1)
            debug_status.num_action_handlers++;
}




#else  /* __DEBUG__ not specified, therefore nop debug operations */

void debugInit() {}
void debugPrint(uint8_t * buff, uint8_t szBuff) {}
void debugPrintF(const char *_format, ...) {}
void debugReadCallback(uint8_t rcvdbyte) {}

void debugRegisterInfoHandler(debug_handler) {}
void debugRegisterStatusHandler(debug_handler) {}
void debugRegisterActionHandler(debug_handler) {}


#endif /* __DEBUG__ */
