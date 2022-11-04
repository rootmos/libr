BUNDLER ?= ./bundle

.PHONY: test
test: \
	$(foreach m, $(shell $(BUNDLER) list), compilation-test.$(m)) \
	$(foreach m, $(shell $(BUNDLER) list --has-test), test.$(m))

test.%:
	$(BUNDLER) test $*

compilation-test.%:
	$(BUNDLER) compilation-test $*
