################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./SUBSYSTEMNAME_main.obj" \
"./dsbase/actuators/rw.obj" \
"./dsbase/bsp/bsp.obj" \
"./dsbase/config/config_bus_uart.obj" \
"./dsbase/core/analogread.obj" \
"./dsbase/core/can.obj" \
"./dsbase/core/debugtools.obj" \
"./dsbase/core/i2c.obj" \
"./dsbase/core/spi.obj" \
"./dsbase/core/timers.obj" \
"./dsbase/core/uart.obj" \
"./dsbase/interfaces/canwrap.obj" \
"./dsbase/interfaces/systeminfo.obj" \
"./dsbase/sensors/imu.obj" \
"./dsbase/sensors/magnetometer.obj" \
"../lnk_msp430fr5994.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibmpu_init.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include dsbase/actuators/subdir_vars.mk
-include dsbase/bsp/subdir_vars.mk
-include dsbase/config/subdir_vars.mk
-include dsbase/core/subdir_vars.mk
-include dsbase/interfaces/subdir_vars.mk
-include dsbase/sensors/subdir_vars.mk
-include subdir_rules.mk
-include dsbase/actuators/subdir_rules.mk
-include dsbase/bsp/subdir_rules.mk
-include dsbase/config/subdir_rules.mk
-include dsbase/core/subdir_rules.mk
-include dsbase/interfaces/subdir_rules.mk
-include dsbase/sensors/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
demo_eeorientationblinky.out \

EXE_OUTPUTS__QUOTED += \
"demo_eeorientationblinky.out" \

BIN_OUTPUTS += \
demo_eeorientationblinky.txt \

BIN_OUTPUTS__QUOTED += \
"demo_eeorientationblinky.txt" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "demo_eeorientationblinky.out" secondary-outputs

# Tool invocations
demo_eeorientationblinky.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config="all" --define=__MSP430FR5994__ --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_BMI160__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_MSP430FR5994_CANMSPBlockv24__ --define=__DEBUG__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"demo_eeorientationblinky.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccsv7/ccs_base/msp430/include" -i"C:/ti/ccsv7/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccsv7/ccs_base/msp430/lib/FR59xx" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/lib" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="demo_eeorientationblinky_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "demo_eeorientationblinky.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

demo_eeorientationblinky.txt: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_17.6.0.STS/bin/hex430" --memwidth=8 --romwidth=8 --ti_txt -o "demo_eeorientationblinky.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "SUBSYSTEMNAME_main.obj" "dsbase\actuators\rw.obj" "dsbase\bsp\bsp.obj" "dsbase\config\config_bus_uart.obj" "dsbase\core\analogread.obj" "dsbase\core\can.obj" "dsbase\core\debugtools.obj" "dsbase\core\i2c.obj" "dsbase\core\spi.obj" "dsbase\core\timers.obj" "dsbase\core\uart.obj" "dsbase\interfaces\canwrap.obj" "dsbase\interfaces\systeminfo.obj" "dsbase\sensors\imu.obj" "dsbase\sensors\magnetometer.obj" 
	-$(RM) "SUBSYSTEMNAME_main.d" "dsbase\actuators\rw.d" "dsbase\bsp\bsp.d" "dsbase\config\config_bus_uart.d" "dsbase\core\analogread.d" "dsbase\core\can.d" "dsbase\core\debugtools.d" "dsbase\core\i2c.d" "dsbase\core\spi.d" "dsbase\core\timers.d" "dsbase\core\uart.d" "dsbase\interfaces\canwrap.d" "dsbase\interfaces\systeminfo.d" "dsbase\sensors\imu.d" "dsbase\sensors\magnetometer.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

