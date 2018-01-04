:: Copy binary files

:: xcopy options:  display full source, verify size after copy, continue even if error occurs, suppress overwrite query
xcopy d:\Repos\DubSat1\src\ssmods\ss_EPS_Dist\Debug\ss_EPS_Dist.txt d:\Repos\DubSat1\deploy\main /F /V /C /Y
xcopy d:\Repos\DubSat1\src\ssmods\ss_EPS_Gen\Debug\ss_EPS_Gen.txt d:\Repos\DubSat1\deploy\main /F /V /C /Y
xcopy d:\Repos\DubSat1\src\ssmods\ss_EPS_Batt\Debug\ss_EPS_Batt.txt d:\Repos\DubSat1\deploy\main /F /V /C /Y
xcopy d:\Repos\DubSat1\src\ssmods\ss_PPT\Debug\ss_PPT.txt d:\Repos\DubSat1\deploy\main /F /V /C /Y