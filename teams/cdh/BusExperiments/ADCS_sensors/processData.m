clear all; close all; clc;

dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp-short\'];
bdotDataDir = [dataDir, 'bdot-short\'];
estimDataDir = [dataDir, 'estim-short\'];

processSPData_UDP(spDataDir);
processBDOTData_UDP(bdotDataDir);

processSPData(spDataDir);
processBDOTData(bdotDataDir);

processESTIMData(estimDataDir);

disp('done processing data');
