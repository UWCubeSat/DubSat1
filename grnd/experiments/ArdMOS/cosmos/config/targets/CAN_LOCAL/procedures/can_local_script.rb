# Script Runner test script
cmd("CAN_LOCAL COMMAND")
wait_check("CAN_LOCAL STATUS BOOL == 'FALSE'", 5)
