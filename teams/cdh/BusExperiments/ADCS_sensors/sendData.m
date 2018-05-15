clear all; close all; clc;

MAX_NUM_AARDVARKS = 10;

dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp\'];
bdotDataDir = [dataDir, 'bdot\'];

% define sensors

imu.name = 'imu';
imu.addr = hex2dec('6A');
imu.id = 2238592316; % unique ID to match up with aardvark

mag1.name = 'mag1';
mag1.addr = hex2dec('1E');
mag1.id = 2238593289;

mag2.name = 'mag2';
mag2.addr = hex2dec('1E');
mag2.id = 0;

sun.name = 'sun';
sun.addr = hex2dec('60');
sun.id = 0;

mag.name = 'bdot-mag';
mag.addr = hex2dec('1E');
mag.id = 2238519142;

sensors = [
    imu
    mag1
    mag2
    sun
    mag
];

% load aardvark library
disp('loading library');
lib = 'aardvark';
libhdr = 'aardvark.h';
if ~libisloaded(lib)
    [load_notfounderrors, load_warnings] = loadlibrary(lib, libhdr);
end

% find aardvarks
disp('assigning sensors to ports');
portsPtr = libpointer('uint16Ptr', uint16(zeros(1, MAX_NUM_AARDVARKS)));
idsPtr = libpointer('uint32Ptr', uint32(zeros(1, MAX_NUM_AARDVARKS)));
numAardvarks = calllib(lib, 'c_aa_find_devices_ext', MAX_NUM_AARDVARKS, portsPtr, MAX_NUM_AARDVARKS, idsPtr);
ports = get(portsPtr, 'Value');
ids = get(idsPtr, 'Value');

% assign ports to each sensor
i = 1;
while i <= length(sensors)
    % find an aardvark for this sensor
    foundAardvark = false;
    for j=1:numAardvarks
        if ids(j) == sensors(i).id
            sensors(i).port = ports(j);
            fprintf('  assigned port %i to %s\n', sensors(i).port, sensors(i).name);
            foundAardvark = true;
            i = i + 1;
            break
        end
    end
    if ~foundAardvark
        % remove sensors that have no aardvarks
        disp(['  no aardvark found for ', sensors(i).name]);
        sensors(i) = [];
    end
end
if isempty(sensors)
    error('no matching aardvarks found!');
end

% load data
disp('loading data');
time = dlmread(strcat(spDataDir, 'time.dat'));
for i=1:length(sensors)
    % load sensors based on their id (but only the ones with an aardvark)
    switch sensors(i).id
        case imu.id
            in_imux_msb = dlmread(sprintf('%sin_imux_msb.dat', spDataDir));
            in_imux_lsb = dlmread(sprintf('%sin_imux_lsb.dat', spDataDir));
            in_imuy_msb = dlmread(sprintf('%sin_imuy_msb.dat', spDataDir));
            in_imuy_lsb = dlmread(sprintf('%sin_imuy_lsb.dat', spDataDir));
            in_imuz_msb = dlmread(sprintf('%sin_imuz_msb.dat', spDataDir));
            in_imuz_lsb = dlmread(sprintf('%sin_imuz_lsb.dat', spDataDir));
            sensors(i).bytes = [in_imux_lsb in_imux_msb in_imuy_lsb in_imuy_msb in_imuz_lsb in_imuz_msb];
        case mag1.id
            in_mag1x_msb = dlmread(sprintf('%sin_mag1x_msb.dat', spDataDir));
            in_mag1x_lsb = dlmread(sprintf('%sin_mag1x_lsb.dat', spDataDir));
            in_mag1y_msb = dlmread(sprintf('%sin_mag1y_msb.dat', spDataDir));
            in_mag1y_lsb = dlmread(sprintf('%sin_mag1y_lsb.dat', spDataDir));
            in_mag1z_msb = dlmread(sprintf('%sin_mag1z_msb.dat', spDataDir));
            in_mag1z_lsb = dlmread(sprintf('%sin_mag1z_lsb.dat', spDataDir));
            sensors(i).bytes = [in_mag1x_msb in_mag1x_lsb in_mag1z_msb in_mag1z_lsb in_mag1y_msb in_mag1y_lsb];
        case mag2.id
            in_mag2x_msb = dlmread(sprintf('%sin_mag2x_msb.dat', spDataDir));
            in_mag2x_lsb = dlmread(sprintf('%sin_mag2x_lsb.dat', spDataDir));
            in_mag2y_msb = dlmread(sprintf('%sin_mag2y_msb.dat', spDataDir));
            in_mag2y_lsb = dlmread(sprintf('%sin_mag2y_lsb.dat', spDataDir));
            in_mag2z_msb = dlmread(sprintf('%sin_mag2z_msb.dat', spDataDir));
            in_mag2z_lsb = dlmread(sprintf('%sin_mag2z_lsb.dat', spDataDir));
            sensors(i).bytes = [in_mag2x_msb in_mag2x_lsb in_mag2z_msb in_mag2z_lsb in_mag2y_msb in_mag2y_lsb];
        case sun.id
            in_sunalpha = dlmread(sprintf('%sin_sunalpha.dat', spDataDir));
            in_sunbeta = dlmread(sprintf('%sin_sunbeta.dat', spDataDir));
            in_sunvalid = dlmread(sprintf('%sin_sunvalid.dat', spDataDir));
            sunLength = length(in_sunalpha);
            sensors(i).bytes = zeros(sunLength, 10);
            for j=1:sunLength
                alphaBytes = typecast(single(in_sunalpha(j)), 'uint8');
                betaBytes = typecast(single(in_sunbeta(j)), 'uint8');
                if in_sunvalid(j)
                    errorCode = 3; % dark
                else
                    errorCode = 0; % OK
                end
                errorCode = uint8(errorCode);
                sensors(i).bytes(j, :) = [ 10 alphaBytes betaBytes errorCode ];
            end
        case mag.id
            xMagMsb = uint8(dlmread(strcat(bdotDataDir, 'xMagMsb_to.dat')));
            yMagMsb = uint8(dlmread(strcat(bdotDataDir, 'yMagMsb_to.dat')));
            zMagMsb = uint8(dlmread(strcat(bdotDataDir, 'zMagMsb_to.dat')));
            xMagLsb = uint8(dlmread(strcat(bdotDataDir, 'xMagLsb_to.dat')));
            yMagLsb = uint8(dlmread(strcat(bdotDataDir, 'yMagLsb_to.dat')));
            zMagLsb = uint8(dlmread(strcat(bdotDataDir, 'zMagLsb_to.dat')));
            sensors(i).bytes = uint8([xMagMsb xMagLsb zMagMsb zMagLsb yMagMsb yMagLsb]);
        otherwise
            error('%s with id %i not recognized!', sensors(i).name, sensors(i).id);
    end
end

% open and enable aardvarks
disp('opening aardvarks');
for i=1:length(sensors)
    % close then open aardvark
    calllib(lib, 'c_aa_close', sensors(i).port);
    sensors(i).hdev = calllib(lib, 'c_aa_open', sensors(i).port);
    if sensors(i).hdev < 0
        error('failed to open %s on port %i (error code: %i)', sensors(i).name, int16(sensors(i).port), sensors(i).hdev);
    end
    
    % enable aardvark
    calllib(lib, 'c_aa_i2c_slave_enable', sensors(i).hdev, sensors(i).addr, 0, 0);
end

% set responses
disp('sending data');
while 1
    tic
    index = 1;
    while index < length(time)
        if (toc >= time(index + 1))
            for i=1:length(sensors)
                bytes = sensors(i).bytes(index, :);
                calllib(lib, 'c_aa_i2c_slave_set_response', sensors(i).hdev, length(bytes), bytes);
            end
            index = index + 1;
        end
    end
    disp('ran out of input data -- reseting!')
end

disp('closing');
for i=1:length(sensors)
    calllib(lib, 'c_aa_close', sensors(i).port);
end
