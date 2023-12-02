#pragma once
#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <array>
#include <string>
#include <cstdint>

//
// Model of TI-2500 calculator.
//
class Calculator {
public:
    // Get contents of the display.
    std::string get_display() const;

    // Execute one instruction.
    void step();

    // Run the code until stopped on wait.
    void run();

    // Press key on keyboard.
    void press_key(char k);

    // Send a string of input.
    void send(const std::string &input);

    // Enable tracing.
    void set_trace(bool flag) { trace_flag = flag; }

private:
    // Number of digits in calculator registers.
    static const unsigned REG_LEN = 11;

    // A register is represented as array of digits.
    using Reg = std::array<int8_t, REG_LEN>;

    // Hardware registers.
    Reg a{};    // Register A
    Reg b{};    // Register B
    Reg c{};    // Register C
    Reg af{};   // A flags
    Reg bf{};   // B flags
    Reg mask{}; // Mask value obtained from instruction opcode

    // Flag for conditional branch.
    bool condition_code{};

    // Which key is currently pressed.
    char key_pressed{};

    // Program counter.
    unsigned program_counter{};

    // D counter in range 0-9.
    unsigned d_phase{};

    // Executable code.
    static const unsigned code_tab[];

    // Sinclair Scientific mode.
    const bool sinclair_flag{};

    // Trace mode.
    bool trace_flag{ false };

    // Previous state for tracing.
    Reg prev_a{};
    Reg prev_b{};
    Reg prev_c{};
    Reg prev_af{};
    Reg prev_bf{};
    bool prev_condition_code{};
    char prev_key_pressed{};

private:
    // Set mask value from bits 3:0 of instruction.
    void decode_mask(unsigned instruction);

    static bool op_with_mask(unsigned class_bits, unsigned opcode);
    static bool op_with_k(unsigned opcode);

    // Arithmetic.
    void add(const Reg &x, const Reg &y, Reg &z, bool hex_flag = false);
    void sub(const Reg &x, const Reg &y, Reg &z, bool hex_flag = false);
    void compare(const Reg &x, const Reg &y);
    void copy(const Reg &x, Reg &z);
    void shift_left(Reg &x);
    void shift_right(Reg &x);
    void exchange(Reg &x, Reg &y);
    void set_flags(Reg &x, int val);
    void compare_flags(const Reg &x, const Reg &y);
    void test_flags(const Reg &x);

    // Poll keyboard.
    bool keyStrobeKN() const; // Keys 1 2 3 4 5 6 7 8 9
    bool keyStrobeKO() const; // Keys 0 . = + - * / C CE
    bool keyStrobeKP() const; // Unused

    void trace_instruction(unsigned instruction) const;
    void trace_keyboard();
    void trace_state();
    std::string disassemble_opcode(unsigned class_bits, unsigned opcode) const;
    std::string disassemble_operand(unsigned instruction) const;
    std::string disassemble_mask() const;

    friend std::ostream &operator<<(std::ostream &out, const Reg &x);

    // Instruction opcodes.
    enum {
        // Class 3
        OP_AABA  = 0,  // A+B -> A
        OP_AAKA  = 1,  // A+K -> A
        OP_AAKC  = 2,  // A+K -> C
        OP_ABOA  = 3,  // B -> A
        OP_ACBB  = 3,  // C+B -> B -- Sinclair
        OP_ABOC  = 4,  // B -> C
        OP_ACKA  = 5,  // C+K -> A
        OP_AKCB  = 6,  // C+K -> B
        OP_SABA  = 7,  // A-B -> A
        OP_SABC  = 8,  // A-B -> C
        OP_SAKA  = 9,  // A-K -> A
        OP_SCBC  = 10, // C-B -> C
        OP_SCKC  = 11, // C-K -> C
        OP_CAB   = 12, // compare A-B
        OP_CAK   = 13, // compare A-K
        OP_CCB   = 14, // compare C-B
        OP_CCK   = 15, // compare C-K
        OP_AKA   = 16, // K -> A
        OP_AKB   = 17, // K -> B
        OP_AKC   = 18, // K -> C
        OP_EXAB  = 19, // exchange A and B
        OP_SLLA  = 20, // shift A left
        OP_SLLB  = 21, // shift B left
        OP_SLLC  = 22, // shift C left
        OP_SRLA  = 23, // shift A right
        OP_SRLB  = 24, // shift B right
        OP_SRLC  = 25, // shift C right
        OP_AKCN  = 26, // A+K -> A until key down on N or D11
        OP_AAKAH = 27, // A+K -> A hex
        OP_SCBA  = 27, // C-B -> A -- Sinclair
        OP_SAKAH = 28, // A-K -> A hex
        OP_SCKB  = 28, // C-K -> B -- Sinclair
        OP_ACKC  = 29, // C+K -> C
        OP_AABC  = 30, // A+B -> C -- Sinclair
        OP_ACBC  = 31, // C+B -> C -- Sinclair

        // Class 2
        OP_NOP16  = 16, // no operation
        OP_WAITDK = 17, // wait for display key
        OP_WAITNO = 18, // wait for key or address register overflow
        OP_SFB    = 19, // set flag B
        OP_SFA    = 20, // set flag A
        OP_SYNC   = 21, // hold address until end of D10
        OP_SCAN   = 22, // wait for key
        OP_ZFB    = 23, // zero flag B
        OP_ZFA    = 24, // zero flag A
        OP_TFB    = 25, // test flag B
        OP_TFA    = 26, // test flag A
        OP_FFB    = 27, // flip flag B
        OP_FFA    = 28, // flip flag A
        OP_CF     = 29, // compare flags
        OP_NOP30  = 30, // no operation
        OP_EXF    = 31, // exchange flags
    };
};

std::ostream &operator<<(std::ostream &out, const Calculator::Reg &x);

#endif // _CALCULATOR_H_
