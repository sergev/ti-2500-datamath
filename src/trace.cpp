#include <iostream>
#include <iomanip>
#include "calculator.h"

void Calculator::trace_instruction(unsigned instruction)
{
    unsigned class_bits = instruction >> 9;
    unsigned opcode     = (instruction >> 4) & 0x1f;
    unsigned maskno     = instruction & 0x0f;

    std::string name = "TODO";
    //switch (opcode) {
    //}

    std::string operand;

    std::cout << '(' << d_phase
              << ") " << std::setw(3) << program_counter
              << ": " << class_bits
              << '-' << std::hex << std::setw(2) << std::setfill('0') << opcode
              << '-' << maskno << std::dec
              << "  " << name;

    if (!operand.empty()) {
        std::cout << ' ' << operand;
    }
    std::cout << std::endl;
}
