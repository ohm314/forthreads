# see /usr/include/features.h (on linux) for different possibilites
# _XOPEN_SOURCE >= 700 means SUSv4 (revision 7).
# This is needed to be compliant with the POSIX.1-2001 standard.
# We may be able to relax this to SUSv2 to cover more systems, either way this
# define is most probably only needed when compiling on linux, other systems
# might have to be treated differently.
DEFINES = -D_XOPEN_SOURCE=700
LDFLAGS = -shared -pthread

TARGET  = libforthread.so
SOURCES := $(wildcard *.c)
IFACE_SOURCES := $(wildcard iface/*.f03) $(wildcard iface/*.h)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES)) iface/forthread.o iface/forthread_data.o
all: $(TARGET)

$(TARGET):  $(OBJECTS)
	$(FC) -o $@ $(DEBUG) $(LDFLAGS) $^

iface/forthread.o: $(IFACE_SOURCES)
	$(MAKE) -C iface forthread.o

%.o: %.c
	$(CC) $(DEFINES) $(CFLAGS) $(CDEBUG) -c $< -o $@

%.o: %.mod
	true

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C iface clean
