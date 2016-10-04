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
CCFLAGS1=arm-linux-gnueabihf-g++ -fPIC -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -Ofast -Wall -pthread 
CCFLAGS2=arm-linux-gnueabihf-gcc -fPIC -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -Ofast -Wall -pthread 
endif

# define all programs
OBJECTS= AES.o spi.o bcm2835.o RF24.o RF24Network.o RF24Mesh.o RF24MeshServer.o
LibFolder = ../../LibFolder/

all: MeshServer

MeshServer : $(OBJECTS)
	$(CCFLAGS1) -o MeshServer $(OBJECTS)

AES.o : $(LibFolder)AES/AES.cpp
	$(CCFLAGS1) -c $(LibFolder)AES/AES.cpp -o AES.o

bcm2835.o: $(LibFolder)RF24/utility/RPi/bcm2835.c
	$(CCFLAGS2) -c $(LibFolder)RF24/utility/RPi/bcm2835.c -o bcm2835.o

spi.o: $(LibFolder)RF24/utility/RPi/spi.cpp
	$(CCFLAGS1) -c $(LibFolder)RF24/utility/RPi/spi.cpp -o spi.o

RF24.o: $(LibFolder)RF24/RF24.cpp
	$(CCFLAGS1) -c $(LibFolder)RF24/RF24.cpp -o RF24.o 

RF24Network.o : $(LibFolder)RF24Network/RF24Network.cpp
	$(CCFLAGS1) -c $(LibFolder)RF24Network/RF24Network.cpp -o RF24Network.o

RF24Mesh.o : $(LibFolder)RF24Mesh/RF24Mesh.cpp
	$(CCFLAGS1) -c $(LibFolder)RF24Mesh/RF24Mesh.cpp -o RF24Mesh.o

RF24MeshServer.o : RF24MeshServer.cpp
	$(CCFLAGS1) -c RF24MeshServer.cpp -o RF24MeshServer.o

#interrupt.o: RF24/utility/RPi/interrupt.c
#	$(CCFLAGS1) RF24/utility/RPi/interrupt.c -o interrupt.o

clean:
	rm -rf MeshServer *.o