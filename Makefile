#CC = icc
#CC = gcc
CC = clang
#FC = ifort
FC = gfortran
DEBUG = -g
CDEBUG = $(DEBUG) 
FDEBUG = $(DEBUG) -fbacktrace

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
