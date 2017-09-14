function recordDutyCycle(src, event)
    global dchist;
    
    persistent counter;
    if isempty(counter)
        counter = 0
    end
    
    counter = counter + 1;
    if counter > 300
        src.stop();  
        disp('Stopping ...');   
    end
    
     dc = dutycycle(event.Data, src.Rate);
     dc_avg = mean(dc)
     
     dchist = [dchist dc_avg];
     
end

