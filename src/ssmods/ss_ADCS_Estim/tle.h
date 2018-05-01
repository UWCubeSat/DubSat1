/*
 * tle.h
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 *
 * Reads a TLE from CAN. Each new TLE should flip the ID bit to reset the TLE.
 */

#ifndef TLE_H_
#define TLE_H_

#include <stdint.h>
#include "core/utils.h"
#include "interfaces/canwrap.h"

struct tle {
    tle_1 tle1;
    tle_2 tle2;
    tle_3 tle3;
    tle_4 tle4;
    tle_5 tle5;
    tle_6 tle6;
    uint32_t _present; // bit set for which packets are present
    uint8_t _id;       // the ID matching between each packet
};

void tleInit(struct tle *tle, BOOL isPrepopulated);

/*
 * Processes an incoming CAN packet and writes into the input tle
 */
void tleUpdate(CANPacket *p, struct tle *tle);

BOOL tleIsComplete(struct tle *tle);

#endif /* TLE_H_ */
