clear all;

all_data = csvread('thermal data all.csv',2,0);

voltage = all_data(:,1:2);
voltage = voltage(any(voltage,2),:);
curent = all_data(:,3:4);
curent = curent(any(curent,2),:);
temp = all_data(:,5:6);
temp = temp(any(temp,2),:);
inttemp = all_data(:,7:8);
inttemp = inttemp(any(inttemp,2),:);

figure
hold on
plot(temp(:,1), temp(:,2), inttemp(:,1), inttemp(:,2));
yyaxis right 
plot(curent(:,1), curent(:,2));
yyaxis 
plot(voltage(:,1), voltage(:,2));
legend('Battery temperature','MSP temp','Battery Voltage');

figure hold on
plot(inttemp(:,1), inttemp(:,2), curent(:,1), curent(:,2));


