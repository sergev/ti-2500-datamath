PROG            = calc
CXXFLAGS        = -O3 -g -Wall -Werror -std=c++20
LDFLAGS         = -g
OBJ             = main.o disasm.o cpu.o keys.o code.o

all:		$(PROG)

clean:
		rm -f *.o $(PROG)

calc:           $(OBJ)
		$(CXX) $(LDFLAGS) $(OBJ) -o $@

###
cpu.o: cpu.cpp globals.h
disasm.o: disasm.cpp globals.h
keys.o: keys.cpp globals.h
main.o: main.cpp globals.h
