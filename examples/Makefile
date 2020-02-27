examples := $(foreach cml,$(wildcard */CMakeLists.txt),$(dir $(cml)))

all: $(examples)

$(examples):
	echo "[Building] $@"
	mkdir -p $@/build
	cmake -S $@ -B $@/build
	$(MAKE) -C $@/build

clean:
	rm -rf $(foreach ex,$(examples),$(ex)/build)

.PHONY: $(examples) all clean
