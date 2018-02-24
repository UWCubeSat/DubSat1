require 'cosmos'
require 'cosmos/script'
#~cmd("RWS DIRCHANGE")
cmd("RWS PIDCTRL with NEWSETPOINT 200, P 2000, I 200, D 0" )
puts Dir.pwd;
b=0
CSV.foreach("rw.csv") do |row|
  if b == 9
    a = ((row[2].to_i).abs).clamp(200,10000)
    cmd("RWS PIDCTRL with NEWSETPOINT #{a.round}, P 2000, I 200, D 0" )
    wait(0.4) 
  end
  b = b+1
  b= b%10
end
#~loop do 
#~  10.downto(2) do |i|
#~    cmd("RWS PIDCTRL with NEWSETPOINT #{i*100}, P 2000, I 200, D 0" )
#~    cmd("PS_DP832A GETSTATUS")
#~    wait(10);
#~  end
#~  cmd("RWS DIRCHANGE")
#~  wait(10);
#~  2.upto(10) do |i|
#~    cmd("RWS PIDCTL with NEWSETPOINT #{i*100}, P 2000, I 200, D 0" )
#~      cmd("PS_DP832A GETSTATUS")
#~    wait(10);
#~  end
#~  10.downto(2) do |i|
#~    cmd("RWS PIDCTRL with NEWSETPOINT #{i*1000}, P 2000, I 200, D 0" )
#~    cmd("PS_DP832A GETSTATUS")
#~    wait(10);
#~  end
#~  cmd("RWS DIRCHANGE")
#~  wait(10);
#~  2.upto(10) do |i|
#~    cmd("RWS PIDCTRL with NEWSETPOINT #{i*1000}, P 2000, I 200, D 0" )
#~      cmd("PS_DP832A GETSTATUS")
#~    wait(10);
#~  end
#~end
