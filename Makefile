PREFIX ?= /usr/local
TARGET_LIBS = src/libr.a

build:
	$(MAKE) -C src $@

test: build
	$(MAKE) -C tests $@

install: build
	install -d $(PREFIX)/include/r
	install -m 644 include/*.h $(PREFIX)/include
	install -m 644 include/r/*.h $(PREFIX)/include/r
	install -d $(PREFIX)/lib
	install -m 644 $(TARGET_LIBS) $(PREFIX)/lib

clean:
	$(MAKE) -C src $@
	$(MAKE) -C tests $@

.PHONY: build test install clean
