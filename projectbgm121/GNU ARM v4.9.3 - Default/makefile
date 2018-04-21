################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/tempdrv/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/subdir.mk
-include lcd-graphics/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: projectbgm121.axf

# Tool invocations
projectbgm121.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/home/mortzula/Workspace/ecen_5823/projectbgm121/efr32bg1b232f256gm56.ld" -Xlinker --gc-sections -Xlinker -Map="projectbgm121.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o projectbgm121.axf -Wl,--start-group "./application_properties.o" "./gatt_db.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./lcd-graphics/bmp.o" "./lcd-graphics/display.o" "./lcd-graphics/displayls013b7dh03.o" "./lcd-graphics/displaypalemlib.o" "./lcd-graphics/dmd_display.o" "./lcd-graphics/glib.o" "./lcd-graphics/glib_bitmap.o" "./lcd-graphics/glib_circle.o" "./lcd-graphics/glib_font_narrow_6x8.o" "./lcd-graphics/glib_font_normal_8x8.o" "./lcd-graphics/glib_font_number_16x20.o" "./lcd-graphics/glib_line.o" "./lcd-graphics/glib_rectangle.o" "./lcd-graphics/glib_string.o" "./lcd-graphics/graphics.o" "./lcd-graphics/udelay.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/startup_efr32bg1b.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emdrv/tempdrv/src/tempdrv.o" "./platform/emlib/src/em_adc.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_letimer.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_usart.o" "./src/cmu.o" "./src/events.o" "./src/gpio.o" "./src/letimer.o" "./src/lightsens.o" "./src/persistent_data.o" "./src/power_level.o" "./src/setters.o" "./src/shared_resources.o" "./src/soil_moisture.o" "./src/tempsense.o" "/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/radio/rail_lib/autogen/librail_release/librail_efr32xg1_gcc_release.a" "/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/libpsstore.a" "/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/libbluetooth.a" "/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/libmbedtls.a" "/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/binapploader.o" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: projectbgm121.hex'
	arm-none-eabi-objcopy -O ihex "projectbgm121.axf" "projectbgm121.hex"
	@echo ' '

	@echo 'Building bin file: projectbgm121.bin'
	arm-none-eabi-objcopy -O binary "projectbgm121.axf" "projectbgm121.bin"
	@echo ' '

	@echo 'Building s37 file: projectbgm121.s37'
	arm-none-eabi-objcopy -O srec "projectbgm121.axf" "projectbgm121.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "projectbgm121.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) projectbgm121.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
