 clear all; close all; clc;
data_path = '..\ADCS_Bdot_Experiment\data_file\';

STATUS_OPCODE = 1;
ANG_V_OPCODE = 2;
ENV_MAG_OPCODE = 3;

u = udp('127.0.0.1', 4012);
fopen(u);
%% Setting up Aardvark 
lib = 'aardvark';
libhdr = 'aardvark.h';
if ~libisloaded(lib)
    [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
end

hport = 0;
calllib(lib, 'c_aa_close', hport);
hdev = calllib(lib, 'c_aa_open', hport);
if (hdev < 0)
    error('Unable to open port 0.');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
slaveaddr = hex2dec('1E');
% calllib(lib, 'c_aa_close', hport);

%% CAN setup
% global bdot_cmd_data;
% bdot_cmd_data = [0 0 0];
% global can_index;
% can_index = 1;
% 
% can_id = 304545825;
% rxChannel = canChannel('Kvaser','Leaf Light v2 1',1);
% configBusSpeed(rxChannel, 125000);
% dbc = canDatabase('E:\Repos\DubSat1\teams\cdh\CAN\CANDB-MASTER-DS1.dbc');
% rxChannel.Database = dbc;
% filterAllowOnly(rxChannel, can_id, 'Extended');
% rxChannel.MessageReceivedFcn = @receivingFcn;
% start(rxChannel);


%% Loading data in
time = dlmread(strcat(data_path, 'time_to.dat'));
xMagMsb = uint8(dlmread(strcat(data_path, 'xMagMsb_to.dat')));
yMagMsb = uint8(dlmread(strcat(data_path, 'yMagMsb_to.dat')));
zMagMsb = uint8(dlmread(strcat(data_path, 'zMagMsb_to.dat')));
xMagLsb = uint8(dlmread(strcat(data_path, 'xMagLsb_to.dat')));
yMagLsb = uint8(dlmread(strcat(data_path, 'yMagLsb_to.dat')));
zMagLsb = uint8(dlmread(strcat(data_path, 'zMagLsb_to.dat')));

angular_velocity_x_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_msb_to.dat')));
angular_velocity_y_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_msb_to.dat')));
angular_velocity_z_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_msb_to.dat')));

angular_velocity_x_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_lsb_to.dat')));
angular_velocity_y_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_lsb_to.dat')));
angular_velocity_z_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_lsb_to.dat')));
%% Main Loop to Send and Receive Data

index = 1;
outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)];
outstr = uint8(outstr);

calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
pull_up = calllib(lib, 'c_aa_i2c_pullup', hdev, 0);
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

tjump = .049;
tnext = 0;
dataLength = length(time);
%dataLength = 150000;

thuCount = 0;
tic
while 1
   if (toc >= time(index))
        calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
        fwrite(u, [ANG_V_OPCODE, angular_velocity_x_msb(index), ...
                                angular_velocity_x_lsb(index), ...
                                angular_velocity_y_msb(index), ...
                                angular_velocity_y_lsb(index), ...
                                angular_velocity_z_msb(index), ...
                                angular_velocity_z_lsb(index), ...
                   ]);
        time(index)
        tnext = tnext + tjump;
        outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)];
   else
       %thuCount = thuCount + 1;
   end
   while(index < dataLength && tnext > time(index))
        index = index + 1;
   end
   if(index >= dataLength)
        index = 1;
        tnext = 0;
        tic
   end
end

%% CAN Stuff

function receivingFcn(rxChannel)
    rxMsg = receive(rxChannel, 1);
    %rxMsg.Data;
    global bdot_cmd_data;
    global can_index;
    bdot_cmd_data(can_index, :) = rxMsg.Data
    can_index = can_index + 1;
end




%%
%  clear all; close all; clc;
% 
% lib = 'aardvark';
% libhdr = 'aardvark.h';
% 
% load('time.dat');
% load('xMagMsb.dat');
% load('xMagLsb.dat');
% load('yMagMsb.dat');
% load('yMagLsb.dat');
% load('zMagMsb.dat');
% load('zMagLsb.dat');
% 
% 
% if ~libisloaded(lib)
%     [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
% end
% 
% hport = 0;
% hdev = calllib(lib, 'c_aa_open', hport);
% if (hdev < 0)
%     error('Unable to open port 0.');
% end
% 
% % Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
% slaveaddr = hex2dec('1E');
% 
% outstr = [222 173 190 239];
% outstr = uint8(outstr);
% calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
% calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);
% 
% disp('Device set to slave.  Hit any key to continue ...');
% pause();
% 
% calllib(lib, 'c_aa_close', hport);
% read data example: Import columns as column vectors 
% 
% 
% 
% [X Y Z] = csvimport('vectors.csv', 'columns', {'X, 'Y', 'Z'});
% 
% remove headers
% X(1) = [];
% Y(1) = [];
% Z(1) = [];
% 
% index = 1;
% 
% 
% outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)];
% outstr = uint8(outstr);
% 
% calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
% calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);
% 
% dataLength = length(time);
% thuCount = 0;
% tic
% while 1
%    if (toc >= time(index))
%         calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
%        num2bin(outstr, 8);
%         index = index + 1;
%         outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)]
%    else
%        thuCount = thuCount + 1;
%    end
%    if(index >= dataLength)
%         index = 1;
%         tic
%    end
% 
% end


