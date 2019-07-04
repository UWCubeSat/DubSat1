clear all; close all;

Arduino_data = csvread('heater test arduino data processed.csv');

board_data_msp = csvread('heater test board data processed.csv',1258,2,[1258 2 2700 3]);
board_data_batt = csvread('heater test board data processed.csv',10318,0,[10318 0 21865 1]);

board_data_msp(:,1) = board_data_msp(:,1)-1.521765785000000e+09;
board_data_batt(:,1) = board_data_batt(:,1)-1.521765785000000e+09;

figure
plot(Arduino_data(:,1),Arduino_data(:,4),Arduino_data(:,1),Arduino_data(:,3)...
    ,board_data_msp(:,1),board_data_msp(:,2),board_data_batt(:,1),board_data_batt(:,2));
legend('mounted on end of the battery (best conduction to core)','mounted on casing of third battery',...
      'msp below battery pack','mounted on casing of 1st batery (current position)','Location','SouthOutside');
%figure
%plot(board_data_msp(:,1),board_data_msp(:,2),board_data_batt(:,1),board_data_batt(:,2));

Arduino_data = Arduino_data(35:200,1:4);
board_data_batt = board_data_batt(1500:8500,1:2);

figure
hold on

P1 = polyfit(Arduino_data(:,1),Arduino_data(:,4),1);
P2 = polyfit(board_data_batt(:,1),board_data_batt(:,2),1);

Y1 = polyval(P1, Arduino_data(:,1));
Y2 = polyval(P2, board_data_batt(:,1));

plot(Arduino_data(:,1),Arduino_data(:,4),Arduino_data(:,1),Y1,...
     board_data_batt(:,1),board_data_batt(:,2),board_data_batt(:,1),Y2);
legend('Sesnor on terminal', 'fit slope = 0.0158deg/sec', 'Sensor on side', 'fit slope = 0.0126 deg/sec'...
       ,'Location', 'southeast');
