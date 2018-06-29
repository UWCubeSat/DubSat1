function processBDOTData(data_path)

MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR = 73.0;

sourceDataFile = dir([data_path, '*.csv']);

% Will have header so add offset.
disp(['reading bdot: ', sourceDataFile.name]);
overall_bdot_data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);

disp('processing bdot');

time = overall_bdot_data(:, 1);

angular_velocity_x = overall_bdot_data(:, 2) * 1e6;
angular_velocity_y = overall_bdot_data(:, 3) * 1e6;
angular_velocity_z = overall_bdot_data(:, 4) * 1e6;

env_xMag_nT = overall_bdot_data(:, 5) * 1e9;
env_yMag_nT = overall_bdot_data(:, 6) * 1e9;
env_zMag_nT = overall_bdot_data(:, 7) * 1e9;

xMagDec = overall_bdot_data(:, 11);
yMagDec = overall_bdot_data(:, 12);
zMagDec = overall_bdot_data(:, 13);

xMagDec_nT = floor(overall_bdot_data(:, 11) * 1e9);
yMagDec_nT = floor(overall_bdot_data(:, 12) * 1e9);
zMagDec_nT = floor(overall_bdot_data(:, 13) * 1e9);

xMagDec_nT_conv = xMagDec_nT / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR;
yMagDec_nT_conv = yMagDec_nT / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR;
zMagDec_nT_conv = zMagDec_nT / MAG_HMC5883L_GAIN_1370_NT_CONVERSION_FACTOR;


dlmwrite(strcat(data_path, 'xMag_nT_to.dat'), xMagDec_nT);
dlmwrite(strcat(data_path, 'yMag_nT_to.dat'), yMagDec_nT);
dlmwrite(strcat(data_path, 'zMag_nT_to.dat'), zMagDec_nT);

dlmwrite(strcat(data_path, 'env_xMag_nT_to.dat'), env_xMag_nT);
dlmwrite(strcat(data_path, 'env_yMag_nT_to.dat'), env_yMag_nT);
dlmwrite(strcat(data_path, 'env_zMag_nT_to.dat'), env_zMag_nT);

dlmwrite(strcat(data_path, 'angular_velocity_x_to.dat'), angular_velocity_x);
dlmwrite(strcat(data_path, 'angular_velocity_y_to.dat'), angular_velocity_y);
dlmwrite(strcat(data_path, 'angular_velocity_z_to.dat'), angular_velocity_z);


dataLength = length(time)
count = 0



for i = 1:dataLength
    [xMagMsb(i), xMagLsb(i)] = dec2twos(xMagDec_nT_conv(i));
    [yMagMsb(i), yMagLsb(i)] = dec2twos(yMagDec_nT_conv(i));
    [zMagMsb(i), zMagLsb(i)] = dec2twos(zMagDec_nT_conv(i));
    [angular_velocity_x_msb(i), angular_velocity_x_lsb(i)] = dec2twos(angular_velocity_x(i));
    [angular_velocity_y_msb(i), angular_velocity_y_lsb(i)] = dec2twos(angular_velocity_y(i));
    [angular_velocity_z_msb(i), angular_velocity_z_lsb(i)] = dec2twos(angular_velocity_z(i));
    [env_xMag_msb(i), env_xMag_lsb(i)] = dec2twos(env_xMag_nT(i));
    [env_yMag_msb(i), env_yMag_lsb(i)] = dec2twos(env_yMag_nT(i));
    [env_zMag_msb(i), env_zMag_lsb(i)] = dec2twos(env_zMag_nT(i));
    count = count + 1
end

xMagMsb = uint8(xMagMsb).';
yMagMsb = uint8(yMagMsb).';
zMagMsb = uint8(zMagMsb).';

xMagLsb = uint8(xMagLsb).';
yMagLsb = uint8(yMagLsb).';
zMagLsb = uint8(zMagLsb).';

angular_velocity_x_msb = uint8(angular_velocity_x_msb).';
angular_velocity_y_msb = uint8(angular_velocity_y_msb).';
angular_velocity_z_msb = uint8(angular_velocity_z_msb).';

angular_velocity_x_lsb = uint8(angular_velocity_x_lsb).';
angular_velocity_y_lsb = uint8(angular_velocity_y_lsb).';
angular_velocity_z_lsb = uint8(angular_velocity_z_lsb).';

env_xMag_msb = uint8(env_xMag_msb).';
env_yMag_msb = uint8(env_yMag_msb).';
env_zMag_msb = uint8(env_zMag_msb).';

env_xMag_lsb = uint8(env_xMag_lsb).';
env_yMag_lsb = uint8(env_yMag_lsb).';
env_zMag_lsb = uint8(env_zMag_lsb).';


tic
dlmwrite(strcat(data_path, 'time_to.dat'), time);

dlmwrite(strcat(data_path, 'xMagMsb_to.dat'), xMagMsb);
dlmwrite(strcat(data_path, 'yMagMsb_to.dat'), yMagMsb);
dlmwrite(strcat(data_path, 'zMagMsb_to.dat'), zMagMsb);

dlmwrite(strcat(data_path, 'xMagLsb_to.dat'), xMagLsb);
dlmwrite(strcat(data_path, 'yMagLsb_to.dat'), yMagLsb);
dlmwrite(strcat(data_path, 'zMagLsb_to.dat'), zMagLsb);

dlmwrite(strcat(data_path, 'angular_velocity_x_msb_to.dat'), angular_velocity_x_msb);
dlmwrite(strcat(data_path, 'angular_velocity_y_msb_to.dat'), angular_velocity_y_msb);
dlmwrite(strcat(data_path, 'angular_velocity_z_msb_to.dat'), angular_velocity_z_msb);

dlmwrite(strcat(data_path, 'angular_velocity_x_lsb_to.dat'), angular_velocity_x_lsb);
dlmwrite(strcat(data_path, 'angular_velocity_y_lsb_to.dat'), angular_velocity_y_lsb);
dlmwrite(strcat(data_path, 'angular_velocity_z_lsb_to.dat'), angular_velocity_z_lsb);

dlmwrite(strcat(data_path, 'env_xMag_msb_to.dat'), env_xMag_msb);
dlmwrite(strcat(data_path, 'env_yMag_msb_to.dat'), env_yMag_msb);
dlmwrite(strcat(data_path, 'env_zMag_msb_to.dat'), env_zMag_msb);

dlmwrite(strcat(data_path, 'env_xMag_lsb_to.dat'), env_xMag_lsb);
dlmwrite(strcat(data_path, 'env_yMag_lsb_to.dat'), env_yMag_lsb);
dlmwrite(strcat(data_path, 'env_zMag_lsb_to.dat'), env_zMag_lsb);
toc



% tic
% dlmwrite(strcat(data_path, 'time.dat'), 'time');
% saveData(xMagDec_nT_conv, 'xMag_to', data_path);
% saveData(yMagDec_nT_conv, 'yMag_to', data_path);
% saveData(zMagDec_nT_conv, 'zMag_to', data_path);
% saveData(angular_velocity_x, 'angular_velocity_x_to', data_path);
% saveData(angular_velocity_y, 'angular_velocity_y_to', data_path);
% saveData(angular_velocity_z, 'angular_velocity_z_to', data_path);
% saveData(env_xMag_nT, 'env_xMag_to', data_path);
% saveData(env_xMag_nT, 'env_yMag_to', data_path);
% saveData(env_xMag_nT, 'env_zMag_to', data_path);