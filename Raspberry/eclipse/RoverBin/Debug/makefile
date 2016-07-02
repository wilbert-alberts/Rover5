################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
RoverBin.hex \

SECONDARY_SIZE += \
RoverBin.siz \


# All Target
all: RoverBin

# Tool invocations
RoverBin: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-linux-gnueabihf-g++ -mcpu=cortex-a7 -march=armv7ve -mthumb -O2  -g -Xlinker --gc-sections -L"C:\Localdata\Work\Innovationteam\OOTIProject2016\Rover5\Raspberry\eclipse\RoverLib\Debug" -L../../RoverLib/Debug -Wl,-Map,"RoverBin.map" -o "RoverBin" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

RoverBin.hex: RoverBin
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-linux-gnueabihf-objcopy -O ihex "RoverBin"  "RoverBin.hex"
	@echo 'Finished building: $@'
	@echo ' '

RoverBin.siz: RoverBin
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-linux-gnueabihf-size --format=berkeley "RoverBin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(SECONDARY_SIZE)$(PDE_DEPS)$(C_DEPS)$(CC_DEPS)$(C++_DEPS)$(OBJS)$(CXX_DEPS)$(SECONDARY_FLASH)$(ASM_DEPS)$(INO_DEPS)$(S_UPPER_DEPS)$(CPP_DEPS) RoverBin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
