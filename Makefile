#CC = icc
#CC = gcc
CC = clang
CFLAGS = -fPIC
#CDEBUG = -g -traceback
LDFLAGS = -shared -pthread
TARGET  = libforthread.so
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
all: $(TARGET)

$(TARGET):  $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) $(CDEBUG) -c $< -o $@


clean:
	rm -f *.co *.fo *.mod bench
