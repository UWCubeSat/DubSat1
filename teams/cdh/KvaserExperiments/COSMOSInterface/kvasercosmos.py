from canlib import canlib, Frame, kvadblib
from canlib.canlib import ChannelData
from canlib.kvadblib import enums
import threading
import sys
import pprint
import struct
from functools import partial
import socket
import threading
 
def int_to_bytes(value, length):
    result = []

    for i in range(0, length):
        result.append(value >> (i * 8) & 0xff)

    result.reverse()

    return result

def bytes_to_int(bytes):
    result = 0

    for b in bytes:
        result = result * 256 + int(b)

    return result

def interpretFrame(db, frame,countdict):
    bmsg = db.interpret(frame)._message
    msgname = bmsg.name
    MESSAGE= []
    a = ([x for x in frame.data])
    MESSAGE.extend([0,0x24,0,0x80])
    for i in range(20):
        MESSAGE.append(0)
    c = ("0b"+"001"+(bin(frame.id)[2:]).zfill(29))
    c=int(c,2)
    MESSAGE +=int_to_bytes(c,4)
    MESSAGE += a
    for i in range(36-len(MESSAGE)):
        MESSAGE.append(0)
    sys.stdout.flush()
    s.send(bytes(MESSAGE))


def setUpChannel(channel=0,
        openFlags=canlib.canOPEN_ACCEPT_VIRTUAL,
        bitrate=canlib.canBITRATE_62K,
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

db = kvadblib.Dbc(filename="../../CAN/CANDB-MASTER-DS1.dbc")
print("canlib version:", canlib.dllversion())

ch0 = setUpChannel(channel=0)

frame = Frame(id_=100, data=[1, 2, 3, 4])#, flags=canlib.canMSG_EXT)
counts = {}
data=0
  
  
TCP_IP = '127.0.0.1'
TCP_PORT = 1000
MESSAGE = "a"*36
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
def command():
    while True:
        cmd = s.recv(36)
        frameId = (bytes_to_int(cmd[24:28])-2**29)
        # ch0.writeFrame(id_=frameId,data=bytearray(bytes_to_int(cmd[28:36])))
        print(frameId)
        print(enums.MessageFlag.EXT)
        f = Frame(id_=-2**29+frameId,data=(cmd[28:36]),flags=enums.MessageFlag.EXT)
        print(f.id)
        ch0.write(f)
        print(bytes_to_int((cmd[28:36])))
        sys.stdout.flush()

thread = threading.Thread(target = command)
thread.start()
while True:
    try:
        # s.recv(36)
        frame = ch0.read()
        interpretFrame(db,frame,counts)
    except (canlib.canNoMsg) as ex:
        pass
    except (canlib.canError) as ex:
        print(ex)

tearDownChannel(ch0)
s.close()

