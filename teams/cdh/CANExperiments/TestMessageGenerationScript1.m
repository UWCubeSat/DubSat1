clear all;  close all;  clc;

canch = canChannel('Kvaser','Leaf Light v2 1',1);
candb = canDatabase('ExampleCANDatabase.dbc');
canch.Database = candb;
start(canch);

temp = 0;
temp_bump = 1;

for i=1:100
   msg = canMessage(candb, 'MCUStatus');
   if (rand >= 0.5)
       temp = int8(temp + temp_bump);
   else
       temp = int8(temp - temp_bump);
   end
   
   msg.Signals.MCUTemp = temp;
   transmit(canch, msg);
   display(['Transmitting msg #' num2str(i) ' ...']);
   
   pause(0.1);
   
end

stop(canch);

msgs = receive(canch, Inf, 'OutputFormat', 'timetable');
signalTimetable = canSignalTimetable(msgs, 'MCUStatus');
plot(signalTimetable.Time, signalTimetable.MCUTemp);


