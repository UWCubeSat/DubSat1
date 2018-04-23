# First make sure we are starting clean by double-resetting windup
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 500, RESETWINDUP TRUE")
wait(30)
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 500, RESETWINDUP TRUE")

# Now start ...
loop do 
  5.upto(80) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}")
    wait(5)
  end
  
  79.downto(6) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}")
    wait(5)
  end
end


