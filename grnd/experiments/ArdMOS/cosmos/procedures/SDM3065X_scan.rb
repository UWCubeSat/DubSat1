def scanSetup()
    
  cmd("SDM3065X", "SCAN_SET_UP")
end

def setScanRange(low, high)
  for i in low..high+1
        
    cmd("SDM3065X", "TURN_ON_CHANNEL","CHANNEL"=>i)
  end
  
  cmd("SDM3065X", "SET_SCANNING_RANGE","LOW"=>low, "HIGH"=>high)
end

def startScan()
    
  cmd("SDM3065X", "START_SCAN")
end

def endScan()
  
  cmd("SDM3065X", "END_SCAN")
end

def getData(low, high)
  data=Array.new(high-low)
  for i in low..high
    puts i    
    cmd("SDM3065X", "GET_DATA","CHANNEL"=>i)
    
    val=tlm("SDM3065X DATA DATA")
    data[i-low]=val
    
  end
  return data
end