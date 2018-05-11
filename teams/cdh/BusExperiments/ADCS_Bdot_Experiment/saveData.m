function saveData(data, name, data_path)
    twos_comp_msb = zeros(length(data), 1);
    twos_comp_lsb = zeros(length(data), 1);

    for i = 1:length(data)
        x_temp = int16(data);
        split_x = typecast(x_temp, 'uint8');
        twos_comp_msb(i) = split_x(2);
        twos_comp_lsb(i) = split_x(1);
        i
    end
dlmwrite(strcat(data_path, name, '_msb.dat'), 'twos_comp_msb');
dlmwrite(strcat(data_path, name, '_lsb.dat'), 'twos_comp_msb');

% save(strcat(data_path, name, '_msb.dat'), 'twos_comp_msb', '-ascii')
% save(strcat(data_path, name, '_lsb.dat'), 'twos_comp_lsb', '-ascii')

end

