function aardvarks = i2cOpenAll()

lib = 'aardvark';

% close all
numClosed = calllib(lib, 'c_aa_close', 0);
fprintf('closed %i open aardvarks\n', numClosed);

% find devices
[ids, ports] = i2cFindDevices();

% open all
for i=1:length(ids)
    aardvarks(i) = calllib(lib, 'c_aa_open', ports(i));
    if aardvarks(i) < 0
        status = calllib(lib, 'c_aa_status_string', aardvarks(i));
        error('failed to open port %i: %s', ports(i), status);
    end
end
fprintf('opened %i aardvarks\n', length(aardvarks));
