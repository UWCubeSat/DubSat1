function [ids, ports] = i2cFindDevices()

MAX_NUM_AARDVARKS = 10;

portsPtr = libpointer('uint16Ptr', uint16(zeros(1, MAX_NUM_AARDVARKS)));
idsPtr = libpointer('uint32Ptr', uint32(zeros(1, MAX_NUM_AARDVARKS)));

numAardvarks = calllib('aardvark', 'c_aa_find_devices_ext', MAX_NUM_AARDVARKS, portsPtr, MAX_NUM_AARDVARKS, idsPtr);

ports = get(portsPtr, 'Value');
ports = ports(1:numAardvarks);

ids = get(idsPtr, 'Value');
ids = ids(1:numAardvarks);
