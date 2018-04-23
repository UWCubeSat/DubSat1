# Script Runner test script
cmd("EPS_DIST COMMAND")
wait_check("EPS_DIST STATUS BOOL == 'FALSE'", 5)
