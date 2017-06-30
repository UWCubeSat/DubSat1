# Top Level Overview of All Subsystem and Core Code

All satellite related code for each system as well as the
code related to development and testing.

## Contents
(Documentation for individual files can be found in the
readme files in the corresponding directories)

### /dsbase-dev
Code in development that will be moved to dsbase once it
is properly tested. 

### /dsbase
Standardized APIs that other subsystems can use such
as I2C, SPI, and CAN to communicate with other subsystems and
sensors.

### /ssmods/ss_[Subsystem]_[Submodule]
Code for each subsystem and submodule if applicable.  Each subfolder under /ssmods maps to an individual MSP43x on the satellite.

### /ssmods/ss_TEST
Specifically for working out template and CCS build issues - do not use as base for new projects until notified.
