clear all; close all; clc;

lib1 = 'promira';
lib2 = 'promact_is';

if ~libisloaded(lib1)
    [notfound1 warnings1] = loadlibrary(lib1, 'promira.h');
end

% Setup for Promira on "my machine" at the moment
ipaddrstr = '10.1.8.149';
appstr = 'com.totalphase.promact_is';

% First, load app, then load library that Total Phase internals "downloads"
% from the device directly
hprom = calllib('promira', 'c_pm_open', ipaddrstr);
calllib('promira', 'c_pm_load', hprom, appstr);
if ~libisloaded(lib2)
    [notfound2 warnings2] = loadlibrary(lib2, 'promact_is.h');
end

% Now setup the connection and channel
hconn = calllib(lib2, 'c_ps_app_connect', ipaddrstr);
hchan = calllib(lib2, 'c_ps_channel_open', hconn);

% Do some physical configuration
calllib(lib2,'c_ps_app_configure', hchan, 17);  % I2C and SPI on
calllib(lib2,'c_ps_phy_target_power', hchan, 0);  % Power off
calllib(lib2,'c_ps_i2c_pullup', hchan, 3);  % "Both" pull-ups on

% Prepare response string
slaveaddr = 8;
outbuff = [222 173 190 239];
outbuff = uint8(outbuff);
poutbuff = libpointer('uint8Ptr', outbuff);
calllib(lib2, 'c_ps_i2c_slave_set_resp', hchan, 4, poutbuff); 
calllib(lib2, 'c_ps_i2c_slave_enable', hchan, slaveaddr, 0, 0);

