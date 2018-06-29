function i2cPoll(sensor)

AA_ASYNC_NO_DATA = 0;
AA_ASYNC_I2C_READ = 1;
AA_ASYNC_I2C_WRITE = 2;

AA_I2C_STATUS_BUS_ERROR = 1;
AA_I2C_DROPPED_EXCESS_BYTES = -107;

lib = 'aardvark';
code = calllib(lib, 'c_aa_async_poll', sensor.hdev, 0);

if code == AA_ASYNC_NO_DATA
    return
end

if bitand(code, AA_ASYNC_I2C_READ)
    dumpSize = 4;
    dump = libpointer('uint8Ptr', uint8(zeros(1, dumpSize)));
    numread = calllib(lib, 'c_aa_i2c_slave_read', sensor.hdev, sensor.addr, dumpSize, dump);
    if numread == AA_I2C_DROPPED_EXCESS_BYTES
        fprintf('%s read but dropped excess bytes\n', sensor.name);
    end
end

if bitand(code, AA_ASYNC_I2C_WRITE)
    numwrittenPtr = libpointer('uint16Ptr', uint16(0));
    writeStatus = calllib(lib, 'c_aa_i2c_slave_write_stats_ext', sensor.hdev, numwrittenPtr);
    numwritten = get(numwrittenPtr, 'Value');
%     if numwritten ~= sensor.length % add 1 to account for master addr write
%         fprintf('%s wrote %i bytes, expected %i!\n', sensor.name, numwritten, sensor.length);
%     end
    if writeStatus == AA_I2C_STATUS_BUS_ERROR
        fprintf('%s -- an illegal START, STOP, or RESTART condition appeared on the bus during a transaction\n', sensor.name);
    end
end
