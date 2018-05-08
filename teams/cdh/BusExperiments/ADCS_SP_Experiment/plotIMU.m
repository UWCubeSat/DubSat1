clear all; close all; clc;

dataDir = 'C:\dubsat_data\sp2\';
sourceDataFile = dir(strcat(dataDir, '*.csv'));

load(strcat(dataDir, 'time.dat'));
load(sprintf('%sin_imux_msb.dat', dataDir));
load(sprintf('%sin_imux_lsb.dat', dataDir));

d = zeros(length(in_imux_lsb), 1);
for i = 1:length(d)
    d(i) = typecast(uint8([in_imux_msb(i), in_imux_lsb(i)]), 'int16');
end

plot(time, d);
