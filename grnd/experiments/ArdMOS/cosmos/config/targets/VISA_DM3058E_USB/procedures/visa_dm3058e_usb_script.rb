# Script Runner test script
cmd("VISA_DM3058E_USB COMMAND")
wait_check("VISA_DM3058E_USB STATUS BOOL == 'FALSE'", 5)
