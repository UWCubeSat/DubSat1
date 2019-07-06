function [twos_comp_msb, twos_comp_lsb] = dec2twos(x)
% Good version
%  dec2twos(data,bits)  convert decimal to 2s complement
%                           data - decimal value
%                           bits - how many bits wide is the data (i.e. 8
%                           or 16)

x_temp = int16(x);
split_x = typecast(x_temp, 'uint8');
twos_comp_msb = split_x(2);
twos_comp_lsb = split_x(1);

