function processSPData_UDP(data_path)

sourceDataFile = dir([data_path, '*.csv']);


% Will have header so add offset.
disp(['reading sp: ', sourceDataFile.name]);
data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);
disp('processing sp');

angular_velocity_x = data(:, 2) * 1e3;
angular_velocity_y = data(:, 3) * 1e3;
angular_velocity_z = data(:, 4) * 1e3;
env_mag_x = data(:, 5) * 1e9;
env_mag_y = data(:, 6) * 1e9;
env_mag_z = data(:, 7) * 1e9;

mag_out_x = data(:, 26) * 1e9;
mag_out_y = data(:, 27) * 1e9;
mag_out_z = data(:, 28) * 1e9;

imu_out_x = data(:, 29) * 1e3;
imu_out_y = data(:, 30) * 1e3;
imu_out_z = data(:, 31) * 1e3;

sun_out_x = data(:, 32);
sun_out_y = data(:, 33);
sun_out_z = data(:, 34);


disp('saving sp angular velocity');
saveAsShort(angular_velocity_x, strcat(data_path, 'angular_velocity_x'));
saveAsShort(angular_velocity_y, strcat(data_path, 'angular_velocity_y'));
saveAsShort(angular_velocity_z, strcat(data_path, 'angular_velocity_z'));

disp('saving sp environmental velocity');
saveAsShort(env_mag_x, strcat(data_path, 'env_mag_x'));
saveAsShort(env_mag_y, strcat(data_path, 'env_mag_y'));
saveAsShort(env_mag_z, strcat(data_path, 'env_mag_z'));

disp('saving sp mag output');
saveAsShort(mag_out_x, [data_path 'out_mag_x']);
saveAsShort(mag_out_y, [data_path 'out_mag_y']);
saveAsShort(mag_out_z, [data_path 'out_mag_z']);

disp('saving sp imu output');
saveAsShort(imu_out_x, [data_path 'out_imu_x']);
saveAsShort(imu_out_y, [data_path 'out_imu_y']);
saveAsShort(imu_out_z, [data_path 'out_imu_z']);

disp('saving sp sun output');
save([data_path 'out_sun_x.dat'], 'sun_out_x', '-ascii');
save([data_path 'out_sun_y.dat'], 'sun_out_y', '-ascii');
save([data_path 'out_sun_z.dat'], 'sun_out_z', '-ascii');

disp('done saving sp udp');
