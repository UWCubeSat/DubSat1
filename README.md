# DubSat1 Repository

This is the University of Washington CubeSat Club's DubSat1
library.

More info about our club can be visited [here](http://earthweb.ess.washington.edu/space-propulsion/cubesat.php)

## Project Structure [INCOMPLETE]
DubSat1 is a repository containing several different projects. Each project's
root directory is in `ss_SUBSYSTEMNAME_optionalsubprojecttitle`. Each project
file then includes the search paths to the msp430 support libraries, the project
root, compiler includes, and dsbase or dsbase-dev. You can find these properties
under project properties -> build -> MPS430 Compiler -> Include Options.

## Contents
### /src
All of the source code pertaining to all the code deployed on the
DubSat1 satellite.

### /ref
Reference material used during the development of our project
