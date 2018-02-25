################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/emdrv/tempdrv/src/tempdrv.c 

OBJS += \
./platform/emdrv/tempdrv/src/tempdrv.o 

C_DEPS += \
./platform/emdrv/tempdrv/src/tempdrv.d 


# Each subdirectory must supply rules for building sources it contributes
platform/emdrv/tempdrv/src/tempdrv.o: ../platform/emdrv/tempdrv/src/tempdrv.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__STACK_SIZE=0x800' '-DEFR32BG13P632F512GM48=1' -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/Device/SiliconLabs/EFR32BG13P/Include" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/protocol/bluetooth/ble_stack/inc/soc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/halconfig/inc/hal-config" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/protocol/bluetooth/ble_stack/inc/common" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emlib/src" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/hardware/kit/common/drivers" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emlib/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/CMSIS/Include" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/app/bluetooth/common/stack_bridge" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/hardware/kit/common/halconfig" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/hardware/kit/common/bsp" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/Device/SiliconLabs/EFR32BG13P/Source" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/tempdrv/src" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/radio/rail_lib/common" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/sleep/src" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/tempdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/sleep/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/uartdrv/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/hardware/kit/EFR32BG13_BRD4104A/config" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/gpiointerrupt/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/emdrv/common/inc" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/radio/rail_lib/chip/efr32" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/bootloader/api" -I"/home/mortzula/Workspace/ecen_5823/soc-empty" -I"/home/mortzula/Workspace/ecen_5823/soc-empty/platform/bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/emdrv/tempdrv/src/tempdrv.d" -MT"platform/emdrv/tempdrv/src/tempdrv.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


