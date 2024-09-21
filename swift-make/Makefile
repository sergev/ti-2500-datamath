#
# make
# make all      -- build everything
#
# make clean    -- remove build files
#
PROG    = ti2500

all:    $(PROG)
	mkdir -p $(PROG).app/Contents/MacOS/
	mv $(PROG) $(PROG).app/Contents/MacOS/
	open $(PROG).app

clean:
	rm -f $(PROG) *.o *.a *.abi.json *.swiftdoc *.swiftmodule *.swiftsourceinfo

$(PROG): main.swift Gui.swiftmodule ModelCxx.o
	swiftc -o $@ main.swift ModelCxx.o -L. -I. -lGui -I../src -cxx-interoperability-mode=default -Xcc -std=c++20 -parse-as-library

Gui.swiftmodule: Gui/ContentView.swift Gui/ModelProxy.swift ModelCxx/ModelCxx.h ModelCxx/module.modulemap
	swiftc -module-name Gui -emit-module -emit-library -static Gui/ContentView.swift Gui/ModelProxy.swift -Xcc -fmodule-map-file=ModelCxx/module.modulemap -I ../src -cxx-interoperability-mode=default -Xcc -std=c++20

ModelCxx.o: ModelCxx/ModelCxx.cpp
	clang -c -I../src -std=c++20 $< -o $@
