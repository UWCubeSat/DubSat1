function processSPData_UDP(data_path)

sourceDataFile = dir([data_path, '*.csv']);


% Will have header so add offset.
disp(['reading sp: ', sourceDataFile.name]);
data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);
disp('processing sp');

angular_velocity_x = sourceDataFile(:, 2) * 1e3;
angular_velocity_y = sourceDataFile(:, 3) * 1e3;
angular_velocity_z = sourceDataFile(:, 4) * 1e3;
env_mag_x = sourceDataFile(:, 5) * 1e9;
env_mag_y = sourceDataFile(:, 6) * 1e9;
env_mag_z = sourceDataFile(:, 7) * 1e9;



disp('saving sp angular velocity');
saveAsShort(angular_velocity_x, strcat(dataDir, 'angular_velocity_x'));
saveAsShort(angular_velocity_y, strcat(dataDir, 'angular_velocity_y'));
saveAsShort(angular_velocity_z, strcat(dataDir, 'angular_velocity_z'));
disp('saving sp environmental velocity');
saveAsShort(env_mag_x, strcat(dataDir, 'env_mag_x'));
saveAsShort(env_mag_y, strcat(dataDir, 'env_mag_y'));
saveAsShort(env_mag_z, strcat(dataDir, 'env_mag_z'));

disp('done saving sp udp');