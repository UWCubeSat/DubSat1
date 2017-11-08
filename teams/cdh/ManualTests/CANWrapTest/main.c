#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "CanWrap.h"
int main() {
    printf("hello\n");
    BatteryStatus *b = malloc(sizeof(BatteryStatus));
    b -> batteryFullChargeCount = 3;
    b -> batteryTemperature = 15;
    b -> batteryVoltage = 75;
    b -> LowestBatteryVoltage = 901;
    b -> underVoltageEvents = 65535;
    printf("batteryChargeCount: %i\n",
           b -> batteryFullChargeCount
    );
    CANPacket *p = encodeBatteryStatus(b);
    printf("id: %i data: %X, %X, %X, %X, %X, %X, %X, %X\n",
           p -> id,
           p -> data[0],
           p -> data[1],
           p -> data[2],
           p -> data[3],
           p -> data[4],
           p -> data[5],
           p -> data[6],
           p -> data[7]
    );
    BatteryStatus *c = decodeBatteryStatus(p);
    printf("decoded data: \n\tcharge count: %i\n\ttemp: %i\n\tvoltage: %i\n\tlowest v: %i\n\tundervoltage events: %i",
           c -> batteryFullChargeCount,
           c -> batteryTemperature,
           c -> batteryVoltage,
           c -> LowestBatteryVoltage,
           c -> underVoltageEvents
    );
    return 0;
}