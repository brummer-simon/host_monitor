################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Endpoint.cpp \
../src/HostMonitor.cpp \
../src/TestConnection.cpp 

OBJS += \
./src/Endpoint.o \
./src/HostMonitor.o \
./src/TestConnection.o 

CPP_DEPS += \
./src/Endpoint.d \
./src/HostMonitor.d \
./src/TestConnection.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I../include -Isrc/include -O3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


