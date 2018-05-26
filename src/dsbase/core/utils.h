/*
 * utils.h
 *
 *  Created on: May 1, 2017
 *      Author: jeffc
 */

#ifndef CORE_UTILS_H_
#define CORE_UTILS_H_
#include <stdint.h>

// Generic handle for buses and devices
typedef uint8_t hBus;
typedef uint8_t hDev;

typedef uint8_t telemid_t;

// Used for static variables in files (rather than in functions)
// Designed to melt away when running under a text context, so those variables
// become accessible
#ifndef TEST
#define FILE_STATIC static
#else
#define FILE_STATIC
#endif

#define NULLP ((void*)0)

#define BOOL uint8_t
#define TRUE 1
#define FALSE 0

#define PACKED_STRUCT __attribute__((__packed__))
#define TLM_SEGMENT typedef struct __attribute__((__packed__))
#define CMD_SEGMENT typedef struct __attribute__((__packed__))

// THIS HEADER MUST BE THE FIRST FIELD IN OUTGOING BINARY TELEMETRY STRUCTS
TLM_SEGMENT _bctlm_header {
    uint8_t syncpattern;
    uint8_t length;
    uint8_t id;
} BcTlmHeader;

CMD_SEGMENT _bccmd_header {
    uint8_t syncpattern;
    uint8_t length;
    uint8_t entityid;
    uint8_t opcode;
} BcCmdHeader;

typedef enum {
    Bus_HealthUnknown,
    Bus_MinorFault,
    Bus_FatalError,
    Bus_Healthy,
    Bus_BuffOverrunHandled,
    Bus_BuffOverrunFatal,
    Bus_Underrun,
    Bus_BadAddr,
    Bus_Uninitialized,
} bus_health;

//puts the value of reg to outVal at a time when reg is not being written to
#define safeRead(reg, outVal) do {outVal = reg;} while (outVal != reg)

#endif /* CORE_UTILS_H_ */
