/*
 * tle.c
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 */

#define TLE_BIT_NONE 1
#define TLE_BIT_1    2
#define TLE_BIT_2    4
#define TLE_BIT_3    8
#define TLE_BIT_4    16
#define TLE_BIT_5    32

#include <string.h>
#include <math.h>

#include "tle.h"
#include "interfaces/canwrap.h"

FILE_STATIC const uint8_t completeSet = TLE_BIT_1
                                      | TLE_BIT_2
                                      | TLE_BIT_3
                                      | TLE_BIT_4
                                      | TLE_BIT_5;

void tleInit(struct tle *tle, BOOL isPrepopulated)
{
    if (isPrepopulated)
    {
        tle->_present = completeSet;
        tle->_prevId = signbit(tle->tle1.tle_1_mna);
    }
    else
    {
        tle->_present = 0;
    }
}

uint8_t canIdToTleBit(uint32_t canId)
{
	uint8_t tleBit;
	switch(canId)
	{
	case CAN_ID_TLE_1:
		tleBit = TLE_BIT_1;
		break;
	case CAN_ID_TLE_2:
		tleBit = TLE_BIT_2;
		break;
	case CAN_ID_TLE_3:
		tleBit = TLE_BIT_3;
		break;
	case CAN_ID_TLE_4:
		tleBit = TLE_BIT_4;
		break;
	case CAN_ID_TLE_5:
		tleBit = TLE_BIT_5;
		break;
	default:
		tleBit = TLE_BIT_NONE;
		break;
	}
	return tleBit;
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

    uint8_t tleBit = canIdToTleBit(p->id);
    switch (tleBit)
    {
    case TLE_BIT_1:
        decodetle_1(p, &tle->tle1);
        tleId = signbit(tle->tle1.tle_1_mna);
        tle->tle1.tle_1_mna = fabsf(tle->tle1.tle_1_mna);
        break;
    case TLE_BIT_2:
        decodetle_2(p, &tle->tle2);
        tleId = signbit(tle->tle2.tle_2_day);
        tle->tle2.tle_2_day = fabs(tle->tle2.tle_2_day);
        break;
    case TLE_BIT_3:
        decodetle_3(p, &tle->tle3);
        tleId = signbit(tle->tle3.tle_3_ecc);
        tle->tle3.tle_3_ecc = fabsf(tle->tle3.tle_3_ecc);
        break;
    case TLE_BIT_4:
        decodetle_4(p, &tle->tle4);
        tleId = signbit(tle->tle4.tle_4_aop);
        tle->tle4.tle_4_aop = fabsf(tle->tle4.tle_4_aop);
        break;
    case TLE_BIT_5:
        decodetle_5(p, &tle->tle5);
        tleId = signbit(tle->tle5.tle_5_mnm);
        tle->tle5.tle_5_mnm = fabs(tle->tle5.tle_5_mnm);
        break;
    default:
        // ignore non-TLE CAN packets
        return;
    }

    if (tle->_id != tleId)
    {
        // this is the start of a new TLE. Reset the TLE state.
        tle->_present = 0;
        tle->_prevId = tle->_id;
    }

    // mark this segment as present
    tle->_present |= tleBit;
    tle->_id = tleId;
}

BOOL tleIsComplete(struct tle *tle)
{
    return tle->_present == completeSet;
}

uint8_t tleAck(struct tle *tle)
{
	uint8_t ack = 0;
	ack |= tle->_prevId;
	ack |= tle->_present & TLE_BIT_1;
	ack |= tle->_present & TLE_BIT_2;
	ack |= tle->_present & TLE_BIT_3;
	ack |= tle->_present & TLE_BIT_4;
	ack |= tle->_present & TLE_BIT_5;
	ack |= tle->_id << 6;
	return ack;
}
