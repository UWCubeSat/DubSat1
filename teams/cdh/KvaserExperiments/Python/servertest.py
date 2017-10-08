import socket
import sys
import time
import binascii

import canlib.canlib as canlib

# Create the TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
can = canlib.canlib()
can.initializeLibrary()

num_channels = can.getNumberOfChannels();
print("Found %d channels" % num_channels)
for ch in range(0, num_channels):
    print("%d. %s (%s / %s)" % (ch, can.getChannelData_Name(ch),
                                can.getChannelData_EAN(ch),
                                can.getChannelData_Serial(ch)))
print("Using CAN channel 0 currently.")

# Bind socket to port
server_address = ('localhost', 10000)
print >>sys.stderr, 'Starting up on %s port %s' % server_address
s.bind(server_address)

s.listen(1)

# We are now listening and waiting to accept a connection
connection, client_address = s.accept()

canchan = can.openChannel(0, canlib.canOPEN_ACCEPT_VIRTUAL)
canchan.setBusOutputControl(canlib.canDRIVER_NORMAL)
canchan.setBusParams(canlib.canBITRATE_250K)
canchan.busOn()

print("Connection received, waiting to pass along CAN traffic.")
while True:
    id, msg, dlc, flag, time = canchan.read(-1)
    #canmsg = canchan.read(-1)
    #print("Received data: %s" % candata)
    idhex = format(id, '02x')
    if len(idhex) % 2 != 0:
        idhex += '0'
    idstr = binascii.a2b_hex(idhex)
    dlcstr = binascii.a2b_hex(format(dlc, '02x'))
    connection.sendall(idstr)
    connection.sendall(dlcstr)
    connection.sendall(msg)
    #time.sleep(.2)

