function [twos_comp, twos_comp_msb, twos_comp_lsb] = dec2twos(x, bits)
%  dec2twos(data,bits)  convert decimal to 2s complement
%                           data - decimal value
%                           bits - how many bits wide is the data (i.e. 8
%                           or 16)

twos_comp = dec2bin(mod((x),2^bits),bits);
%twos_comp = str2num(twos_comp)
x_twos_length = length(twos_comp);
sign = twos_comp(1);
sign_extend = '';
for i = 1:(bits - x_twos_length)
    sign_extend(i) = sign;
end
twos_comp = strcat(sign_extend, twos_comp);
twos_comp_msb = twos_comp(1: bits / 2);
twos_comp_msb = bin2dec(twos_comp_msb);
twos_comp_lsb = twos_comp(bits / 2 + 1 : bits);
twos_comp_lsb = bin2dec(twos_comp_lsb);
twos_comp = bin2dec(twos_comp);

%% print out stuff
%{
twos_comp
twos_comp_msb
twos_comp_lsb
%}

