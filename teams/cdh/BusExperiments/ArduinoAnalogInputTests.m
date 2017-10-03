%% Arduino Voltage Measurement Tests
% The goal of this script is to use the Arduino hardware support package
% with MATLAB to see what the maximum update rate is reading three analog
% voltage inputs simultaneously, for the HIL testing with the Bdot
% controller (among other things).

clear all; close all; clc;

a = arduino();
configurePin(a, 'a0', 'AnalogInput');
configurePin(a, 'a1', 'AnalogInput');
configurePin(a, 'a2', 'AnalogInput');

num_samps = 200;
samples = zeros(num_samps, 4);

va1 = 0;
va2 = 0;
tic
for s=1:num_samps
    now = toc;
    va0 = readVoltage(a, 'a0');
    %va1 = readVoltage(a, 'a1');
    %va2 = readVoltage(a, 'a2');
    
    samples(s,:) = [now va0 va1 va2];
    disp(['Sample set taken: ' num2str(s)]);
end

%%
plot(samples(:,1), samples(:,2));

clear a;