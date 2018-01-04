:: Tool for Updating All Flatsat/Satellite Boards with most recent builds
:: General flow:  get most recently built binaries (in TI's .txt hex format);  then run a custom TI Gang Programmer script that
:: loads a series of binaries, one at a time, and deploys it on an assigned output channel (again, one at a time)
::
:: To add a new subsystem module:
::   1)  Add another xcopy line in getbins.bat (use the same command line options, different file location of course)
::   2)  Open the TI Gang Programmer software, place in Interactive Mode, and activate the (single) programmer channel for that subsystem module
::   3)  Change the code file to the uniquely-named .txt file you copied locally
::   4)  Save the project (File-->Save Project As ...) as a config file, and add another F_LOADCFGFILE and F_GO pair of commands
::   Voila!

call getbins.bat
call msp-gang.exe -sf binburn.mspgangsf