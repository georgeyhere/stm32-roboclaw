################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/roboclaw/roboclaw.c 

C_DEPS += \
./Core/Inc/roboclaw/roboclaw.d 

OBJS += \
./Core/Inc/roboclaw/roboclaw.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/roboclaw/%.o Core/Inc/roboclaw/%.su: ../Core/Inc/roboclaw/%.c Core/Inc/roboclaw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/george/Documents/repos/stm32-roboclaw/roboclaw-dev/Core/Inc/roboclaw" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-roboclaw

clean-Core-2f-Inc-2f-roboclaw:
	-$(RM) ./Core/Inc/roboclaw/roboclaw.d ./Core/Inc/roboclaw/roboclaw.o ./Core/Inc/roboclaw/roboclaw.su

.PHONY: clean-Core-2f-Inc-2f-roboclaw

