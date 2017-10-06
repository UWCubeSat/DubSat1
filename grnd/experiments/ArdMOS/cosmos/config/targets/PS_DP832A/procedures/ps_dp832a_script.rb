# Script Runner test script
cmd("PS_DP832A COMMAND")
wait_check("PS_DP832A STATUS BOOL == 'FALSE'", 5)
