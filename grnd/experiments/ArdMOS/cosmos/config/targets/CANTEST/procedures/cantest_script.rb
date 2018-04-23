# Script Runner test script
cmd("CANTEST COMMAND")
wait_check("CANTEST STATUS BOOL == 'FALSE'", 5)
