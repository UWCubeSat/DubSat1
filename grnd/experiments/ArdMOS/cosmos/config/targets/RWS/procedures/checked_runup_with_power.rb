
cmd("RWS DIRCHANGE")
cmd("RWS PIDCTRL with NEWSETPOINT 200, P 2000, I 200, D 0" )
loop do 
  10.downto(2) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}, P 2000, I 200, D 0" )
#~    cmd("PS_DP832A GETSTATUS")
    wait(10);
  end
  cmd("RWS DIRCHANGE")
  wait(10);
  2.upto(10) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}, P 2000, I 200, D 0" )
#~      cmd("PS_DP832A GETSTATUS")
    wait(10);
  end
  10.downto(2) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*1000}, P 2000, I 200, D 0" )
#~    cmd("PS_DP832A GETSTATUS")
    wait(10);
  end
  cmd("RWS DIRCHANGE")
  wait(10);
  2.upto(10) do |i|
    cmd("RWS PIDCTRL with NEWSETPOINT #{i*1000}, P 2000, I 200, D 0" )
#~      cmd("PS_DP832A GETSTATUS")
    wait(10);
  end
end