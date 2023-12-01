#include <iostream>
#include "calculator.h"

//
// Press key on keyboard.
//
void Calculator::press_key(char k)
{
    key_pressed = k;
    run();
}

//
// Send a string of input.
// Ignore invalid characters.
//
void Calculator::send(const std::string &input)
{
    static const std::string valid_chars = "cC=+-*/eE.0123456789";

    for (char ch : input) {
        if (valid_chars.find(ch) != std::string::npos) {
            press_key(ch);
        }
    }
}

//
// Poll keys 1 2 3 4 5 6 7 8 9
//
bool Calculator::keyStrobeKN() const
{
    switch (key_pressed) {
    case '1': return (d_phase == 0);
    case '2': return (d_phase == 1);
    case '3': return (d_phase == 2);
    case '4': return (d_phase == 3);
    case '5': return (d_phase == 4);
    case '6': return (d_phase == 5);
    case '7': return (d_phase == 6);
    case '8': return (d_phase == 7);
    case '9': return (d_phase == 8);
    default: return false;
    }
}

//
// Poll keys 0 . = + - * / C CE
//
bool Calculator::keyStrobeKO() const
{
    switch (key_pressed) {
    case 'c':
    case 'C': return (d_phase == 0); // Clear key pressed?
    case '=': return (d_phase == 1); // Equal key pressed?
    case '+': return (d_phase == 2); // Plus key pressed?
    case '-': return (d_phase == 3); // Minus key pressed?
    case '*': return (d_phase == 4); // Mult key pressed?
    case '/': return (d_phase == 5); // Divide key pressed?
    case 'e':
    case 'E': return (d_phase == 6); // CE key pressed?
    case '.': return (d_phase == 7); // Decimal point key pressed?
    case '0': return (d_phase == 8); // Zero key pressed?
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
