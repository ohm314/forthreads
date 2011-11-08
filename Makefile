CC = icc
#CC = gcc
#CC = clang
FC = ifort
#FC = gfortran
CFLAGS = -fPIC
#CFLAGS = -fPIC -Wimplicit-function-declaration
DEBUG = -g -debug
CDEBUG = $(DEBUG)  -I/usr/include/x86_64-linux-gnu/
FDEBUG = $(DEBUG) -fPIC -traceback -C -FR -fpp -stand f03

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
