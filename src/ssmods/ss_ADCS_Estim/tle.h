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
    uint8_t _present; // bit set for which packets are present
    uint8_t _id;      // the ID matching between each packet
    uint8_t _prevId;  // the ID of the last complete packet
};

void tleInit(struct tle *tle, BOOL isPrepopulated);

/*
 * Processes an incoming CAN packet and writes into the input tle
 */
void tleUpdate(CANPacket *p, struct tle *tle);

BOOL tleIsComplete(struct tle *tle);

/**
 * Returns an acknowledgment byte that encodes the status of reading a TLE.
 *
 * Starting from the least significant bit:
 *
 * 0: ID of the current TLE
 * 1: tle1 packet received (waiting for other packets to come in)
 * 2: tle2 packet received
 * 3: tle3 packet received
 * 4: tle4 packet received
 * 5: tle5 packet received
 * 6: ID of the incoming TLE
 * 7: No meaning
 */
uint8_t tleAck(struct tle *tle);

/**
 * Mean anomaly (degrees)
 */
inline float tleMna(struct tle *tle) { return tle->tle1.tle_1_mna; }

/**
 * B-star drag term
 */
inline float tleBStar(struct tle *tle) { return tle->tle1.tle_1_bstar; }

/**
 * J2000 epoch, days
 */
inline double tleDay(struct tle *tle) { return tle->tle2.tle_2_day; }

/**
 * Eccentricty
 */
inline float tleEcc(struct tle *tle) { return tle->tle3.tle_3_ecc; }

/**
 * Inclination (degrees)
 */
inline float tleInc(struct tle *tle) { return tle->tle3.tle_3_inc; }

/**
 * Argument of perigee (degrees)
 */
inline float tleAop(struct tle *tle) { return tle->tle4.tle_4_aop; }

/**
 * Right ascension of the ascending node (degrees)
 */
inline float tleRaan(struct tle *tle) { return tle->tle4.tle_4_raan; }

/**
 * Mean motion (revolutions per day)
 */
inline double tleMnm(struct tle *tle) { return tle->tle5.tle_5_mnm; }

inline uint8_t tleId(struct tle *tle) { return tle->_id; }

inline uint8_t tleLastId(struct tle *tle) { return tle->_prevId; }

#endif /* TLE_H_ */
