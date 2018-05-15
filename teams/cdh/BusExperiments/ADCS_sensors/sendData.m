clear all; close all; clc;

MAX_NUM_AARDVARKS = 10;
PORT_ERROR_VALUE = uint16(10000);

% define sensors

imu.msp = 'sp';
imu.name = 'imu';
imu.addr = hex2dec('6A');
imu.enabled = true;
imu.id = 2238597684;      % unique ID to match up with aardvark
imu.port = PORT_ERROR_VALUE;

mag1.msp = 'sp';
mag1.name = 'mag1';
mag1.addr = hex2dec('1E');
mag1.enabled = false;
mag1.id = 2238593289;
mag1.port = PORT_ERROR_VALUE;

mag2.msp = 'sp';
mag2.name = 'mag2';
mag2.addr = hex2dec('1E');
mag2.enabled = false;
mag2.id = 0;
mag2.port = PORT_ERROR_VALUE;

sun.msp = 'sp';
sun.name = 'sun';
sun.addr = hex2dec('60');
sun.enabled = false;
sun.id = 0;
sun.port = PORT_ERROR_VALUE;

mag.msp = 'bdot';
mag.name = 'mag';
mag.addr = hex2dec('1E');
mag.enabled = false;
mag.id = 0;
mag.port = PORT_ERROR_VALUE;

sensors = [];
if (imu.enabled)
    sensors = [sensors, imu];
end
if (mag1.enabled)
    sensors = [sensors, mag1];
end
if (mag2.enabled)
    sensors = [sensors, mag2];
end
if (sun.enabled)
    sensors = [sensors, sun];
end
if (mag.enabled)
    sensors = [sensors, mag];
end

% load aardvark library
disp('loading library');
lib = 'aardvark';
libhdr = 'aardvark.h';
if ~libisloaded(lib)
    [load_notfounderrors, load_warnings] = loadlibrary(lib, libhdr);
end

% find aardvarks
disp('initializing aardvarks');
portsPtr = libpointer('uint16Ptr', uint16(zeros(1, MAX_NUM_AARDVARKS)));
idsPtr = libpointer('uint32Ptr', uint32(zeros(1, MAX_NUM_AARDVARKS)));
numAardvarks = calllib(lib, 'c_aa_find_devices_ext', MAX_NUM_AARDVARKS, portsPtr, MAX_NUM_AARDVARKS, idsPtr);
ports = get(portsPtr, 'Value');
ids = get(idsPtr, 'Value');

% assign ports to each sensor
for i=1:length(sensors)
    % find an aardvark for this sensor
    for j=1:numAardvarks
        if ids(j) == sensors(i).id
            sensors(i).port = ports(j);
        end
    end
    if sensors(i).port == PORT_ERROR_VALUE
        error('no aardvark found for %s!', sensors(i).name);
    end
end

% open and enable aardvarks
for i=1:length(sensors)
    % close then open aardvark
    fprintf('trying to do stuff with port %i\n', sensors(i).port);
    calllib(lib, 'c_aa_close', sensors(i).port);
    sensors(i).hdev = calllib(lib, 'c_aa_open', sensors(i).port);
    
    % enable aardvark
    calllib(lib, 'c_aa_i2c_slave_enable', sensors(i).hdev, sensors(i).addr, 0, 0);
    disp(['  ' sensors(i).name ' enabled']);
end

% load data
disp('loading data');
dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp\'];
bdotDataDir = [dataDir, 'bdot\'];
load(strcat(spDataDir, 'time.dat')); % loading from sensor proc
if (imu.enabled)
    load(sprintf('%sin_imux_msb.dat', spDataDir));
    load(sprintf('%sin_imux_lsb.dat', spDataDir));
    load(sprintf('%sin_imuy_msb.dat', spDataDir));
    load(sprintf('%sin_imuy_lsb.dat', spDataDir));
    load(sprintf('%sin_imuz_msb.dat', spDataDir));
    load(sprintf('%sin_imuz_lsb.dat', spDataDir));
    load(sprintf('%sin_imuvalid.dat', spDataDir));
    imu.bytes = [in_imux_msb in_imux_lsb in_imuy_msb in_imuy_lsb in_imuz_msb in_imuz_lsb];
    imu.bytes = uint8(imu.bytes);
end
if (mag1.enabled)
    load(sprintf('%sin_mag1x_msb.dat', spDataDir));
    load(sprintf('%sin_mag1x_lsb.dat', spDataDir));
    load(sprintf('%sin_mag1y_msb.dat', spDataDir));
    load(sprintf('%sin_mag1y_lsb.dat', spDataDir));
    load(sprintf('%sin_mag1z_msb.dat', spDataDir));
    load(sprintf('%sin_mag1z_lsb.dat', spDataDir));
    load(sprintf('%sin_mag1valid.dat', spDataDir));
    mag1.bytes = [in_mag1x_msb in_mag1x_lsb in_mag1z_msb in_mag1z_lsb in_mag1y_msb in_mag1y_lsb];
    mag1.bytes = uint8(mag1.bytes);
end
if (mag2.enabled)
    load(sprintf('%sin_mag2x_msb.dat', spDataDir));
    load(sprintf('%sin_mag2x_lsb.dat', spDataDir));
    load(sprintf('%sin_mag2y_msb.dat', spDataDir));
    load(sprintf('%sin_mag2y_lsb.dat', spDataDir));
    load(sprintf('%sin_mag2z_msb.dat', spDataDir));
    load(sprintf('%sin_mag2z_lsb.dat', spDataDir));
    load(sprintf('%sin_mag2valid.dat', spDataDir));
    mag2.bytes = [in_mag2x_msb in_mag2x_lsb in_mag2z_msb in_mag2z_lsb in_mag2y_msb in_mag2y_lsb];
    mag2.bytes = uint8(mag2.bytes);
end
if (sun.enabled)
    load(sprintf('%sin_sunalpha.dat', spDataDir));
    load(sprintf('%sin_sunbeta.dat', spDataDir));
    load(sprintf('%sin_sunvalid.dat', spDataDir));
    sunLength = length(in_sunalpha);
    alphaBytes = zeros(1, sunLength);
    betaBytes = zeros(1, sunLength);
    errorCodes = zeros(1, sunLength);
    sun.bytes = zeros(1, sunLength);
    for i=1:length(alphaBytes)
        alphaBytes(i) = typecast(single(in_sunalpha(index)), 'uint8');
        betaBytes(i) = typecast(single(in_sunbeta(index)), 'uint8');
        if in_sunvalid(index)
            errorCode = 3; % dark
        else
            errorCode = 0; % OK
        end
        errorCodes(i) = typecast(errorCode, 'uint8');
        sun.bytes(i) = [ 10 alphaBytes betaBytes errorCode ];
    end
end
if (mag.enabled)
    xMagMsb = uint8(dlmread(strcat(bdotDataDir, 'xMagMsb.dat')));
    yMagMsb = uint8(dlmread(strcat(bdotDataDir, 'yMagMsb.dat')));
    zMagMsb = uint8(dlmread(strcat(bdotDataDir, 'zMagMsb.dat')));
    xMagLsb = uint8(dlmread(strcat(bdotDataDir, 'xMagLsb.dat')));
    yMagLsb = uint8(dlmread(strcat(bdotDataDir, 'yMagLsb.dat')));
    zMagLsb = uint8(dlmread(strcat(bdotDataDir, 'zMagLsb.dat')));
    mag.bytes = [xMagMsb xMagLsb zMagMsb zMagLsb yMagMsb yMagLsb];
    mag.bytes = uint8(mag.bytes);
end

% set responses
disp('sending data');
while 1
    tic
    index = 1;
    while index < length(time)
        if (toc >= time(index + 1))
            for i=1:length(sensors)
                sensor = sensors(i);
                if ~sensor.enabled
                    continue 
                end
                bytes = sensor.bytes(index, :);
                calllib(lib, 'c_aa_i2c_slave_set_response', sensor.hdev, length(bytes), bytes);
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
