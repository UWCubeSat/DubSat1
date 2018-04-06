#include <msp430.h>
#include <msp430fr5994.h>
#include <stdint.h>
#include "MET.h"

uint8_t overflowReg;

uint8_t confirmed;

void METInit()
{
	RTCCTL0_H = RTCKEY_H;                   // Unlock RTC

	RTCCTL0_L = RTCTEVIE_L;                 // RTC event interrupt enable
	RTCCTL13 = RTCSSEL_2 | RTCTEV_0 | RTCHOLD; // Counter Mode, RTC1PS, 8-bit ovf
	RTCPS0CTL = RT0PSDIV1;                  // ACLK, /8
	RTCPS1CTL = RT1SSEL1 | 0x3000; // out from RT0PS, /128; increment ~= 8ms (

	overflowReg = 0;

	RTCCTL1 = RTCTEV_3; //32-bit interrupt

	RTCCNT1 = 0;
	RTCCNT2 = 0;
	RTCCNT3 = 0;
	RTCCNT4 = 0;

	RTCCTL13 &= ~(RTCHOLD);                 // Start RTC

	confirmed = 0;

}

void updateMET(timeStamp newTime)
{
    RTCCTL13 |= RTCHOLD;
    RTCCNT1 = newTime.count1;
    RTCCNT2 = newTime.count2;
    RTCCNT3 = newTime.count3;
    RTCCNT4 = newTime.count4;
    overflowReg = newTime.count5;
    RTCCTL13 &= ~(RTCHOLD);

    confirmed = 1;
}

timeStamp getTimeStamp()
{
	timeStamp now = {0};
	now.count1 |= RTCCNT1; //TODO: these reads are unpredictable
	now.count2 |= RTCCNT2;
	now.count3 |= RTCCNT3;
	now.count4 |= RTCCNT4;
	now.count5 |= overflowReg;

	return now;
}
#pragma vector=RTC_C_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV__RT1PSIFG))
    {
        case RTCIV__RTCTEVIFG: // RTCEVIFG
            overflowReg++;
            break;

        default: break;
    }
}
