%-- 10/14/2017 8:36 AM --%
sunsensor_i2c_checkout
[125 56 240 11]
format compact
a = [125 56 240 11]
dec2hex(a)
ah = dec2hex(a)
ah
float(ah)
double(ah)
double(a)
5 << 2
a
a = [hex2dec('40') hex2dec('b1') hex2dec('99') hex2dec('9a')]
af = typecast(a,
af = typecast(a, 'single')
a
a = uint8(a)
af = typecast(a, 'single')
af
af = typecast(fliplr(a), 'single')
a
ah
hex2dec(a)
hex2dec(ah)
ah
a
a = [64 177 153 154]
a
dec2hex(a)
length(a)
error('what?')
ah = '40b1999a'
hex2dec(ah)
hexbytes2fp(['40' 'b1' '99' '9a'], 'MSB')
ah = ['40' 'b1' '99' '9a']
hex2dec(ah)
decbytes2fp(
decbytes2fp
for i = 1:10
end
i = 1:10
i = 1:10:2
i = 1:2:10
decbytes2fp(
decbytes2fp([hex2dec('40') hex2dec('b1') hex2dec('99' hex2dec('9a')], 'MSB')
decbytes2fp([hex2dec('40') hex2dec('b1') hex2dec('99') hex2dec('9a')], 'MSB')
decbytes2fp([hex2dec('40') hex2dec('b1') hex2dec('99') hex2dec('9a')])
sunsensor_i2c_checkout
v = visa()
v = visa('rigol')
%-- 10/14/2017 3:49 PM --%
instrhwinfo
instrfindall
instrhelp
instrhwinfo
obj = visa('ni', 'USB::0x1AB1::0x09C4::DM3R181400475::INSTR')
obj2 = visa('ni', 'USB::0x1AB1::0x09C4::DM3R181400475::INffSTR')
obj
fopen(obj)
fprintf(obj,'*IDN?')
idn = fscanf(fprintf(obj,'*IDN?'))
idn = fscanf(obj)
cmdstr = ':MEAS:VOLT:DC'
fprintf(obj,cmdstr)
fscanf(obj)
cmdstr = ':MEAS:VOLT:DC?'
fprintf(obj,cmdstr)
fscanf(obj)
cmdstr = 'SYST:BEEP'
fprintf(obj,cmdstr)
cmdstr = 'SYST:BEEP'
fprintf(obj,cmdstr)
fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);fprintf(obj,cmdstr);
for i=1:100
fprintf(obj, cmdstr);
pause(.1);
end
for i=1:100
obj2 = visa('ni', 'USB::0x1AB1::0x09C4::DM3R181400475::INffSTR')
obj2 = visa('ni', 'USB::0x1AB1::0x09C4::DM3R181400475::INSTR')
fopen(obj)
fprintf(obj,'*IDN?')
idn = fscanf(fprintf(obj,'*IDN?'))
idn = fscanf(obj)
format compact
for i=1:100
fprintf(obj, cmdstr);
pause(.1);
end
obj1 = instrfind('Type', 'tcpip', 'RemoteHost', '172.28.23.178', 'RemotePort', 5555, 'Tag', '');
ps = obj1
ps
fopen(ps)
cmdstr
printf(ps,cmdstr)
fprintf(ps,cmdstr)
for i=1:100
fprintf(obj, cmdstr);
fprintf(ps, cmdstr);
pause(.1);
end
for i=1:100
fprintf(obj, cmdstr);
pause(.05);
fprintf(ps, cmdstr);
pause(.05);
end