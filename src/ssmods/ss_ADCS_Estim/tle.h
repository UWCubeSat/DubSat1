/*
 * tle.h
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 *
 * Reads a TLE from CAN. A TLE is only recognized as complete if the last two
 * packets from each segment match.
 */

#ifndef TLE_H_
#define TLE_H_

#define TLE_TIMEOUT_MS 500

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
    uint8_t _present1;
    uint8_t _present2;
};

void tleInit(struct tle *tle, BOOL isPrepopulated);

/*
 * Processes an incoming CAN packet and writes into the input tle
 */
void tleUpdate(CANPacket *p, struct tle *tle);

BOOL tleIsComplete(struct tle *tle);

#endif /* TLE_H_ */
