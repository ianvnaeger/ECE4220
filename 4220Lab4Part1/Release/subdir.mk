################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../4220Lab4Part1.c \
../serial_ece4220.c 

OBJS += \
./4220Lab4Part1.o \
./serial_ece4220.o 

C_DEPS += \
./4220Lab4Part1.d \
./serial_ece4220.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gcc -I/home/students/ivnqm2/nfsroot/usr/realtime/include -I/home/students/ivnqm2/nfsroot/usr/src/linux24/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


