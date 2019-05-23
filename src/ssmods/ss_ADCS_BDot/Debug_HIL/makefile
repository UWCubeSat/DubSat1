################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./adcs_bdot_main.obj" \
"./bdot_controller_lib.obj" \
"./bdot_controller_lib_data.obj" \
"./ert_main.obj" \
"./dsbase/actuators/rw.obj" \
"./dsbase/bsp/bsp.obj" \
"./dsbase/config/config_bus_uart.obj" \
"./dsbase/core/MET.obj" \
"./dsbase/core/agglib.obj" \
"./dsbase/core/autosequence.obj" \
"./dsbase/core/can.obj" \
"./dsbase/core/debugtools.obj" \
"./dsbase/core/i2c.obj" \
"./dsbase/core/spi.obj" \
"./dsbase/core/timer.obj" \
"./dsbase/core/timers.obj" \
"./dsbase/core/uart.obj" \
"./dsbase/interfaces/canwrap.obj" \
"./dsbase/interfaces/rollcall.obj" \
"./dsbase/interfaces/systeminfo.obj" \
"./dsbase/sensors/analogsensor.obj" \
"./dsbase/sensors/coulomb_counter.obj" \
"./dsbase/sensors/imu.obj" \
"./dsbase/sensors/magnetometer.obj" \
"./dsbase/sensors/pcvsensor.obj" \
"./dsbase/sensors/photodiode.obj" \
"./dsbase/sensors/sun_sensor.obj" \
"./dsbase/sensors/gps/BufferedReader.obj" \
"./dsbase/sensors/gps/crc.obj" \
"./dsbase/sensors/gps/gps.obj" \
"../lnk_msp430fr5994.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
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
-include dsbase/sensors/gps/subdir_vars.mk
-include subdir_rules.mk
-include dsbase/actuators/subdir_rules.mk
-include dsbase/bsp/subdir_rules.mk
-include dsbase/config/subdir_rules.mk
-include dsbase/core/subdir_rules.mk
-include dsbase/interfaces/subdir_rules.mk
-include dsbase/sensors/subdir_rules.mk
-include dsbase/sensors/gps/subdir_rules.mk
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
ss_ADCS_BDot.out \

EXE_OUTPUTS__QUOTED += \
"ss_ADCS_BDot.out" \

BIN_OUTPUTS += \
ss_ADCS_BDot.txt \

BIN_OUTPUTS__QUOTED += \
"ss_ADCS_BDot.txt" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ss_ADCS_BDot.out" secondary-outputs

# Tool invocations
ss_ADCS_BDot.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv7/tools/compiler/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=__SS_ADCS_BDOT__ --define=__DEBUG_UART_SPEED_9600__ --define=__INITIAL_DEBUG_MODE__=2 --define=__INITIAL_TRACE_LEVEL__=1 --define=__BSP_HW_IMU_LSM6DSM__ --define=__BSP_HW_MAGTOM_HMC5883L__ --define=__BSP_Board_SS__ --define=__DEBUG__ --define=__HIL_AA_NOEXTRAREADS__ --define=__HIL_AA_GLITCHFILTER__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"ss_ADCS_BDot.map" --heap_size=160 --stack_size=1024 --cinit_hold_wdt=on -i"C:/ti/ccsv7/ccs_base/msp430/include" -i"C:/ti/ccsv7/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccsv7/ccs_base/msp430/lib/FR59xx" -i"C:/ti/ccsv7/tools/compiler/lib" -i"C:/ti/ccsv7/tools/compiler/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ss_ADCS_BDot_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "ss_ADCS_BDot.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

ss_ADCS_BDot.txt: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/bin/hex430" --memwidth=8 --romwidth=8 --ti_txt -o "ss_ADCS_BDot.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "adcs_bdot_main.obj" "bdot_controller_lib.obj" "bdot_controller_lib_data.obj" "ert_main.obj" "dsbase\actuators\rw.obj" "dsbase\bsp\bsp.obj" "dsbase\config\config_bus_uart.obj" "dsbase\core\MET.obj" "dsbase\core\agglib.obj" "dsbase\core\autosequence.obj" "dsbase\core\can.obj" "dsbase\core\debugtools.obj" "dsbase\core\i2c.obj" "dsbase\core\spi.obj" "dsbase\core\timer.obj" "dsbase\core\timers.obj" "dsbase\core\uart.obj" "dsbase\interfaces\canwrap.obj" "dsbase\interfaces\rollcall.obj" "dsbase\interfaces\systeminfo.obj" "dsbase\sensors\analogsensor.obj" "dsbase\sensors\coulomb_counter.obj" "dsbase\sensors\imu.obj" "dsbase\sensors\magnetometer.obj" "dsbase\sensors\pcvsensor.obj" "dsbase\sensors\photodiode.obj" "dsbase\sensors\sun_sensor.obj" "dsbase\sensors\gps\BufferedReader.obj" "dsbase\sensors\gps\crc.obj" "dsbase\sensors\gps\gps.obj" 
	-$(RM) "adcs_bdot_main.d" "bdot_controller_lib.d" "bdot_controller_lib_data.d" "ert_main.d" "dsbase\actuators\rw.d" "dsbase\bsp\bsp.d" "dsbase\config\config_bus_uart.d" "dsbase\core\MET.d" "dsbase\core\agglib.d" "dsbase\core\autosequence.d" "dsbase\core\can.d" "dsbase\core\debugtools.d" "dsbase\core\i2c.d" "dsbase\core\spi.d" "dsbase\core\timer.d" "dsbase\core\timers.d" "dsbase\core\uart.d" "dsbase\interfaces\canwrap.d" "dsbase\interfaces\rollcall.d" "dsbase\interfaces\systeminfo.d" "dsbase\sensors\analogsensor.d" "dsbase\sensors\coulomb_counter.d" "dsbase\sensors\imu.d" "dsbase\sensors\magnetometer.d" "dsbase\sensors\pcvsensor.d" "dsbase\sensors\photodiode.d" "dsbase\sensors\sun_sensor.d" "dsbase\sensors\gps\BufferedReader.d" "dsbase\sensors\gps\crc.d" "dsbase\sensors\gps\gps.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

