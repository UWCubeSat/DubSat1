import visa
import socket
import sys
import time
import binascii
import math

# Various helper functions
def beep(instr, numbeeps):
    for i in range(numbeeps):
        instr.write("SYST:BEEP")
        time.sleep(.15)

def bstrify(inputstr):
    return  binascii.a2b_hex(inputstr)



# Setup SCPI/VISA connection to multimeter
rm = visa.ResourceManager()
resources = rm.list_resources()

dm = rm.open_resource(resources[0])
print("Using '%s' as instrument ..." % resources[0])

syncpattern = bstrify('53')
id = bstrify('01')

# Create the TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind socket to port
server_address = ('localhost', 10000)
print('Starting up on %s port %s' % server_address)
s.bind(server_address)

s.listen(1)

# We are now listening and waiting to accept a connection
connection, client_address = s.accept()

beep(dm, 3)
dm.write("FUNC:FREQ")

print("Connection received, waiting to pass along instrument data ...")
while True:

    time.sleep(.25)

    efreq = dm.query(":MEAS:FREQ?")

    rpm = math.floor(30 * float(efreq))
    rpmbstr = bstrify(format(rpm, '04x'))
    lenbstr = bstrify(format(5, '02x'))
    msgbstr = syncpattern + id + lenbstr + rpmbstr
    connection.sendall(msgbstr)



