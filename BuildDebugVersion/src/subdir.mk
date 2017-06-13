################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HostMonitor.cpp \
../src/HostMonitorImpl.cpp \
../src/TestConnection.cpp 

OBJS += \
./src/HostMonitor.o \
./src/HostMonitorImpl.o \
./src/TestConnection.o 

CPP_DEPS += \
./src/HostMonitor.d \
./src/HostMonitorImpl.d \
./src/TestConnection.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../include -Isrc/include -O0 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


