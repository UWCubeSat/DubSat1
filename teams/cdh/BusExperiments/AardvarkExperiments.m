
[notfound warnings] = loadlibrary('aardvark', 'aardvark.h', 'mfilename', 'aaproto')
unloadlibrary aardvark
[notfound warnings] = loadlibrary('aardvark', 'aardvark.h', 'mfilename', 'aaproto')
libfunctionsview aardvark
calllib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
pPorts
pPorts.Value
ports
unique_ids
calllib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
libfunctionsview aardvark
ports
pPorts = libpointer('uint16Ptr', ports)
pPorts.Value
unique_ids
pUnique_ids = libpointer('uint32Ptr', unique_ids)
pUnique_ids.Value
calllib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
ports
unique_ids
[outnum outports outuids] = callib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
[outnum outports outuids] = calllib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
outuids
outports
outnum
int32(unique_ids)
unique_ids = int32(unique_ids)
ports - int16(ports)
ports = int16(ports)
pPorts = libpointer('uint16Ptr', ports)
pUnique_ids = libpointer('uint32Ptr', unique_ids)
[outnum outports outuids] = calllib(lib, 'c_aa_find_devices_ext', 16, pPorts, 16, pUnique_ids)
outuids
outports
outids
outuids
calllib(lib, 'c_aa_open', 0)
h = 1
calllib(lib, 'c_aa_open', 0)
calllib(lib, 'c_aa_close', 1)
h = calllib(lib, 'c_aa_open', 0)
h
str = hex2dec('deadbeef');
str
outstr = [222 173 190 239]
outstr = uint8(outstr)
outstr
calllib(lib, 'c_aa_i2c_slave_set_response', h, 4, outstr)
calllib(lib, 'c_aa_i2c_slave_enable', h, 5, 4, 4)
calllib(lib, 'c_aa_close', 1)
h = calllib(lib, 'c_aa_open', 0)
calllib(lib, 'c_aa_i2c_slave_set_response', h, 4, outstr)
calllib(lib, 'c_aa_i2c_slave_enable', h, 5, 4, 4)
outstr
outstr(2) = 193
calllib(lib, 'c_aa_i2c_slave_set_response', h, 4, outstr)
outstr(2) = 173
calllib(lib, 'c_aa_i2c_slave_set_response', h, 4, outstr)