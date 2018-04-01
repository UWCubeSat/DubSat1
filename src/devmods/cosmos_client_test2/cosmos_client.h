#ifndef COSMOS_CLIENT_H_
#define COSMOS_CLIENT_H_

#define MY_TELEM_DISABLED 0
#define MY_TELEM_ENABLED 1

#define TLM_ID_MY_TELEM 2

#define OPCODE_MY_CMD 1

#include <stdint.h>

#include "interfaces/systeminfo.h"
#include "core/utils.h"
#include "core/debugtools.h"

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float alpha;
    float beta;
    uint8_t status;
} my_telem;

CMD_SEGMENT {
    uint8_t enable;
} enable_segment;

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* COSMOS_CLIENT_H_ */
