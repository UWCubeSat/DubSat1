function saveAsShort(data, name)

twos = zeros(length(data), 1);
msb = zeros(length(data), 1);
lsb = zeros(length(data), 1);

for i = 1:length(data)
    [twos(i), msb(i), lsb(i)] = dec2twos(data(i), 16);
end

save(strcat(name, '_twos.dat'), 'twos', '-ascii')
save(strcat(name, '_msb.dat'), 'msb', '-ascii')
save(strcat(name, '_lsb.dat'), 'lsb', '-ascii')
