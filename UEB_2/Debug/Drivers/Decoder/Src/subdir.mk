################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Decoder/Src/decoder.c 

OBJS += \
./Drivers/Decoder/Src/decoder.o 

C_DEPS += \
./Drivers/Decoder/Src/decoder.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Decoder/Src/%.o Drivers/Decoder/Src/%.su: ../Drivers/Decoder/Src/%.c Drivers/Decoder/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/Communication -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../DT_protocol/Inc -I../EventQueue/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Decoder-2f-Src

clean-Drivers-2f-Decoder-2f-Src:
	-$(RM) ./Drivers/Decoder/Src/decoder.d ./Drivers/Decoder/Src/decoder.o ./Drivers/Decoder/Src/decoder.su

.PHONY: clean-Drivers-2f-Decoder-2f-Src

