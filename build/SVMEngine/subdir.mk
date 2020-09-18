################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SVMEngine/chrom_tools.c \
../SVMEngine/svm_engine.c \
../SVMEngine/svm_record_tools.c 

C_DEPS += \
./SVMEngine/chrom_tools.d \
./SVMEngine/svm_engine.d \
./SVMEngine/svm_record_tools.d 

OBJS += \
./SVMEngine/chrom_tools.o \
./SVMEngine/svm_engine.o \
./SVMEngine/svm_record_tools.o 


# Each subdirectory must supply rules for building sources it contributes
SVMEngine/%.o: ../SVMEngine/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


