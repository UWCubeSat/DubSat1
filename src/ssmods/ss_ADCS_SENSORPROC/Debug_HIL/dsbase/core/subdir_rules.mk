################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
dsbase/core/MET.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/MET.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/MET.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/agglib.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/agglib.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/agglib.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/can.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/can.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/can.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/dataArray.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/dataArray.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/dataArray.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/debugtools.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/debugtools.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/debugtools.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/i2c.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/i2c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/i2c.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/spi.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/spi.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/timer.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/timer.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/timers.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/timers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/timers.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dsbase/core/uart.obj: C:/Users/Thu\ Phan/Documents/DubSat1/src/dsbase/core/uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/DubSat1/src/ssmods/ss_ADCS_SENSORPROC" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR5994__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=__I2C_DONT_WRITE_IMU__ --define=__I2C_DONT_WRITE_SUN__ --define=__BSP_HW_GPS_OEM719__ --define=__SS_ADCS_SENSORPROC__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dsbase/core/uart.d_raw" --obj_directory="dsbase/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


