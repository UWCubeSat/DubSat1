function [vec] = vectorToByteArray(x, y, z)
%SAVEVECTOR Save three matrices as a combined byte array

len = length(x);
vec = zeros(len, 24);
for i=1:len
    xbytes = typecast(double(x(i)), 'uint8');
    ybytes = typecast(double(y(i)), 'uint8');
    zbytes = typecast(double(z(i)), 'uint8');
    vec(i, :) = [fliplr(xbytes) fliplr(ybytes) fliplr(zbytes)];
end

end

