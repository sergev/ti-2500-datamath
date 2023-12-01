#include <sstream>
#include "calculator.h"

std::string Calculator::get_display() const
{
    std::stringstream out;
    bool zeroSuppress = true;

    for (int i = 0; i < 9; i++) {
        if (i == 8 || a[i] != 0) {
            zeroSuppress = false;
        }

        if (a[i] == 14) {
            out << '-';
        } else if (!zeroSuppress) {
            out << (char)(a[i] + '0');
        }

        if (b[i] == 2) {
            out << '.';
        }

        if (b[i+1] == 2) {
            zeroSuppress = false;
        }
    }
    return out.str();
}
