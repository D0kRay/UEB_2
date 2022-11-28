################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Communication/com_provider.c \
../Drivers/Communication/communication.c 

OBJS += \
./Drivers/Communication/com_provider.o \
./Drivers/Communication/communication.o 

C_DEPS += \
./Drivers/Communication/com_provider.d \
./Drivers/Communication/communication.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Communication/%.o Drivers/Communication/%.su: ../Drivers/Communication/%.c Drivers/Communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/Communication -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Communication

clean-Drivers-2f-Communication:
	-$(RM) ./Drivers/Communication/com_provider.d ./Drivers/Communication/com_provider.o ./Drivers/Communication/com_provider.su ./Drivers/Communication/communication.d ./Drivers/Communication/communication.o ./Drivers/Communication/communication.su

.PHONY: clean-Drivers-2f-Communication

