################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Usr/Src/user.c 

C_DEPS += \
./Usr/Src/user.d 

OBJS += \
./Usr/Src/user.o 


# Each subdirectory must supply rules for building sources it contributes
Usr/Src/%.o Usr/Src/%.su Usr/Src/%.cyclo: ../Usr/Src/%.c Usr/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Drivers/CMSIS/Include -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/Dioxider/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.3/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Usr/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Usr-2f-Src

clean-Usr-2f-Src:
	-$(RM) ./Usr/Src/user.cyclo ./Usr/Src/user.d ./Usr/Src/user.o ./Usr/Src/user.su

.PHONY: clean-Usr-2f-Src

