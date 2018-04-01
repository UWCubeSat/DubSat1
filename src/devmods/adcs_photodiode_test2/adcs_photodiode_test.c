#define PD_ADDR_LH 0x14
#define PD_ADDR_LF 0x15
#define PD_ADDR_FH 0x17
#define PD_ADDR_FF 0x24
#define PD_ADDR_HH 0x26
#define PD_ADDR_HF 0x27

#define I2C_BUS_PHOTODIODES I2CBus1

#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

#include <msp430.h> 

#include "bsp/bsp.h"

#include "sensors/photodiode.h"
#include "core/i2c.h"

int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

    LED_DIR |= LED_BIT;

    uint8_t pdHandleCenter = photodiodeInit(PD_ADDR_HH, I2C_BUS_PHOTODIODES);
    uint8_t pdHandleRight = photodiodeInit(PD_ADDR_FF, I2C_BUS_PHOTODIODES);
    uint8_t pdHandleLeft = photodiodeInit(PD_ADDR_HF, I2C_BUS_PHOTODIODES);

    uint32_t prev = 0;

    while (1)
    {
        uint32_t center = photodiodeRead(defaultWrite, pdHandleCenter);
        uint32_t right = photodiodeRead(defaultWrite, pdHandleRight);
        uint32_t left = photodiodeRead(defaultWrite, pdHandleLeft);
        if (prev != center)
        {
            LED_OUT ^= LED_BIT;
            debugPrintF("center: %x\r\n", center);
            prev = center;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}
