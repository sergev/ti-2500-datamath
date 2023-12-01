#include <iostream>
#include "calculator.h"

//
// Press key on keyboard.
//
void Calculator::press_key(char k)
{
    std::cout << "--- Press key '" << k << "'" << std::endl;
    keyPressed = k;
    run();
}

//
// Poll keys 1 2 3 4 5 6 7 8 9
//
bool Calculator::keyStrobeKN() const
{
    switch (keyPressed) {
    case '1': return (dActive == 1);
    case '2': return (dActive == 2);
    case '3': return (dActive == 3);
    case '4': return (dActive == 4);
    case '5': return (dActive == 5);
    case '6': return (dActive == 6);
    case '7': return (dActive == 7);
    case '8': return (dActive == 8);
    case '9': return (dActive == 9);
    default: return false;
    }
}

//
// Poll keys 0 . = + - * / C CE
//
bool Calculator::keyStrobeKO() const
{
    switch (keyPressed) {
    case 'c':
    case 'C': return (dActive == 1); // Clear key pressed?
    case '=': return (dActive == 2); // Equal key pressed?
    case '+': return (dActive == 3); // Plus key pressed?
    case '-': return (dActive == 4); // Minus key pressed?
    case '*': return (dActive == 5); // Mult key pressed?
    case '/': return (dActive == 6); // Divide key pressed?
    case 'e':
    case 'E': return (dActive == 7); // CE key pressed?
    case '.': return (dActive == 8); // Decimal point key pressed?
    case '0': return (dActive == 9); // Zero key pressed?
    default: return false;
    }
}

//
// Poll keys: unused
//
bool Calculator::keyStrobeKP() const
{
    return false;
}
