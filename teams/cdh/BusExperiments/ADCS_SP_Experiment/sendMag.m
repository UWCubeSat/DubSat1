clear all; close all; clc;

% Use bus 1 for mag1 and 2 for mag2
bus = '1';

lib = 'aardvark';
libhdr = 'aardvark.h';

dataDir = 'C:\dubsat_data\sp2\';

if ~libisloaded(lib)
    [load_notfounderrors load_warnings] = loadlibrary(lib, libhdr);
end

disp('loading .dat files');
load(strcat(dataDir, 'time.dat'));
load(sprintf('%sin_mag%sx_msb.dat', dataDir, bus));
load(sprintf('%sin_mag%sx_lsb.dat', dataDir, bus));
load(sprintf('%sin_mag%sy_msb.dat', dataDir, bus));
load(sprintf('%sin_mag%sy_lsb.dat', dataDir, bus));
load(sprintf('%sin_mag%sz_msb.dat', dataDir, bus));
load(sprintf('%sin_mag%sz_lsb.dat', dataDir, bus));
load(sprintf('%sin_mag%svalid.dat', dataDir, bus));

if (bus == '1')
    outstr = [in_mag1x_msb in_mag1x_lsb in_mag1z_msb in_mag1z_lsb in_mag1y_msb in_mag1y_lsb];
else
    outstr = [in_mag2x_msb in_mag2x_lsb in_mag2z_msb in_mag2z_lsb in_mag2y_msb in_mag2y_lsb];
end
outstr = uint8(outstr);

disp('doing slave stuff');

hport = 0;
hdev = calllib(lib, 'c_aa_open', hport);
if (hdev < 0)
    error('Unable to open port 0.');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
slaveaddr = hex2dec('1E');
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

