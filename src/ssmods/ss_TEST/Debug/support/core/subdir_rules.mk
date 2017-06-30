################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
support/core/debugtools.obj: C:/Src/UWCubeSat/DubSat1/src/dsbase/core/debugtools.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/Src/UWCubeSat/DubSat1/src/dsbase" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Src/UWCubeSat/DubSat1/src/ss_TEST" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_MSP430FR5994LaunchPad__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="support/core/debugtools.d" --obj_directory="support/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

support/core/i2c.obj: C:/Src/UWCubeSat/DubSat1/src/dsbase/core/i2c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/Src/UWCubeSat/DubSat1/src/dsbase" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Src/UWCubeSat/DubSat1/src/ss_TEST" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_MSP430FR5994LaunchPad__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="support/core/i2c.d" --obj_directory="support/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

support/core/uart.obj: C:/Src/UWCubeSat/DubSat1/src/dsbase/core/uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/Src/UWCubeSat/DubSat1/src/dsbase" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Src/UWCubeSat/DubSat1/src/ss_TEST" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_MSP430FR5994LaunchPad__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="support/core/uart.d" --obj_directory="support/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


