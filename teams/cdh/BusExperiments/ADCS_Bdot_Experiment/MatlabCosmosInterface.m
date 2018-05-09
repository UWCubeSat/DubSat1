clear all; close all; clc;

STATUS_OPCODE = 1;
ANG_V_OPCODE = 2;
ENV_MAG = 3;

u = udp('127.0.0.1', 4012);
fopen(u);

% OPCODE = 1;
i = 0;
index = 1;

load('time.dat');
load('angular_velocity_x.dat');
load('angular_velocity_y.dat');
load('angular_velocity_z.dat');
load('env_xMag.dat');
load('env_yMag.dat');
load('env_zMag.dat');
dataLength = length(time);
tic
while (1)
    if (toc >= time(index))
        fwrite(u, [STATUS_OPCODE abs(floor(255 * sin(i)))]);
        ang_v = [angular_velocity_x(index) angular_velocity_y(index) angular_velocity_x(index)]
        fwrite(u, [ANG_V_OPCODE ang_v]);
        env_mag = [env_xMag(index) env_yMag(index) env_zMag(index)]
        fwrite(u, [ENV_MAG env_mag]);
        i = i + .05;
    end
    if (index >= dataLength)
        index = 1;
        tic
    end
    pause(0.05);
end


    
% while 1
%     fwrite(u, [OPCODE abs(floor(255 * sin(i)))]);
%     i = i + .05;
%     pause(0.05);
% end

