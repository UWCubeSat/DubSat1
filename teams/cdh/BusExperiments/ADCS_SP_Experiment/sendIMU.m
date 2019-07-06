clear all; close all; clc;

lib = 'aardvark';
libhdr = 'aardvark.h';

dataDir = 'C:\dubsat_data\sp\';

if ~libisloaded(lib)
    [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
end

disp('loading .dat files');
load(strcat(dataDir, 'time.dat'));
load(sprintf('%sin_imux_msb.dat', dataDir));
load(sprintf('%sin_imux_lsb.dat', dataDir));
load(sprintf('%sin_imuy_msb.dat', dataDir));
load(sprintf('%sin_imuy_lsb.dat', dataDir));
load(sprintf('%sin_imuz_msb.dat', dataDir));
load(sprintf('%sin_imuz_lsb.dat', dataDir));
load(sprintf('%sin_imuvalid.dat', dataDir));

% outstr = [in_imux_msb in_imux_lsb in_imuy_msb in_imuy_lsb in_imuz_msb in_imuz_lsb];
outstr = [in_imux_lsb in_imux_msb in_imuy_lsb in_imuy_msb in_imuz_lsb in_imuz_msb];
outstr = uint8(outstr);

disp('doing slave stuff');

hport = 0;
calllib(lib, 'c_aa_close', hport);
hdev = calllib(lib, 'c_aa_open', hport);
if (hdev < 0)
    error('Unable to open port 0.');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
slaveaddr = hex2dec('6A');
calllib(lib, 'c_aa_i2c_slave_enable', hdev, slaveaddr, 0, 0);

% Sending response
while 1
    tic
    index = 1;
    while index < length(outstr)
        if (toc >= time(index + 1))
            thisout = outstr(index, :);
            calllib(lib, 'c_aa_i2c_slave_set_response', hdev, length(thisout), thisout);
            index = index + 1;
        end
    end
    disp('ran out of input data -- reseting!')
end

calllib(lib, 'c_aa_close', hport);

