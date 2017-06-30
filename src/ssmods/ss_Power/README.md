# Power Subsystem

This code will drive the MSP430 that the Power Subsystem uses.
## Power distribution System:
```
+---------------------------------+
|   ---[V_batt]--+                |
|                |                |
|                +--/ ----------->|
|                +--/ ----------->|
|                +--/ ----------->|
|                                 |   --> to subsystems
|   ---[3.3V]-------/ ----------->|
|                                 |
|   ---[5V]---------/ ----------->|
|                                 |
+---------------------------------+
```
## TODO
### Logic
* Sync pulses (~2s, ~90s)
* Current monitors will have a set current and voltage limit, flip a switch if
outside limit turning off MPPTs
* Regulate power to subsystems
### Battery Board Control
* Update State of Charge (SOC) every 10 seconds (Only reads in/out amps)
* Log pat are full, reset SOC to 100%
* Log when/#of times battery heater turns on/off and poll current state.
* Log # of tomes the battery balancer turns on.
### Power Generation Board Control
* Current Protection Alerts, Switch things on/off
