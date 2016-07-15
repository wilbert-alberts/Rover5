################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/rv.cpp \
../src/rv_exchange.cpp \
../src/rv_log.cpp \
../src/rv_loop.cpp \
../src/rv_reg.cpp \
../src/rv_trace.cpp 

OBJS += \
./src/rv.o \
./src/rv_exchange.o \
./src/rv_log.o \
./src/rv_loop.o \
./src/rv_reg.o \
./src/rv_trace.o 

CPP_DEPS += \
./src/rv.d \
./src/rv_exchange.d \
./src/rv_log.d \
./src/rv_loop.d \
./src/rv_reg.d \
./src/rv_trace.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I../../wiringPi/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


