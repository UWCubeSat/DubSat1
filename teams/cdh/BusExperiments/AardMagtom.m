%% Test Scripts
% Experimental scripts for interacting with Aardvark I2C emulator, 
% reading an HMC5883 magnetometer, and performing some statistical
% characterization on it.

%% Configure Aardvark as Master
% Here, since we want the Aardvark to just be an I2C master, we can use the
% Instrument Control Toolbox stuff, which is easier than the
% loadlibrary/wrapper needed for slave scenarios.
clear all; close all; clc;

% 7-bit address for 5883/5983 = 0x1E
slave_addr = '1Eh';
magtom = i2c('aardvark', 0, slave_addr);
total_time = 60;   % Seconds
period = 0.1;     % Seconds
num_samps = floor(total_time/period);
fopen(magtom);

% Configure magnetometer, based on datasheet values
fwrite(magtom, [0 hex2dec('14')]);   % Output rate = 30Hz, no averaging
fwrite(magtom, [1 hex2dec('00')]);   % Maximum gain
fwrite(magtom, [2 hex2dec('00')]);   % Continuous operating mode

%% Collect Data
% Now collect data from the sensor.
results = zeros(num_samps, 4);

tic;
for r = 1:num_samps
    now = toc;
    fwrite(magtom, 3);   % Moves cursor to start of data
    raw = dec2hex(fread(magtom, 6));
    xvalstr = [raw(1,:) raw(2,:)];
    yvalstr = [raw(5,:) raw(6,:)];
    zvalstr = [raw(3,:) raw(4,:)];
    
    xval = twos2decimal(hex2dec(xvalstr), 16);
    yval = twos2decimal(hex2dec(yvalstr), 16);
    zval = twos2decimal(hex2dec(zvalstr), 16);
    
    %disp(['Raw:  ' xvalstr ', ' yvalstr ', ' zvalstr]);
    disp([num2str(xval) ',' num2str(yval) ',' num2str(zval)]);
    results(r,:) = [now xval yval zval];
    
    pause(period);
end

fclose(magtom);
delete(magtom);
clear('magtom');

%t = (1:num_samps) * period;
t = results(:,1);
figure();
hold on;
plot(t, results(:,2));
plot(t, results(:,3));
plot(t, results(:,4));
title('Raw Results');
legend('X','Y','Z');

%% Calculate Allan Variance/Deviation
% Use the standalone Allan calculation function to generate the Allan
% variance/deviation, and then plot it.

pts = 100;
fs = 1/period;
[Tx, sigmax] = overlapped_allan_dev(results(:,2), fs, pts);
[Ty, sigmay] = overlapped_allan_dev(results(:,3), fs, pts);
[Tz, sigmaz] = overlapped_allan_dev(results(:,4), fs, pts);
figure();
hold on;
plot(Tx, sigmax);
plot(Ty, sigmay);
plot(Tz, sigmaz);
title('Allan (Overlapped) Deviation');
legend('X-axis', 'Y-axis', 'Z-axis');


