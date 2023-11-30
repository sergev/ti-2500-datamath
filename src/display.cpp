#include <sstream>
#include "calculator.h"

std::string Calculator_t::get_display() const
{
    std::stringstream out;

    bool zeroSuppress = true;
    out << '[';
    for (int i = 0; i < 9; i++) {
        if (i == 8 || a[i] != 0) {
            zeroSuppress = false;
        }

        if (a[i] == 0 && zeroSuppress) {
            out << ' ';
        } else if (a[i] == 14) {
            out << '-';
        } else {
            out << (char)(a[i] + '0');
        }

        if (b[i] == 2) {
            // dpt = i;
            out << '.';
        }

        if (b[i+1] == 2) {
            zeroSuppress = false;
        }
    }
    out << ']';
    return out.str();
}
