 clear all; close all; clc;

%% Setting up UDP COSMOS Interface
STATUS_OPCODE = 1;
ANG_V_OPCODE = 2;
ENV_MAG = 3;

u = udp('127.0.0.1', 4012);
fopen(u);
 
%% Setting up Aardvark 
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
%read data example: Import columns as column vectors 

%[X Y Z] = csvimport('vectors.csv', 'columns', {'X, 'Y', 'Z'});

%remove headers
%X(1) = [];
%Y(1) = [];
%Z(1) = [];

%% Loading data in
load('time.dat');

load('xMagMsb.dat');
load('xMagLsb.dat');
load('yMagMsb.dat');
load('yMagLsb.dat');
load('zMagMsb.dat');
load('zMagLsb.dat');

load('angular_velocity_x.dat');
load('angular_velocity_y.dat');
load('angular_velocity_z.dat');
load('env_xMag.dat');
load('env_yMag.dat');
load('env_zMag.dat');

%% Main Loop to Send and Receive Data

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
        index = index + 1;
        outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)]
        fwrite(u, [STATUS_OPCODE abs(floor(255 * sin(i)))]);
        ang_v = [angular_velocity_x(index) angular_velocity_y(index) angular_velocity_x(index)]
        fwrite(u, [ANG_V_OPCODE ang_v]);
        env_mag = [env_xMag(index) env_yMag(index) env_zMag(index)]
        fwrite(u, [ENV_MAG env_mag]);
        i = i + .05;
   else
       %thuCount = thuCount + 1;
   end
   if(index >= dataLength)
        index = 1;
        i = 0;
        tic
   end
end
