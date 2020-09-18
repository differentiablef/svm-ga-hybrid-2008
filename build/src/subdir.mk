################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/Chrom1DBinary.cc \
../src/Chrom1DTernary.cc \
../src/Chromosome.cc \
../src/LifeForm.cc 

OBJS += \
./src/Chrom1DBinary.o \
./src/Chrom1DTernary.o \
./src/Chromosome.o \
./src/LifeForm.o 

CC_DEPS += \
./src/Chrom1DBinary.d \
./src/Chrom1DTernary.d \
./src/Chromosome.d \
./src/LifeForm.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


