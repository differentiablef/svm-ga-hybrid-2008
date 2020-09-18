################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SVMEngine/svm_light/svm_common.c \
../SVMEngine/svm_light/svm_hideo.c \
../SVMEngine/svm_light/svm_learn.c 

C_DEPS += \
./SVMEngine/svm_light/svm_common.d \
./SVMEngine/svm_light/svm_hideo.d \
./SVMEngine/svm_light/svm_learn.d 

OBJS += \
./SVMEngine/svm_light/svm_common.o \
./SVMEngine/svm_light/svm_hideo.o \
./SVMEngine/svm_light/svm_learn.o 


# Each subdirectory must supply rules for building sources it contributes
SVMEngine/svm_light/%.o: ../SVMEngine/svm_light/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


