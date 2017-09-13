################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/4220Lab2Part2Fixed.c 

OBJS += \
./src/4220Lab2Part2Fixed.o 

C_DEPS += \
./src/4220Lab2Part2Fixed.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gcc -I/home/students/ivnqm2/nfsroot/usr/realtime/include -I/home/students/ivnqm2/nfsroot/usr/src/linux24/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


