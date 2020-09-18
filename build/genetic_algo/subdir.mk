################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../genetic_algo/ga_main.cc 

OBJS += \
./genetic_algo/ga_main.o 

CC_DEPS += \
./genetic_algo/ga_main.d 


# Each subdirectory must supply rules for building sources it contributes
genetic_algo/%.o: ../genetic_algo/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


