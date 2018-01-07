/*
 * GPSIDs.h
 *
 *  Created on: Dec 10, 2017
 *      Author: djdup
 */

#ifndef GPSIDS_H_
#define GPSIDS_H_

#define MSGID_RXSTATUS 93
#define MSGID_RXSTATUSEVENT 94
#define MSGID_TIME 101
#define MSGID_BESTXYZ 241
#define MSGID_HWMONITOR 963
#define MSGID_SATVIS2 1043

#define EVTID_POSITION 19
#define EVTID_UTC 18
#define EVTID_CLOCK 22

#define EVTTYPE_CLEAR 0
#define EVTTYPE_SET 1

// see table 97
#define SATSYSTEM_GPS 0
#define SATSYSTEM_GLONASS 1
#define SATSYSTEM_SBAS 2

#define CMDID_STATUSCONFIG 95

#endif /* GPSIDS_H_ */
