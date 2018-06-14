/*
 * analogsensor.c
 *
 *  Created on: Nov 8, 2017
 *      Author: SatLab
 */

#include "sensors/analogsensor.h"

FILE_STATIC uint16_t CALADC_30C, CALADC_85C;

FILE_STATIC asensor_info asensors[NUM_MAX_ANALOG_SENSORS];

FILE_STATIC VPositiveReference voltageref;
FILE_STATIC float VpLSB;

FILE_STATIC uint16_t ADCIn_ADC12MCTL[] = { ADC12INCH_0, ADC12INCH_1, ADC12INCH_2, ADC12INCH_3, ADC12INCH_4, ADC12INCH_5, ADC12INCH_12, ADC12INCH_13, ADC12INCH_14, ADC12INCH_15, ADC12INCH_TEMPSENSOR };
FILE_STATIC uint16_t ADCMEMnum_ADC12CSTART[] = { ADC12CSTARTADD_0, ADC12CSTARTADD_1, ADC12CSTARTADD_2, ADC12CSTARTADD_3, ADC12CSTARTADD_4, ADC12CSTARTADD_5, ADC12CSTARTADD_6, ADC12CSTARTADD_7, ADC12CSTARTADD_8, ADC12CSTARTADD_9, ADC12CSTARTADD_10 };
FILE_STATIC uint16_t RefV_REFCTL0_SetAndOn[] = { 0x00, REFVSEL_2 | REFON, REFVSEL_1 | REFON, REFVSEL_0 | REFON };
FILE_STATIC uint16_t RefV_ADC12MCTL_Set[] =    { ADC12VRSEL_0, ADC12VRSEL_1, ADC12VRSEL_1, ADC12VRSEL_1 };

FILE_STATIC volatile BOOL capture_complete = 0;
FILE_STATIC uint8_t num_asense_channels = 0;

FILE_STATIC BOOL asensorinitialized = FALSE;

FILE_STATIC float tempIntConvertRawtoCelsius(uint16_t raw)
{
    return (float)(((float)raw - CALADC_30C) * (85 - 30)) / (CALADC_85C - CALADC_30C) + 30.0f;
}

// TODO:  Implement formula for the TMP-36
FILE_STATIC float tempExtConvertRawtoCelsius(unsigned int raw)
{
    // To be implemented, using datasheet for the TMP-36
    // NOTE:  zero-crossing of transfer function has +0.5V offset, it does NOT cross at 0
    return -13.0f;
}

// Loops through all activated channels and makes sure the EOS (end of sequence) bit
// is placed properly (i.e. at the end)
#define END_SEQUENCE        ADC12EOS
#define NOT_END_SEQUENCE    (~ADC12EOS)
void configSequence(uint8_t lastchan)
{
    if (lastchan == 0)
    {
        ADC12MCTL0 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL0 &= NOT_END_SEQUENCE;

    if (lastchan == 1)
    {
        ADC12MCTL1 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL1 &= NOT_END_SEQUENCE;

    if (lastchan == 2)
    {
        ADC12MCTL2 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL2 &= NOT_END_SEQUENCE;

    if (lastchan == 3)
    {
        ADC12MCTL3 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL3 &= NOT_END_SEQUENCE;

    if (lastchan == 4)
    {
        ADC12MCTL4 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL4 &= NOT_END_SEQUENCE;

    if (lastchan == 5)
    {
        ADC12MCTL5 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL5 &= NOT_END_SEQUENCE;

    if (lastchan == 6)
    {
        ADC12MCTL6 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL6 &= NOT_END_SEQUENCE;

    if (lastchan == 7)
    {
        ADC12MCTL7 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL7 &= NOT_END_SEQUENCE;

    if (lastchan == 8)
    {
        ADC12MCTL8 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL8 &= NOT_END_SEQUENCE;

    if (lastchan == 9)
    {
        ADC12MCTL9 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL9 &= NOT_END_SEQUENCE;

    if (lastchan == 10)
    {
        ADC12MCTL10 |= END_SEQUENCE;
        return;
    }
    else
        ADC12MCTL10 &= NOT_END_SEQUENCE;

    // Fall through to last channel = 11 or something invalid, so ...
    ADC12MCTL11 |= END_SEQUENCE;
    return;
}

FILE_STATIC void enableMultiSampleMode()
{
    ADC12CTL0 |= ADC12MSC_1;        // Sampling sequence only requires one Start toggle to start whole sequence (one time through)
    ADC12CTL1 |= ADC12CONSEQ_1;      // Single samples, in a sequence
    ADC12CTL3 = ADC12CSTARTADD_0 | ADC12TCMAP;  // Always starts at 0, to include temperature sensor

    configSequence(num_asense_channels-1);
}

FILE_STATIC void enableSingleSampleMode(hDev hSensor)
{
    ADC12CTL0 &= ~ADC12MSC;
    ADC12CTL1 &= ~ADC12CONSEQ;
    ADC12CTL3 = ADCMEMnum_ADC12CSTART[(uint8_t)hSensor] | ADC12TCMAP;
}

void configChannel(uint8_t capmem, uint16_t mctlx)
{
    switch (capmem)
    {
        case 0:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL0 = mctlx;
            ADC12IER0 |= BIT0;
            break;
        case 1:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL1 = mctlx;
            ADC12IER0 |= BIT1;
            break;
        case 2:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL2 = mctlx;
            ADC12IER0 |= BIT2;
            break;
        case 3:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL3 = mctlx;
            ADC12IER0 |= BIT3;
            break;
        case 4:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL4 = mctlx;
            ADC12IER0 |= BIT4;
            break;
        case 5:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL5 = mctlx;
            ADC12IER0 |= BIT5;
            break;
        case 6:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL6 = mctlx;
            ADC12IER0 |= BIT6;
            break;
        case 7:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL7 = mctlx;
            ADC12IER0 |= BIT7;
            break;
        case 8:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL8 = mctlx;
            ADC12IER0 |= BIT8;
            break;
        case 9:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL9 = mctlx;
            ADC12IER0 |= BIT9;
            break;
        case 10:   // For the built-in temp sensor, as it is always the first sensor added
            ADC12MCTL10 = mctlx;
            ADC12IER0 |= BITA;
            break;
        default:
            break;
    }
}

FILE_STATIC hDev coreActivateChannel(ACHANNEL newchan, sensor_type stype)
{
    uint8_t refindex = (uint8_t)voltageref;

    if (num_asense_channels >= NUM_MAX_ANALOG_SENSORS)
        return 0;   // TODO:  add data collection to figure out what happened

    uint8_t newchannum = num_asense_channels++;

    asensors[newchannum].activated = TRUE;
    asensors[newchannum].capmem = newchannum;
    asensors[newchannum].channel = newchan;
    asensors[newchannum].stype = stype;
    asensors[newchannum].lastrawvalue = 0.0f;

    uint16_t MCTLx = (RefV_ADC12MCTL_Set[refindex] | ADCIn_ADC12MCTL[(uint8_t)newchan]);
    configChannel(asensors[newchannum].capmem, MCTLx);
    configSequence(newchannum);

    return (hDev)newchannum;
}

FILE_STATIC initializeTempSensor()
{
    // Setup the temperature sensor - used on ALL MSPs
    ADC12CTL3 |= ADC12TCMAP;                 // Enable internal temperature sensor

    // Temp sensor always uses ADC12MEM0
    coreActivateChannel(CHAN_TEMPSENSOR, Type_IntTempC);
}

FILE_STATIC initializeVoltageRef()
{
    uint8_t refindex = (uint8_t)voltageref;

    // First, wait until we can actually do somthing with the vref generator
    // TODO:  add a timeout of some kind, so we don't hang here under odd circumstances
    while(REFCTL0 & REFGENBUSY);

    // Now, power up the reference voltage generator with the requested voltage
    // NOTE that if Vcc chosen as VR+ reference voltage, vref module NOT turned on at all
    REFCTL0 = RefV_REFCTL0_SetAndOn[refindex];

    // Pull correct calibration values out of the INFORMATION segment
    switch (voltageref)
    {
        case Ref_1p2V:
            VpLSB = 1.2f / ADC12_CONVERSION_FACTOR;
            CALADC_30C = CALADC12_12V_30C;
            CALADC_85C = CALADC12_12V_85C;
            break;
        case Ref_2V:
            VpLSB = 2.0f / ADC12_CONVERSION_FACTOR;
            CALADC_30C = CALADC12_20V_30C;
            CALADC_85C = CALADC12_20V_85C;
            break;
        case Ref_2p5V:
            VpLSB = 2.5f / ADC12_CONVERSION_FACTOR;
            CALADC_30C = CALADC12_25V_30C;
            CALADC_85C = CALADC12_25V_85C;
            break;
        default:
            break;
    }

    // Lastly, wait to make sure reference generator up and running (can take a bit), then turn the ADC unit back on
    while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator to settle
}

void asensorInit(VPositiveReference vref)
{
    if (asensorinitialized == TRUE)
        return;
    asensorinitialized = TRUE;

    // All public APIs for the ADC must start with disabling the engine, and enabling it at the end
    // This is NOT done in the internal/FILE_STATIC "helper" functions
    disableADC();   // MUST do this before changing any other settings

    voltageref = vref;

    // Start configuring
    // Remember MEM0-MEM7 AND the temp MEM (30) uses SHT0 bits; others use SHT1 bits ... set both the same to preserve sanity
    // These settings are common to ALL chosen sampling modes
    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT1_10 | ADC12ON;      // Set sample time (512) and power on
    ADC12CTL1 = ADC12SHP;                    // Enable sample timer, sequence of channels
    ADC12CTL2 |= ADC12RES_2;                                // Make sure we're in 12-bit mode

    initializeTempSensor();
    initializeVoltageRef();

    enableMultiSampleMode();

    // Only re-enable the ADC engine as the last step before returning to
    // the API user - earlier than that and it's easy to accidentally try setting some register
    // on the ADC12 that is locked when the engine is enabled
    __enable_interrupt();
    enableADC();
}


// Break out functionality for opening a channel to make it useful both from public
// APIs (where the engine must be disabled explicitly) and the internal calls (where
// other code has already disabled the engine)
hDev asensorActivateChannel(ACHANNEL newchan, sensor_type stype)
{
    // All public APIs for the ADC must start with disabling the engine, and enabling it at the end
    // This is NOT done in the internal/FILE_STATIC "helper" functions
    disableADC();   // MUST do this before changing any other settings

    hDev hSensor = coreActivateChannel(newchan, stype);

    // Only re-enable the ADC engine as the last step before returning to
    // the API user - earlier than that and it's easy to accidentally try setting some register
    // on the ADC12 that is locked when the engine is enabled
    enableADC();

    return hSensor;
}

float convertRawToFloat(hDev hSensor)
{
    float convtval = 0.0f;
    sensor_type stype = asensors[(uint8_t)hSensor].stype;
    uint16_t rawval = asensors[(uint8_t)hSensor].lastrawvalue;

    if (stype == Type_GeneralV)
    {
        convtval = (float)rawval * VpLSB;
        asensors[(uint8_t)hSensor].lastvalueV = convtval;
    }
    else if (stype == Type_IntTempC)
    {
        convtval = tempIntConvertRawtoCelsius(rawval);
        asensors[(uint8_t)hSensor].lastvalueC = convtval;
    }
    else if (stype == Type_ExtTempC)
    {
        // TODO:  implement this!!!
        convtval = tempExtConvertRawtoCelsius(rawval);
        asensors[(uint8_t)hSensor].lastvalueC = convtval;
    }
    return convtval;
}

uint16_t asensorReadSingleSensorRaw(hDev hSensor)
{
    disableADC();
    enableSingleSampleMode(hSensor);
    enableADC();

    // Start capture
    capture_complete = 0;
    ADC12CTL0 |= ADC12SC;

    // Wait for both captures to complete
    while (capture_complete < 1) {}

    // capture_complete == 1 after interrupt copies appropriate ADC12MEMx value
    // to the asensor_info struct for the correct sensor
    return asensors[(uint8_t)hSensor].lastrawvalue;
}

float asensorGetLastValueV(hDev hSensor)
{
    // Convert in case it hasn't happened before
    return convertRawToFloat(hSensor);
}

// Reads last-captured value of temp, without triggering a new capture
float asensorGetLastIntTempC()
{
    // Convert in case it hasn't happened before
    return convertRawToFloat(TEMPSENSOR_DEV);
}

float asensorGetLastExtTempC(hDev hSensor)
{
    // TODO:  implement this!
    return convertRawToFloat(hSensor);
}

// Read a single channel
float asensorReadSingleSensorV(hDev hSensor)
{
    asensorReadSingleSensorRaw(hSensor);
    return convertRawToFloat(hSensor);
}

// Takes a new reading
float asensorReadIntTempC()
{
    asensorReadSingleSensorRaw(TEMPSENSOR_DEV);
    return convertRawToFloat(TEMPSENSOR_DEV);
}
uint16_t asensorReadIntTempRawC(){
    return asensorReadSingleSensorRaw(TEMPSENSOR_DEV);
}

float asensorReadExtTempC(hDev hSensor)
{
    // TODO:  implement this!
    return 0.0f;
}

void asensorUpdateAllSensors()
{
    disableADC();
    enableMultiSampleMode();
    enableADC();

    // Start capture
    capture_complete = 0;
    ADC12CTL0 |= ADC12SC;

    // Wait for both captures to complete
    // TODO:  add a timeout here as well, so we don't hang in this loop
    // TODO:  obviously add an async option to this function
    while (capture_complete < num_asense_channels) {}

    return;
}

uint16_t compressMSPTemp(float raw)
{
    return (uint16_t)(raw * 1000);
}

#pragma vector=ADC12_B_VECTOR
__interrupt void ADC12ISR (void)
{
    switch(__even_in_range(ADC12IV, ADC12IV__ADC12RDYIFG))
    {
        case ADC12IV__NONE:        break;   // Vector  0:  No interrupt
        case ADC12IV__ADC12OVIFG:  break;   // Vector  2:  ADC12MEMx Overflow
        case ADC12IV__ADC12TOVIFG: break;   // Vector  4:  Conversion time overflow
        case ADC12IV__ADC12HIIFG:  break;   // Vector  6:  ADC12BHI
        case ADC12IV__ADC12LOIFG:  break;   // Vector  8:  ADC12BLO
        case ADC12IV__ADC12INIFG:  break;   // Vector 10:  ADC12BIN
        case ADC12IV__ADC12IFG0:            // Vector 12:  ADC12MEM0 Interrupt
            asensors[0].lastrawvalue = ADC12MEM0;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG1:            // Vector 14:  ADC12MEM1 Interrupt
            asensors[1].lastrawvalue = ADC12MEM1;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG2:
            asensors[2].lastrawvalue = ADC12MEM2;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG3:
            asensors[3].lastrawvalue = ADC12MEM3;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG4:
            asensors[4].lastrawvalue = ADC12MEM4;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG5:
            asensors[5].lastrawvalue = ADC12MEM5;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG6:
            asensors[6].lastrawvalue = ADC12MEM6;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG7:
            asensors[7].lastrawvalue = ADC12MEM7;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG8:
            asensors[8].lastrawvalue = ADC12MEM8;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG9:
            asensors[9].lastrawvalue = ADC12MEM9;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG10:
            asensors[10].lastrawvalue = ADC12MEM10;               // Move results, IFG is cleared
            capture_complete += 1;
            break;
        case ADC12IV__ADC12IFG11:  break;   // Vector 34:  ADC12MEM11
        case ADC12IV__ADC12IFG12:  break;   // Vector 36:  ADC12MEM12
        case ADC12IV__ADC12IFG13:  break;   // Vector 38:  ADC12MEM13
        case ADC12IV__ADC12IFG14:  break;   // Vector 40:  ADC12MEM14
        case ADC12IV__ADC12IFG15:  break;   // Vector 42:  ADC12MEM15
        case ADC12IV__ADC12IFG16:  break;   // Vector 44:  ADC12MEM16
        case ADC12IV__ADC12IFG17:  break;   // Vector 46:  ADC12MEM17
        case ADC12IV__ADC12IFG18:  break;   // Vector 48:  ADC12MEM18
        case ADC12IV__ADC12IFG19:  break;   // Vector 50:  ADC12MEM19
        case ADC12IV__ADC12IFG20:  break;   // Vector 52:  ADC12MEM20
        case ADC12IV__ADC12IFG21:  break;   // Vector 54:  ADC12MEM21
        case ADC12IV__ADC12IFG22:  break;   // Vector 56:  ADC12MEM22
        case ADC12IV__ADC12IFG23:  break;   // Vector 58:  ADC12MEM23
        case ADC12IV__ADC12IFG24:  break;   // Vector 60:  ADC12MEM24
        case ADC12IV__ADC12IFG25:  break;   // Vector 62:  ADC12MEM25
        case ADC12IV__ADC12IFG26:  break;   // Vector 64:  ADC12MEM26
        case ADC12IV__ADC12IFG27:  break;   // Vector 66:  ADC12MEM27
        case ADC12IV__ADC12IFG28:  break;   // Vector 68:  ADC12MEM28
        case ADC12IV__ADC12IFG29:  break;   // Vector 70:  ADC12MEM29
        case ADC12IV__ADC12IFG30:  break;   // Vector 72:  ADC12MEM30
        case ADC12IV__ADC12IFG31:  break;   // Vector 74:  ADC12MEM31
        case ADC12IV__ADC12RDYIFG: break;   // Vector 76:  ADC12RDY
        default: break;
    }
}

