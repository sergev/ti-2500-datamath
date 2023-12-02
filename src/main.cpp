#include <iostream>
#include "calculator.h"

int main()
{
    Calculator calc;

    std::cout << "Try calculations on TI-2500.\n";
    std::cout << "Don't forget '=' at the end!\n";
    std::cout << "Use q to quit.\n";
    std::cout << "\n";

    calc.run();
    for (;;) {
        std::cout << calc.get_display() << std::endl;

        std::string input;
        std::cout << "> " << std::flush;
        std::getline(std::cin, input);

        if (input == "q" || input == "Q" || std::cin.eof()) {
            break;
        }
        calc.send(input);
    }
}
