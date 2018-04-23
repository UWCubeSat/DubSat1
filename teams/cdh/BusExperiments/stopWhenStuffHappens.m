function stopWhenStuffHappens(src, event)
    if any(event.Data > 6.0)
        disp('Event listener:  Detected voltage exceeds 6V, stopping.');
        src.stop();
        plot(event.TimeStamps, event.Data);
    else
        disp('Event listener:  Continuing to acquire ...');
    end
end