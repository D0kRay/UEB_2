################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EventQueue/Src/EventQueue.c 

OBJS += \
./EventQueue/Src/EventQueue.o 

C_DEPS += \
./EventQueue/Src/EventQueue.d 


# Each subdirectory must supply rules for building sources it contributes
EventQueue/Src/%.o EventQueue/Src/%.su: ../EventQueue/Src/%.c EventQueue/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/Communication -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../DT_protocol/Inc -I../EventQueue/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EventQueue-2f-Src

clean-EventQueue-2f-Src:
	-$(RM) ./EventQueue/Src/EventQueue.d ./EventQueue/Src/EventQueue.o ./EventQueue/Src/EventQueue.su

.PHONY: clean-EventQueue-2f-Src

