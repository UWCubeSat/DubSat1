clear all; close all; clc;
MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR = 73.0;


% Will have header so add offset.
overall_bdot_data = csvread('test_bdot_02-May-2018.csv', 1);
time = overall_bdot_data(:, 1);

angular_velocity_x = overall_bdot_data(:, 2);
angular_velocity_y = overall_bdot_data(:, 3);
angular_velocity_z = overall_bdot_data(:, 4);

env_xMag = overall_bdot_data(:, 5);
env_yMag = overall_bdot_data(:, 6);
env_zMag = overall_bdot_data(:, 7);

xMagDec = overall_bdot_data(:, 11);
yMagDec = overall_bdot_data(:, 12);
zMagDec = overall_bdot_data(:, 13);

%plot(time, env_xMag, 'r'); hold on
%plot(time, xMagDec, 'g');

xMagDec_nT = floor(overall_bdot_data(:, 11) * 10e9 / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR);
yMagDec_nT = floor(overall_bdot_data(:, 12) * 10e9 / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR);
zMagDec_nT = floor(overall_bdot_data(:, 13) * 10e9 / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR);

count = 0;
for i = 1:length(xMagDec_nT)
    [xMagTwos(i), xMagMsb(i), xMagLsb(i)] = dec2twos(xMagDec_nT(i), 16);
    [yMagTwos(i), yMagMsb(i), yMagLsb(i)] = dec2twos(yMagDec_nT(i), 16);
    [zMagTwos(i), zMagMsb(i), zMagLsb(i)] = dec2twos(zMagDec_nT(i), 16);
    count = count + 1
end

xMagTwos = xMagTwos.';
xMagMsb = xMagMsb.';
xMagLsb = xMagLsb.';

yMagTwos = yMagTwos.';
yMagMsb = yMagMsb.';
yMagLsb = yMagLsb.';

zMagTwos = zMagTwos.';
zMagMsb = zMagMsb.';
zMagLsb = zMagLsb.';

save('xMagTwos.dat', 'xMagTwos', '-ascii');
save('xMagMsb.dat', 'xMagMsb', '-ascii');
save('xMagLsb.dat', 'xMagLsb', '-ascii');

save('yMagTwos.dat', 'yMagTwos', '-ascii');
save('yMagMsb.dat', 'yMagMsb', '-ascii');
save('yMagLsb.dat', 'yMagLsb', '-ascii');

save('zMagTwos.dat', 'zMagTwos', '-ascii');
save('zMagMsb.dat', 'zMagMsb', '-ascii');
save('zMagLsb.dat', 'zMagLsb', '-ascii');

save('time.dat', 'time', '-ascii');

save('angular_velocity_x.dat', 'angular_velocity_x', '-ascii');
save('angular_velocity_y.dat', 'angular_velocity_y', '-ascii');
save('angular_velocity_z.dat', 'angular_velocity_z', '-ascii');

save('env_xMag.dat', 'env_xMag', '-ascii');
save('env_yMag.dat', 'env_yMag', '-ascii');
save('env_zMag.dat', 'env_zMag', '-ascii');




