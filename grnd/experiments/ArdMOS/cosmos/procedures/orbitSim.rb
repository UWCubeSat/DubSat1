
load "powerControl.rb"


def cycleChannel(up, cycleTime, voltage, resolution)
	
	if up
                    puts("start up cycle")
		for i in 0..resolution
			v=(voltage*i)/resolution
			setVoltage(2, v)
                              wait(cycleTime/(2*resolution))
                              stat = getStatus
                              wait(cycleTime/(2*resolution))
                              #puts "voltage"
                              
                              #puts stat.voltage(2)
                              
			
		end
                    puts("done with up cycle")
	else
                    puts("start down cycle")
		for i in 0..resolution
                              
			v=voltage-(voltage*i)/resolution
                              setVoltage(2, v)
                              wait(cycleTime/(2*resolution))
                              stat = getStatus
                              wait(cycleTime/(2*resolution))
                              
                              #puts "voltage"
                              
                              #puts stat.voltage(2)
		end
                    puts("done with down cycle")	
	end
end

N=0
setVoltage(2, 0.0)
channelOn(2)
puts "start"
for i in 0..10
cycleChannel(true, 5.0, 10, 10)
for i in 0..120*N
	stat = getStatus
	wait(.5)
end

cycleChannel(false, 5.0, 10, 10)
for i in 0..120*N
	stat = getStatus
	wait(.5)
end
end
puts "done"