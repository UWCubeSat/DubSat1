from canlib import canlib, Frame, kvadblib
from canlib.canlib import ChannelData
import threading
import sys
import pprint
from functools import partial

def interpretFrame(db, frame,countdict):
    bmsg = db.interpret(frame)
    msgname = bmsg._message.name
    if msgname in countdict:
    	countdict[msgname] = countdict[msgname]+1
    else:
    	countdict[msgname] = 1

def setUpChannel(channel=0,
                 openFlags=canlib.canOPEN_ACCEPT_VIRTUAL,
                 bitrate=canlib.canBITRATE_125K,
                 bitrateFlags=canlib.canDRIVER_NORMAL):
    ch = canlib.openChannel(channel, openFlags)
    print("Using channel: %s, EAN: %s" % (ChannelData(channel).device_name,
                                          ChannelData(channel).card_upc_no)
                                              )
    # ch.setBusOutputControl(bitrateFlags)
    ch.setBusParams(bitrate)
    ch.busOn()
    return ch


def tearDownChannel(ch):
    ch.busOff()
    ch.close()

db = kvadblib.Dbc(filename="../CANDB-MASTER-DS1.dbc")
print("canlib version:", canlib.dllversion())

ch0 = setUpChannel(channel=0)

frame = Frame(id_=100, data=[1, 2, 3, 4], flags=canlib.canMSG_EXT)
counts = {}


def printit():
	global counts
	threading.Timer(5.0, printit).start()
	print(chr(27) + "[2J")
	pprint.pprint(counts)
	sys.stdout.flush()

printit()

while True:
    try:
        frame = ch0.read()
        interpretFrame(db,frame,counts)
    except (canlib.canNoMsg) as ex:
        pass
    except (canlib.canError) as ex:
        print(ex)

tearDownChannel(ch0)
