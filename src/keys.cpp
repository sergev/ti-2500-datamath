#include "calculator.h"

// const char PrintableKeys[19] = "12+E0v-378X654/9^C";

const char KeysKN[10] = "156789234";
const char KeysKO[10] = "C/*^E0v+-";

int lastSelectDigit = 0;

int analogRead(int pin)
{
    // TODO: replace with user input
    return 0;
}

uint8_t readKey(Calculator_t &calc)
{
    uint8_t key = 0;

    if (analogRead(7) > 100) {
        calc.keyStrobeKN = 1;
        key = KeysKN[lastSelectDigit - 1];
    } else {
        calc.keyStrobeKN = 0;
    }

    if (analogRead(6) > 100) {
        calc.keyStrobeKO = 1;
        key = KeysKO[lastSelectDigit - 1];
    } else {
        calc.keyStrobeKO = 0;
    }

    // ensure C has priority
    if ((lastSelectDigit == 1) && (calc.keyStrobeKO)) {
        key = 'C';
    }

    return key;
}
