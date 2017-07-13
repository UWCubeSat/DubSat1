/*
 * systeminfo.h
 *
 *  Created on: Jun 29, 2017
 *      Author: jeffc
 */
#include "../core/debugtools.h"

#ifndef INTERFACES_SYSTEMINFO_H_
#define INTERFACES_SYSTEMINFO_H_


typedef enum _subsystemModule {
    Module_Test = 0,
    Module_ADCS_BDot = 1,
} SubsystemModule;

uint8_t infoReport(DebugMode mode);
uint8_t *getSubsystemModulePath();

#endif /* INTERFACES_SYSTEMINFO_H_ */
