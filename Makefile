BUNDLER ?= ./bundle

.PHONY: test
test: $(foreach m, $(shell $(BUNDLER) --list), test.$(m))

test.%:
	$(BUNDLER) --output=/dev/null $*
