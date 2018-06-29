#include <msp430.h>
#include <msp430fr5994.h>
#include <stdint.h>
#include "MET.h"
#include "utils.h"

uint8_t confirmed;
uint8_t isDist;

timeStamp recentTime = {0};

void METInitWithTime(timeStamp t)
{
	RTCCTL0_H = RTCKEY_H;                   // Unlock RTC

	RTCCTL0_L = RTCTEVIE_L;                 // RTC event interrupt enable
	RTCCTL1 = RTCSSEL_2 | RTCTEV_3 | RTCHOLD; // Counter Mode, RTC1PS, 8-bit ovf, 32-bit interrupt
	RTCPS0CTL = RT0PSDIV1 | RT1PSDIV1;                  // ACLK, /8
	RTCPS1CTL = RT1SSEL1 | RT1PSDIV__16; // out from RT0PS, /16; increment ~= 4 ms

	RTCCNT1 = t.count1;
	RTCCNT2 = t.count2;
	RTCCNT3 = t.count3;
	RTCCNT4 = t.count4;
	recentTime = t;

	isDist = 1;
	confirmed = 1;

	RTCCTL13 &= ~(RTCHOLD);                 // Start RTC
}

void METInit()
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

	confirmed = 0;
	isDist = 0;

	RTCCTL13 &= ~(RTCHOLD);                 // Start RTC
}

uint32_t getMETPrimary()
{
    timeStamp t = getMETTimestamp();
    uint32_t res = (uint32_t) t.count1;
    res |= ((uint32_t) t.count2) << 8;
    res |= ((uint32_t) t.count3) << 16;
    res |= ((uint32_t) t.count4) << 24;
    return res;
}

uint8_t getMETOverflow()
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

timeStamp getMETTimestamp()
{
	timeStamp now = {0};
	safeRead(RTCCNT1, now.count1);
	safeRead(RTCCNT2, now.count2);
	safeRead(RTCCNT3, now.count3);
	safeRead(RTCCNT4, now.count4);
	now.count5 |= recentTime.count5;

	return now;
}

uint64_t metConvertToInt(timeStamp t)
{
    uint64_t res = (uint64_t) t.count1;
    res |= ((uint64_t) t.count2) << 8;
    res |= ((uint64_t) t.count3) << 16;
    res |= ((uint64_t) t.count4) << 24;
    res |= ((uint64_t) t.count5) << 32;
    return res;
}

uint32_t metConvertToSeconds(timeStamp t)
{
    return metConvertFromIntToSeconds(metConvertToInt(t));
}

uint32_t metConvertFromIntToSeconds(int64_t t)
{
    return (uint32_t)(t >> 8);
}

void metFromInt(int64_t t, uint32_t *primary, uint8_t *overflow)
{
    *primary = t;
    *overflow = t >> 32;
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
            //RTCCNT1 not necessary b/c always 0
            safeRead(RTCCNT2, recentTime.count2);
            safeRead(RTCCNT3, recentTime.count3);
            safeRead(RTCCNT4, recentTime.count4);
            if (!(recentTime.count2 | recentTime.count3 | recentTime.count4))
                recentTime.count5++;
            break;

        default: break;
    }
}
