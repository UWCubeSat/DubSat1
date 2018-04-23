:: Tool for Updating All Flatsat/Satellite Boards with most recent builds
:: General flow:  get most recently built binaries (in TI's .txt hex format);  then run a custom TI Gang Programmer script that
:: loads a series of binaries, one at a time, and deploys it on an assigned output channel (again, one at a time)
::
:: To add a new subsystem module:
::   1)  Add another xcopy line in getbins.bat (use the same command line options, different file location of course)
::   2)  Open the TI Gang Programmer software, place in Interactive Mode, and activate the (single) programmer channel for that subsystem module
::   3)  Change the code file to the uniquely-named .txt file you copied locally
::   4)  Save the project (File-->Save Project As ...) as a config file, and add another F_LOADCFGFILE and F_GO pair of commands
::   5)  NOTE:  because the Gang programmer is a buggy mess, you MUST choose gang 1 or gang 2 and put the entries in the correct script and project file ... sorry.  :(
::   This is because you can't reliably change from one programmer to another (between COM ports, basically) within one script, the changeover seems to block the normal reads that have to happen
::   so only the part of the script that matches what COM port was "loaded"/active when the programmer software was last closed works.  Very annoying to debug.
::   Voila!

call msp-gang.exe -prj dep_switchtoGANG1.mspgangproj -sf binburn-GANG1.mspgangsf
call msp-gang.exe -prj dep_switchtoGANG2.mspgangproj -sf binburn-GANG2.mspgangsf

