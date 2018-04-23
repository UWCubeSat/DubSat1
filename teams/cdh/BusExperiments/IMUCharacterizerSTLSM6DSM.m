%% Sensor Characterization Tool
% Designed to use MATLAB's Instrument Control Toolbox to directly address a
% sensor, and perform basic analysis on it (e.g. noise).

%% Configure Sensor
% Here, since we want the Aardvark to just be an I2C master, we can use the
% Instrument Control Toolbox stuff, which is easier than the
% loadlibrary/wrapper needed for slave scenarios.
clear all; close all; clc;

% Configure core parameters
slave_addr = '6Ah'; % ST LSM6DSM for when SDO/SA1 is low
% slave_addr = '6Bh'; % for when SDO/SA1 is high

% Figure out number of samples
% Raw number of samples method
% num_samps = 50000;
targetlens = 3600;  % target length in seconds (3600 = 1 hour)

% resultsfolder = 'LSM6DSMResults';
resultsfolder = [pwd '\postvibe3'];

% Connect to device
dut = i2c('aardvark', 0, slave_addr);
fopen(dut);

%% Configure sensor

% Important config registers on ST LSM6DSM IMU
CTRL2_G = hex2dec('11');
CTRL7_G = hex2dec('16');
OUTPUT_DATA_REG = hex2dec('22');

DATA_NUM_BYTES = 6;
ENGR_UNITS_CONV_FACTOR = .004375;   
PAUSE_AFTER_SETTINGS_CHANGE = 5.0;  % seconds

% Handy constants
ODR_12p5_FS125  = hex2dec('12');
ODR_26_FS125    = hex2dec('22');
ODR_52_FS125    = hex2dec('32');
ODR_104_FS125   = hex2dec('42');
HIGH_PERF_OFF   = hex2dec('80');
HIGH_PERF_ON    = hex2dec('00'); 

% Create cell array of configs we want to use
% Format:  test title, filename root, master register write bursts
NUM_TEST_CONFIG_FIELDS = 6;
config_12p5_low = { '12.5 Hz (Low Power)', 
                    'AVAR12p5_HzLowPower',
                    .12,
                    floor(targetlens * 12.5),
                    [CTRL2_G ODR_12p5_FS125],
                    [CTRL7_G HIGH_PERF_OFF] };
config_12p5_high = { '12.5 Hz (High Perf)', 
                    'AVAR12p5Hz_HighPerf',
                    .12,
                    floor(targetlens * 12.5),
                    [CTRL2_G ODR_12p5_FS125],
                    [CTRL7_G HIGH_PERF_ON] };
config_26_low = { '26 Hz (Low Power)',
                  'AVAR26Hz_LowPower',
                  0.0385, % Assuming this delay should be ~1/26, the period
                  floor(targetlens * 26),
                  [CTRL2_G ODR_26_FS125],
                  [CTRL7_G HIGH_PERF_OFF] };
config_26_high = { '26 Hz (High Perf)',
                  'AVAR26Hz_HighPerf',
                  0.0385,
                  floor(targetlens * 26),
                  [CTRL2_G ODR_26_FS125],
                  [CTRL7_G HIGH_PERF_ON] };
config_52_low = { '52 Hz (Low Power)', 
                    'AVAR52Hz_LowPower',
                    0.02,
                    floor(targetlens * 52),
                    [CTRL2_G ODR_52_FS125],
                    [CTRL7_G HIGH_PERF_OFF] };
config_52_high = { '52 Hz (High Perf)', 
                    'AVAR52Hz_HighPerf',
                    0.02,
                    floor(targetlens * 52),
                    [CTRL2_G ODR_52_FS125],
                    [CTRL7_G HIGH_PERF_ON] };
config_104_low = { '104 Hz (Low Power/"Normal")',
                    'AVAR104Hz_LowPower',
                    .008,
                    floor(targetlens * 104),
                    [CTRL2_G ODR_104_FS125],
                    [CTRL7_G HIGH_PERF_OFF] };               
config_104_high = { '104 Hz (High Perf)', 
                    'AVAR104Hz_HighPerf',
                    .008,
                    floor(targetlens * 104),
                    [CTRL2_G ODR_104_FS125],
                    [CTRL7_G HIGH_PERF_ON] };

% configurations = [config_12p5_low,
%                   config_12p5_high,
%                   config_52_low,
%                   config_52_high,
%                   config_104_low,
%                   config_104_high];
configurations = [ config_26_high  ];
              
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

    save([resultsfolder '\' testbasefilename '_SUMMARY_RESULTS.mat'], 'allresults');

end

fclose(dut);
delete(dut);
clear('aardvark');

