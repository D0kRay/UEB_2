################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DT_protocol/Src/DT_interface.c \
../DT_protocol/Src/DT_memory.c \
../DT_protocol/Src/DT_runable.c 

OBJS += \
./DT_protocol/Src/DT_interface.o \
./DT_protocol/Src/DT_memory.o \
./DT_protocol/Src/DT_runable.o 

C_DEPS += \
./DT_protocol/Src/DT_interface.d \
./DT_protocol/Src/DT_memory.d \
./DT_protocol/Src/DT_runable.d 


# Each subdirectory must supply rules for building sources it contributes
DT_protocol/Src/%.o DT_protocol/Src/%.su: ../DT_protocol/Src/%.c DT_protocol/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/Communication -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../DT_protocol/Inc -I../EventQueue/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-DT_protocol-2f-Src

clean-DT_protocol-2f-Src:
	-$(RM) ./DT_protocol/Src/DT_interface.d ./DT_protocol/Src/DT_interface.o ./DT_protocol/Src/DT_interface.su ./DT_protocol/Src/DT_memory.d ./DT_protocol/Src/DT_memory.o ./DT_protocol/Src/DT_memory.su ./DT_protocol/Src/DT_runable.d ./DT_protocol/Src/DT_runable.o ./DT_protocol/Src/DT_runable.su

.PHONY: clean-DT_protocol-2f-Src

