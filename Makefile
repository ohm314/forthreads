#CC = icc
#CC = gcc
CC = clang
#FC = ifort
FC = gfortran
CDEBUG =  -g
FDEBUG = -g -fbacktrace

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
