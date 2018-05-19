function i2cPoll(sensor)

AA_ASYNC_NO_DATA = 0;
AA_ASYNC_I2C_READ = 1;
AA_ASYNC_I2C_WRITE = 2;

AA_I2C_STATUS_BUS_ERROR = 1;
AA_I2C_DROPPED_EXCESS_BYTES = -107;

code = calllib(lib, 'c_aa_async_poll', sensor.hdev, 0);

fprintf('i2cPoll %s\n', sensor.name);

if code == AA_ASYNC_NO_DATA
    fprintf('  no data\n');
end

if bitand(code, AA_ASYNC_I2C_READ)    
    dumpSize = 4;
    dump = libpointer('uint8Ptr', uint8(zeros(1, dumpSize)));
    numread = calllib(lib, 'c_aa_i2c_slave_read', sensor.hdev, sensor.addr, dumpSize, dump);
    if numread == AA_I2C_DROPPED_EXCESS_BYTES
        fprintf('  read but dropped excess bytes\n');
    else
        fprintf('  read %i bytes\n', numread);
    end
end

if bitand(code, AA_ASYNC_I2C_WRITE)
    numwrittenPtr = libpointer('uint16Ptr', uint16(0));
    writeStatus = calllib(lib, 'c_aa_i2c_slave_write_stats_ext', sensor.hdev, numwrittenPtr);
    numwritten = get(numwrittenPtr, 'Value');
    if numwritten ~= length(bytes)
        fprintf('  wrote %i bytes, expected %i!\n', numwritten, length(bytes));
    else
        fprintf('  wrote %i bytes\n', numwritten);
    end
    if writeStatus == AA_I2C_STATUS_BUS_ERROR
        fprintf('  an illegal START, STOP, or RESTART condition appeared on the bus during a transaction\n');
    end
end
