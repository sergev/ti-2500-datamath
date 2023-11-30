#include <iostream>
#include "calculator.h"

//const char KeysKN[10] = "156789234";
//const char KeysKO[10] = "C/*^E0v+-";

//
// Press key on keyboard.
//
void Calculator_t::press_key(char k)
{
    if (k != 0) {
        std::cout << "--- Press key '" << k << "'" << std::endl;
    } else {
        std::cout << "--- Release key '" << keyPressed << "'" << std::endl;
    }
    keyPressed = k;
    run();
}

//
// Poll keys 1 2 3 4 5 6 7 8 9
//
bool Calculator_t::keyStrobeKN() const
{
    switch (keyPressed) {
    case '1': return (dActive == 1); // TODO
    case '2': return (dActive == 2); // TODO
    case '3': return (dActive == 3); // TODO
    case '4': return (dActive == 4); // TODO
    case '5': return (dActive == 5); // TODO
    case '6': return (dActive == 6); // TODO
    case '7': return (dActive == 7); // TODO
    case '8': return (dActive == 8); // TODO
    case '9': return (dActive == 9); // TODO
    default: return false;
    }
}

//
// Poll keys 0 . = + - * / C CE
//
bool Calculator_t::keyStrobeKO() const
{
    switch (keyPressed) {
    case '0': return (dActive == 1); // TODO
    case '.': return (dActive == 2); // TODO
    case '=': return (dActive == 3); // TODO
    case '+': return (dActive == 4); // TODO
    case '-': return (dActive == 5); // TODO
    case '*': return (dActive == 6); // TODO
    case '/': return (dActive == 7); // TODO
    case 'C': return (dActive == 8); // TODO
    case 'E': return (dActive == 9); // TODO
    default: return false;
    }
}

//
// Poll keys: unused
//
bool Calculator_t::keyStrobeKP() const
{
    return false;
}
