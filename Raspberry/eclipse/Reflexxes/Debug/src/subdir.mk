################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ReflexxesAPI.cpp \
../src/TypeIIRMLCalculatePositionalExtrems.cpp \
../src/TypeIIRMLDecisionTree1A.cpp \
../src/TypeIIRMLDecisionTree1B.cpp \
../src/TypeIIRMLDecisionTree1C.cpp \
../src/TypeIIRMLDecisionTree2.cpp \
../src/TypeIIRMLDecisions.cpp \
../src/TypeIIRMLFallBackStrategy.cpp \
../src/TypeIIRMLIsPhaseSynchronizationPossible.cpp \
../src/TypeIIRMLPolynomial.cpp \
../src/TypeIIRMLPosition.cpp \
../src/TypeIIRMLQuicksort.cpp \
../src/TypeIIRMLSetupModifiedSelectionVector.cpp \
../src/TypeIIRMLStep1.cpp \
../src/TypeIIRMLStep1IntermediateProfiles.cpp \
../src/TypeIIRMLStep1Profiles.cpp \
../src/TypeIIRMLStep2.cpp \
../src/TypeIIRMLStep2IntermediateProfiles.cpp \
../src/TypeIIRMLStep2PhaseSynchronization.cpp \
../src/TypeIIRMLStep2Profiles.cpp \
../src/TypeIIRMLStep2WithoutSynchronization.cpp \
../src/TypeIIRMLStep3.cpp \
../src/TypeIIRMLVelocity.cpp \
../src/TypeIIRMLVelocityCalculatePositionalExtrems.cpp \
../src/TypeIIRMLVelocityFallBackStrategy.cpp \
../src/TypeIIRMLVelocityIsPhaseSynchronizationPossible.cpp \
../src/TypeIIRMLVelocityMethods.cpp \
../src/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.cpp 

OBJS += \
./src/ReflexxesAPI.o \
./src/TypeIIRMLCalculatePositionalExtrems.o \
./src/TypeIIRMLDecisionTree1A.o \
./src/TypeIIRMLDecisionTree1B.o \
./src/TypeIIRMLDecisionTree1C.o \
./src/TypeIIRMLDecisionTree2.o \
./src/TypeIIRMLDecisions.o \
./src/TypeIIRMLFallBackStrategy.o \
./src/TypeIIRMLIsPhaseSynchronizationPossible.o \
./src/TypeIIRMLPolynomial.o \
./src/TypeIIRMLPosition.o \
./src/TypeIIRMLQuicksort.o \
./src/TypeIIRMLSetupModifiedSelectionVector.o \
./src/TypeIIRMLStep1.o \
./src/TypeIIRMLStep1IntermediateProfiles.o \
./src/TypeIIRMLStep1Profiles.o \
./src/TypeIIRMLStep2.o \
./src/TypeIIRMLStep2IntermediateProfiles.o \
./src/TypeIIRMLStep2PhaseSynchronization.o \
./src/TypeIIRMLStep2Profiles.o \
./src/TypeIIRMLStep2WithoutSynchronization.o \
./src/TypeIIRMLStep3.o \
./src/TypeIIRMLVelocity.o \
./src/TypeIIRMLVelocityCalculatePositionalExtrems.o \
./src/TypeIIRMLVelocityFallBackStrategy.o \
./src/TypeIIRMLVelocityIsPhaseSynchronizationPossible.o \
./src/TypeIIRMLVelocityMethods.o \
./src/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.o 

CPP_DEPS += \
./src/ReflexxesAPI.d \
./src/TypeIIRMLCalculatePositionalExtrems.d \
./src/TypeIIRMLDecisionTree1A.d \
./src/TypeIIRMLDecisionTree1B.d \
./src/TypeIIRMLDecisionTree1C.d \
./src/TypeIIRMLDecisionTree2.d \
./src/TypeIIRMLDecisions.d \
./src/TypeIIRMLFallBackStrategy.d \
./src/TypeIIRMLIsPhaseSynchronizationPossible.d \
./src/TypeIIRMLPolynomial.d \
./src/TypeIIRMLPosition.d \
./src/TypeIIRMLQuicksort.d \
./src/TypeIIRMLSetupModifiedSelectionVector.d \
./src/TypeIIRMLStep1.d \
./src/TypeIIRMLStep1IntermediateProfiles.d \
./src/TypeIIRMLStep1Profiles.d \
./src/TypeIIRMLStep2.d \
./src/TypeIIRMLStep2IntermediateProfiles.d \
./src/TypeIIRMLStep2PhaseSynchronization.d \
./src/TypeIIRMLStep2Profiles.d \
./src/TypeIIRMLStep2WithoutSynchronization.d \
./src/TypeIIRMLStep3.d \
./src/TypeIIRMLVelocity.d \
./src/TypeIIRMLVelocityCalculatePositionalExtrems.d \
./src/TypeIIRMLVelocityFallBackStrategy.d \
./src/TypeIIRMLVelocityIsPhaseSynchronizationPossible.d \
./src/TypeIIRMLVelocityMethods.d \
./src/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"..\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


