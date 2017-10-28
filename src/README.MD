# Top Level Overview of All Subsystem and Core Code

All satellite related code for each system as well as the
code related to development and testing.

## Contents
(Documentation for individual files can be found in the
readme files in the corresponding directories)

### /dsbase
Standardized APIs that other subsystems can use such
as I2C, SPI, and CAN to communicate with other subsystems and
sensors.

### /devmods/[Whatever]
For code that eventually will exist in the context of a subsystem module (meaning it will fly, and be on a MSP43x), but is for testing e.g. a vehicle for testing new dsbase-based libraries.

### /ssmods/ss_[Subsystem]_[Submodule]
Code for each subsystem and submodule if applicable.  Each subfolder under /ssmods maps to an individual MSP43x on the satellite.

### /ssmods/ss_TEST
Specifically for working out template and CCS build issues - do not use as base for new projects until notified.
