clear all; close all; clc;

% Configure test
num_runs = 1000;
update_rate = 10;   % In Hz

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

CMD_UNFILTERED_CELL_VOLTAGES = [CMD_CODE_UNFILTERED_CELL_VOLTAGES hex2dec('01') CMD_CODE_UNFILTERED_CELL_VOLTAGES_CHKSUM];
CMD_FILTERED_CELL_VOLTAGES = [CMD_CODE_FILTERED_CELL_VOLTAGES hex2dec('01') CMD_CODE_FILTERED_CELL_VOLTAGES_CHKSUM];
CMD_ANGULAR_POSITION = [CMD_CODE_ANGULAR_POSITION hex2dec('01') CMD_CODE_ANGULAR_POSITION_CHKSUM];

RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES = 19;
RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES = 19;
RESPONSE_LENGTH_ANGULAR_POSITION = 12;

results = zeros(2, num_runs);
for i = 1:num_runs
    
    % Issue command
    fwrite(dut, CMD_ANGULAR_POSITION);
    rawresp = fread(dut, RESPONSE_LENGTH_ANGULAR_POSITION);
    
    % Confirm it's the correct response!
    if (rawresp(1) ~= CMD_CODE_ANGULAR_POSITION)
        disp('Error:  response type does not match command type.');
    else
        results(1,i) = decbytes2fp(rawresp(3:6), 'MSB');
        results(2,i) = decbytes2fp(rawresp(7:10), 'MSB');
    end
end

plot(results);

fclose(dut);
delete(dut);
clear('dut');

