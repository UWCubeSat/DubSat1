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
slaveaddr = hex2dec('1E');

% outstr = [222 173 190 239];
% outstr = uint8(outstr);
% calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
% calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);
% 
% disp('Device set to slave.  Hit any key to continue ...');
% pause();
% 
% calllib(lib, 'c_aa_close', hport);

outstr = [0 50 100 150 200 0];
outstr = uint8(outstr);

calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

t = 0:.02:1000;
scalefactor = 127;
outx = floor(sin(t) * scalefactor);
outy = floor(sin(t+0.5*pi) * scalefactor);
outz = floor(sin(t+pi) * scalefactor);

i = 0;
count = 0;
while 1
%    outstr = outstr + uint8([1 2 2 3 3 4]);
%    rollover = (outstr >= 255);
%    outstr(rollover) = 0;
   i = i + 1;
   if (i >=length(t))
       i = 1;
   end
   outstr = int8([outx(i) 0 outz(i) 0 outy(i) 0]);
   calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
   pause(.1)
   if (mod(count,4)==0)
       calllib(lib, 'c_aa_i2c_free_bus', hdev);
   end
   count = count + 1;
   count
   outstr
end

           
