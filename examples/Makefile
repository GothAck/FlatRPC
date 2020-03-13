examples := $(shell find . -maxdepth 2 -name CMakeLists.txt -printf '%P\n' | sed 's|/CMakeLists.txt||g' | sort)

all: $(examples)

run_tests: $(examples:%=%_as_test)

%/build:
	@mkdir -p $@

$(examples): %: %/build
	@echo "[Building] $@"
	@cmake -S $@ -B $<
	@$(MAKE) -C $<

%_as_test: %
	@echo "[Running $< as a test]"
	$</build/server &
	sleep 0.5
	$</build/client Testing
	wait

clean:
	@rm -rf $(foreach ex,$(examples),$(ex)/build)

.PHONY: $(examples) all run_tests  clean
