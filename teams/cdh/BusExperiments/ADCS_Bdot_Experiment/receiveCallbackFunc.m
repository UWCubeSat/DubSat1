function [outputVect] = receiveCallbackFunc(rxChannel)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
  rxMsg = receive(rxChannel, 1);
  outputVect = 
end

function receivingFcn(rxChannel, index, bdot_cmd_data)
    rxMsg = receive(rxChannel, 1);
    index = length(bdot_cmd_data) + 1;
    bdot_cmd_data(index) = rxMsg.Data;
end