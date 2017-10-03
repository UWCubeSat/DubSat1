4.upto(255) do |i|
  cmd("BLINK DELAY with DELAY #{i}")
  wait_check("BLINK SOH DELAY == #{i}", 5)
end
