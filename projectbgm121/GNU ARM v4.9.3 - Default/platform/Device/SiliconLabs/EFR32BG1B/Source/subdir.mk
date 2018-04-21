################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.c 

OBJS += \
./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o 

C_DEPS += \
./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.d 


# Each subdirectory must supply rules for building sources it contributes
platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o: ../platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-D__HEAP_SIZE=0xD00' '-DEFR32BG1B232F256GM56=1' -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib/glib" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//util/silicon_labs/silabs_core/graphics" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib/dmd" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//hardware/kit/common/drivers" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/lcd-graphics" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/lcd-graphics_inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/CMSIS/Include" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/hardware/kit/common/bsp" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/sleep/inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emlib/src" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/tempdrv/src" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/halconfig/inc/hal-config" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/common/inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emlib/inc" -I"/home/mortzula/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.1//lcd-graphics_inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/tempdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/hardware/kit/common/halconfig" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/app/bluetooth_2.7/common/stack_bridge" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/bootloader/api" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/radio/rail_lib/common" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/hardware/kit/common/drivers" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/uartdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/sleep/src" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/emdrv/gpiointerrupt/inc" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/radio/rail_lib/chip/efr32" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121" -I"/home/mortzula/Workspace/ecen_5823/projectbgm121/platform/bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.d" -MT"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


