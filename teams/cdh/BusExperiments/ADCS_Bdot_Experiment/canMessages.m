%clear all; close all; clc;


dbc = canDatabase('CANDB-MASTER-DS1.dbc');
global bdot_cmd_data;
bdot_cmd_data = [0 0 0];
global index;
index = 1;

can_id = 304545825;
rxChannel = canChannel('Kvaser','Leaf Light v2 1',1);
configBusSpeed(rxChannel, 125000);
rxChannel.Database = dbc;
rxChannel.MessageReceivedFcn = @receivingFcn;
filterAllowOnly(rxChannel, can_id, 'Extended');
start(rxChannel);

function receivingFcn(rxChannel)
    rxMsg = receive(rxChannel, 1);
    rxMsg.Data
    global bdot_cmd_data;
    global index;
    bdot_cmd_data(index, :) = rxMsg.Data;
    index = index + 1;
end

