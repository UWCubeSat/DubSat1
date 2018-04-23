/*
 * tle.c
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 */

#define MAKE_FLAG16(N) (((uint16_t) 1) << N)
#define TLE_BIT_YEAR  MAKE_FLAG16(0)
#define TLE_BIT_DAY   MAKE_FLAG16(1)
#define TLE_BIT_BSTAR MAKE_FLAG16(2)
#define TLE_BIT_INC   MAKE_FLAG16(3)
#define TLE_BIT_RAAN  MAKE_FLAG16(4)
#define TLE_BIT_ECC   MAKE_FLAG16(5)
#define TLE_BIT_AOP   MAKE_FLAG16(6)
#define TLE_BIT_MNA   MAKE_FLAG16(7)
#define TLE_BIT_MNM   MAKE_FLAG16(8)

#include "tle.h"
#include "interfaces/canwrap.h"
#include "core/met.h"

FILE_STATIC const uint64_t completeSet = TLE_BIT_YEAR
                                       | TLE_BIT_DAY
                                       | TLE_BIT_BSTAR
                                       | TLE_BIT_INC
                                       | TLE_BIT_RAAN
                                       | TLE_BIT_ECC
                                       | TLE_BIT_AOP
                                       | TLE_BIT_MNA
                                       | TLE_BIT_MNM;

FILE_STATIC BOOL isComplete(uint16_t presentSet);
FILE_STATIC BOOL isTimedOut(uint64_t startTime);
FILE_STATIC void resetTLE(struct tle *tle);

void tleInit(struct tle *tle, BOOL isPrepopulated)
{
    if (isPrepopulated)
    {
        tle->_present = completeSet;
        tle->_startTime = getTimeStampInt();
    }
    else
    {
        tle->_present = 0;
    }
}

// TODO should this do something about duplicate fields?
void tleUpdate(CANPacket *p, struct tle *tle)
{
    /*
     * If this is a brand new timer or the last TLE read finished / timed out,
     * reset the TLE.
     */
    if (tle->_present == 0 || isTimedOut(tle->_startTime))
    {
        resetTLE(tle);
    }

    tle_1 tle1;
    tle_2 tle2;
    tle_3 tle3;
    tle_4 tle4;
    tle_5 tle5;
    tle_6 tle6;

    switch (p->id)
    {
    case CAN_ID_TLE_1:
        // if this is the first packet in a new TLE, reset it
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_1(p, &tle1);

        tle->bstar = tle1.tle_1_bstar;
        tle->year = tle1.tle_1_year + 2000;

        tle->_present |= TLE_BIT_BSTAR | TLE_BIT_YEAR;
        break;
    case CAN_ID_TLE_2:
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_2(p, &tle2);

        tle->day = tle2.tle_2_day;

        tle->_present |= TLE_BIT_DAY;
        break;
    case CAN_ID_TLE_3:
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_3(p, &tle3);

        tle->inc = tle3.tle_3_inc;
        tle->raan = tle3.tle_3_raan;

        tle->_present |= TLE_BIT_INC | TLE_BIT_RAAN;
        break;
    case CAN_ID_TLE_4:
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_4(p, &tle4);

        tle->aop = tle4.tle_4_aop;
        tle->ecc = tle4.tle_4_ecc;

        tle->_present |= TLE_BIT_AOP | TLE_BIT_ECC;
        break;
    case CAN_ID_TLE_5:
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_5(p, &tle5);

        tle->mna = tle5.tle_5_mna;

        tle->_present |= TLE_BIT_MNA;
        break;
    case CAN_ID_TLE_6:
        if (isComplete(tle->_present)) resetTLE(tle);

        decodetle_6(p, &tle6);

        tle->mnm = tle6.tle_6_mnm;

        tle->_present |= TLE_BIT_MNM;
        break;
    }
}

BOOL tleIsComplete(struct tle *tle)
{
    return isComplete(tle->_present);
}

FILE_STATIC BOOL isComplete(uint16_t presentSet)
{
    return presentSet == completeSet;
}

FILE_STATIC BOOL isTimedOut(uint64_t startTime)
{
    // this depends on the units of getTimeStampInt being 2^-8 s
    return getTimeStampInt() - startTime > TLE_TIMEOUT_MS * 256000;
}

FILE_STATIC void resetTLE(struct tle *tle)
{
    tle->_present = 0;
    tle->_startTime = getTimeStampInt();
}
