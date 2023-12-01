#include <iostream>
#include "calculator.h"

int main()
{
    Calculator calc;

    for (;;) {
        calc.step();

        if (calc.address == 42) { // WAITNO instruction: waiting for input
            std::cout << calc.get_display() << std::endl;
        }
#if 0
        char key = readKey(calc);

        if (calc.resetinprogress) {
            if (calc.address > 8) {
                calc.resetinprogress = false;
            }
        }

        if (key == 'C') {
            if (calc.resetinprogress == false) {
                calc.address = 0;
                calc.keyStrobeKN = 0;
                calc.keyStrobeKO = 0;
                calc.keyStrobeKP = 0;
                calc.dActive = 1;
                calc.resetinprogress = true;
            }
        }
#endif
    }
}
