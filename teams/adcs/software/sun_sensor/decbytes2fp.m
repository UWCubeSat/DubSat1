function floatout = decbytes2fp(dbytes, endianness)

    dbytes = uint8(dbytes);  %  Make sure we're 8-bit values
    len = length(dbytes);
    
    if (len == 4)
        typestr = 'single';
    elseif (len == 8)
        typestr = 'double';
    else
        error('Packet size must be either 4 or 8 bytes.');
    end
    
    % MATLAB wants this, if byte order is MSB-first
    if (endianness == 'MSB')
        dbytes = fliplr(dbytes);
    end
    
    floatout = typecast(dbytes, typestr);
end

