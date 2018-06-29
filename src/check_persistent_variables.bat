@echo off
findstr /i /s /b /n /c:"#pragma PERSISTENT" *.h *.c | findstr /v "msp432"
pause
