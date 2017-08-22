#include "analogread.h"

/*
 * analogRead
 *
 * Reads the voltage on an analog pin.
 *
 * Input:
 * channel - the pin you want to read. This will have the form
 *           ADC12INCH_x (e.g. to read A5 you'd use ADC12INCH_5)
 *
 * Returns - an integer between 0 and 4096 corresponding
 *           to the read voltage compared to the reference voltage.
 * */
uint16_t analogRead(uint8_t channel){

    ADC12CTL0 &= ~ADC12ENC;                 // disable ADC
    ADC12CTL0 = ADC12ON | ADC12SHT0_4;      // turn ADC ON; sample + hold @ 64 x ADC12CLKs
    ADC12CTL1 = ADC12SSEL_0 | ADC12DIV_4;   // ADC12OSC as ADC12CLK (~5MHz) / 5
    ADC12CTL1 |= ADC12SHP;                  // ADCCLK = MODOSC; sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit resolution
    ADC12IFGR0 = 0;                         // Clear Flags
    ADC12IER0 |= ADC12IE0;                  // Enable interrupts
    while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT

    REFCTL0 = ADC12VRSEL_0;                 //set the reference voltage TODO: let the user specify this
    ADC12MCTL0 = channel | ADC12VRSEL_0; // set channel and reference

    if (REFCTL0 & REFON)
        while(!(REFCTL0 & REFGENRDY));      // wait till ref generator is ready
    ADC12CTL0 |= ADC12ENC | ADC12SC;        // enable ADC and start conversion
    while (ADC12CTL1 & ADC12BUSY) {         // sleep and wait for completion
        __bis_SR_register(CPUOFF + GIE);    // LPM0 with interrupts enabled
    }
    /* POWER: Turn ADC and reference voltage off to conserve power */
    ADC12CTL0 &= ~(ADC12ENC);
    ADC12CTL0 &= ~(ADC12ON);
    REFCTL0 &= ~(REFON);

    return ADC12MEM0;

}

__attribute__((interrupt(ADC12_VECTOR)))
void ADC12_ISR(void)
{
    switch(ADC12IV,12) {
        case  0: break;                          // No interrupt
        case  2: break;                          // conversion result overflow
        case  4: break;                          // conversion time overflow
        case  6: break;                          // ADC12HI
        case  8: break;                          // ADC12LO
        case 10: break;                          // ADC12IN
        case 12:
                 ADC12IFGR0 = 0;                 // Clear Flags
                 __bic_SR_register_on_exit(CPUOFF);        // return to active mode
                 break;                          // Clear CPUOFF bit from 0(SR)
        default: break;
    }

}
