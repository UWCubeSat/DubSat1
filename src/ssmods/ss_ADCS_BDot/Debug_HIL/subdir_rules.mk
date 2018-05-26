################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
adcs_bdot_main.obj: ../adcs_bdot_main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/ssmods/ss_ADCS_BDot" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__SS_ADCS_BDOT__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="adcs_bdot_main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

bdot_controller_lib.obj: ../bdot_controller_lib.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/ssmods/ss_ADCS_BDot" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__SS_ADCS_BDOT__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="bdot_controller_lib.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

bdot_controller_lib_data.obj: ../bdot_controller_lib_data.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/ssmods/ss_ADCS_BDot" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__SS_ADCS_BDOT__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="bdot_controller_lib_data.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ert_main.obj: ../ert_main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/dsbase" --include_path="C:/Users/Thu Phan/Documents/2017-2018/DubSat1/src/ssmods/ss_ADCS_BDot" --include_path="C:/ti/ccsv7/tools/compiler/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__SS_ADCS_BDOT__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ert_main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


