all: ./build
	@cmake -S . -B ./build
	@$(MAKE) -C ./build
	@make test

./build:
	@mkdir -p $@

test: ./build
	@cmake -S . -B ./build
	@$(MAKE) -C ./build test
	@find build/test -type f -executable -print0 | xargs -0 -n1 /usr/bin/env

