#include <msp430.h>
#include <msp430fr5994.h>
#include <stdint.h>
#include "MET.h"

uint8_t confirmed;

timeStamp recentTime = {0};
#pragma PERSISTENT(recentTime)

uint8_t isDist;

void METInit(uint8_t _isDist)
{
	RTCCTL0_H = RTCKEY_H;                   // Unlock RTC

	RTCCTL0_L = RTCTEVIE_L;                 // RTC event interrupt enable
	RTCCTL1 = RTCSSEL_2 | RTCTEV_3 | RTCHOLD; // Counter Mode, RTC1PS, 8-bit ovf, 32-bit interrupt
	RTCPS0CTL = RT0PSDIV1 | RT1PSDIV1;                  // ACLK, /8
	RTCPS1CTL = RT1SSEL1 | RT1PSDIV__16; // out from RT0PS, /16; increment ~= 4 ms

	RTCCNT1 = 0;
	RTCCNT2 = 0;
	RTCCNT3 = 0;
	RTCCNT4 = 0;

	RTCCTL13 &= ~(RTCHOLD);                 // Start RTC

	isDist = _isDist;
	if(isDist)
	{
	    RTCCNT1 = recentTime.count1;
        RTCCNT2 = recentTime.count2;
        RTCCNT3 = recentTime.count3;
        RTCCNT4 = recentTime.count4;
	    //TODO: load time from memory
	    confirmed = 1;
	}
	else
	    confirmed = 0;
}

uint32_t getPrimaryTime()
{
    timeStamp t = getTimeStamp();
    uint32_t res = (uint32_t) RTCCNT1;
    res |= ((uint32_t) RTCCNT2) << 8;
    res |= ((uint32_t) RTCCNT3) << 16;
    res |= ((uint32_t) RTCCNT4) << 24;
    return res;
}

uint8_t getOverflowTime()
{
    return recentTime.count5;
}

void updateMET(timeStamp newTime)
{
    RTCCTL13 |= RTCHOLD;
    RTCCNT1 = newTime.count1;
    RTCCNT2 = newTime.count2;
    RTCCNT3 = newTime.count3;
    RTCCNT4 = newTime.count4;
    recentTime.count5 = newTime.count5;
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
	now.count5 |= recentTime.count5;

	return now;
}

uint64_t getTimeStampInt()
{
    timeStamp t = getTimeStamp();
    uint64_t res = (uint64_t) t.count1;
    res |= ((uint64_t) t.count2) << 8;
    res |= ((uint64_t) t.count3) << 16;
    res |= ((uint64_t) t.count4) << 24;
    res |= ((uint64_t) t.count5) << 32;
    return res;
}

double getTimeStampSeconds()
{
    return ((double) getTimeStampInt()) / 256.0;
}

timeStamp constructTimestamp(uint32_t primary, uint8_t overflow)
{
    timeStamp newTime = {0};
    newTime.count1 = primary & 0xff;
    newTime.count2 = (primary >> 8) & 0xff;
    newTime.count3 = (primary >> 16) & 0xff;
    newTime.count4 = (primary >> 24) & 0xff;
    newTime.count5 = overflow;
    return newTime;
}

#pragma vector=RTC_C_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV__RT1PSIFG))
    {
        case RTCIV__RTCTEVIFG: // RTCEVIFG
            if(isDist)
                //RTCCNT1 not necessary b/c always 0
                recentTime.count2 = RTCCNT2;
                recentTime.count3 = RTCCNT3;
                recentTime.count4 = RTCCNT4;
                recentTime.count5 = recentTime.count5;
                if (!(RTCCNT1 | RTCCNT2 | RTCCNT3 | RTCCNT4))
                    recentTime.count5++;
            else
                recentTime.count5++;
            break;

        default: break;
    }
}
