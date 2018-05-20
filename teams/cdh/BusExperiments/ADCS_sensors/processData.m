clear all; close all; clc;

dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp-short\'];
bdotDataDir = [dataDir, 'bdot-short\'];

processSPData_UDP(spDataDir);
processBDOTData_UDP(bdotDataDir);

processSPData(spDataDir);
processBDOTData(bdotDataDir);

disp('done processing data');
