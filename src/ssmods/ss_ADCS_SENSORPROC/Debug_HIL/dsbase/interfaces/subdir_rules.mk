################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
dsbase/interfaces/canwrap.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/interfaces/canwrap.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/interfaces/canwrap.d_raw" --obj_directory="dsbase/interfaces" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/interfaces/rollcall.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/interfaces/rollcall.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/interfaces/rollcall.d_raw" --obj_directory="dsbase/interfaces" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/interfaces/systeminfo.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/interfaces/systeminfo.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/interfaces/systeminfo.d_raw" --obj_directory="dsbase/interfaces" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


