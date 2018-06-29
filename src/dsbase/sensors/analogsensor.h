/*
 * analogsensor.h
 *
 *  Created on: Nov 8, 2017
 *      Author: SatLab
 */

#ifndef DSBASE_SENSORS_ANALOGSENSOR_H_
#define DSBASE_SENSORS_ANALOGSENSOR_H_

#include <stdint.h>

#include <msp430.h>
#include "bsp/bsp.h"
#include "core/utils.h"
#include "config/config_sensor_analog.h"

#define NUM_MAX_NONTEMP_ANALOG_SENSORS   10  // Number of ADC channels (NOT including built-in temp)
#define NUM_MAX_ANALOG_SENSORS  (NUM_MAX_NONTEMP_ANALOG_SENSORS+1)

#define TEMPSENSOR_INDEX  (0)  // Temp is first, always
#define TEMPSENSOR_DEV  (hDev)0

//See device datasheet for TLV table memory mapping
// Temperature in Celsius. See the Device Descriptor Table section in the
// System Resets, Interrupts, and Operating Modes, System Control Module
// chapter in the device user's guide for background information on the
// used formula.
#define CALADC12_12V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C
#define CALADC12_12V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C
#define CALADC12_20V_30C  *((unsigned int *)0x1A1E)
#define CALADC12_20V_85C  *((unsigned int *)0x1A20)
#define CALADC12_25V_30C  *((unsigned int *)0x1A22)
#define CALADC12_25V_85C  *((unsigned int *)0x1A24)

#define ADC12INCH_TEMPSENSOR ADC12INCH_30

#define ADC12_CONVERSION_FACTOR   4096.0f

// Note that the negative ref VR- is always set to analog Vss (e.g. 0V)
typedef enum
{
    Ref_Vcc,
    Ref_2p5V,
    Ref_2V,
    Ref_1p2V
} VPositiveReference;

typedef enum {
    CHAN_A0,    //  0, P1.0
    CHAN_A1,    //  1, P1.1
    CHAN_A2,    //  2, P1.2
    CHAN_A3,    //  3, P1.3
    CHAN_A4,    //  4, P1.4
    CHAN_A5,    //  5, P1.5
    CHAN_12,    //  6, P3.0
    CHAN_13,    //  7, P3.1
    CHAN_14,    //  8, P3.2
    CHAN_15,    //  9, P3.3
    CHAN_TEMPSENSOR,  // 10, --> A30, internal
} ACHANNEL;

typedef enum {
    Type_GeneralV,
    Type_IntTempC,
    Type_ExtTempC,
} sensor_type;



typedef struct {
    BOOL activated;
    uint8_t capmem;  // stores the ADC12MEMx number for this sensor
    ACHANNEL channel;
    sensor_type stype;

    uint16_t lastrawvalue;
    float lastvalueV;  // for sensors that will store a voltage
    float lastvalueC;  // for sensors using a conversion to a temperature (internal or TMP-36)
} asensor_info;



void asensorInit(VPositiveReference vref);
hDev asensorActivateChannel(ACHANNEL newchan, sensor_type stype);
uint16_t asensorReadSingleSensorRaw(hDev hSensor);
float asensorReadSingleSensorV(uint8_t val);
void asensorUpdateAllSensors();
float asensorGetLastValueV(hDev hSensor);
float asensorGetLastIntTempC();
float asensorGetLastExtTempC(hDev hSensor);
float asensorReadIntTempC();
float asensorReadExtTempC(hDev hSensor);
uint16_t asensorReadIntTempRawC();
int16_t compressMSPTemp(float raw);

FILE_STATIC void inline enableADC() { ADC12CTL0 |= ADC12ENC; }
FILE_STATIC void inline disableADC() { ADC12CTL0 &= ~ADC12ENC; }

#define ASENSE_SEL1 P1SEL1
#define ASENSE_SEL0 P1SEL0
#define ASENSE_BIT BIT2
#define ASENSE_DIR P1DIR  // doesn't matter, however, not used by A2


#endif /* DSBASE_SENSORS_ANALOGSENSOR_H_ */
