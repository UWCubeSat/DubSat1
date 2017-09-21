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
FILE_STATIC char debugConsoleCmdBuff[CONFIGM_debug_consoleinputbuffsize];

FILE_STATIC uint8_t consoleBytesRead = 0;
FILE_STATIC uint8_t consoleBuildingCommand = 0;

FILE_STATIC debug_context registeredDebugEntities[CONFIGM_debug_maxentities];
FILE_STATIC uint8_t entityCount = 0;

FILE_STATIC svc_status_debug debug_status;

// KEEP THESE STRINGS IN SYNC WITH HEADER FILE
FILE_STATIC uint8_t *DebugEntityFriendlyNames[] =  {
                                                    "Test Entity",
                                                    "Debug Service",
                                                    "I2C Bus",
                                                    "SPI Bus",
                                                    "CAN Bus",
                                                    "Core/BSP",
                                                    "UART Bus",
                                                    "RWheels",};

uint8_t reportStatusCallback(DebugMode mode)
{
    if (mode == InteractiveMode)
        {
            debugPrintF("**Debug Service Status:\r\n");
            debugPrintF("Trace level: %d\r\n\r\n", debug_status.trace_level);
            debugPrintF("*Registered Entities: %d\r\n", entityCount);
        }
        else
        {
            debugPrintF("Stuff without as many words (e.g. just CSV)");
        }
        return 1;
}

uint8_t actionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == InteractiveMode)
    {
        if (strlen(cmdstr) == 0)
        {
            debugPrintF("Here you would list the command syntax for all actions provided by this entity ...\r\n");
        }
        else
        {
            debugPrintF("Here you would act on the specific action string '%s' passed into the command.\r\n", cmdstr);
        }
    }
}

FILE_STATIC hBus handle;

void debugInit()
{
    if (debug_status.initialized != 0)
        return;

    debug_status.initialized = 1;
    debug_status.trace_level = __INITIAL_TRACE_LEVEL__;
    debug_status.debug_mode = InteractiveMode;

    handle = uartInit(BackchannelUART, 1);
    uartRegisterRxCallback(handle, debugReadCallback);

    debugRegisterEntity(Entity_DebugService, 'd', NULL, reportStatusCallback, actionCallback);
}

void debugPrint(uint8_t * buff, uint8_t szBuff)
{
    uartTransmit(handle, buff, szBuff);
}

void debugPrintF(const char *_format, ...)
{
    int numBytes;

    va_list argptr;
    va_start(argptr, _format);
    numBytes = vsnprintf(debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
    uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
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
        uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
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
        memset(debugConsoleInputBuff, 0, CONFIGM_debug_consoleinputbuffsize);
    }
    else
    {
        debugConsoleInputBuff[consoleBytesRead] = rcvdbyte;
        consoleBytesRead++;
    }
}

void debugRegisterEntity(entityID id, uint8_t pathchar,
                         simple_debug_handler infohandler, simple_debug_handler statushandler, param_debug_handler actionhandler)
{
    if (entityCount >= CONFIGM_debug_maxentities)
    {
        debug_status.registration_errors++;
        return;
    }

    // TODO:
    debug_context  *newEntity = &(registeredDebugEntities[entityCount++]);
    newEntity->id = id;
    newEntity->pathchar = pathchar;
    newEntity->info_handler = infohandler;
    newEntity->status_handler = statushandler;
    newEntity->action_handler = actionhandler;
}

void coreCallSimpleHandlers(simple_handler_type t)
{
    int i;
    simple_debug_handler handler;

    for (i = 0; i < entityCount; i++)
    {
        if (t == Handler_Info)
            handler = registeredDebugEntities[i].info_handler;
        else if (t == Handler_Status)
            handler = registeredDebugEntities[i].status_handler;
        else
        {
            debug_status.registration_errors++;
            return;
        }

        if (handler != 0)
            (handler)(debug_status.debug_mode);
        debugPrintF("\r\n");
    }
}

void cmdInfo()
{
    debugPrintF("\r\nSubsystem information (static):\r\n--------------------------------\r\n");
    coreCallSimpleHandlers(Handler_Info);
}

void cmdStatus()
{
    debugPrintF("\r\nSubsystem status (dynamic):\r\n----------------------------\r\n");
    coreCallSimpleHandlers(Handler_Status);
}

void cmdAction(uint8_t * cmdstr)
{
    int i;
    param_debug_handler handler;
    uint8_t foundhandler = 0;
    uint8_t foundpathchar = 0;
    uint8_t len = strlen(cmdstr);

    debugPrintF("\r\n");

    // If cmdstr is null, then user typed just '!', so give a list of path chars/entities
    if (len == 0)
    {
        debugPrintF("No entity path specified, so listing available entities ... \r\n\r\n");
                debugPrintF("\tEntity\t\tPath Char\r\n");
        debugPrintF("\t------\t\t---------\r\n\r\n");
        for (i = 0; i < entityCount; i++)
        {
            debugPrintF("\t%s\t\t%c\r\n", DebugEntityFriendlyNames[registeredDebugEntities[i].id], registeredDebugEntities[i].pathchar);

        }

    }
    else   // Pass it all along to the matching handler, if any
    {
        for (i = 0; i < entityCount; i++)
        {
            if (registeredDebugEntities[i].pathchar == cmdstr[0])
            {
                foundpathchar = 1;

                handler = registeredDebugEntities[i].action_handler;
                if (handler != NULL)
                {
                    foundhandler = 1;

                    (handler)(debug_status.debug_mode, cmdstr+1);
                    debugPrintF("\r\n");
                }
            }
        }

        if (foundhandler == 0)
        {
            if (foundpathchar == 0)
                debugPrintF("No debug entity with path character '%c' Use '!' alone to retrieve list of entities.\r\n", cmdstr[0]);
            else
                debugPrintF("Debug entity for '%c' found, but it doesn't have any actions available.\r\n", cmdstr[0]);

        }
    }
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
        case '!':
            debug_status.trace_level = 0;
            cmdAction(&cmdbuff[1]);
            break;
        default:
            // NOP
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


#else  /* __DEBUG__ not specified, therefore nop debug operations */

void debugInit() {}
void debugPrint(uint8_t * buff, uint8_t szBuff) {}
void debugPrintF(const char *_format, ...) {}
void debugReadCallback(uint8_t rcvdbyte) {}

void debugRegisterInfoHandler(debug_handler) {}
void debugRegisterStatusHandler(debug_handler) {}
void debugRegisterActionHandler(debug_handler) {}


#endif /* __DEBUG__ */
