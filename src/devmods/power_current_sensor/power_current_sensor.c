#include <power_current_sensor.h>
FILE_STATIC uint8_t i2cInitialized = 0;

int main(void) {}

void currentSensorInt() {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    if (i2cInitialized != 0)
            return;
}
