#CC = icc
#CC = gcc
CC = clang
FC = gfortran
#CDEBUG = -g -traceback
LDFLAGS = -L../src/ -lforthread

CSOURCES := $(wildcard *.c)
FSOURCES := $(wildcard *.f03)
CTARGETS := $(patsubst %.c,%,$(CSOURCES))
FTARGETS := $(patsubst %.f03,%,$(FSOURCES))

all: $(CTARGETS) $(FTARGETS) 

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(CDEBUG) $(LDFLAGS) 
	./$@

%: %.f03
	$(FC) -o $@ $< $(FFLAGS) $(FDEBUG) $(LDFLAGS) 
	./$@

clean:
	rm -f $(CTARGETS) $(FTARGETS)
