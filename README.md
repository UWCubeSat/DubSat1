# DubSat1 Repository

This is the University of Washington CubeSat Team's DubSat1
library.

More info about our club can be visited [here](http://earthweb.ess.washington.edu/space-propulsion/cubesat.php)

## Repository Contents
DubSat1 is a repository containing configuration-managed assets for the University of Washington's Dubsat-1 (DS-1) satellite, scheduled to fly at the end of the summer 2018.  Assets included here include source code for all subsystems, PCB and schematic design files, and physical design and analysis assets.  

## Repository Branches
The following branches are maintained in the DubSat1 project.  Please follow the guidelines provided.
* master:  Should ALWAYS be kept fully-functional, and ideally tagged to reflect completed milestones/sprint payloads, etc.  *NO CHECKINS SHOULD BE MADE DIRECTLY INTO THIS BRANCH* except by integration managers.
* develop:  Most work should be either committed to this branch, or a feature/* branch based off of develop (see below).  Non-coding work (e.g. PCB board design or SolidWORKS iterations) can go into this branch directly.
* feature/<featurename>:  Most significant chunks of coding work should be done in small, separate feature branches.  Feel free to create these as needed, but do name them using the indicated scheme e.g. "feature/I2Cv2".  However, *BE SURE TO BASE NEW FEATURE BRANCHES OFF OF DEVELOP* and not the master branch.  This helps quite a bit in certain merge scenarios down the road.

## Repository Directories
### /src
Contains the source code designed to run on prototype, test, or flight hardware.  Sub-directories include:

* /src/dsbase:	Here are the core satellite base libraries, used to wrap access to common functionality across the spacecraft, including SPI, I2C and CAN bus access, debug functionality, and general board support packages.
* /src/ssmods:	Standing for "sub-system modules", it is a hard requirement that each subdirectory under ssmods corresponds to one executable unit, to be deployed on one compute element (generally either the MSP430 or MSP432) on the spacecraft, and only the machine-independent files should be saved here (though the .gitignore file SHOULD be setup to screen out most commonly occurring machine-specific build artifacts that we don't want to store).  Anything that doesn't fit this description should go elsewhere.
* /src/devmods:	Used to store experimental work for prototypes or test hardware that is "close" to being moved over to a "real" subsystem module, sort of as a staging ground.  Projects under this folder should also be MSP43x-based, as well.
* /src/dsbase-dev:  GOING AWAY VERY SOON, JeffC will coordinate with folks that typically write to this folder to make the transition to the new branch-based model sometime later in sprint 3.

### /ref
Reference material that is dynamic, including current pin assignments, microcontroller peripheral instance usage, libray usage notes, and Code Composer Studio setup requirements.  Static content - e.g. product datasheets - should be stashed on the team wiki instead.

### /teams
This set of folders is for individual teams to store whatever other assets they have that a) need any level of change tracking, or b) general coordination between multiple contributors, and c) aren't massive.  As a point of reference, each Git repo needs to stay under about 1GB, so if you are routinely checking in 25MB file chunks, please contact the CDH team for alternative arrangements - it can really add up.



