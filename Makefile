#
# make
# make all      -- build everything
#
# make test     -- run unit tests
#
# make clean    -- remove build files
#

all:    build
	$(MAKE) -C build $@

test:   build
	$(MAKE) -C build test

clean:
	rm -rf build
	make -C src clean

build:
	mkdir $@
	cmake -B $@ .

.PHONY: clean test
