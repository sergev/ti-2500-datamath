PROG            = calc
CXXFLAGS        = -O3 -g -Wall -Werror -std=c++20
LDFLAGS         = -g
OBJ             = main.o disasm.o cpu.o keys.o code.o display.o

all:		$(PROG)

clean:
		rm -f *.o $(PROG)

calc:           $(OBJ)
		$(CXX) $(LDFLAGS) $(OBJ) -o $@

###
cpu.o: cpu.cpp calculator.h
disasm.o: disasm.cpp calculator.h
keys.o: keys.cpp calculator.h
main.o: main.cpp calculator.h
