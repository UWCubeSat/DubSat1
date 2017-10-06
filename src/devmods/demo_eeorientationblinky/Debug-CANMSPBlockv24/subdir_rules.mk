################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
SUBSYSTEMNAME_main.obj: ../SUBSYSTEMNAME_main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="D:/Repos/DubSat1/src/dsbase" --include_path="D:/Repos/DubSat1/src/devmods/demo_eeorientationblinky" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/include" --advice:power=all --advice:hw_config="all" --define=__MSP430FR5994__ --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_BMI160__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_MSP430FR5994_CANMSPBlockv24__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="SUBSYSTEMNAME_main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


