
.PHONY: all clean test

all clean forthread:
	$(MAKE) -C src $@
	$(MAKE) -C test clean

test:
	$(MAKE) -C test
