/*
 * tle.h
 *
 *  Created on: Apr 21, 2018
 *      Author: djdup
 *
 * Reads a TLE from CAN. Prevents reading half-baked TLEs.
 */

#ifndef TLE_H_
#define TLE_H_

#define TLE_TIMEOUT_MS 500

#include <stdint.h>
#include "core/utils.h"
#include "interfaces/canwrap.h"

struct tle {
    uint16_t year;
    double day;
    float bstar;
    float inc;
    float raan;
    float ecc;
    float aop;
    float mna;
    double mnm;
    uint16_t _present;
    uint64_t _startTime;
};

void tleInit(struct tle *tle, BOOL isPrepopulated);

/*
 * Processes an incoming CAN packet and writes into the input tle
 */
void tleUpdate(CANPacket *p, struct tle *tle);

BOOL tleIsComplete(struct tle *tle);

#endif /* TLE_H_ */
