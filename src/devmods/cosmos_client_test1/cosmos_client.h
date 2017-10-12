/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef COSMOS_CLIENT_H_
#define COSMOS_CLIENT_H_

#include <stdint.h>

#define MODE_ON 1
#define MODE_OFF 0


#define OPCODE_DELAY 1
#define OPCODE_MODE  2

#define MINDELAY  15

typedef enum _cmdtype {
    CMD_NONE,
    CMD_DELAY,
    CMD_MODE,
} cmdtype;

typedef enum _cmdstate {
    STATE_START,
    STATE_PARAMWAIT,

} cmdstate;

typedef struct _soh_t
{
    uint8_t syncpattern;
    uint8_t length;
    uint8_t id;
    uint8_t delay;
    uint8_t mode;

    uint8_t cmds_accepted;
    uint8_t cmds_received;
} soh_t;

#endif /* COSMOS_CLIENT_H_ */
