clear all; close all; clc;

dataDir = 'C:\dubsat_data\';
spDataDir = [dataDir, 'sp\'];
bdotDataDir = [dataDir, 'bdot\'];

processSPData(spDataDir);
processBDOTData(bdotDataDir);

disp('done processing data');
