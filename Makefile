#CC = icc
#CC = gcc
CC = clang
#FC = ifort
FC = gfortran
#CFLAGS = -fPIC
PLATFORM = -D__DARWIN
CFLAGS = $(PLATFORM) -fPIC -Wimplicit-function-declaration
FFLAGS = $(PLATFORM) -cpp -fPIC -frecursive
DEBUG = -g
#CDEBUG = $(DEBUG)  -I/usr/include/x86_64-linux-gnu/
CDEBUG = $(DEBUG)  
FDEBUG = $(DEBUG)

export

.PHONY: all clean test

all: forthread

forthread:
	$(MAKE) -C src


test:
	$(MAKE) -C test

clean:
	$(MAKE) -C src $@
	$(MAKE) -C test clean
