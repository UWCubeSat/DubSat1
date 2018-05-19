function fixEnv()

setenv('path', strrep(getenv('path'), '"', ''));
