# Script Runner test script
cmd("MATLAB COMMAND")
wait_check("MATLAB STATUS BOOL == 'FALSE'", 5)
