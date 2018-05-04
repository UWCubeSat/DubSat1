 clear all; close all; clc;

lib = 'aardvark';
libhdr = 'aardvark.h';

load('time.dat');
load('xMagMsb.dat');
load('xMagLsb.dat');
load('yMagMsb.dat');
load('yMagLsb.dat');
load('zMagMsb.dat');
load('zMagLsb.dat');


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
%read data example: Import columns as column vectors 



%[X Y Z] = csvimport('vectors.csv', 'columns', {'X, 'Y', 'Z'});

%remove headers
%X(1) = [];
%Y(1) = [];
%Z(1) = [];

index = 1;


outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)];
outstr = uint8(outstr);

calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

dataLength = length(time);
thuCount = 0;
tic
while 1
   if (toc >= time(index))
        calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
  %      num2bin(outstr, 8);
        index = index + 1;
        outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)]
   else
       %thuCount = thuCount + 1;
   end
   if(index >= dataLength)
        index = 1;
        tic
   end

end


