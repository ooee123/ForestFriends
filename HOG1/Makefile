#--------------------------------------------------------------------------------------
# File:    Makefile for an AVR project
#          The makefile is the standard way to control the compilation and linking of 
#          C/C++ files into an executable file. This makefile is also used to control
#          the downloading of the executable file to the target processor and the 
#          generation of documentation for the project. 
#
# Version:  4-11-2004 JRR Original file
#           6-19-2006 JRR Modified to use AVR-JTAG-ICE for debugging
#          11-21-2008 JRR Added memory locations and removed extras for bootloader
#          11-26-2008 JRR Cleaned up; changed method of choosing programming method
#          11-14-2009 JRR Added make support to put library files into subdirectory
#           9-28-2012 JRR Restructured to work with FreeRTOS subdirectory
#
# Relies   The avr-gcc compiler and avr-libc library
# on:      The avrdude downloader, if downloading through an ISP port
#          AVR-Insight or DDD and avarice, if debugging with the JTAG port
#          Doxygen, for automatic documentation generation
#
# Copyright 2006-2012 by JR Ridgely.  This makefile is intended for use in educational 
# courses only, but its use is not restricted thereto. It is released under the terms 
# of the Lesser GNU Public License with no warranty whatsoever, not even an implied
# warranty of merchantability or fitness for any particular purpose. Anyone who uses 
# this file agrees to take all responsibility for any and all consequences of that use. 
#--------------------------------------------------------------------------------------

# The name of the program you're building, usually the file which contains main(). 
# The name without its extension (.c or .cpp or whatever) must be given here. 
TARGET = main

# A list of the source (.c, .cc, .cpp) files in the project, including $(TARGET). Files
# in library subdirectories do not go in this list; they're automatically in LIB_OBJS
ADDITIONAL_FILES = encoder_task.cpp encoder_driver.cpp read_serial_driver.cpp read_serial_task.cpp z_motor_task.cpp z_encoder_driver.cpp
SRC = $(TARGET).cpp motor_task.cpp motor_driver.cpp $(ADDITIONAL_FILES)
HEADER_FILES = constants.h pinLayout.h
# Clock frequency of the CPU, in Hz. This number should be an unsigned long integer.
# For example, 16 MHz would be represented as 16000000UL. 
F_CPU = 16000000UL

# These codes are used to switch on debugging modes if they're being used. Several can
# be placed on the same line together to activate multiple debugging tricks at once.
# -DSERIAL_DEBUG       For general debugging through a serial device
# -DTRANSITION_TRACE   For printing state transition traces on a serial device
# -DTASK_PROFILE       For doing profiling, measurement of how long tasks take to run
# -DUSE_HEX_DUMPS      Include functions for printing hex-formatted memory dumps
#OTHERS = -DSERIAL_DEBUG -DDEBUG -DZ_AXIS -DMOTOR_DEBUG -DZ_CODE_TO_HEIGHT

OTHERS = -DSERIAL_DEBUG -DDEBUG -DZ_AXIS -DZ_CODE_TO_HEIGHT -DFAST_ENCODER

# If the code -DTASK_SETUP_AND_LOOP is specified, ME405/FreeRTOS tasks classes will be
# required to provide methods setup() and loop(). Otherwise, they must only provide a
# a method called run() which is called just once by the scheduler.
OTHERS += 

# Other codes, used for turning on special features in a given project, can be put here
# -DSWOOP_BOARD        Tells the nRF24L01 radio driver to set up for the Swoop 1 board
# -DME405_BOARD_V05    Sets up radio driver for old ME405 board with 1 motor driver
# -DME405_BOARD_V06    Sets up radio driver for new ME405 board with 2 motor drivers
# -DME405_BREADBOARD   Sets up radio driver for ATmegaXX 40-pin on breadboard
# -DPOLYDAQ_BOARD      Sets up radio and other stuff for a PolyDAQ board
OTHERS += -DSWOOP_BOARD

# This define is used to choose the type of programmer from the following options: 
# bsd        - Parallel port in-system (ISP) programmer using SPI interface on AVR
# jtagice    - Serial or USB interface JTAG-ICE mk I clone from ETT or Olimex
# bootloader - Resident program in the AVR which downloads through USB/serial port
# PROG = bsd
# PROG = jtagice
# PROG = bootloader
PROG = usbtiny

# These defines specify the ports to which the downloader device is connected. 
# PPORT is for "bsd" on a parallel port, lpt1 on Windows or /dev/parport0 on Linux.
# JPORT is for "jtagice" on a serial port such as com1 or /dev/ttyS0, or usb-serial 
#       such as com4 or /dev/ttyUSB1, or aliased serial port such as /dev/avrjtag
# BPORT is for "bootloader", the USB/serial port program downloader on the AVR
# The usbtiny programmer doesn't need a port specification; it has a USB identifier
PPORT = /dev/parport0
JPORT = /dev/ttyUSB1
BPORT = /dev/ttyUSB0

#--------------------------------------------------------------------------------------
# This section specifies the type of CPU; uncomment one line for your processor. To add
# a new chip to the file, put its designation here and also set fuse bytes below. 

 MCU = atmega128
# MCU = atmega1281
# MCU = atmega32
# MCU = atmega324p
# MCU = atmega328p
# MCU = atmega644
# MCU = atmega644p
# MCU = atmega1284p

#######################################################################################
################ End of the stuff the user is expected to need to change ##############

# This is the name of the library file which will hold object code which has been
# compiled from all the source files in the library subdirectories
LIB_NAME = me405.a

# A list of directories in which source files (*.cpp, *.c) and headers (.h) for the
# library are kept
LIB_DIRS = lib/freertos lib/frtcpp lib/misc lib/comm lib/sensors lib/sd_card \
           lib/serial

#--------------------------------------------------------------------------------------
# In this section, default settings for fuse bytes are given for each processor which 
# this makefile supports. New chip specifications can be added to this file as needed. 

# ATmega128 set up for ME405 board with JTAG enabled
ifeq ($(MCU), atmega128)
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega128 set up for ME405 board with JTAG disabled
else ifeq ($(MCU), atmega1281)
  EFUSE = 0xFF
  HFUSE = 0xD1
  LFUSE = 0xEF
# ATmega32 configured for Swoop sensor board with JTAG disabled to save power
else ifeq ($(MCU), atmega32)
  EFUSE = 
  HFUSE = 0x99
  LFUSE = 0xEF
# ATmega324P configured for Swoop sensor board with JTAG disabled
# Standard fuses FF19EF, bootloader fuses FFC8EF, low power fuses FF11EF
else ifeq ($(MCU), atmega324p)
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega328P configured for Super Ministrone board
# Standard fuses FF19EF, bootloader fuses FFC8EF, low power fuses FF11EF
else ifeq ($(MCU), atmega328p)
  EFUSE = 0x07
  HFUSE = 0xD1
  LFUSE = 0xEF
# ATmega644 (note: the 644P needs a different MCU)
else ifeq ($(MCU), atmega644)
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega644P configured for Swoop-2 sensor board
# Standard fuses FF19EF, bootloader fuses FFC8EF, low power fuses FF11EF, JTAG OFF!
else ifeq ($(MCU), atmega644p)
  EFUSE = 0xFF
  HFUSE = 0xD1
  LFUSE = 0xEF
# ATmega1284P configured for Swoop sensor board
# Standard fuses FF19EF, bootloader fuses FFC8EF, low power fuses FF11EF
else ifeq ($(MCU), atmega1284p)
  EFUSE = 0xFF
  HFUSE = 0xD1
  LFUSE = 0xEF
endif

#--------------------------------------------------------------------------------------
# Tell the compiler how hard to try to optimize the code. Optimization levels are:
# -O0  Don't try to optimize anything (even leaves empty delay loops in)
# -O1  Some optimizations; code usually smaller and faster than O0
# -O2  Pretty high level of optimization; often good compromise of speed and size
# -O3  Tries really hard to make code run fast, even if code size gets pretty big
# -Os  Tries to make code size small. Sometimes -O1 makes it smaller, though(?!)
OPTIM = -O2

# Warnings which need to be given
C_WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align \
             -Wsign-compare -Wstrict-prototypes -Wmissing-prototypes -Wunused \
             -Wmissing-declarations -Waggregate-return 

CPP_WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wsign-compare \
               -Wmissing-declarations -Wunused

C_FLAGS = -D GCC_MEGA_AVR -D F_CPU=$(F_CPU) -D _GNU_SOURCE \
          -D Z_AXIS \
          -fsigned-char -funsigned-bitfields -fpack-struct -fshort-enums \
          -std=gnu99 -g $(OPTIM) -mmcu=$(MCU) $(OTHERS) $(C_WARNINGS) \
          $(patsubst %,-I%,$(LIB_DIRS))

CPP_FLAGS = -D GCC_MEGA_AVR -D F_CPU=$(F_CPU) -D _GNU_SOURCE \
            -fsigned-char -funsigned-bitfields -fshort-enums \
            -g $(OPTIM) -mmcu=$(MCU) $(OTHERS) $(CPP_WARNINGS) \
            $(patsubst %,-I%,$(LIB_DIRS))

# This section makes a list of object files from the source files in the SRC list, 
# separating the C++ source files, the C source files, and assembly source files
OBJS = $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC))) \
       $(patsubst %.c, %.o, $(filter %.c, $(SRC))) \
       $(ASRC:.S=.o)

# This section makes a list of object files from the source files in subdirectories
# in the LIB_DIRS list, separating the C++, C, and assembly source files
LIB_SRC =  $(foreach A_DIR, $(LIB_DIRS), $(wildcard $(A_DIR)/*.cpp)) \
           $(foreach A_DIR, $(LIB_DIRS), $(wildcard $(A_DIR)/*.cc)) \
           $(foreach A_DIR, $(LIB_DIRS), $(wildcard $(A_DIR)/*.c)) \
           $(foreach A_DIR, $(LIB_DIRS), $(wildcard $(A_DIR)/*.S))

LIB_OBJS = $(patsubst %.cpp, %.o, $(filter %.cpp, $(LIB_SRC))) \
           $(patsubst %.cc, %.o, $(filter %.cc, $(LIB_SRC))) \
           $(patsubst %.c, %.o, $(filter %.c, $(LIB_SRC))) \
           $(LIB_ASRC:.S=.o)

#--------------------------------------------------------------------------------------
# Inference rules show how to process each kind of file. 

# How to compile a .c file into a .o file
.c.o: $(HEADER_FILES)
	@echo $<
	@avr-gcc -c $(C_FLAGS) $< -o $@

# How to compile a .cc file into a .o file
.cc.o: $(HEADER_FILES)
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

# How to compile a .cpp file into a .o file
.cpp.o: $(HEADER_FILES)
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@
#--------------------------------------------------------------------------------------
# Make the main target of this project.  This target is invoked when the user types 
# 'make' as opposed to 'make <target>.'  This must be the first target in Makefile.

all:  $(TARGET).hex $(HEADER_FILES)


motor_driver.o: motor_driver.cpp $(HEADER_FILES) motor_driver.h
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

motor_task.o: motor_task.cpp $(HEADER_FILES) motor_task.h motor_driver.h
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

z_motor_task.o: z_motor_task.cpp $(HEADER_FILES) motor_driver.h
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

main.o: main.cpp $(HEADER_FILES)
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

read_serial_task.o: read_serial_task.cpp $(HEADER_FILES) read_serial_task.h
	@echo $<
	@avr-gcc -c $(CPP_FLAGS) $< -o $@

#--------------------------------------------------------------------------------------
# This rule creates a .hex format downloadable file. A raw binary file which can be 
# used by some bootloaders can be created; a listing file is also created. 

$(TARGET).hex:  $(TARGET).elf $(HEADER_FILES)
	@avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	@avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	@avr-size $(TARGET).elf

#--------------------------------------------------------------------------------------
# This rule controls the linking of the target program from object files. The target 
# is saved as an ELF debuggable binary.

$(TARGET).elf:  library $(OBJS) $(HEADER_FILES)
	avr-gcc $(OBJS) $(LIB_NAME) -g -mmcu=$(MCU) -o $(TARGET).elf

#--------------------------------------------------------------------------------------
# This is a dummy target that doesn't do anything. It's included because the author 
# belongs to a faculty labor union and has been instilled with reverence for laziness.

nothing:

#--------------------------------------------------------------------------------------
# 'make install' will make the project, then download the program using whichever 
# method has been selected -- ISP cable, JTAG-ICE module, or USB/serial bootloader

install:  $(TARGET).hex
  ifeq ($(PROG), bsd)
	avrdude -p $(MCU) -P $(PPORT) -c bsd -V -E noreset -Uflash:w:$(TARGET).hex
  else ifeq ($(PROG), jtagice)
	avarice -e -p -f $(TARGET).elf -j $(JPORT)
  else ifeq ($(PROG), bootloader)
	@echo "ERROR: No bootloader set up for this Makefile"
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(MCU) -c usbtiny -B 1 -Uflash:w:$(TARGET).hex
  else
	@echo "ERROR: No programmer" $(PROG) "in the Makefile"
  endif

#--------------------------------------------------------------------------------------
# 'make fuses' will set up the processor's fuse bits in a "standard" mode. Standard is
# a setup in which there is no bootloader but the ISP and JTAG interfaces are enabled. 

fuses: nothing
  ifeq ($(PROG), bsd)
	avrdude -p $(MCU) -P $(PPORT) -c $(PROG) -V -E noreset -Ulfuse:w:$(LFUSE):m 
	avrdude -p $(MCU) -P $(PPORT) -c $(PROG) -V -E noreset -Uhfuse:w:$(HFUSE):m 
	avrdude -p $(MCU) -P $(PPORT) -c $(PROG) -V -E noreset -Uefuse:w:$(EFUSE):m
  else ifeq ($(PROG), jtagice)
	@echo "ERROR: Fuse byte programming not set up for JTAG in this Makefile"
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(MCU) -c usbtiny -q -V -E noreset -Ulfuse:w:$(LFUSE):m
	avrdude -p $(MCU) -c usbtiny -q -V -E noreset -Uhfuse:w:$(HFUSE):m
	avrdude -p $(MCU) -c usbtiny -q -V -E noreset -Uefuse:w:$(EFUSE):m
  else
	@echo "ERROR: Only bsd or USBtiny set to program fuse bytes in this Makefile"
  endif

#--------------------------------------------------------------------------------------
# 'make readfuses' will see what the fuses currently hold

readfuses: nothing
  ifeq ($(PROG), bsd)
	@echo "ERROR: Not yet programmed to read fuses with bsd/ISP cable"
  else ifeq ($(PROG), jtagice)
	@avarice -e -j $(JPORT) --read-fuses
  else ifeq ($(PROG), bootloader)
	@echo "ERROR: Not yet programmed to read fuses via bootloader"
  else
	@echo "ERROR: No known device specified to read fuses"
  endif

#-----------------------------------------------------------------------------
# 'make reset' will read a byte of lock bits, ignore it, and reset the chip

reset:
  ifeq ($(PROG), bsd)
	avrdude -c bsd -p $(MCU) -P $(PPORT) -c $(PROG) -V -E noreset \
		-Ulfuse:r:/dev/null:r
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(MCU) -c usbtiny -q -V -Ulfuse:r:/dev/null:r
  else
	@echo "ERROR: make reset only works with parallel ISP cable"
  endif

#--------------------------------------------------------------------------------------
# 'make doc' will use Doxygen to create documentation for the project. 'make libdoc'
# will do the same for the subdirectories which include ME405 library files.

.PHONY: doc libdoc
doc:
	@doxygen doxygen.conf

libdoc:
	@doxygen doxy_lib.conf

#--------------------------------------------------------------------------------------
# 'make clean' will erase the compiled files, listing files, etc. so you can restart
# the building process from a clean slate. It's also useful before committing files to
# a CVS or SVN or Git repository. 

clean:
	@echo -n Cleaning compiled files and documentation...
	@rm -f $(LIB_NAME) *.o *.hex *.lst *.elf *~
	@for subdir in $(LIB_DIRS); do \
		rm -f $$subdir/*.o; \
		rm -f $$subdir/*.lst; \
		rm -f $$subdir/*~; \
	done
	@echo done.

#--------------------------------------------------------------------------------------
# 'make library' will build the library file, using an automatically generated list of
# all the C, C++, and assembly source files in the library directories in LIB_DIRS

library: $(LIB_OBJS)
	@avr-ar -r $(LIB_NAME) $(LIB_OBJS)

#--------------------------------------------------------------------------------------
# 'make term' will run a PuTTY terminal using the settings that are most commonly used
# to talk to an AVR microcontroller connected through a USB-serial port. One must have
# installed PuTTY on the local computer and configured the default settings, of course

term:
	@putty -load "Default Settings" &

#--------------------------------------------------------------------------------------
# 'make help' will show a list of things this makefile can do

help:
	@echo 'make          - Build program file ready to download'
	@echo 'make install  - Build program and download with parallel ISP cable'
	@echo 'make reset    - Reset processor with parallel cable RESET line'
	@echo 'make doc      - Generate documentation with Doxygen'
	@echo 'make clean    - Remove compiled files from all directories'
	@echo ' '
	@echo 'Notes: 1. Other less commonly used targets are in the Makefile'
	@echo '       2. You can combine targets, as in "make clean all"'
