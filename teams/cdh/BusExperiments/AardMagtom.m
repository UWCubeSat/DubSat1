%% Test Scripts
% Experimental scripts for interacting with Aardvark I2C emulator, 
% reading an HMC5883 magnetometer

% Using the Instrument Control Toolbox stuff
clear all; close all; clc;

% Address for 5883/5983
slave_addr = '1Eh';
magtom = i2c('aardvark', 0, slave_addr);
total_time = 5;   % Seconds
period = 0.1;     % Seconds
num_samps = floor(total_time/period);

fopen(magtom);

%%

% Configure magnetometer
fwrite(magtom, [0 hex2dec('74')]);
fwrite(magtom, [1 hex2dec('A0')]);
fwrite(magtom, [2 hex2dec('00')]);

results = zeros(num_samps, 3);

for r = 1:num_samps
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
    results(r,:) = [xval yval zval];
    
    pause(period);
end

fclose(magtom);
delete(magtom);
clear('magtom');

t = (1:num_samps) * period;
figure();
hold on;
plot(t, results(:,1));
plot(t, results(:,2));
plot(t, results(:,3));
legend('X','Y','Z');

