from canlib import canlib, Frame, kvadblib
from canlib.canlib import ChannelData
import threading
import sys
import pprint
import struct
from functools import partial

distCount=0
def interpretFrame(db, frame,countdict):
    bmsg = db.interpret(frame)._message
    msgname = bmsg.name
    if msgname == "rc_eps_dist_h1":
        a = ([x for x in frame.data])
        global distCount
        distCount=0
        for i in range(len(a)):
            distCount+=pow(256,(7-i))*a[i]
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
data=0


def printit():
    global counts
    threading.Timer(5, printit).start()
    print(chr(27) + "[2J")
    global distCount
    print("what the packet reports: " + str(distCount))
    total =0
    for x in counts.keys():
        if "rc" in x and "rc_eps_dist" not in x: 
            total += counts[x]
    print("what the computer thinks: " + str(total))
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


