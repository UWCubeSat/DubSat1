/*
 * tle.c
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 */

#define MAKE_FLAG(N) (((uint8_t) 1) << N)
#define TLE_BIT_1 MAKE_FLAG(0)
#define TLE_BIT_2 MAKE_FLAG(1)
#define TLE_BIT_3 MAKE_FLAG(2)
#define TLE_BIT_4 MAKE_FLAG(3)
#define TLE_BIT_5 MAKE_FLAG(4)
#define TLE_BIT_6 MAKE_FLAG(5)

#include <string.h>

#include "tle.h"
#include "interfaces/canwrap.h"

FILE_STATIC const uint8_t completeSet = TLE_BIT_1
                                      | TLE_BIT_2
                                      | TLE_BIT_3
                                      | TLE_BIT_4
                                      | TLE_BIT_5
                                      | TLE_BIT_6;

void tleInit(struct tle *tle, BOOL isPrepopulated)
{
    if (isPrepopulated)
    {
        tle->_present1 = completeSet;
        tle->_present2 = completeSet;
    }
    else
    {
        tle->_present1 = 0;
        tle->_present2 = 0;
    }
}

FILE_STATIC void updateSegment(struct tle *tle, uint8_t bit, uint8_t *storedTLE,
                               uint8_t *canTLE, uint8_t size)
{
    // check if this TLE segment has been seen already
    if (tle->_present1 | bit)
    {
        // check if this TLE segment matches the last one recorded
        if (memcmp(storedTLE, canTLE, size) == 0)
        {
            // if they match, indicate the second pass and return
            tle->_present2 |= bit;
            return;
        }
        else
        {
            // if they don't match, this is a brand new TLE. Reset the entire
            // TLE state and allow the new segment to be copied in
            tle->_present1 = 0;
            tle->_present2 = 0;
        }
    }

    // mark this segment as present and save the data
    tle->_present1 |= bit;
    memcpy(storedTLE, canTLE, size);
}

void tleUpdate(CANPacket *p, struct tle *tle)
{
    tle_1 tle1;
    tle_2 tle2;
    tle_3 tle3;
    tle_4 tle4;
    tle_5 tle5;
    tle_6 tle6;

    switch (p->id)
    {
    case CAN_ID_TLE_1:
        decodetle_1(p, &tle1);
        updateSegment(tle, TLE_BIT_1, (uint8_t *) &tle->tle1, (uint8_t *) &tle1, sizeof(tle1));
        break;
    case CAN_ID_TLE_2:
        decodetle_2(p, &tle2);
        updateSegment(tle, TLE_BIT_2, (uint8_t *) &tle->tle2, (uint8_t *) &tle2, sizeof(tle2));
        break;
    case CAN_ID_TLE_3:
        decodetle_3(p, &tle3);
        updateSegment(tle, TLE_BIT_3, (uint8_t *) &tle->tle3, (uint8_t *) &tle3, sizeof(tle3));
        break;
    case CAN_ID_TLE_4:
        decodetle_4(p, &tle4);
        updateSegment(tle, TLE_BIT_4, (uint8_t *) &tle->tle4, (uint8_t *) &tle4, sizeof(tle4));
        break;
    case CAN_ID_TLE_5:
        decodetle_5(p, &tle5);
        updateSegment(tle, TLE_BIT_5, (uint8_t *) &tle->tle5, (uint8_t *) &tle5, sizeof(tle5));
        break;
    case CAN_ID_TLE_6:
        decodetle_6(p, &tle6);
        updateSegment(tle, TLE_BIT_6, (uint8_t *) &tle->tle6, (uint8_t *) &tle6, sizeof(tle6));
        break;
    }
}

BOOL tleIsComplete(struct tle *tle)
{
    // only recognize it as complete if each segment has been seen twice
    return tle->_present1 == completeSet && tle->_present2 == completeSet;
}
