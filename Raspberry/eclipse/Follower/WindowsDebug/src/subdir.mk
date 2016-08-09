################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/actuator.cpp \
../src/controller.cpp \
../src/filter.cpp \
../src/main.cpp \
../src/pid.cpp \
../src/sensor.cpp \
../src/tracing.cpp 

OBJS += \
./src/actuator.o \
./src/controller.o \
./src/filter.o \
./src/main.o \
./src/pid.o \
./src/sensor.o \
./src/tracing.o 

CPP_DEPS += \
./src/actuator.d \
./src/controller.d \
./src/filter.d \
./src/main.d \
./src/pid.d \
./src/sensor.d \
./src/tracing.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I../../RoverLib/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


