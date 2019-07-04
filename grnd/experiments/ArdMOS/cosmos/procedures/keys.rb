def setDisplayKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME DISPLAY, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME DISPLAY, STATE OFF")
	end
end

def setStorageKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME STORAGE, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME STORAGE, STATE OFF")
	end
end

def setUtilityKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME UTILITY, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME UTILITY, STATE OFF")
	end
end

def setHelpKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME HELP, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME HELP, STATE OFF")
	end
end

def setTimerKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME TIMER, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME TIMER, STATE OFF")
	end
end

def setAdvanceKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME ADVANCE, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME ADVANCE, STATE OFF")
	end
end

def setChannelKey(on, channel)
	if on
                    
		cmd("PS_DP832A SETKEYS with KEYNAME CH#{channel}, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME CH#{channel}, STATE OFF")
	end
end

def setRangeKey(on, range)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME RANGE#{range}, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME RANGE#{range}, STATE OFF")
	end
end

def setNumKey(on, num)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME NUM#{num}, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME NUM#{num}, STATE OFF")
	end
end


def setLeftKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME LEFT, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME LEFT, STATE OFF")
	end
end


def setRightKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME RIGHT, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME RIGHT, STATE OFF")
	end
end

def setUpKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME UP, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME UP, STATE OFF")
	end
end

def setOutputKey(on, output)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME OUTPUT#{output}, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME OUTPUT#{output}, STATE OFF")
	end
end

def setKnobKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME KNOB, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME KNOB, STATE OFF")
	end
end

def setOkKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME OK, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME OK, STATE OFF")
	end
end

def setBackKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME BACK, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME BACK, STATE OFF")
	end
end

def setDialKey(on)
	if on
		cmd("PS_DP832A SETKEYS with KEYNAME DIAL, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME DIAL, STATE OFF")
	end
end

def setMenuKey(on, menu)
          if on
		cmd("PS_DP832A SETKEYS with KEYNAME M#{menu}, STATE ON")
	else
		cmd("PS_DP832A SETKEYS with KEYNAME M#{menu}, STATE OFF")
	end
end