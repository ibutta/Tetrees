################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/TetreesEngine.cpp \
../Source/TetreesMainApp.cpp \
../Source/TetreesUI.cpp \
../Source/TetreesUtils.cpp 

OBJS += \
./Source/TetreesEngine.o \
./Source/TetreesMainApp.o \
./Source/TetreesUI.o \
./Source/TetreesUtils.o 

CPP_DEPS += \
./Source/TetreesEngine.d \
./Source/TetreesMainApp.d \
./Source/TetreesUI.d \
./Source/TetreesUtils.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/mirclient -I/usr/include/mircommon -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gio-unix-2.0/ -I"/home/igor/eclipse-cpp-workspace/Tetrees/Headers" -I"/home/igor/eclipse-cpp-workspace/Tetrees/Misc" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


