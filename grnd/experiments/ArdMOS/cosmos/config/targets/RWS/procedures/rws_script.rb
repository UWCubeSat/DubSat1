# Script Runner test script
cmd("RWS COMMAND")
wait_check("RWS STATUS BOOL == 'FALSE'", 5)
