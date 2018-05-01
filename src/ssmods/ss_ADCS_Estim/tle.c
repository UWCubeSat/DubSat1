/*
 * tle.c
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 */

#include <string.h>
#include <math.h>

#include "tle.h"
#include "interfaces/canwrap.h"

FILE_STATIC const uint32_t completeSet = CAN_ID_TLE_1
                                       | CAN_ID_TLE_2
                                       | CAN_ID_TLE_3
                                       | CAN_ID_TLE_4
                                       | CAN_ID_TLE_5
                                       | CAN_ID_TLE_6;

void tleInit(struct tle *tle, BOOL isPrepopulated)
{
    if (isPrepopulated)
    {
        tle->_present = completeSet;
        tle->_id = tle->tle1.tle_1_id;
    }
    else
    {
        tle->_present = 0;
    }
}

void tleUpdate(CANPacket *p, struct tle *tle)
{
    /*
     * tleId is a boolean that helps identify new and old TLEs. The ID should
     * flip on each new TLE sent. For some packets the ID is explicit. In others
     * the TLE is encoded as the sign bit of an unsigned floating point number.
     * In this second case `signbit` is used to extract the ID.
     */
    uint8_t tleId;

    switch (p->id)
    {
    case CAN_ID_TLE_1:
        decodetle_1(p, &tle->tle1);
        tleId = tle->tle1.tle_1_id;
        break;
    case CAN_ID_TLE_2:
        decodetle_2(p, &tle->tle2);
        tleId = signbit(tle->tle2.tle_2_day);
        tle->tle2.tle_2_day = fabs(tle->tle2.tle_2_day);
        break;
    case CAN_ID_TLE_3:
        decodetle_3(p, &tle->tle3);
        tleId = signbit(tle->tle3.tle_3_ecc);
        tle->tle3.tle_3_ecc = fabsf(tle->tle3.tle_3_ecc);
        break;
    case CAN_ID_TLE_4:
        decodetle_4(p, &tle->tle4);
        tleId = signbit(tle->tle4.tle_4_aop);
        tle->tle4.tle_4_aop = fabsf(tle->tle4.tle_4_aop);
        break;
    case CAN_ID_TLE_5:
        decodetle_5(p, &tle->tle5);
        tleId = tle->tle5.tle_5_id;
        break;
    case CAN_ID_TLE_6:
        decodetle_6(p, &tle->tle6);
        tleId = signbit(tle->tle6.tle_6_mnm);
        tle->tle6.tle_6_mnm = fabs(tle->tle6.tle_6_mnm);
        break;
    default:
        // ignore non-TLE CAN packets
        return;
    }

    if (tle->_id != tleId)
    {
        // this is the start of a new TLE. Reset the TLE state.
        tle->_present = 0;
    }

    // mark this segment as present
    tle->_present |= p->id;
    tle->_id = tleId;
}

BOOL tleIsComplete(struct tle *tle)
{
    return tle->_present == completeSet;
}
