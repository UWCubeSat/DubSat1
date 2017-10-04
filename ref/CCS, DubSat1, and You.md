# Code Composer Studio, DubSat1, and You

This document is designed to capture notes about how best to setup Code Composer Studio to work the DubSat1 code.  Some of these notes are just suggestions, while others are hard-and-fast rules that need to be adopted to preserve everyone's sanity. 

## Structure of a Subsystem Module

It is vital that all subsystem modules are built with as many of the same settings and structural properties as possible. The folder called ss_Test in the ../src/ssmods folder captures the currently understood best-practices.

#### Core principles:
1.  The most important point is to NEVER HAVE ANY ABSOLUTE  PATHS in any of the project assets, so that anyone can pull the repo and build (assuming their toolsets are configured properly).
2.  To preserve our sanity, EACH FOLDER UNDER SSMODS represents the code that a single MSP43x will execute on the satellite.  No extra folders should be added under ssmods, nor should there be multiple projects under folders.
3.  Certain key predefined symbols MUST BE SET IN ORDER FOR THE PROJECT TO BE BUILT CORRECTLY.

As a result of these principles, the following needs to be kept in mind as individual subsystem modules are developed:
- Never check-in any files that describe your local CCS/Eclipse workspace, these are NOT machine indepedent (i.e. they have absolute paths)
- However you choose to setup your workspace, you must set things up such that the file structure of the overall DubSat1 project matches that in the repo.  
- Never copy files from someone in the repo to anywhere else, even if you think it's "just in this workspace" or project - nobody will be happy with the results

There are several clever CCS features, along with key settings, that help enable this in CCS.  Here is how to use those features to set things up properly:
- "Linked Resources":  this is the CCS way of creating a path variable that you can use in other toolset configuration places, making it easy to point CCS at the right place.  You access this by right-clicking into Properties from your project in Project Explorer (be sure NOT to click on some file underneath, CCS allows to set some settings on individual files, which can lead to heartbreak), and going to Resource->Linked Resources, on the Path Variables tab.  Create a New... one called 'dsbase', and BASE IT ON THE PROJECT LOCATION VARIABLE (use the Variable... button on the subdialog).  This should then give you a variable with name 'DSBASE' with value ${PROJECT_LOC}\..\..\dsbase
- Next, you must add the dsbase files to the include path.  The way to do this is with the DSBASE variable you just created.  Again, in Project-->Properties (again, from the project, not a file within the project), go to Build (or CCS Build, if you have "Advanced Settings" shown)-->MSP430 Compiler-->Include Options, and click the little green plus to add a new include folder.  Click Variables ... on the dialog that pops up to select the DSBASE variable you created in the last step.  Now you should be able include the right dsbase header files by using the format #include "folder/header.h"
- The last step is a bit cumbersome, but it's required to get the source (rather than just header) files to build within the dsbase folder structure, as well as the subsystem-specific code.  To do this, I added a folder in the project simply called dsbase.  Into THAT folder, I add several more folders - but those folders aren't normal folders.  Instead, we'll be using virtual folders that redirect to the dsbase folder structure.  To do this, right-click on the 'dsbase' folder you added to your project and select New-->Folder.  In the New Folder dialog, click Advanced... down at the bottom, select the last radio button option for Link to Alternate Location, then in the textbox below that you'll put the RELATIVE path to the dsbase source files.  You MUST use the DSBASE variable again to do this (hit the "Variables..." button), otherwise absolute paths will work their way in.  Unfortunately, using this technique means you have to add each of the separate dsbase folders to the dsbase folder (currently, that is 5:  bsp, config, core, sensors, and interfaces).  If you can figure out how to make it work without doing that, have at, and share how you did it!

## Subsystem Module Predefined Symbols

Last but not least, to make a project that will actually build correctly, there is a growing list of predefined symbols that need to be, well, defined for things to build and function properly.  Again, go to the Project Properties-->Build-->MSP430 Compiler-->Predefined Symbols.  Hit the green plus button in the Pre-Define NAME box to add whatever is missing.  Note that the symbols you add ONLY apply to the configuration chosen at the top of the Properties page, so make sure the right one is selected for what you're trying to achieve.

Important symbols include (note:  underbars are DOUBLED (i.e. '__') for the beginning and end of most of these):

* `__DEBUG__`   :  Should be set for debug builds.  All the debug printing and other infrastructure is keyed off of this
* `__INITIAL_TRACE_LEVEL__`   :  Optional, defaults to 0 if not specified, and to 1 in standard templates.  Allows you to immediately pick up trace statements without having to change trace levels at startup.  
* `__INITIAL_DEBUG_MODE__`   :   Optional, defaults to 0 if not specified.  0=interactive ASCII console, 1="headless" ASCII mode, 2=COSMOS-compatible binary telemetry mode
* `__DEBUG_UART_SPEED_<spd>__`  :  Optional, <spd> can be "9600", "38400", or "115200" - defaults to 115200.

* Board selection (ONE must be selected - it will help the dsbase infrastructure choose the right pins, and make other assumptions):
    * `__BSP_Board_MSP430FR5994LaunchPad__`:  The standard MSP430FR5994 LaunchPad board used for testing.
    * `__BSP_Board_MSP430FR5994_CANMSPBlockv1__`:  New (as of 5/12/17) boards for testing custom boards.``
	* `__BSP_Board_MSP430FR5994_CANMSPBlockv24__`:  Current test boards, circa September, 2017.
    * TODO:  MSP432 symbol will be needed very shortly

* Other hardware selection (optional, if a given piece of hardware is used - i.e. if the header is included and the define isn't included, you get a compile error):

    * Magnetometer sensor (for magnetometer.c/.h):
        * `__BSP_HW_MAGTOM_HMC5883L__`:  Honeywell HMC 5883L magnetometer
        * `__BSP_HW_MAGTOM_MAG3110__`:  Freescale MAG3110
        * TODO:  the closely related HMC5983L will likely get its own definition and code chunks ... 
    * IMU sensor (for imu.c/.h):
        * `__BSP_HW_IMU_BMI160__`:  Bosch BMI160 IMU (default if no imu symbol defined)

## Workspace Creation Strategy

I have to say that I'm still working out the best way to do this.  Due to the way workspaces are structured in CCS, all the projects need to exist in a flat structure "across" a workspace directory.  With that requirement, you are then forced to have a workspace on your machine (and only yours - remember, never check-in any workspace stuff, they seem to always have absolute paths in them!) that "straddles" the whole ssmods directory, and something ELSE to get to the dsbase files (e.g. the stuff over in core, interfaces, etc.).  Now, it turns out once you've setup the subsystem module project in ssmods per the notes given above, you will have virtual links to the dsbase content in the ssmods-based project, and double-clicking and editing a dsbase file from within the ssmods-based project WILL OPEN AND EDIT THE "REAL" DSBASE FILE.  So, as long as this doesn't confuse you too much and doesn't blow your mind, it's definitely one way to proceed.  Unfortunately, if you have test code that is not part of ssmods (and test code should NOT be part of it), you will need to be operating out of a different set of folders, and therefore a different workspace.  Fortunately, Code Composer Studio supports multi-instancing, and each instance can have (and in fact, MUST have) a different workspace open.  I typically go this route.  Again, though, just a reminder:  DO NOT CHECK-IN WORKSPACE FILES INTO THE REPO.  Violators will be shot.

		




