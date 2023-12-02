#include <iostream>
#include <sstream>
#include "calculator.h"

std::string Calculator::get_display() const
{
    std::stringstream out;
    bool zeroSuppress = true;

    // Print registers for debug.
    //std::cout << "--- a  = " << a << std::endl;
    //std::cout << "--- b  = " << b << std::endl;
    //std::cout << "--- c  = " << c << std::endl;
    //std::cout << "--- af = " << af << std::endl;
    //std::cout << "--- bf = " << bf << std::endl;

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
