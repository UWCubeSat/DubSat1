#include <msp430.h> 
#include "asense_test2.h"


/*
 * main.c
 */
#define NUM_MAX_ANALOG_SENSORS   8

//See device datasheet for TLV table memory mapping
// Temperature in Celsius. See the Device Descriptor Table section in the
// System Resets, Interrupts, and Operating Modes, System Control Module
// chapter in the device user's guide for background information on the
// used formula.
#define CALADC12_12V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C
#define CALADC12_12V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C

#define ADC12INCH_TEMPSENSOR ADC12INCH_30

unsigned int CALADC_30C, CALADC_85C;

FILE_STATIC unsigned int raw_data[NUM_MAX_ANALOG_SENSORS];

FILE_STATIC float temperature;
FILE_STATIC volatile float temperatureDegC;
FILE_STATIC volatile float temperatureDegF;

// Note that the negative ref VR- is always set to analog Vss (e.g. 0V)
typedef enum
{
    Ref_Vcc,
    Ref_2p5V,
    Ref_2V,
    Ref_1p2V
} VPositiveReference;

FILE_STATIC uint16_t RefV_REFCTL0_SetAndOn[] = { 0x00, REFVSEL_2 | REFON, REFVSEL_1 | REFON, REFVSEL_0 | REFON };
FILE_STATIC uint16_t RefV_ADC12MCTL_Set[] =    { ADC12VRSEL_0, ADC12VRSEL_1, ADC12VRSEL_1, ADC12VRSEL_1 };

FILE_STATIC volatile BOOL capture_complete = 0;

void asensorInit(VPositiveReference ref)
{
    uint8_t refindex = (uint8_t)ref;

    // First, wait until we can actually do somthing with the ref generator
    // TODO:  add a timeout of some kind, so we don't hang here under odd circumstances
    while(REFCTL0 & REFGENBUSY);

    // Now, power up the reference voltage generator with the requested voltage
    // NOTE that if Vcc chosen as VR+ reference voltage, ref module NOT turned on at all
    REFCTL0 = RefV_REFCTL0_SetAndOn[refindex];

    // Disable ADC12 and start configuring
    // Remember MEM0-MEM7 AND the temp MEM (30) uses SHT0 bits; others use SHT1 bits ... set both the same to preserve sanity
    ADC12CTL0 &= ~ADC12ENC;                                 // Disable ADC12
    ADC12CTL0 = ADC12SHT0_10 + ADC12SHT1_10 + ADC12ON;        // Set sample time (512) and power on
    ADC12CTL1 = ADC12SHP;                                   // Enable sample timer


    // Setup the temperature sensor - used on ALL MSPs
    ADC12CTL3 = ADC12TCMAP;                 // Enable internal temperature sensor
    // TODO:  Create the calibration factors necessary for each of the reference voltage possibilities
    switch (ref)
    {
        case Ref_1p2V:
            CALADC_30C = CALADC12_12V_30C;
            CALADC_85C = CALADC12_12V_85C;
            break;
        // todo:  add other cases
        default:
            break;
    }

    ADC12MCTL0 = RefV_ADC12MCTL_Set[refindex] | ADC12INCH_TEMPSENSOR;   // Set for temperature sensor channel (ch. 30)
    ADC12IER0 = 0x001;                        // Turns on interrupt for ADC12IFG0 (for memory 0)

    // Lastly, wait to make sure reference generator up and running (can take a bit), then turn the ADC unit back on
    while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator to settle
    ADC12CTL0 |= ADC12ENC;
}

float tempConvertRawtoCelsius(unsigned int raw)
{
    return (float)(((float)raw - CALADC_30C) * (85 - 30)) / (CALADC_85C - CALADC_30C) + 30.0f;
}

float asensorReadSensorSync()
{
    capture_complete = 0;
    ADC12CTL0 |= ADC12SC;               // Sampling and conversion start

    while (capture_complete == 0) {}

    capture_complete = 0;
    float tempC = tempConvertRawtoCelsius(raw_data[0]);
    return tempC;
}



int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    asensorInit(Ref_1p2V);

    __bis_SR_register(GIE); // LPM4 with interrupts enabled


    while(1)
    {
        temperature = asensorReadSensorSync();
        debugPrintF("Temp:  %f C\r\n", temperature);

        //__delay_cycles(0.5 * SEC);
    }
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
            raw_data[0] = ADC12MEM0;               // Move results, IFG is cleared
            capture_complete = 1;
            break;
        case ADC12IV__ADC12IFG1:   break;   // Vector 14:  ADC12MEM1
        case ADC12IV__ADC12IFG2:   break;   // Vector 16:  ADC12MEM2
        case ADC12IV__ADC12IFG3:   break;   // Vector 18:  ADC12MEM3
        case ADC12IV__ADC12IFG4:   break;   // Vector 20:  ADC12MEM4
        case ADC12IV__ADC12IFG5:   break;   // Vector 22:  ADC12MEM5
        case ADC12IV__ADC12IFG6:   break;   // Vector 24:  ADC12MEM6
        case ADC12IV__ADC12IFG7:   break;   // Vector 26:  ADC12MEM7
        case ADC12IV__ADC12IFG8:   break;   // Vector 28:  ADC12MEM8
        case ADC12IV__ADC12IFG9:   break;   // Vector 30:  ADC12MEM9
        case ADC12IV__ADC12IFG10:  break;   // Vector 32:  ADC12MEM10
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

