%% Sensor Characterization Tool
% Designed to use MATLAB's Instrument Control Toolbox to directly address a
% sensor, and perform basic analysis on it (e.g. noise).

%% Configure Sensor
% Here, since we want the Aardvark to just be an I2C master, we can use the
% Instrument Control Toolbox stuff, which is easier than the
% loadlibrary/wrapper needed for slave scenarios.
clear all; close all; clc;

% Configure core parameters
% slave_addr = '1Eh';   % 5883/5983 magnetometers
slave_addr = '68h';    % Bosch BMI160 IMU
total_time = 10;   % Seconds
period = 0.1;     % Seconds

%num_samps = floor(total_time/period);
num_samps = 500000;
dut = i2c('aardvark', 0, slave_addr);
fopen(dut);

% Configure sensor

% % Configure 5883/5983
% fwrite(dut, [0 hex2dec('14')]);   % Output rate = 30Hz, no averaging
% fwrite(dut, [1 hex2dec('00')]);   % Maximum gain
% fwrite(dut, [2 hex2dec('00')]);   % Continuous operating mode

% Configure IMU
%  First, enable both the IMU and accelerometer
GYR_CONF = hex2dec('42');
GYR_RANGE = hex2dec('43');
CMDREG = hex2dec('7E');
fwrite(dut, [CMDREG hex2dec('15')]);
fwrite(dut, [CMDREG hex2dec('11')]);
fwrite(dut, [GYR_CONF 42]);  % ODR = 400Hz
fwrite(dut, [GYR_RANGE 4]);  % Range at +/- 125dps (smallest/most sens.)

%% Collect Data
% Now collect data from the sensor.

% results = zeros(num_samps, 4);  % Magtom
%outputreadreg = 3;  % Magtom
%readbytes = 6;  % Magtom

% BMI160
results = zeros(num_samps, 4); 
outputreadreg = hex2dec('0C');
readbytes = 12;

tic;
for r = 1:num_samps
    now = toc;
    fwrite(dut, outputreadreg);   % Moves cursor to start of data
    raw = dec2hex(fread(dut, readbytes));
    
    % IMU BMI160:  Little endian!
    gyroXstr = [raw(2,:) raw(1,:)];
    gyroYstr = [raw(4,:) raw(3,:)];
    gyroZstr = [raw(6,:) raw(5,:)];
    
    gyroX = twos2decimal(hex2dec(gyroXstr), 16);
    gyroY = twos2decimal(hex2dec(gyroYstr), 16);
    gyroZ = twos2decimal(hex2dec(gyroZstr), 16);
    
    %disp([num2str(gyroX) ',' num2str(gyroY) ',' num2str(gyroZ)]);
    results(r,:) = [now gyroX gyroY gyroZ];
    
%     FOR MAGTOM:
%     xvalstr = [raw(1,:) raw(2,:)];
%     yvalstr = [raw(5,:) raw(6,:)];
%     zvalstr = [raw(3,:) raw(4,:)];
%     
%     xval = twos2decimal(hex2dec(xvalstr), 16);
%     yval = twos2decimal(hex2dec(yvalstr), 16);
%     zval = twos2decimal(hex2dec(zvalstr), 16);
%     
%     %disp(['Raw:  ' xvalstr ', ' yvalstr ', ' zvalstr]);
%     disp([num2str(xval) ',' num2str(yval) ',' num2str(zval)]);
%     results(r,:) = [now xval yval zval];
    
    %pause(period);
end
totaltime = toc;
samprate = (num_samps)/totaltime;
disp(['Samples taken: ' num2str(num_samps)]);
disp(['Elapsed time: ' num2str(totaltime) ' s']);
disp(['Avg. sample rate: ' num2str(samprate) ' Hz']);

%%
fclose(dut);
delete(dut);
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

% Convert to engineering units (using most sensitive
euresults = results(:,2:4) * .0038;

pts = 100;
fs = 1/period;
[Tx, sigmax] = overlapped_allan_dev(euresults(:,1), fs, pts);
[Ty, sigmay] = overlapped_allan_dev(euresults(:,2), fs, pts);
[Tz, sigmaz] = overlapped_allan_dev(euresults(:,3), fs, pts);

%%
figure();
loglog(Tx, sigmax);
hold on;
loglog(Ty, sigmay);
loglog(Tz, sigmaz);
title('Allan (Overlapped) Deviation');
legend('X-axis', 'Y-axis', 'Z-axis');


