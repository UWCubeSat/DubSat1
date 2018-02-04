# First make sure we are starting clean by double-resetting windup
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 1500, RESETWINDUP TRUE")
wait(30)
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 1500, RESETWINDUP TRUE")

# Now start ...
loop do 
  3.upto(12) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
        cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")

#~    1.upto(20) do |i|
#~      cmd("PS_DP832A GETSTATUS")
    wait(4)
#~    end
  end
  
  11.downto(3) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
        cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*500}")

#~    1.upto(20) do |i|
#~      cmd("PS_DP832A GETSTATUS")
    wait(4)
#~    end
  end
end

#~loop do 
#~  cmd("PS_DP832A GETSTATUS")
#~
#~end
  