clear all; close all; clc;

u = udp('127.0.0.1', 4012);
fopen(u);

OPCODE = 1;
i = 0;

while 1
    fwrite(u, [OPCODE abs(floor(255 * sin(i)))]);
    i = i + .05;
    pause(0.1);
end

