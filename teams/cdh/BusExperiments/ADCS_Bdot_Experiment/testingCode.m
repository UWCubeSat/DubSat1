%clear all; close all; clc;

%% testing dec2twos.m
count = 0;
bits = 8;
%for i = -128:0
%    x(i + 129) = i;
%    [x_twos(i + 129), msb(i + 129), lsb(i + 129)] = dec2twos(x(i + 129), bits); % in string 
 %   x_twos
%    jeff_answer = twos2decimal(x_twos,bits)
%    if(jeff_answer ~= x_twos)
%        count = count + 1;
%    end
%end

x = int16(-1674)
%x = -1674
[x_twos, msb, lsb] = dec2twos(x, 16)











