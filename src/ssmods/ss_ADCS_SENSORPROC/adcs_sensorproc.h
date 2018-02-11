/*
 * adcs_sensorproc.h
 */

#ifndef ADCS_SENSORPROC_H_
#define ADCS_SENSORPROC_H_

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"

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

// photodiode i2c bus
#define I2C_BUS_PHOTODIODES I2CBus1

// sun sensor i2c bus
#define I2C_BUS_SUNSENSOR I2CBus1

void handlePPTFiringNotification();
void handleRollCall();

// general telemetry functions
void sendHealthSegment();
void sendMetaSegment();

// sun sensor functions
void sunSensorUpdate();
void sendSunSensorData();

// photodiode functions
void photodiodeInitAll();
void photodiodeUpdate();
void sendPhotodiodeData();

#endif /* ADCS_SENSORPROC_H_ */
