# Script Runner test script
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 500, RESETWINDUP TRUE")
wait(20)

3.upto(80) do |i|
  cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}")
  wait(15)
end

