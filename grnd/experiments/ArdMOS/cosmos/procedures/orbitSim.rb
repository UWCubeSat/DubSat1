
load "powerControl.rb"

inSun=ask("Time spent in the sun")
inEclipse=("Time spent in eclipse")
numberOfOrbits=("Number of orbits")

def cycleChannel(up, cycleTime, voltage, resolution)
	
	if up
                    puts("start up cycle")
		for i in 0..resolution
			v=(voltage*i)/resolution
			setVoltage(1, v)
                              wait(cycleTime/(2*resolution))
                              stat = getStatus
                              wait(cycleTime/(2*resolution))
		end
                    puts("done with up cycle")
	else
                    puts("start down cycle")
		for i in 0..resolution
                              
			v=voltage-(voltage*i)/resolution
                              setVoltage(1, v)
                              wait(cycleTime/(2*resolution))
                              stat = getStatus
                              wait(cycleTime/(2*resolution))
		end
                    puts("done with down cycle")	
	end
end



setVoltage(1, 0.0)
channelOn(3)
channelOn(1)
setVoltage(3,3.3)
puts "start"


for i in 0..numberOfOrbits
cycleChannel(true, 5.0, 16, 10)
for i in 0..120*inSun
	stat = getStatus
	wait(0.5)
end

cycleChannel(false, 5.0, 16, 10)
for i in 0..120*N
	stat = getStatus
	wait(0.5)
end
end
puts "done"