################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Source/Driver/UART/%.o: ../Source/Driver/UART/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/BSP" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/MIRTOS/inc" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/CMSIS" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/Common" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/Interface" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test" -I"C:/ti/ccs1220/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos/portable/Common" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos/portable/GCC/ARM_CM4F" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos/portable/MemMang" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos/include" -I"C:/Users/Saksham Raj/workspace_v12/Drone_test/Source/FreeRtos/portable" -O0 -ffunction-sections -fdata-sections -g -gdwarf-3 -Wall -specs="nosys.specs" -MMD -MP -MF"Source/Driver/UART/$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

