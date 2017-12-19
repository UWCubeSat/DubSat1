%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Incremental conductance Max Power Point Tracking algorythm
% By Sean Poulter
%
% Incremental conductance is a class of Max Power Point Tracking (MPPT)
% algorythms that automaticaly find the optimum voltage, along an I-V curve,
% to set a solar panel to in order to reacive maximum power from the solar
% panel.
%
% A logical diagram of the MPPT algorythm can be found here:
% 
% This algorythm will be used on HuskySat1.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function Vmpp = Incremental_Conductance(Vpv,Ipv)

%create persistant variables
persistent Vmpp_prev Vpv_prev Ipv_prev 

%if this is the first time it's run, then set to an initial guess.
if isEmpty(Vmpp_prev)
    Vmpp_prev = 0;
    Vpv_prev = 0;
    Ipv_prev = 0;
end

%Define change in Vmpp per pass
Vmpp_increment = 1; %1mv

%find increment states
deltaV = Vpv - Vpv_prev;
deltaI = Ipv - Ipv_prev;

%Choose whether to increment or decrement Vmpp
if deltaV ~= 0 % check if voltage has changed
    %if Voltage has changed then compare conductances
    if (deltaI / deltaV) > -Ipv/Vpv 
        Vmpp = Vmpp_prev + Vmpp_increment; %if change in conductance is - then increase Vmpp
    elseif (deltaI / deltaV) < -Ipv/Vpv
        Vmpp = Vmpp_prev - Vmpp_increment;  %if change in conductance is + then Decrease Vmpp
    else
        Vmpp = Vmpp_prev; %if no change in conductance then you have reached the MPP
    end
else %If no change in voltage then check change in current
    if deltaI > 0
        Vmpp = Vmpp_prev - Vmpp_increment; %if voltage stays the same but current increases then the MPP has decreased
    elseif deltaI < 0
        Vmpp = Vmpp_prev + Vmpp_increment; %if voltage stays the same but current decreases then the MPP has increased
    else 
        Vmpp = Vmpp_prev; %if voltage and current stays the same then you remain at the max power point.
    end
end
        
Vpv_prev = Vmpp;
Ipv_prev = Ipv;
Vmpp_prev = Vmpp;
