clear all; close all; clc;
clear all; close all; clc;
data_path = '..\ADCS_Bdot_Experiment\data_file\';
STATUS_OPCODE = 1;
ANG_V_OPCODE = 2;
ENV_MAG_OPCODE = 3;

u = udp('127.0.0.1', 4012);
fopen(u);

% OPCODE = 1;
i = 0;
index = 1;
time = dlmread(strcat(data_path, 'time.dat'));
angular_velocity_x_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_msb.dat')));
angular_velocity_y_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_msb.dat')));
angular_velocity_z_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_msb.dat')));

angular_velocity_x_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_lsb.dat')));
angular_velocity_y_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_lsb.dat')));
angular_velocity_z_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_lsb.dat')));

env_xMag_msb = uint8(dlmread(strcat(data_path, 'env_xMag_msb.dat')));
env_yMag_msb = uint8(dlmread(strcat(data_path, 'env_yMag_msb.dat')));
env_zMag_msb = uint8(dlmread(strcat(data_path, 'env_zMag_msb.dat')));

env_xMag_lsb = uint8(dlmread(strcat(data_path, 'env_xMag_lsb.dat')));
env_yMag_lsb = uint8(dlmread(strcat(data_path, 'env_yMag_lsb.dat')));
env_zMag_lsb = uint8(dlmread(strcat(data_path, 'env_zMag_lsb.dat')));

dataLength = length(time);
tic
while (1)
    if (toc >= time(index))
        fwrite(u, [STATUS_OPCODE abs(floor(255 * sin(i)))]);
        fwrite(u, [ANG_V_OPCODE, uint8(angular_velocity_x_msb(index)), ...
                                uint8(angular_velocity_x_lsb(index)), ...
                                uint8(angular_velocity_y_msb(index)), ...
                                uint8(angular_velocity_y_lsb(index)), ...
                                uint8(angular_velocity_z_msb(index)), ...
                                uint8(angular_velocity_z_lsb(index)), ...
                   ]);
        fwrite(u, [ENV_MAG_OPCODE uint8(env_xMag_msb(index)) ...
                                  uint8(env_xMag_lsb(index)) ...
                                  uint8(env_yMag_msb(index)) ...
                                  uint8(env_yMag_lsb(index)) ...
                                  uint8(env_zMag_msb(index)) ...
                                  uint8(env_zMag_lsb(index)) ...
                   ]);
        i = i + .05
    end
    if (index >= dataLength)
        index = 1;
        tic
    end
end


% 
% clear all; close all; clc;
% data_path = '..\ADCS_Bdot_Experiment\data_file\';
% 
% %% Setting up Aardvark 
% lib = 'aardvark';
% libhdr = 'aardvark.h';
% if ~libisloaded(lib)
%     [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
% end
% 
% hport = 0;
% calllib(lib, 'c_aa_close', hport);
% hdev = calllib(lib, 'c_aa_open', hport);
% if (hdev < 0)
%     error('Unable to open port 0.');
% end
% 
% %% Slave Setup
% % Configures Aardvark to listen as a slave, using native API wrapper
% slaveaddr = hex2dec('1E');
% % calllib(lib, 'c_aa_close', hport);
% 
% %% Sets up udp connection and COSMOS interface
% u = udp('127.0.0.1', 4012);
% fopen(u);
% STATUS_OPCODE = 1;
% ANG_V_OPCODE = 2;
% ENV_MAG_OPCODE = 3;
% 
% %% Load in Data
% time = dlmread(strcat(data_path, 'time.dat'));
% angular_velocity_x_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_msb.dat')));
% angular_velocity_y_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_msb.dat')));
% angular_velocity_z_msb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_msb.dat')));
% 
% angular_velocity_x_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_x_lsb.dat')));
% angular_velocity_y_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_y_lsb.dat')));
% angular_velocity_z_lsb = uint8(dlmread(strcat(data_path, 'angular_velocity_z_lsb.dat')));
% 
% env_xMag_msb = uint8(dlmread(strcat(data_path, 'env_xMag_msb.dat')));
% env_yMag_msb = uint8(dlmread(strcat(data_path, 'env_yMag_msb.dat')));
% env_zMag_msb = uint8(dlmread(strcat(data_path, 'env_zMag_msb.dat')));
% 
% env_xMag_lsb = uint8(dlmread(strcat(data_path, 'env_xMag_lsb.dat')));
% env_yMag_lsb = uint8(dlmread(strcat(data_path, 'env_yMag_lsb.dat')));
% env_zMag_lsb = uint8(dlmread(strcat(data_path, 'env_zMag_lsb.dat')));
% 
% xMagMsb = uint8(dlmread(strcat(data_path, 'xMagMsb.dat')));
% yMagMsb = uint8(dlmread(strcat(data_path, 'yMagMsb.dat')));
% zMagMsb = uint8(dlmread(strcat(data_path, 'zMagMsb.dat')));
% 
% xMagLsb = uint8(dlmread(strcat(data_path, 'xMagLsb.dat')));
% yMagLsb = uint8(dlmread(strcat(data_path, 'yMagLsb.dat')));
% zMagLsb = uint8(dlmread(strcat(data_path, 'zMagLsb.dat')));
% 
% 
% %% While Loop
% dataLength = length(time);
% i = 0;
% index = 1;
% 
% outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) yMagMsb(index) yMagLsb(index)];
% outstr = uint8(outstr);
% 
% tic
% while (1)
%     if (toc >= time(index))
%         
%         calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(outstr), outstr);
%         index = index + 1;
%         outstr = [xMagMsb(index) xMagLsb(index) zMagMsb(index) zMagLsb(index) ...
%                   yMagMsb(index) yMagLsb(index)]
%               
%               
%         fwrite(u, [STATUS_OPCODE abs(floor(255 * sin(i)))]);
%         fwrite(u, [ANG_V_OPCODE, angular_velocity_x_msb(index), ...
%                                 angular_velocity_x_lsb(index), ...
%                                 angular_velocity_y_msb(index), ...
%                                 angular_velocity_y_lsb(index), ...
%                                 angular_velocity_z_msb(index), ...
%                                 angular_velocity_z_lsb(index), ...
%                    ]);
%         fwrite(u, [ENV_MAG_OPCODE env_xMag_msb(index) ...
%                                   env_xMag_lsb(index) ...
%                                   env_yMag_msb(index) ...
%                                   env_yMag_lsb(index) ...
%                                   env_zMag_msb(index) ...
%                                   env_zMag_lsb(index) ...
%                    ]);
%         i = i + .05
%     end
%     if (index >= dataLength)
%         index = 1;
%         tic
%     end
% end

