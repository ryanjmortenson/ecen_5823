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
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/mortzula/Workspace/ecen_5823/project" -I"/home/mortzula/Workspace/ecen_5823/project/platform/radio/rail_lib/chip/efr32" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/gpiointerrupt/inc" -I"/home/mortzula/Workspace/ecen_5823/project/hardware/kit/common/halconfig" -I"/home/mortzula/Workspace/ecen_5823/project/hardware/kit/common/bsp" -I"/home/mortzula/Workspace/ecen_5823/project/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/mortzula/Workspace/ecen_5823/project/platform/CMSIS/Include" -I"/home/mortzula/Workspace/ecen_5823/project/hardware/kit/common/drivers" -I"/home/mortzula/Workspace/ecen_5823/project/platform/bootloader/api" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emlib/src" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emlib/inc" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/uartdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/tempdrv/src" -I"/home/mortzula/Workspace/ecen_5823/project/platform/halconfig/inc/hal-config" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/sleep/inc" -I"/home/mortzula/Workspace/ecen_5823/project/platform/radio/rail_lib/common" -I"/home/mortzula/Workspace/ecen_5823/project/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/common/inc" -I"/home/mortzula/Workspace/ecen_5823/project/app/bluetooth_2.7/common/stack_bridge" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/tempdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/project/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/mortzula/Workspace/ecen_5823/project/platform/emdrv/sleep/src" -I"/home/mortzula/Workspace/ecen_5823/project/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/mortzula/Workspace/ecen_5823/project/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/mortzula/Workspace/ecen_5823/project/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/mortzula/Workspace/ecen_5823/project/platform/bootloader" -I"//home/mortzula/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/mortzula/Workspace/ecen_5823/project/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.d" -MT"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


