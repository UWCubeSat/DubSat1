/*
 * debugtools.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include <string.h>
#include "debugtools.h"
#include "uart.h"
#include "utils.h"
#include "config/config.h"
#include "bsp/bsp.h"

// NOTE:  Virtually everything in this file should be inside of the
// __DEBUG__ #ifdef, as the implementation of all these things
// should go away when compiled for release
#ifdef __DEBUG__

FILE_STATIC uint8_t debugWriteOutputBuff[CONFIGM_debug_outputbuffsize];
FILE_STATIC uint8_t debugConsoleInputBuff[CONFIGM_debug_consoleinputbuffsize];

FILE_STATIC uint8_t consoleBytesRead = 0;
FILE_STATIC uint8_t consoleBuildingCommand = 0;

FILE_STATIC debug_context registeredDebugEntities[CONFIGM_debug_maxentities];
FILE_STATIC uint8_t entityCount = 0;

FILE_STATIC svc_status_debug debug_status;
FILE_STATIC hBus handle;
FILE_STATIC BcCmdState cmd_parsing_state;
FILE_STATIC BcCmdHeader cmd_header;
FILE_STATIC uint8_t binExpectedParamBytes;
FILE_STATIC uint8_t binParamBytesRead = 0;

// KEEP THESE STRINGS IN SYNC WITH HEADER FILE and following array
FILE_STATIC uint8_t *DebugEntityFriendlyNames[] =  {
                                                    "<Null Entity>",
                                                    "Test Entity",
                                                    "Debug Service",
                                                    "I2C Bus",
                                                    "SPI Bus",
                                                    "CAN Bus",
                                                    "Core/BSP",
                                                    "UART Bus",
                                                    "RWheels",
                                                    "Analog Sensors",
                                                    "SUBSYSTEM",  // For the "main" entity in each subsystem module
                                                    };

// KEEP THESE STRINGS IN SYNC WITH HEADER FILE and previous array
// Use lower-case for "system" services and buses, capital letters for
// subsystem-specific stuff
FILE_STATIC uint8_t DebugEntityPathChars[] = {
                                              '.',
                                              '^',
                                              'd',
                                              'i',
                                              'p',
                                              'c',
                                              'b',
                                              'u',
                                              'R',
                                              'a',
                                              '%',
                                            };

uint8_t reportStatusCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
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
    if (mode == Mode_ASCIIInteractive)
    {
        if (strlen((const char *)cmdstr) == 0)
        {
            debugPrintF("Here you would list the command syntax for all actions provided by this entity ...\r\n");
        }
        else
        {
            debugPrintF("Here you would act on the specific action string '%s' passed into the command.\r\n", cmdstr);
        }
    }
    return 1;
}

void debugInit()
{
    if (debug_status.initialized != 0)
        return;

    handle = uartInit(BackchannelUART, 1, DEBUG_UART_SPEED);
    uartRegisterRxCallback(handle, debugReadCallback);

    debug_status.initialized = 1;
    debug_status.trace_level = __INITIAL_TRACE_LEVEL__;
    debugSetMode((DebugMode)__INITIAL_DEBUG_MODE__);
    cmd_parsing_state = STATE_START;
    cmd_header.syncpattern = BCBIN_SYNCPATTERN;

    debugRegisterEntity(Entity_DebugService, NULL, reportStatusCallback, actionCallback);
}

DebugMode debugGetMode()
{
    return debug_status.debug_mode;
}

void debugSetMode(DebugMode newmode)
{
    debug_status.debug_mode = newmode;
    if (debug_status.debug_mode != Mode_ASCIIInteractive)
        uartSetEchoOff(handle);
    else
        uartSetEchoOn(handle);
}

void debugPrint(uint8_t * buff, uint8_t szBuff)
{
    // DO NOT USE debugPrint when performing binary streaming
    if (debug_status.debug_mode == Mode_BinaryStreaming)
            return;

    uartTransmit(handle, buff, szBuff);
}

void debugPrintF(const char *_format, ...)
{
    // DO NOT USE debugPrintF when performing binary streaming
    if (debug_status.debug_mode == Mode_BinaryStreaming)
        return;

    int numBytes;

    va_list argptr;
    va_start(argptr, _format);
    numBytes = vsnprintf((char*)debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
    uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
}

void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff)
{
    // DO NOT USE debugTrace when performing binary streaming
    if (debug_status.debug_mode == Mode_BinaryStreaming)
        return;

    if (normalizeTraceLevel(level) <= debug_status.trace_level)
        debugPrint(buff, szBuff);
}

void debugTraceF(uint8_t level, const char *_format, ...)
{
    // DO NOT USE debugTraceF when performing binary streaming
    if (debug_status.debug_mode == Mode_BinaryStreaming)
        return;

    int numBytes;

    if (normalizeTraceLevel(level) <= debug_status.trace_level)
    {
        va_list argptr;
        va_start(argptr, _format);
        numBytes = vsnprintf((char*)debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
        uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
    }
}

void debugReadCallback(uint8_t rcvdbyte)
{
    if (debug_status.debug_mode != Mode_BinaryStreaming)
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
    else
    {
        switch (cmd_parsing_state)
        {
            case STATE_START:
                binExpectedParamBytes = 0;
                binParamBytesRead = 0;
                if (rcvdbyte == BCBIN_SYNCPATTERN)
                    cmd_parsing_state = STATE_LEN_WAIT;
                else
                {
                    debug_status.unknown_cmd_ids++;
                }
                break;
            case STATE_LEN_WAIT:
                cmd_header.length = rcvdbyte;
                cmd_parsing_state = STATE_ENTID_WAIT;
                break;
            case STATE_ENTID_WAIT:
                cmd_header.entityid = rcvdbyte;
                cmd_parsing_state = STATE_OPCODE_WAIT;
                break;
            case STATE_OPCODE_WAIT:
                cmd_header.opcode = rcvdbyte;
                binExpectedParamBytes = cmd_header.length - 4;
                debugConsoleInputBuff[0] = cmd_header.opcode;

                // If no parameters (but a valid opcode)
                if (binExpectedParamBytes == 0)
                {
                    debugConsoleInputBuff[1] = 0;
                    debugInvokeActionHandler(cmd_header.entityid, debugConsoleInputBuff);
                    cmd_parsing_state = STATE_START;
                }
                else
                    cmd_parsing_state = STATE_GATHERING_PARAMS;
                break;
            case STATE_GATHERING_PARAMS:
                debugConsoleInputBuff[binParamBytesRead+1] = rcvdbyte;
                binParamBytesRead++;

                if (binParamBytesRead >= binExpectedParamBytes)
                {
                    debugConsoleInputBuff[binParamBytesRead+1] = 0;
                    debugInvokeActionHandler(cmd_header.entityid, debugConsoleInputBuff);
                    cmd_parsing_state = STATE_START;
                }
                break;
        }
    }
}

void debugRegisterEntity(entityID id,
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
    newEntity->pathchar = DebugEntityPathChars[(uint8_t) id];
    newEntity->info_handler = infohandler;
    newEntity->status_handler = statushandler;
    newEntity->action_handler = actionhandler;
}

void coreCallSimpleHandlers(simple_handler_type t, entityID onlyid)
{
    int i;
    simple_debug_handler handler;

    for (i = 0; i < entityCount; i++)
    {
        if (onlyid == Entity_NONE || onlyid == registeredDebugEntities[i].id)
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

            // No need to search for more
            if (onlyid != Entity_NONE)
                return;
        }
    }
}

void debugInvokeInfoHandler(entityID id)
{
    coreCallSimpleHandlers(Handler_Info, id);
}

void debugInvokeStatusHandler(entityID id)
{
    coreCallSimpleHandlers(Handler_Status, id);
}

void debugInvokeActionHandler(entityID id, uint8_t * params)
{
    int i;
    param_debug_handler foundhandler = 0;

    for (i = 0; i < entityCount; i++)
    {
        if (registeredDebugEntities[i].id == id)
        {
            foundhandler = registeredDebugEntities[i].action_handler;
        }
    }

    if (foundhandler == NULL)
    {
        debug_status.unknown_entity_ids++;
        return;
    }

    (foundhandler)(debug_status.debug_mode, params);
}

void debugInvokeInfoHandlers()
{
    debugPrintF("\r\nSubsystem information (static):\r\n--------------------------------\r\n");
    coreCallSimpleHandlers(Handler_Info, Entity_NONE);
}

void debugInvokeStatusHandlers()
{
    debugPrintF("\r\nSubsystem status (dynamic):\r\n----------------------------\r\n");
    coreCallSimpleHandlers(Handler_Status, Entity_NONE);
}

void debugInvokeActionHandlers(uint8_t * cmdstr)
{
    int i;
    param_debug_handler handler;
    uint8_t foundhandler = 0;
    uint8_t foundpathchar = 0;
    uint8_t len = strlen((const char *)cmdstr);

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
            debugInvokeInfoHandlers();
            break;
        case 's':   // Call status functions on all entities
            debug_status.trace_level = 0;
            debugInvokeStatusHandlers();
            break;
        case '!':
            debug_status.trace_level = 0;
            debugInvokeActionHandlers(&cmdbuff[1]);
            break;
        case '*':    // Change debug mode - can't get out of binary once in it, however
            debug_status.trace_level = 0;
            debugPrintF("Entering binary telemetry/telecommand streaming mode ... to exit, reset device.\r\n");
            debugSetMode(Mode_BinaryStreaming);
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
    if (debug_status.debug_mode == Mode_ASCIIInteractive)
    {
        debugPrintF("\r\n[%s]>>", getSubsystemModulePath());
    }
    else if (debug_status.debug_mode == Mode_ASCIIHeadless)
    {
        debugPrintF(">");
    }
}

void bcbinPopulateHeader( BcTlmHeader *header, uint8_t opcode, uint8_t fulllen)
{
    header->syncpattern = BCBIN_SYNCPATTERN;
    header->id = opcode;
    header->length = fulllen;
}

void bcbinSendPacket(uint8_t * buff, uint8_t szBuff)
{
    uartTransmit(handle, buff, szBuff);
}

void bcbinPopulateMeta(meta_segment *mpkt, size_t sz)
{
    bcbinPopulateHeader(&(mpkt->header), TLM_ID_SHARED_META, sz);

    mpkt->module = bspGetModule();
    mpkt->compver = __TI_COMPILER_VERSION__;
    mpkt->stdcver = __STDC_VERSION__;

    strncpy( mpkt->compdate, __DATE__, MAX_META_FIELD_LEN);
    strncpy( mpkt->comptime, __TIME__, MAX_META_FIELD_LEN);
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
