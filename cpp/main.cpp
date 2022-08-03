#include <iostream>
#include "globals.h"

struct Calculator_t calc;

void updateDisplay()
{
    auto &out = std::cout;

    bool zeroSuppress = true;
    out << '[';
    for (int i = 0; i < 9; i++) {
        if (i == 8 || calc.a[i] != 0) {
            zeroSuppress = false;
        }

        if (calc.a[i] == 0 && zeroSuppress) {
            out << ' ';
        } else if (calc.a[i] == 14) {
            out << '-';
        } else {
            out << (char)(calc.a[i] + '0');
        }

        if (calc.b[i] == 2) {
            // dpt = i;
            out << '.';
        }

        if (calc.b[i+1] == 2) {
            zeroSuppress = false;
        }
    }
    out << ']' << std::endl;
}

int main()
{
    for (;;) {
        step();

        if (calc.address == 42) // WAITNO instruction: waiting for input
            updateDisplay();

        char key = readKey();

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
