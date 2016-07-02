################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/rv.cpp \
../src/rv_exchange.cpp \
../src/rv_log.cpp \
../src/rv_loop.cpp \
../src/rv_reg.cpp 

OBJS += \
./src/rv.o \
./src/rv_exchange.o \
./src/rv_log.o \
./src/rv_loop.o \
./src/rv_reg.o 

CPP_DEPS += \
./src/rv.d \
./src/rv_exchange.d \
./src/rv_log.d \
./src/rv_loop.d \
./src/rv_reg.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-linux-gnueabihf-g++ -mcpu=cortex-a7 -march=armv7ve -mthumb -O2  -g -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


