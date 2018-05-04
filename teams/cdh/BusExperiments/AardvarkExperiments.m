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
slaveaddr = hex2dec('1E');

% calllib(lib, 'c_aa_close', hport);

outstr = [0 50 100 150 200 0];
outstr = uint8(outstr);

calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

t = 0:.02:1000;
scalefactor = 20;
outx = sin(t);
outy = sin(t+0.5*pi);
outz = sin(t+pi);

i = 0;
count = 0;
blahval = 0;
blahval2 = 0;
while 1
%    outstr = outstr + uint8([1 2 2 3 3 4]);
%    rollover = (outstr >= 255);
%    outstr(rollover) = 0;

   i = i + 1;
   scalefactor = scalefactor + 1;
   if (i >=length(t))
       i = 1;
   end
   if (scalefactor > 127)
       scalefactor = 0;
   end
   outstr = int8([floor(outx(i)*scalefactor) 0 floor(outz(i)*scalefactor) 0 floor(outy(i)*scalefactor) 0]);
   retval = calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
   pause(.1);
   count = count + 1;
   
%    if (mod(count, 3200)==0)
%        blahval = calllib(lib, 'c_aa_i2c_slave_disable', hdev);
%        blahval2 = calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);
%        calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
%    end

   count
   outstr
   retval
end

calllib(lib, 'c_aa_close', hport);
