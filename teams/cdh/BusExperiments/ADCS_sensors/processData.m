clear all; close all; clc;

dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp\'];
bdotDataDir = [dataDir, 'bdot\'];

processSPData_UDP(spDataDir);
processBDOTData_UDP(bdotDataDir);

processSPData(spDataDir);
processBDOTData(bdotDataDir);

disp('done processing data');
