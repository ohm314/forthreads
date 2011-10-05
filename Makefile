#CC = icc
#CC = gcc
CC = clang
# see /usr/include/features.h (on linux) for different possibilites
# _XOPEN_SOURCE >= 700 means SUSv4 (revision 7).
# This is needed to be compliant with the POSIX.1-2001 standard.
# We may be able to relax this to SUSv2 to cover more systems, either way this
# define is most probably only needed when compiling on linux, other systems
# might have to be treated differently.
DEFINES = -D_XOPEN_SOURCE=700
CFLAGS = -fPIC -Wimplicit-function-declaration
#CDEBUG = -g -traceback
LDFLAGS = -shared -pthread
TARGET  = libforthread.so
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
all: $(TARGET)

$(TARGET):  $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.c
	$(CC) $(DEFINES) $(CFLAGS) $(CDEBUG) -c $< -o $@


clean:
	rm -f *.o 
