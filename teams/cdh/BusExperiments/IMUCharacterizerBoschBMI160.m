%% Sensor Characterization Tool
% Designed to use MATLAB's Instrument Control Toolbox to directly address a
% sensor, and perform basic analysis on it (e.g. noise).

%% Configure Sensor
% Here, since we want the Aardvark to just be an I2C master, we can use the
% Instrument Control Toolbox stuff, which is easier than the
% loadlibrary/wrapper needed for slave scenarios.
clear all; close all; clc;

% Configure core parameters
slave_addr = '68h';      % Bosch BMI160 IMU

% Figure out number of samples
% Raw number of samples method
% num_samps = 50000;
targetlens = 1800;  % target length in seconds

% Connect to device
dut = i2c('aardvark', 0, slave_addr);
fopen(dut);

%% Configure sensor

GYR_ODR_100_NORMALFILT      = hex2dec('28');
GYR_ODR_50_NORMALFILT       = hex2dec('27');
GYR_ODR_25_NORMALFILT       = hex2dec('26');

GYR_RANGE_MIN = hex2dec('04');

CMDREG = hex2dec('7E');
POWER_ON1   = hex2dec('15');
POWER_ON2   = hex2dec('11');

GYR_CONF_REG        = hex2dec('42');
GYR_RANGE_REG       = hex2dec('43');

fwrite(dut, [CMDREG POWER_ON1]);
fwrite(dut, [CMDREG POWER_ON2]);

OUTPUT_DATA_REG = hex2dec('0C'); 
DATA_NUM_BYTES = 12;
ENGR_UNITS_CONV_FACTOR = .0038;   
PAUSE_AFTER_SETTINGS_CHANGE = 5.0;  % seconds

% Create cell array of configs we want to use
% Format:  test title, filename root, master register write bursts
NUM_TEST_CONFIG_FIELDS = 6;
config_25 = {       '25 Hz', 
                    'AVAR25_Hz',
                    .041,
                    floor(targetlens * 25),
                    [GYR_CONF_REG GYR_ODR_25_NORMALFILT],
                    [GYR_RANGE_REG GYR_RANGE_MIN] };
config_50 = {       '50 Hz', 
                    'AVAR50Hz',
                    .0182,
                    floor(targetlens * 50),
                    [GYR_CONF_REG GYR_ODR_50_NORMALFILT],
                    [GYR_RANGE_REG GYR_RANGE_MIN] };
config_100 = {      '100 Hz', 
                    'AVAR100Hz',
                    0.0078,
                    floor(targetlens * 100),
                    [GYR_CONF_REG GYR_ODR_100_NORMALFILT],
                    [GYR_RANGE_REG GYR_RANGE_MIN] };


% configurations = [config_12p5_low,
%                   config_12p5_high,
%                   config_52_low,
%                   config_52_high,
%                   config_104_low,
%                   config_104_high];
configurations = [ config_25,
                   config_50,
                   config_100];

resultsfolder = 'BMI160Results';              
              
numconfigs = length(configurations)/NUM_TEST_CONFIG_FIELDS;
allresults = zeros(numconfigs, 10);

for i = 1:numconfigs
    
    % Calc janky strides because cell arrays are bizarre
    rootindex = ((i-1)*NUM_TEST_CONFIG_FIELDS)+1;
    testtitle = cell2mat(configurations(rootindex));
    testbasefilename = cell2mat(configurations(rootindex + 1));
    testdelayseconds = cell2mat(configurations(rootindex + 2));
    num_samps = cell2mat(configurations(rootindex + 3));
    registerwrite1 = cell2mat(configurations(rootindex + 4));
    registerwrite2 = cell2mat(configurations(rootindex + 5));
    
    disp(['Starting test ' num2str(i) ' (' testtitle '), ' num2str(num_samps) ' samples ...']);

    % Configure IMU for this test
    % TODO:  add a RESET in between?
    fwrite(dut, registerwrite1); 
    fwrite(dut, registerwrite2);  % 80 = highperf disabled
    
    disp(['Settings have been changed for test ' num2str(i) ', waiting to settle ...']);
    pause(PAUSE_AFTER_SETTINGS_CHANGE);
    disp(['... settled.  Proceeding with test ...']);

    results = zeros(num_samps, 4); 
    tic;
    for r = 1:num_samps
        now = toc;
        fwrite(dut, OUTPUT_DATA_REG);   % Moves cursor to start of data
        raw = dec2hex(fread(dut, DATA_NUM_BYTES));

        % IMU BMI160 and ST LSM6DSM both:  Little endian!
        gyroXstr = [raw(2,:) raw(1,:)];
        gyroYstr = [raw(4,:) raw(3,:)];
        gyroZstr = [raw(6,:) raw(5,:)];

        gyroX = twos2decimal(hex2dec(gyroXstr), 16);
        gyroY = twos2decimal(hex2dec(gyroYstr), 16);
        gyroZ = twos2decimal(hex2dec(gyroZstr), 16);

        results(r,:) = [now gyroX gyroY gyroZ];
        
        %pause(period);
        pause(testdelayseconds);
    end
    
    totaltime = toc;
    samprate = (num_samps)/totaltime;
    disp(['Test:  ' testtitle]);
    disp(['Samples taken: ' num2str(num_samps)]);
    disp(['Elapsed time: ' num2str(totaltime) ' s']);
    disp(['Avg. sample rate: ' num2str(samprate) ' Hz']);

    %t = (1:num_samps) * period;
    t = results(:,1);
    hrawfig = figure();
    hold on;
    plot(t, results(:,2));
    plot(t, results(:,3));
    plot(t, results(:,4));
    title('Raw Results');
    xlabel('Time (s)');
    legend('X','Y','Z');
    saveas(hrawfig, [resultsfolder '\' testbasefilename '_raw_results'], 'fig');

    %% Calculate Allan Variance/Deviation
    % Use the standalone Allan calculation function to generate the Allan
    % variance/deviation, and then plot it.

    % Convert to engineering units (using most sensitive

    % For ST LSM6DSM - +/-125dps mode
    euresults = results(:,2:4) * ENGR_UNITS_CONV_FACTOR;
    save([resultsfolder '\' testbasefilename '_captured_data.mat'], 'euresults');

    pts = 100;
    [Tx, sigmax] = overlapped_allan_dev(euresults(:,1), samprate, pts);
    [Ty, sigmay] = overlapped_allan_dev(euresults(:,2), samprate, pts);
    [Tz, sigmaz] = overlapped_allan_dev(euresults(:,3), samprate, pts);

    %%
    havarfig = figure();
    loglog(Tx, sigmax);
    grid on;
    hold on;
    loglog(Ty, sigmay);
    loglog(Tz, sigmaz);
    title('Allan (Overlapped) Deviation');
    xlabel('Time Cluster Size (s)');
    ylabel('Allan Deviation');
    legend('X-axis', 'Y-axis', 'Z-axis');
    saveas(havarfig, [resultsfolder '\' testbasefilename '_avar_analysis'], 'fig');

    % Calculate Angle Random Walk
    arwX = sigmax(10);
    arwY = sigmay(10);
    arwZ = sigmaz(10);

    % Calculate Bias Instability
    binstXdps = min(sigmax);
    binstXdph = binstXdps * 3600;
    binstYdps = min(sigmay);
    binstYdph = binstYdps * 3600;
    binstZdps = min(sigmaz);
    binstZdph = binstZdps * 3600;

    disp(['ARW X/Y/Z = ' num2str(arwX) '/' num2str(arwY) '/' num2str(arwZ)]);
    disp(['Bias instability X = ' num2str(binstXdps) ' dps (' num2str(binstXdph) ' dph)']);
    disp(['Bias instability Y = ' num2str(binstYdps) ' dps (' num2str(binstYdph) ' dph)']);
    disp(['Bias instability Z = ' num2str(binstZdps) ' dps (' num2str(binstZdph) ' dph)']);
    
    allresults(i,:) = [samprate arwX arwY arwZ binstXdps binstYdps binstZdps binstXdph binstYdph binstZdph];
   

end

save([resultsfolder '\' testbasefilename '_SUMMARY_RESULTS.mat'], 'allresults');

fclose(dut);
delete(dut);
clear('magtom');

