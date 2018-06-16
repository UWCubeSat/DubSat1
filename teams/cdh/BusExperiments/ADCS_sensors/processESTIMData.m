function processESTIMData(data_path)

sourceDataFile = dir([data_path, '*.csv']);

% Will have header so add offset.
disp(['reading estim: ', sourceDataFile.name]);
data = csvread(sprintf('%s\\%s', sourceDataFile.folder, sourceDataFile.name), 1);
disp('processing estim');

time = data(:, 1);

in_tleYear = data(:, 2);
in_tleDay = data(:, 3);
in_tleBstar = data(:, 4);
in_tleRaan = data(:, 5);
in_tleInc = data(:, 6);
in_tleEcc = data(:, 7);
in_tleAop = data(:, 8);
in_tleMna = data(:, 9);
in_tleMnm = data(:, 10);

in_met = data(:, 11);
in_epoch = data(:, 12);

out_inSun = data(:, 13);
out_aboveGS = data(:, 14);

out_sunX = data(:, 15);
out_sunY = data(:, 16);
out_sunZ = data(:, 17);

out_magX = data(:, 18);
out_magY = data(:, 19);
out_magZ = data(:, 20);

out_posX = data(:, 21) * 1000;
out_posY = data(:, 22) * 1000;
out_posZ = data(:, 23) * 1000;

out_velX = data(:, 24) * 1000;
out_velY = data(:, 25) * 1000;
out_velZ = data(:, 26) * 1000;

disp('saving estim time');
save([data_path, 'time.mat'], 'time');

disp('saving estim in_met');
save([data_path, 'in_met.mat'], 'in_met');
save([data_path, 'in_epoch.mat'], 'in_epoch');

disp('saving estim booleans');
out_state = zeros(length(out_inSun), 2);
for i=1:length(out_inSun)
    out_state(i, :) = [uint8(out_inSun(i)) uint8(out_aboveGS(i))];
end
save([data_path, 'out_state.mat'], 'out_state');

disp('saving estim sun vector');
out_sun = vectorToByteArray(out_sunX, out_sunY, out_sunZ);
save([data_path, 'out_sun.mat'], 'out_sun');

disp('saving estim mag vector');
out_mag = vectorToByteArray(out_magX, out_magY, out_magZ);
save([data_path, 'out_mag.mat'], 'out_mag');

disp('saving estim pos vector')
out_pos = vectorToByteArray(out_posX, out_posY, out_posZ);
save([data_path, 'out_pos.mat'], 'out_pos');

disp('saving estim vel vector')
out_vel = vectorToByteArray(out_velX, out_velY, out_velZ);
save([data_path, 'out_vel.mat'], 'out_vel');
