clear all; close all; clc;
% Graph and compare data
ANG_VEC = 1;
ENV_MAG = 4;
SENSOR_MAG = 7;
choose_graph = ANG_VEC;

data_path = '..\ADCS_Bdot_Experiment\data_file\';

time = dlmread(strcat(data_path, 'time.dat'));
angular_velocity_x_csv = dlmread(strcat(data_path, 'angular_velocity_x_to.dat'));
angular_velocity_y_csv = dlmread(strcat(data_path, 'angular_velocity_y_to.dat'));
angular_velocity_z_csv = dlmread(strcat(data_path, 'angular_velocity_z.dat'));
env_xMag_csv = dlmread(strcat(data_path, 'env_xMag_nT.dat'));
env_yMag_csv = dlmread(strcat(data_path, 'env_yMag_nT.dat'));
env_zMag_csv = dlmread(strcat(data_path, 'env_zMag_nT.dat'));
xMag_nT_csv = dlmread(strcat(data_path, 'xMagDec_nT.dat'));
yMag_nT_csv = dlmread(strcat(data_path, 'yMagDec_nT.dat'));
zMag_nT_csv = dlmread(strcat(data_path, 'zMagDec_nT.dat'));

variables    = [angular_velocity_x_csv, ...
                angular_velocity_y_csv, ...
                angular_velocity_z_csv, ...
                env_xMag_csv, ...
                env_yMag_csv, ...
                env_zMag_csv, ...
                xMag_nT_csv, ...
                yMag_nT_csv, ...
                zMag_nT_csv];

graph_x = variables(:, choose_graph);
graph_y = variables(:, choose_graph + 1);
graph_z = variables(:, choose_graph + 2);

figure();
hold on;
plot(time, graph_x, 'r');
plot(time, graph_y, 'g');
plot(time, graph_z, 'b');
title('graph');
legend('X','Y','Z');
