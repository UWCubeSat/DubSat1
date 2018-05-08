clear all; close all; clc;
MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR = 73.0;
IMU_RAW_TO_DPS = 0.004375;
DEG_TO_RAD = 3.14159265358979323846 / 180.0;

dataDir = 'C:\dubsat_data\sp2\';
sourceDataFile = dir(strcat(dataDir, '*.csv'));

disp('reading csv');

% Will have header so add offset.
data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);
time = data(:, 1);

% Read mag inputs and convert them to the raw units
in_mag1 = int16(data(:, 11:13) * 10e9 / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR);
in_mag1valid = data(:, 14);
in_mag2 = int16(data(:, 15:17) * 10e9 / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR);
in_mag2valid = data(:, 18);

% Read imu inputs and convert them to the raw units
in_imu = int16(data(:, 19:21) / (IMU_RAW_TO_DPS * DEG_TO_RAD));
in_imuvalid = data(:, 22);

% Read sun sensor. Already in proper units.
in_sunalpha = data(:, 23);
in_sunbeta = data(:, 24);
in_sunvalid = data(:, 25);

out_mag = data(:, 26:28);
out_imu = data(:, 29:31);
out_sun = data(:, 32:34);

% figure
% plot(time, data(:, 19:21));
% title('IMU readings (raw)');
% xlabel('time (s)');
% ylabel('angular rate');
% legend('x', 'y', 'z');

% figure
% plot(time, data(:, 11:13));
% title('MAG1 readings (raw)');
% xlabel('time (s)');
% ylabel('???');
% legend('x', 'y', 'z');

figure
plot(time, [in_sunalpha, in_sunbeta]);
title('Sun');
xlabel('time (s)');
ylabel('degrees');
legend('alpha', 'beta');

disp('saving mag1');
saveAsShort(in_mag1(:, 1), strcat(dataDir, 'in_mag1x'));
saveAsShort(in_mag1(:, 2), strcat(dataDir, 'in_mag1y'));
saveAsShort(in_mag1(:, 3), strcat(dataDir, 'in_mag1z'));
save(strcat(dataDir, 'in_mag1valid.dat'), 'in_mag1valid', '-ascii');

disp('saving mag2');
saveAsShort(in_mag2(:, 1), strcat(dataDir, 'in_mag2x'));
saveAsShort(in_mag2(:, 2), strcat(dataDir, 'in_mag2y'));
saveAsShort(in_mag2(:, 3), strcat(dataDir, 'in_mag2z'));
save(strcat(dataDir, 'in_mag2valid.dat'), 'in_mag2valid', '-ascii');

disp('saving imu');
saveAsShort(in_imu(:, 1), strcat(dataDir, 'in_imux'));
saveAsShort(in_imu(:, 2), strcat(dataDir, 'in_imuy'));
saveAsShort(in_imu(:, 3), strcat(dataDir, 'in_imuz'));
save(strcat(dataDir, 'in_imuvalid.dat'), 'in_imuvalid', '-ascii');

disp('saving sun');
save(strcat(dataDir, 'in_sunalpha.dat'), 'in_sunalpha', '-ascii');
save(strcat(dataDir, 'in_sunbeta.dat'), 'in_sunbeta', '-ascii');
save(strcat(dataDir, 'in_sunvalid.dat'), 'in_sunvalid', '-ascii');

disp('saving time');
save(strcat(dataDir, 'time.dat'), 'time', '-ascii');

disp('done');
