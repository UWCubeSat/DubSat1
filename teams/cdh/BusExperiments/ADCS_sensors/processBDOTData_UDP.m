function processBDOTData_UDP(data_path)

sourceDataFile = dir([data_path, '*.csv']);


% Will have header so add offset.
disp(['reading bdot: ', sourceDataFile.name]);
data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);
disp('processing bdot');

angular_velocity_x = data(:, 2) * 1e3;
angular_velocity_y = data(:, 3) * 1e3;
angular_velocity_z = data(:, 4) * 1e3;

disp('saving bdot angular velocity');
saveAsShort(angular_velocity_x, strcat(data_path, 'angular_velocity_x'));
saveAsShort(angular_velocity_y, strcat(data_path, 'angular_velocity_y'));
saveAsShort(angular_velocity_z, strcat(data_path, 'angular_velocity_z'));
disp('done saving bdot udp');
