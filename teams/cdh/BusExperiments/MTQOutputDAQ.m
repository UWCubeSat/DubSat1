%% NI DAQ Experiments
% Here we want to try out various methods for observing the output at the
% magnetorquer "end" of the Bdot chain, either by observing the PWM coming
% from the MTQ MSP, or looking at the current waveforms in the coils of the
% MTQ.
clear all; close all; clc;

% Monitor voltage on device 'Dev2', using AI0 single-ended analog input
s = daq.createSession('ni');
addAnalogInputChannel(s, 'Dev2', 0, 'Voltage');

%%% First, perform a foreground capture for the set duration, then display
% the signal and duty cycle observed.
s.Rate = 10000;            % Hz
s.DurationInSeconds = 5;   % Seconds

[data, time] = s.startForeground;

figure();
plot(time, data);

figure();
dutycycle(data, s.Rate)

%%
% Now, we will use background acquisition to things a bit differently.  On
% this first one, we'll use an anonymous function to "do something" each
% callback.

s.Rate = 2000;
s.DurationInSeconds = 5;

lh = addlistener(s, 'DataAvailable', @(src, event) plot(event.TimeStamps, event.Data));
s.NotifyWhenDataAvailableExceeds = 2000;
s.startBackground();
s.wait();
delete(lh);

%%
%  Now we'll call a separate "real" function instead of an anonymous one,
%  so we can do more.  We'll also show off new, magical stopping behavior
%  (allowing the handler to stop things, instead of being just data count 
% driven.
global dchist;

s.Rate = 10000;  %  Set this high enough so dutycycle() can succeed!
s.IsNotifyWhenDataAvailableExceedsAuto = true;
%lh = addlistener(s, 'DataAvailable', @stopWhenStuffHappens);
lh = addlistener(s, 'DataAvailable', @recordDutyCycle);
s.IsContinuous = true;
s.startBackground();

while s.IsRunning
    pause(0.5);
    %fprintf('While loop:  scans acquired = %d\n', s.ScansAcquired);
end

fprintf('Acquisition has terminated with %d scans acquired.\n', s.ScansAcquired);
delete(lh);



