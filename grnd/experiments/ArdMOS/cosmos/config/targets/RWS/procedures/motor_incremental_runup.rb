# Script Runner test script
cmd("RWS SETPOINTCH with LENGTH 0, ENTITYID 8, OPCODE 115, NEWSETPOINT 300")
wait(45)

3.upto(80) do |i|
  cmd("RWS SETPOINTCH with LENGTH 0, ENTITYID 8, OPCODE 115, NEWSETPOINT #{i*100}")
  wait(15)
end

