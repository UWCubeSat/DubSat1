function saveAsShort(data, name)

msb = zeros(length(data), 1);
lsb = zeros(length(data), 1);

for i = 1:length(data)
    bytes = typecast(int16(data(i)), 'uint8');
    bytes = swapbytes(bytes);
    lsb(i) = bytes(1);
    msb(i) = bytes(2);
end

save(strcat(name, '_msb.dat'), 'msb', '-ascii')
save(strcat(name, '_lsb.dat'), 'lsb', '-ascii')
