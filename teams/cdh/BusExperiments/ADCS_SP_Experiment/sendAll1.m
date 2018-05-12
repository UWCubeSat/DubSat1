clear all; close all; clc;

lib = 'aardvark';
libhdr = 'aardvark.h';

dataDir = 'C:\dubsat_data\sp2\';

if ~libisloaded(lib)
    [load_notfounderrors, load_warnings] = loadlibrary(lib, libhdr);
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
load(sprintf('%sin_mag1x_msb.dat', dataDir));
load(sprintf('%sin_mag1x_lsb.dat', dataDir));
load(sprintf('%sin_mag1y_msb.dat', dataDir));
load(sprintf('%sin_mag1y_lsb.dat', dataDir));
load(sprintf('%sin_mag1z_msb.dat', dataDir));
load(sprintf('%sin_mag1z_lsb.dat', dataDir));
load(sprintf('%sin_mag1valid.dat', dataDir));
load(sprintf('%sin_sunalpha.dat', dataDir));
load(sprintf('%sin_sunbeta.dat', dataDir));
load(sprintf('%sin_sunvalid.dat', dataDir));

in_imu = [in_imux_msb in_imux_lsb in_imuy_msb in_imuy_lsb in_imuz_msb in_imuz_lsb];
in_imu = uint8(in_imu);

in_mag = [in_mag1x_lsb in_mag1x_msb in_mag1z_lsb in_mag1z_msb in_mag1y_lsb in_mag1y_msb];
in_mag = uint8(in_mag);

disp('doing slave stuff');

imu_hport = 0;
mag_hport = 1;
sun_hport = 2;
imu_hdev = calllib(lib, 'c_aa_open', imu_hport);
mag_hdev = calllib(lib, 'c_aa_open', mag_hport);
sun_hdev = calllib(lib, 'c_aa_open', sun_hport);
if (imu_hdev < 0 || mag_hdev < 0 || sun_hdev < 0)
    error('Unable to open port');
end

%% Slave Setup
% Configures Aardvark to listen as a slave, using native API wrapper
imu_slaveaddr = hex2dec('6A');
mag_slaveaddr = hex2dec('1E');
sun_slaveaddr = hex2dec('60');
calllib(lib, 'c_aa_i2c_slave_enable', imu_hdev, imu_slaveaddr, 0, 0);
calllib(lib, 'c_aa_i2c_slave_enable', mag_hdev, mag_slaveaddr, 0, 0);
calllib(lib, 'c_aa_i2c_slave_enable', sun_hdev, sun_slaveaddr, 0, 0);

% Sending response
while 1
    tic
    index = 1;
    while index < length(in_sunalpha)
        if (toc >= time(index + 1))
            % send imu part
            imuBytes = in_imu(index, :);
            calllib(lib, 'c_aa_i2c_slave_set_response', imu_hdev, length(imuBytes), imuBytes);
            
            % send mag part
            magBytes = in_mag(index, :);
            calllib(lib, 'c_aa_i2c_slave_set_response', mag_hdev, length(magBytes), magBytes);
            
            % send sun part
            alphaBytes = typecast(single(in_sunalpha(index)), 'uint8');
            betaBytes = typecast(single(in_sunbeta(index)), 'uint8');
            if in_sunvalid(index)
                errorCode = 3; % dark
            else
                errorCode = 0; % OK
            end
            errorCode = typecast(errorCode, 'uint8');
            sunBytes = [ 10 alphaBytes betaBytes errorCode ];
            calllib(lib, 'c_aa_i2c_slave_set_response', sun_hdev, length(sunBytes), sunBytes);
            
            index = index + 1;
        end
    end
    disp('ran out of input data -- reseting!')
end

calllib(lib, 'c_aa_close', hport);
