# First make sure we are starting clean by double-resetting windup
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 500, RESETWINDUP TRUE")
wait(30)
cmd_no_hazardous_check("RWS PIDCTRL with NEWSETPOINT 500, RESETWINDUP TRUE")

# Now start ...
loop do 
  5.upto(80) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}")
    
    1.upto(20) do |i|
      cmd("PS_DP832A GETSTATUS")
      wait(0.25)
    end
  end
  
  79.downto(6) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}")
    
    1.upto(20) do |i|
      cmd("PS_DP832A GETSTATUS")
      wait(0.25)
    end
  end
end


