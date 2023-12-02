PROG            = ti2500
CXXFLAGS        = -O3 -g -Wall -Werror -std=c++20
LDFLAGS         = -g
OBJ             = main.o trace.o cpu.o keys.o code.o display.o

all:		$(PROG)

clean:
		rm -f *.o $(PROG)

ti2500:         $(OBJ)
		$(CXX) $(LDFLAGS) $(OBJ) -o $@

###
code.o: code.cpp calculator.h
cpu.o: cpu.cpp calculator.h
display.o: display.cpp calculator.h
keys.o: keys.cpp calculator.h
main.o: main.cpp calculator.h
trace.o: trace.cpp calculator.h
