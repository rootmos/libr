BUNDLER ?= ./bundle

.PHONY: test
test: $(foreach m, $(shell $(BUNDLER) --list-names-only), test.$(m))

test.%:
	$(BUNDLER) --output=/dev/null $*

.PHONY: clean
clean:
	rm -f *-tests-*.tar.gz
