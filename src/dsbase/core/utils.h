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

#endif /* CORE_UTILS_H_ */
