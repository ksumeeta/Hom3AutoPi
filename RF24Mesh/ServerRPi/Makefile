#############################################################################
#
# Makefile for librf24mesh examples on Raspberry Pi
#
# By:  TMRh20
# Date:    2013/09
#
# Description:
# ------------
# use make all and make install to install the examples
# You can change the install directory by editing the prefix line
#
prefix := /usr/local

ARCH=armv6zk
ifeq "$(shell uname -m)" "armv7l"
ARCH=armv7-a
endif

# Detect the Raspberry Pi from cpuinfo
#Count the matches for BCM2708 or BCM2709 in cpuinfo
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2708)
ifneq "${RPI}" "1"
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2709)
endif

ifeq "$(RPI)" "1"
# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=$(ARCH) -mtune=arm1176jzf-s -std=c++0x
CCFLAGS1=arm-linux-gnueabihf-g++ -fPIC -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -Ofast -Wall -pthread -c
CCFLAGS2=arm-linux-gnueabihf-gcc -fPIC -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -Ofast -Wall -pthread -c
endif

# define all programs
PROGRAMS = RF24MeshServer
SOURCES = ${PROGRAMS:=.cpp}
OBJECTS= spi.o bcm2835.o RF24.o

all: LibAES $(OBJECTS) LibRF24Network LibRF24Mesh ${PROGRAMS}

LibAES: AES/AES.cpp
	g++ ${CCFLAGS} -Wall -c AES/AES.cpp -o AES.o

bcm2835.o: RF24/utility/RPi/bcm2835.c
	$(CCFLAGS2) RF24/utility/RPi/bcm2835.c -o bcm2835.o

spi.o: RF24/utility/RPi/spi.cpp
	$(CCFLAGS1) RF24/utility/RPi/spi.cpp -o spi.o

#interrupt.o: RF24/utility/RPi/interrupt.c
#	$(CCFLAGS1) RF24/utility/RPi/interrupt.c -o interrupt.o

RF24.o: RF24/RF24.cpp
	$(CCFLAGS1) RF24/RF24.cpp -o RF24.o 

LibRF24Network: RF24Network/RF24Network.cpp
	g++ ${CCFLAGS} -Wall -c RF24Network/RF24Network.cpp -o RF24Network.o

LibRF24Mesh: RF24Mesh/RF24Mesh.cpp
	g++ ${CCFLAGS} -Wall -c RF24Mesh/RF24Mesh.cpp -o RF24Mesh.o

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -I../ $@.cpp -o $@ AES.o spi.o bcm2835.o  RF24.o RF24Network.o RF24Mesh.o




clean:
	rm -rf $(PROGRAMS) *.o
	
install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
