THIS := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT ?= $(realpath $(THIS)/..)

.PHONY: all
all: $(ROOT)/README.md \

$(ROOT)/README.md: README.in.md modules.list.md \
	.usage.bundle.sh .usage.build.sh .usage.output.txt
	./include "$<" "$@"

modules.list.md: $(ROOT)/libr.toml modules.py
	./modules.py "$<" > "$@"

.usage.bundle.sh .usage.build.sh .usage.output.txt:
	./usage

.PHONY: clean
clean:
	rm -f modules.list.md .usage.*
