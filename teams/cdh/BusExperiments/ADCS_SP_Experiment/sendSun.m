clear all; close all; clc;

lib = 'aardvark';
libhdr = 'aardvark.h';

dataDir = 'C:\dubsat_data\sp2\';

if ~libisloaded(lib)
    [load_notfounderrors, load_warnings] = loadlibrary(lib, libhdr);
end

disp('loading .dat files');
load(strcat(dataDir, 'time.dat'));
load(sprintf('%sin_sunalpha.dat', dataDir));
load(sprintf('%sin_sunbeta.dat', dataDir));
load(sprintf('%sin_sunvalid.dat', dataDir));

disp('doing slave stuff');

hport = 0;
hdev = calllib(lib, 'c_aa_open', hport);
if (hdev < 0)
    error('Unable to open port 0.');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
slaveaddr = hex2dec('60');
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

% Sending response
while 1
    tic
    index = 1;
    while index < length(in_sunalpha)
        if (toc >= time(index + 1))
            alphaBytes = typecast(single(in_sunalpha(index)), 'uint8');
            betaBytes = typecast(single(in_sunbeta(index)), 'uint8');
            if in_sunvalid(index)
                errorCode = 3; % dark
            else
                errorCode = 0; % OK
            end
            errorCode = typecast(errorCode, 'uint8');
            thisout = [ 10 alphaBytes betaBytes errorCode ];
            calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(thisout), thisout);
            
            index = index + 1;
        end
    end
    disp('ran out of input data -- reseting!')
end

calllib(lib, 'c_aa_close', hport);

