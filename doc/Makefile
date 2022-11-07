THIS := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT ?= $(realpath $(THIS)/..)

.PHONY: all
all: $(ROOT)/README.md \

$(ROOT)/README.md: README.in.md modules.list.md
	./include "$<" "$@"

modules.list.md: $(ROOT)/libr.toml modules.py
	./modules.py "$<" > "$@"

.PHONY: clean
clean:
	rm -f modules.list.md
