function i2cSetPullupsEnabled(enabled, aardvarks)

lib = 'aardvark';

PULLUP_ENABLE = 3;
PULLUP_DISABLE = 0;

% if no input aardvark handle array is provided, open all and make one
if nargin < 2
    aardvarks = i2cOpenAll();
end

if enabled == true
    desiredPullupState = PULLUP_ENABLE;
else
    desiredPullupState = PULLUP_DISABLE;
end

for i=1:length(aardvarks)
    pullupState = calllib(lib, 'c_aa_i2c_pullup', aardvarks(i), desiredPullupState);
    if pullupState ~= desiredPullupState
        error('failed to set pullup state');
    end
end
