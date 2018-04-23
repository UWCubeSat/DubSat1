%% Sensor Characterization Tool
% Designed to use MATLAB's Instrument Control Toolbox to directly address a
% sensor, and perform basic analysis on it (e.g. noise).

%% Configure Sensor
% Here, since we want the Aardvark to just be an I2C master, we can use the
% Instrument Control Toolbox stuff, which is easier than the
% loadlibrary/wrapper needed for slave scenarios.
clear all; close all; clc;

% Configure core parameters
slave_addr = '6Ah'; % ST LSM6DSM for when SDO/SA1 is low
% slave_addr = '6Bh'; % for when SDO/SA1 is high

% Figure out number of samples
targetlens = 5;  % target length in seconds (3600 = 1 hour)

resultsfolder = [pwd '\thermal\test1'];

% Connect to device
dut = i2c('aardvark', 0, slave_addr);
fopen(dut);

%% Configure sensor

% Important config registers on ST LSM6DSM IMU
CTRL2_G = hex2dec('11');
CTRL7_G = hex2dec('16');
OUTPUT_DATA_REG = hex2dec('22');

DATA_NUM_BYTES = 6;
ENGR_UNITS_CONV_FACTOR = .004375;   
PAUSE_AFTER_SETTINGS_CHANGE = 5.0;  % seconds

% Handy constants
ODR_26_FS125    = hex2dec('22');
HIGH_PERF_ON    = hex2dec('00');

testtitle = '26 Hz (High Perf)';
testbasefilename = 'AVAR26Hz_HighPerf';
testdelayseconds = 0.0385;
num_samps = floor(targetlens * 26);
registerwrite1 = [CTRL2_G ODR_26_FS125];
registerwrite2 = [CTRL7_G HIGH_PERF_ON];

disp(['Starting test (' testtitle '), ' num2str(num_samps) ' samples ...']);

% Configure IMU for this test
% TODO:  add a RESET in between?
fwrite(dut, registerwrite1); 
fwrite(dut, registerwrite2);  % 80 = highperf disabled

disp('Settings have been changed for test, waiting to settle ...');
pause(PAUSE_AFTER_SETTINGS_CHANGE);
disp('... settled.  Proceeding with test ...');

results = zeros(num_samps, 4); 
tic;
for r = 1:num_samps
    now = toc;
    fwrite(dut, OUTPUT_DATA_REG);   % Moves cursor to start of data
    raw = dec2hex(fread(dut, DATA_NUM_BYTES));

    % IMU BMI160 and ST LSM6DSM both:  Little endian!
    gyroXstr = [raw(2,:) raw(1,:)];
    gyroYstr = [raw(4,:) raw(3,:)];
    gyroZstr = [raw(6,:) raw(5,:)];

    gyroX = twos2decimal(hex2dec(gyroXstr), 16);
    gyroY = twos2decimal(hex2dec(gyroYstr), 16);
    gyroZ = twos2decimal(hex2dec(gyroZstr), 16);

    results(r,:) = [now gyroX gyroY gyroZ];

    %pause(period);
    pause(testdelayseconds);
end

totaltime = toc;
samprate = (num_samps)/totaltime;
disp(['Test:  ' testtitle]);
disp(['Samples taken: ' num2str(num_samps)]);
disp(['Elapsed time: ' num2str(totaltime) ' s']);   
disp(['Avg. sample rate: ' num2str(samprate) ' Hz']);

t = results(:,1);
hrawfig = figure();
hold on;
plot(t, results(:,2));
plot(t, results(:,3));
plot(t, results(:,4));
title('Raw Results');
xlabel('Time (s)');
legend('X','Y','Z');
saveas(hrawfig, [resultsfolder '\' testbasefilename '_raw_results'], 'fig');
save([resultsfolder '\' testbasefilename '_captured_data.mat'], 'results');

fclose(dut);
delete(dut);
clear('aardvark');

