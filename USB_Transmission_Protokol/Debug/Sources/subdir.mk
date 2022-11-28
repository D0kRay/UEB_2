################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/USB_Packaging.c 

C_DEPS += \
./Sources/USB_Packaging.d 

OBJS += \
./Sources/USB_Packaging.o 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c Sources/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Sources

clean-Sources:
	-$(RM) ./Sources/USB_Packaging.d ./Sources/USB_Packaging.o

.PHONY: clean-Sources

