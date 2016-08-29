###############################################################################
# "THE BEER-WARE LICENSE" (Revision 42):
# <msmith@FreeBSD.ORG> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a beer in return
###############################################################################
#
# Makefile for building the baseflight firmware.
#
# Invoke this with 'make help' to see the list of supported targets.
# 

###############################################################################
# Things that the user might override on the commandline
#

# The target to build, must be one of NAZE, FY90Q OR OLIMEXINO
TARGET		?= SKYROVER

# Compile-time options
OPTIONS		?=

# Debugger optons, must be empty or GDB
DEBUG ?=

# BootLoader options.
BOOT ?= 1


# Serial port/Device for flashing
SERIAL_DEVICE	?= /dev/ttyUSB0

###############################################################################
# Things that need to be maintained as the source changes
#

VALID_TARGETS	 = SKYROVER

# Working directories
ROOT		 = $(dir $(lastword $(MAKEFILE_LIST)))
CUPDRONEAPI_DIR = $(ROOT)/cupdroneAPI
LIB_DIR		 = $(ROOT)/lib
SRC_DIR		 = $(ROOT)/src
CMSIS_DIR	 = $(ROOT)/lib/CMSIS
STDPERIPH_DIR	 = $(ROOT)/lib/STM32F10x_StdPeriph_Driver
OBJECT_DIR	 = $(ROOT)/obj
BIN_DIR		 = $(ROOT)/obj
RTOS_DIR	 = $(ROOT)/lib/FreeRTOS
U8GLIB_DIR	 = $(ROOT)/lib/u8glib

HARDWARESERIAL_DIR = $(ROOT)/src/hardwareSerial
MODECONTROLLER_DIR = $(ROOT)/src/modeController
COMMANDPARSER_DIR = $(ROOT)/src/commandParser
GLOBALCLASS_DIR = $(ROOT)/src/globalClass
POSMODE_DIR = $(ROOT)/src/posMode
ATTITUDEMODE_DIR = $(ROOT)/src/attitudeMode
ACTUATORMODE_DIR = $(ROOT)/src/actuatorMode
ESTIMATION_DIR = $(ROOT)/src/estimation
INERTIALSENSOR_DIR = $(ROOT)/src/inertialSensor
MOTOR_DIR = $(ROOT)/src/motor

# Source files for SkyRover
SKYROVER_SRC	= startup_stm32f10x_md_gcc.s \
		   $(CUPDRONEAPI_SRC) \
		   $(CUPDRONEAPICPP_SRC) \
		   $(CMSIS_SRC) \
		   $(STDPERIPH_SRC) \
		   $(RTOS_1_SRC) \
		   $(RTOS_2_SRC) \
		   $(RTOS_3_SRC) \
		   $(USB_1_SRC)  \
		   $(USB_2_SRC)	 \
		   $(THREAD_SRC) \
		   $(U8GLIB_SRC) \
		   $(MAIN_SRC) \
		   $(MAINCPP_SRC) \
		   $(HARDWARESERIAL_SRC) \
		   $(HARDWARESERIALCPP_SRC) \
		   $(MODECONTROLLER_SRC) \
		   $(MODECONTROLLERCPP_SRC) \
		   $(COMMANDPARSER_SRC) \
		   $(COMMANDPARSERCPP_SRC) \
		   $(GLOBALCLASS_SRC) \
		   $(GLOBALCLASSCPP_SRC) \
		   $(POSMODE_SRC) \
		   $(POSMODECPP_SRC) \
		   $(ATTITUDEMODE_SRC) \
		   $(ATTITUDEMODECPP_SRC) \
		   $(ACTUATORMODE_SRC) \
		   $(ACTUATORMODECPP_SRC) \
		   $(ESTIMATION_SRC) \
		   $(ESTIMATIONCPP_SRC) \
		   $(INERTIALSENSOR_SRC) \
		   $(INERTIALSENSORCPP_SRC) \
		   $(MOTOR_SRC) \
		   $(MOTORCPP_SRC) \

RTOS_SRC		= cmsis_os.c \
		   port.c \
		   croutine.c \
		   list.c \
		   queue.c \
		   tasks.c \
		   timers.c 



		   
# In some cases, %.s regarded as intermediate file, which is actually not.
# This will prevent accidental deletion of startup code.
.PRECIOUS: %.s

# Search path for baseflight sources
#VPATH		:= $(SRC_DIR):$(SRC_DIR)/baseflight_startups
#VPATH		:= $(SRC_DIR)
VPATH		:= $(CUPDRONEAPI_DIR)
CUPDRONEAPI_SRC 	= $(notdir $(wildcard $(CUPDRONEAPI_DIR)/*.c))
CUPDRONEAPICPP_SRC 	= $(notdir $(wildcard $(CUPDRONEAPI_DIR)/*.cpp))

# Search path and source files for the CMSIS sources
VPATH		:= $(VPATH):$(CMSIS_DIR)/Device/ST/STM32F10x
#CMSIS_SRC	 = $(notdir $(wildcard $(CMSIS_DIR)/CM3/CoreSupport/*.c \
#			               $(CMSIS_DIR)/CM3/DeviceSupport/ST/STM32F10x/*.c))
CMSIS_SRC	 = $(notdir $(wildcard $(CMSIS_DIR)/Device/ST/STM32F10x/*.c))

# Search path and source files for the ST stdperiph library
VPATH		:= $(VPATH):$(STDPERIPH_DIR)/src
STDPERIPH_SRC	 = $(notdir $(wildcard $(STDPERIPH_DIR)/src/*.c))

# Search path and source files for FreeRTOS
VPATH		:= $(VPATH):$(RTOS_DIR)/Source
RTOS_1_SRC	 = $(notdir $(wildcard $(RTOS_DIR)/Source/*.c))
VPATH		:= $(VPATH):$(RTOS_DIR)/Source/CMSIS_RTOS
RTOS_2_SRC	 = $(notdir $(wildcard $(RTOS_DIR)/Source/CMSIS_RTOS/*.c))
VPATH		:= $(VPATH):$(RTOS_DIR)/Source/portable/GCC/ARM_CM3
RTOS_3_SRC	 = $(notdir $(wildcard $(RTOS_DIR)/Source/portable/GCC/ARM_CM3/*.c))


# Search path and source files for USB
VPATH		:= $(VPATH):$(LIB_DIR)/STM32_USB-FS-Device_Driver/src
USB_1_SRC	 = $(notdir $(wildcard $(LIB_DIR)/STM32_USB-FS-Device_Driver/src/*.c))
VPATH		:= $(VPATH):$(LIB_DIR)/USB_Port
USB_2_SRC	 = $(notdir $(wildcard $(LIB_DIR)/USB_Port/*.c))

# Search path and source files for thread
VPATH		:= $(VPATH):$(CUPDRONEAPI_DIR)/thread
THREAD_SRC	 = $(notdir $(wildcard $(CUPDRONEAPI_DIR)/thread/*.c))

# Search path and source files for u8glib
VPATH		:= $(VPATH):$(U8GLIB_DIR)/src
U8GLIB_SRC	 = $(notdir $(wildcard $(U8GLIB_DIR)/src/*.c))

VPATH       := $(VPATH):$(SRC_DIR)
MAIN_SRC     = $(notdir $(wildcard $(SRC_DIR)/*.c))
MAINCPP_SRC  = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

VPATH       := $(VPATH):$(HARDWARESERIAL_DIR)
HARDWARESERIAL_SRC		= $(notdir $(wildcard $(HARDWARESERIAL_DIR)/*.c))
HARDWARESERIALCPP_SRC	= $(notdir $(wildcard $(HARDWARESERIAL_DIR)/*.cpp))

VPATH       := $(VPATH):$(MODECONTROLLER_DIR)
MODECONTROLLER_SRC		= $(notdir $(wildcard $(MODECONTROLLER_DIR)/*.c))
MODECONTROLLER_SRC		= $(notdir $(wildcard $(MODECONTROLLER_DIR)/*.cpp))

VPATH       := $(VPATH):$(COMMANDPARSER_DIR)
COMMANDPARSER_SRC		= $(notdir $(wildcard $(COMMANDPARSER_DIR)/*.c))
COMMANDPARSERCPP_SRC	= $(notdir $(wildcard $(COMMANDPARSER_DIR)/*.cpp))

VPATH       := $(VPATH):$(GLOBALCLASS_DIR)
GLOBALCLASS_SRC			= $(notdir $(wildcard $(GLOBALCLASS_DIR)/*.c))
GLOBALCLASSCPP_SRC		= $(notdir $(wildcard $(GLOBALCLASS_DIR)/*.cpp))

VPATH       := $(VPATH):$(POSMODE_DIR)
POSMODE_SRC				= $(notdir $(wildcard $(POSMODE_DIR)/*.c))
POSMODECPP_SRC			= $(notdir $(wildcard $(POSMODE_DIR)/*.cpp))

VPATH       := $(VPATH):$(ATTITUDEMODE_DIR)
ATTITUDEMODE_SRC		= $(notdir $(wildcard $(ATTITUDEMODE_DIR)/*.c))
ATTITUDEMODECPP_SRC		= $(notdir $(wildcard $(ATTITUDEMODE_DIR)/*.cpp))

VPATH       := $(VPATH):$(ACTUATORMODE_DIR)
ACTUATORMODE_SRC		= $(notdir $(wildcard $(ACTUATORMODE_DIR)/*.c))
ACTUATORMODECPP_SRC		= $(notdir $(wildcard $(ACTUATORMODE_DIR)/*.cpp))

VPATH       := $(VPATH):$(ESTIMATION_DIR)
ESTIMATION_SRC			= $(notdir $(wildcard $(ESTIMATION_DIR)/*.c))
ESTIMATIONCPP_SRC		= $(notdir $(wildcard $(ESTIMATION_DIR)/*.cpp))

VPATH       := $(VPATH):$(INERTIALSENSOR_DIR)
INERTIALSENSOR_SRC		= $(notdir $(wildcard $(INERTIALSENSOR_DIR)/*.c))
INERTIALSENSORCPP_SRC	= $(notdir $(wildcard $(INERTIALSENSOR_DIR)/*.cpp))

VPATH       := $(VPATH):$(MOTOR_DIR)
MOTOR_SRC				= $(notdir $(wildcard $(MOTOR_DIR)/*.c))
MOTORCPP_SRC			= $(notdir $(wildcard $(MOTOR_DIR)/*.cpp))

###############################################################################
# Things that might need changing to use different tools
#

# Tool names
CC		 = arm-none-eabi-gcc
CXX      = arm-none-eabi-g++
OBJCOPY	 = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size
#
# Tool options.
#
INCLUDE_DIRS	 = $(CUPDRONEAPI_DIR) \
           $(SRC_DIR) \
		   $(STDPERIPH_DIR)/inc \
		   $(CMSIS_DIR)/Include \
		   $(CMSIS_DIR)/Device/ST/STM32F10x \
		   $(RTOS_DIR)/Source/CMSIS_RTOS \
		   $(RTOS_DIR)/Source/include \
		   $(RTOS_DIR)/Source/portable/GCC/ARM_CM3 \
		   $(LIB_DIR)/STM32_USB-FS-Device_Driver/inc \
		   $(LIB_DIR)/USB_Port \
		   $(CUPDRONEAPI_DIR)/thread \
		   $(U8GLIB_DIR)/src \
		   $(HARDWARESERIAL_DIR) \
		   $(MODECONTROLLER_DIR) \
		   $(COMMANDPARSER_DIR) \
		   $(GLOBALCLASS_DIR) \
		   $(POSMODE_DIR) \
		   $(ATTITUDEMODE_DIR) \
		   $(ACTUATORMODE_DIR) \
		   $(ESTIMATION_DIR) \
		   $(INERTIALSENSOR_DIR) \
		   $(MOTOR_DIR) \




ARCH_FLAGS	 = -mthumb -mcpu=cortex-m3
BASE_CFLAGS	 = $(ARCH_FLAGS) \
		   $(addprefix -D,$(OPTIONS)) \
		   $(addprefix -I,$(INCLUDE_DIRS)) \
		   -Wall \
		   -ffunction-sections \
		   -fdata-sections \
		   -DSTM32F10X_MD \
		   -DUSE_STDPERIPH_DRIVER \
		   -D$(TARGET) \
		   -DUSE_BOOT=$(BOOT)

ASFLAGS		 = $(ARCH_FLAGS) \
		   -x assembler-with-cpp \
		   $(addprefix -I,$(INCLUDE_DIRS))

# XXX Map/crossref output?

ifeq ($(BOOT),1)
LD_SCRIPT	 = $(ROOT)/stm32_flash_boot.ld
else
LD_SCRIPT	 = $(ROOT)/stm32_flash.ld
endif

LDFLAGS		 = -lm \
		   $(ARCH_FLAGS) \
		   -static \
		   -Wl,-gc-sections,-Map,$(TARGET_MAP) \
		   -T$(LD_SCRIPT) \
		   --specs=nosys.specs $(OTHER_LINK_OPTIONS) \

###############################################################################
# No user-serviceable parts below
###############################################################################

#
# Things we will build
#
ifeq ($(filter $(TARGET),$(VALID_TARGETS)),)
$(error Target '$(TARGET)' is not valid, must be one of $(VALID_TARGETS))
endif

ifeq ($(DEBUG),GDB)
CFLAGS = $(BASE_CFLAGS) \
	-ggdb \
	-O0
else
CFLAGS = $(BASE_CFLAGS) \
	-Os
endif


TARGET_HEX	 = $(BIN_DIR)/$(TARGET).hex
TARGET_BIN	 = $(BIN_DIR)/$(TARGET).bin
TARGET_ELF	 = $(BIN_DIR)/$(TARGET).elf
TARGET_OBJS	 = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $($(TARGET)_SRC))))
TARGET_MAP   = $(OBJECT_DIR)/$(TARGET).map


all: create_dir $(TARGET_HEX)

create_dir:
	@echo "Create $(OBJECT_DIR)/$(TARGET)"
ifeq ($(ComSpec),)
	$(shell [ -d $(OBJECT_DIR)/$(TARGET) ] || mkdir -p $(OBJECT_DIR)/$(TARGET))
else
	@echo "@md $(OBJECT_DIR)\$(TARGET) >NUL 2>&1"
	@md obj\$(TARGET) >NUL 2>&1 || echo "" >NUL
endif


# List of buildable ELF files and their object dependencies.
# It would be nice to compute these lists, but that seems to be just beyond make.

$(TARGET_HEX): $(TARGET_ELF) $(TARGET_BIN)
	$(OBJCOPY) -O ihex --set-start 0x8000000 $< $@

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(TARGET_ELF):  $(TARGET_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 

# Compile
$(OBJECT_DIR)/$(TARGET)/%.o: %.c
#	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(CFLAGS) $<
$(OBJECT_DIR)/$(TARGET)/%.o: %.cpp
#	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CXX) -c -o $@ $(CFLAGS) $<

# Assemble
$(OBJECT_DIR)/$(TARGET)/%.o: %.s
#	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $< 
$(OBJECT_DIR)/$(TARGET)/%.o): %.S
#	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $< 


all: $(TARGET_ELF) $(TARGET_BIN)

clean:
	rm -f $(TARGET_HEX) $(TARGET_ELF) $(TARGET_OBJS) $(TARGET_MAP)

flash_$(TARGET): $(TARGET_HEX)
	stty -F $(SERIAL_DEVICE) raw speed 115200 -crtscts cs8 -parenb -cstopb -ixon
	echo -n 'R' >$(SERIAL_DEVICE)
	stm32flash -w $(TARGET_HEX) -v -g 0x0 -b 115200 $(SERIAL_DEVICE)

flash: flash_$(TARGET)


unbrick_$(TARGET): $(TARGET_HEX)
	stty -F $(SERIAL_DEVICE) raw speed 115200 -crtscts cs8 -parenb -cstopb -ixon
	stm32flash -w $(TARGET_HEX) -v -g 0x0 -b 115200 $(SERIAL_DEVICE)

unbrick: unbrick_$(TARGET)

ELFSIZE = $(SIZE) -A $(TARGET_ELF)

sizeinfo:
	$(ELFSIZE)

help:
	@echo ""
	@echo "Makefile for the baseflight firmware"
	@echo ""
	@echo "Usage:"
	@echo "        make [TARGET=<target>] [OPTIONS=\"<options>\"]"
	@echo ""
	@echo "Valid TARGET values are: $(VALID_TARGETS)"
	@echo ""
