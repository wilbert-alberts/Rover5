################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PID.cpp \
../src/POS.cpp \
../src/RoverPID.cpp 

OBJS += \
./src/PID.o \
./src/POS.o \
./src/RoverPID.o 

CPP_DEPS += \
./src/PID.d \
./src/POS.d \
./src/RoverPID.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../RoverLib/include -I../../Reflexxes/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


