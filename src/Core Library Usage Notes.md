# Core Library Usage Notes
This document is a place to record any important notes to others about how core (i.e. anything in dsbase) should or should not be used, coding patterns that are important to follow, etc.  Ideally, API information will eventually be moved to design capture documents.

Important coding patterns
-------------------------
There are a number of coding patterns that have been, or are in the process of being established.  At the simplest level, if you find you are doing something that you suspect most or all subsystems or CDH team developers would need, at the very least there's a good chance somebody has already taken a stab at coming up with the pattern, and in a growing number of cases, significant amounts of code have been written assuming everyone will follow the pattern.  So search this document and look around in the code base before committing to a potentially different route from everybody else.

Examples include:
- Subsystem initialization:  ALL subsystem modules should IMMEDIATELY call bpsInit(Module_subsystem_modulename) at the start of main - this does a LOT of configuration, including slapping down the watchdog timer and setting up the debug console apparatus, so don't forget!
- Config pattern (see below for details) - learn it, live it, love it, suggestions for improvement always welcome
- status tracking pattern (see UART notes for details) - not fully fleshed out yet, but will be in sprint 2, I suspect
- Use of FILE_STATIC for static usage on what would otherwise be global-scoped variables (using a macro so that unit tests can define it away and have access to private state, if so desired) - again, see uart.c/.h for examples of how this works.



Debug Console/Functionality
---------------------------
In general, the functions in debugtools.c/.h should be used for making your code more easily (and consistently) debuggable.  debugPrint/debugPrintF always output your text, while debugTrace/debugTraceF does so with a given "priority"/trace level, which is settable from the console (see below).  In general, once a feature/component has been added to the repo and is "done" (at least a v1 version), the only debugPrint's should be those directly supporting debug console commands (i.e. if the console "asks" your component to debugPrintF your status).  Otherwise, the trace functions are more appropriate.  Note that the intention is that on Release builds, ALL of the print/trace functions will compile away to nothing (including the string construction buffers ... this is why sprintf is wrapped by these functions).

Various notes:
- Added DebugPrintF, which does better job of compiling away to nothing on release builds (including text buffers)
- Added DebugTrace (F and non-F versions):  these take a "trace level" (integer 0-3) to indicate how much trace information should be shared over the debug interface
	- 0:  SHOULDN'T USE THIS UNLESS IT'S AN EMERGENCY MESSAGE THAT MUST GET OUT (like brown-out imminent, out of memory)
	- 1:  should be used for one-shot (initialization) or very low-volume but important events
	- 2:  low volume events of moderate importance
	- 3:  higher volume 
	- 4:  highest volume events at the SUBSYSTEM level
	- 5:  highest volume events at the BUS/CORE level (except for the debug/UART system, else it'd be tracing itself!)
- Once a subsytem module (or any code that has run bspInit or debugInit), the UART is claimed by the debug console, and will listen for commands
- Hitting 'enter' by itself will display the prompt
- Use 't<num>' to set the trace level.  Once tracing has begun (e.g. text is scrolling rapidly along), exit out with another hit of 'enter' (same as setting 't0')
- Use 'i' to get a report dump from every entity - will eventually add "levels" to this, maybe, or a way to target the command?



Other Usage Notes
-----------------

### General UART usage:
- In general, use the stuff in debugtools.h/.c instead of the uart.c/.h interface directly, there's support there, and more support planned
- Bumped default speed from 9600bps to max for 8Mhz, which is 115.2kbps ... but if you find stability starts to suffer, please let me know
- Expanded default and supported tx/rx buffer sizes (8- --> 16-bit indexers now)
- Fixed my original lame in-use status tracking mechanism
- All interesting status being stored in a struct

### Known issues:
- for some reason, there is quite a bit of latency in the UART code stack - Jeff is tracking this down (TODO)
- the current implementation throws away in UART write requests that occur while a write is already in progress - eventually this needs to be fixed by adding it to the transmit buffer instead (TODO)

### Status storage/sharing patterns:
- The idea is that each entity - a bus, for instance - will have a relatively-consistent way of storing relevant status in a structure or set of structures
- Each entity will also implement a small set of status accessors that will allow the debug console to get full dumps of all this info
- Note that these same structs are what store data that will be shared in the satellite-wide CAN packets, but due to limited space, that data will only be a subset of the stored information
- Eventually (aka soon aka probably sprint 2) we'll add a "pluggable" mechanism in so that entities can "register" with the debug console, should make things cleaner.  Nothing fancy required, just an array of function pointers that get called in sequence by the debug console command processor, each invoked function would simply debugPrintF their spew (either with fields, if interactive mode, or without fields if headless).  Presto, you're done.

### New simple configuration pattern:
- Created new folder and file set for storing configuration in orderly way, setting us up for automatic config file generation
- See ../core/uart.c and .h, and ../config/* for a demonstration of how to use
- Currently, there are three LEVELS of config, with an implied 4th:
	1)  Macros:  (CONFIGM...) - these are read-only, but because they are to be used in other C initialization (classic case:  buffer sizes) they can't be variables, event const ones
	2)  Read only:  (CONFIGRO...) - these are const variables, specified and fixed at build time
	3)  Dynamic/configurable:  (CONFIGRW...) - for values we decide we want to be able to change on-orbit
	4)  Everything else:  settings that are TRULY not that interesting to tweak in the future can still be set in the normal header file for a source file ... but these should be kept to a minimum
- To add configuration for a new area:
	1)  Add a new config_your_thing.h to the config folder, and add an #include to that in the config/config.h file (central header)
	2)  Add CONFIGM's definitions to your .h new config_your_thing.h file (see examples in UART)
	3)  Add CONFIGRO's and CONFIGRW's in both the config_your_thing.h header (declared with extern, and const if CONFIGRO), and ALSO in a new config_your_thing.c source file for the actual definition that sets up the storage.  Again, see examples in UART.
	
