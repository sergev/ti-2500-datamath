#include <iostream>
#include "calculator.h"

int main()
{
    Calculator_t calc;

    for (;;) {
        step(calc);

        if (calc.address == 42) { // WAITNO instruction: waiting for input
            std::cout << calc.get_display() << std::endl;
        }

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
    }
}
