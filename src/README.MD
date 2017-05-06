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

### /ss_[Subsystem]_[Submodule]
Code for each subsystem and submodule if applicable.

### /ss_TEST
Test code for any of the components above.
