clear all; close all; clc;

% Configure test
num_runs = 100;
update_rate = 5;   % In Hz

update_period = 1/update_rate;   % In seconds

% Configure core parameters for device
slave_addr = '60h';   % Hard-wired on sun sensor, no choice/configurability

% Build initial connection to device
dut = i2c('aardvark', 0, slave_addr);
fopen(dut);

% Command codes and corresponding checksums
CMD_CODE_UNFILTERED_CELL_VOLTAGES    = hex2dec('01');
CMD_CODE_FILTERED_CELL_VOLTAGES      = hex2dec('03');
CMD_CODE_ANGULAR_POSITION            = hex2dec('04');

CMD_CODE_UNFILTERED_CELL_VOLTAGES_CHKSUM    = hex2dec('02');
CMD_CODE_FILTERED_CELL_VOLTAGES_CHKSUM      = hex2dec('04');
CMD_CODE_ANGULAR_POSITION_CHKSUM            = hex2dec('05');

% Original definitions, based on first read of sun sensor spec
% CMD_UNFILTERED_CELL_VOLTAGES = [CMD_CODE_UNFILTERED_CELL_VOLTAGES hex2dec('01') CMD_CODE_UNFILTERED_CELL_VOLTAGES_CHKSUM];
% CMD_FILTERED_CELL_VOLTAGES = [CMD_CODE_FILTERED_CELL_VOLTAGES hex2dec('01') CMD_CODE_FILTERED_CELL_VOLTAGES_CHKSUM];
% CMD_ANGULAR_POSITION = [CMD_CODE_ANGULAR_POSITION hex2dec('01') CMD_CODE_ANGULAR_POSITION_CHKSUM];
% RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES = 19;
% RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES = 19;
% RESPONSE_LENGTH_ANGULAR_POSITION = 12;

CMD_UNFILTERED_CELL_VOLTAGES = [CMD_CODE_UNFILTERED_CELL_VOLTAGES];
CMD_FILTERED_CELL_VOLTAGES = [CMD_CODE_FILTERED_CELL_VOLTAGES];
CMD_ANGULAR_POSITION = [CMD_CODE_ANGULAR_POSITION];

RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES = 18;
RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES = 18;
RESPONSE_LENGTH_ANGULAR_POSITION = 10;

results = zeros(2, num_runs);
for i = 1:num_runs
    
    % Issue command
    fwrite(dut, CMD_ANGULAR_POSITION);
    rawresp = fread(dut, RESPONSE_LENGTH_ANGULAR_POSITION);
    
    % Confirm it's the correct response!
    if (rawresp(1) ~= RESPONSE_LENGTH_ANGULAR_POSITION)
        disp('Error:  response length does not match expected response length.');
    else
        results(1,i) = decbytes2fp(rawresp(2:5), 'MSB');
        results(2,i) = decbytes2fp(rawresp(6:9), 'MSB');
        errorcode = rawresp(10);
        disp([num2str(results(1,i)) ',' num2str(results(2,i)) '[' num2str(errorcode) ']']);
        
        
    end
    
    pause(update_period);
end

plot(results(1,:));
hold on;
plot(results(2,:));

fclose(dut);
delete(dut);
clear('dut');

