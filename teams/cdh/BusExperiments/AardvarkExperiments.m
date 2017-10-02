clear all; close all; clc;

lib = 'aardvark';
libhdr = 'aardvark.h';

if ~libisloaded(lib)
    [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
end

hport = 0;
hdev = calllib(lib, 'c_aa_open', hport);
if (hdev < 0)
    error('Unable to open port 0.');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
slaveaddr = 9;
outstr = [222 173 190 239];
outstr = uint8(outstr);
calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

disp('Device set to slave.  Hit any key to continue ...');
pause();

calllib(lib, 'c_aa_close', hport);



