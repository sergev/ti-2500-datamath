#include <iostream>
#include "calculator.h"

bool Calculator::op_with_mask(unsigned class_bits, unsigned opcode)
{
    switch (class_bits) {
    case 3:
        return true;
    case 2:
        return (opcode > 18) && (opcode != 21) && (opcode != 22);
    default:
        return false;
    }
}

bool Calculator::op_with_k(unsigned opcode)
{
    // 0011'1100'0000'0111'1010'1010'0110'0110b;
    static const unsigned long OPS_WITH_K = 0x3c07aa66;

    return (OPS_WITH_K >> opcode) & 1;
}

void Calculator::decode_mask(unsigned instruction)
{
    // Bits 3:0 of instruction opcode are mapped into pre-defined mask values.
    static const char mask_tab[16][REG_LEN+1] = {
        { "          7" }, // M0
        { "         4 " }, // M1
        { "        1  " }, // M2
        { "       0   " }, // M3
        { "      0    " }, // M4
        { "     0     " }, // M5
        { "    0      " }, // M6
        { " 0         " }, // M7
        { "1          " }, // M8
        { "        000" }, // M9
        { "01         " }, // M10
        { "000000001  " }, // M11
        { "000000000  " }, // M12
        { "         01" }, // M13
        { "         00" }, // M14
        { "00000000000" }, // M15
    };
    unsigned class_bits = instruction >> 9;
    unsigned opcode     = (instruction >> 4) & 0x1f;

    if (op_with_mask(class_bits, opcode)) {
        unsigned maskno = instruction & 0x0f;

        for (unsigned i = 0; i < REG_LEN; i++) {
            char maskdigit = mask_tab[maskno][i];

            if (maskdigit == ' ') {
                mask[i] = maskdigit;
            } else if (class_bits == 3 && op_with_k(opcode)) {
                // Register instruction
                mask[i] = maskdigit - '0';
            } else {
                mask[i] = 'x';
            }
        }
    }
}

void Calculator::add(const Reg &x, const Reg &y, Reg &z, bool hex_flag)
{
    bool carry = false;

    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            int result = x[i] + y[i] + carry;
            if (!hex_flag && result >= 10) {
                result -= 10;
                carry = true;
            } else if (hex_flag && result >= 16) {
                result -= 16;
                carry = true;
            } else {
                carry = false;
            }
            z[i] = result;
        }
    }
    if (carry) {
        condition_code = carry;
        // True: overflow, false: no overflow.
    }
}

void Calculator::sub(const Reg &x, const Reg &y, Reg &z, bool hex_flag)
{
    bool borrow = false;

    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            int result = x[i] - y[i] - borrow;
            if (result < 0) {
                result += hex_flag ? 16 : 10;
                borrow = true;
            } else {
                borrow = false;
            }
            z[i] = result;
        }
    }
    if (borrow) {
        condition_code = borrow;
        // True: borrow, false: no borrow.
    }
}

void Calculator::compare(const Reg &x, const Reg &y)
{
    Reg tmp{};

    sub(x, y, tmp);
    // Compare sets condition if not borrow.
    // True: less than, false: greater or equal.
}

void Calculator::copy(const Reg &x, Reg &z)
{
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            z[i] = x[i];
        }
    }
}

void Calculator::sll(Reg &x)
{
    int digit = 0;
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            int newdigit = x[i];
            x[i] = digit;
            digit = newdigit;
        }
    }
}

void Calculator::srl(Reg &x)
{
    int digit = 0;
    for (int i = 0; i < REG_LEN; i++) {
        if (mask[i] != ' ') {
            int newdigit = x[i];
            x[i] = digit;
            digit = newdigit;
        }
    }
}

void Calculator::exchange(Reg &x, Reg &y)
{
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            int t = x[i];
            x[i] = y[i];
            y[i] = t;
        }
    }
}

//
// Flip flags if val is negative, otherwise set to val.
//
void Calculator::set_flags(Reg &x, int val)
{
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            x[i] = (val < 0) ? (1 - x[i]) : val;
        }
    }
}

void Calculator::compare_flags(const Reg &x, const Reg &y)
{
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (x[i] != y[i]) {
                condition_code = true;
                // Flags not equal.
            }
        }
    }
}

void Calculator::test_flags(const Reg &x)
{
    for (int i = REG_LEN - 1; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (x[i]) {
                condition_code = true;
                // Flag set.
            }
        }
    }
}

void Calculator::step()
{
    unsigned instruction = code_tab[program_counter];
    unsigned class_bits = instruction >> 9;
    unsigned opcode = (instruction >> 4) & 0x1f;
    unsigned next_pc = program_counter + 1;

    decode_mask(instruction);
    if (trace_flag) {
        trace_keyboard();
        trace_instruction(instruction);
    }

    if (class_bits == 3) {
        // Register instruction
        switch (opcode) {
        case OP_AABA: // A+B -> A
            add(a, b, a);
            break;
        case OP_AAKA: // A+K -> A
            add(a, mask, a);
            break;
        case OP_AAKC: // A+K -> C
            add(a, mask, c);
            break;
        case OP_ABOA: // B -> A
            if (sinclair_flag) {
                add(c, b, b); // ACBB C+B -> B
            } else {
                copy(b, a);
            }
            break;
        case OP_ABOC: // B -> C
            copy(b, c);
            break;
        case OP_ACKA: // C+K -> A
            add(c, mask, a);
            break;
        case OP_AKCB: // C+K -> B
            add(c, mask, b);
            break;
        case OP_SABA: // A-B -> A
            sub(a, b, a);
            break;
        case OP_SABC: // A-B -> C
            sub(a, b, c);
            break;
        case OP_SAKA: // A-K -> A
            sub(a, mask, a);
            break;
        case OP_SCBC: // C-B -> C
            sub(c, b, c);
            break;
        case OP_SCKC: // C-K -> C
            sub(c, mask, c);
            break;
        case OP_CAB: // compare A-B
            compare(a, b);
            break;
        case OP_CAK: // compare A-K
            compare(a, mask);
            break;
        case OP_CCB: // compare C-B
            compare(c, b);
            break;
        case OP_CCK: // compare C-K
            compare(c, mask);
            break;
        case OP_AKA: // K -> A
            copy(mask, a);
            break;
        case OP_AKB: // K -> B
            copy(mask, b);
            break;
        case OP_AKC: // K -> C
            copy(mask, c);
            break;
        case OP_EXAB: // exchange A and B
            exchange(a, b);
            break;
        case OP_SLLA: // shift A left
            sll(a);
            break;
        case OP_SLLB: // shift B left
            sll(b);
            break;
        case OP_SLLC: // shift C left
            sll(c);
            break;
        case OP_SRLA: // shift A right
            srl(a);
            break;
        case OP_SRLB: // shift B right
            srl(b);
            break;
        case OP_SRLC: // shift C right
            srl(c);
            break;
        case OP_AKCN: // A+K -> A until key down on N or D11 [sic]
            // Patent says sets condition if key down, but real behavior
            // is to set condition if addition overflows (i.e. no key down)
            add(a, mask, a);
            if (keyStrobeKN()) {
                // Advance to next instruction
            } else if (d_phase != REG_LEN - 2) {
                // Hold at current instruction and continue scan
                next_pc = program_counter;
            } else {
                // For state d10, fall through
            }
            break;
        case OP_AAKAH: // A+K -> A hex
            if (sinclair_flag) {
                sub(c, b, a); // C-B -> A -- Sinclair
            } else {
                add(a, mask, a, true /* hex */);
                condition_code = false;
            }
            break;
        case OP_SAKAH: // A-K -> A hex
            if (sinclair_flag) {
                sub(c, mask, b); // C-K -> B -- Sinclair
            } else {
                sub(a, mask, a, true /* hex */);
                condition_code = false;
            }
            break;
        case OP_ACKC: // C+K -> C
            add(c, mask, c);
            break;
        case OP_AABC: // A+B -> C -- Sinclair
            if (sinclair_flag) {
                add(a, b, c);
                break;
            }
        case OP_ACBC: // C+B -> C -- Sinclair
            if (sinclair_flag) {
                add(c, b, c);
                break;
            }
        default:
            throw std::runtime_error("Bad class3 instruction");
        }
    } else if ((instruction >> 8) == 5) {
        // Flag instruction
        switch (opcode) {
        case OP_NOP16: // no operation
            break;
        case OP_WAITDK: // wait for display key - what does it mean?
            if (key_pressed == 1) { // Never happens
                // Jump
                next_pc = instruction & 0x1ff;
            } else {
                // Hold address until DK pressed
                next_pc = program_counter;
            }
            break;
        case OP_WAITNO: // wait for key
            if (key_pressed) {
                // Jump
                next_pc = instruction & 0x1ff;
            } else {
                // Hold address until key pressed
                next_pc = program_counter;
            }
            break;
        case OP_SFB: // set flag B
            set_flags(bf, 1);
            break;
        case OP_SFA: // set flag A
            set_flags(af, 1);
            break;
        case OP_SYNC: // hold address until end of D10
            if (d_phase != REG_LEN - 2) {
                next_pc = program_counter;
            }
            condition_code = false;
            break;
        case OP_SCAN: // wait for key
            if (key_pressed) {
                condition_code = true;
                // Key is pressed.
            } else {
                condition_code = false;
                // No key pressed.

                if (d_phase != REG_LEN - 2) {
                    // Hold address until end of D10
                    next_pc = program_counter;
                }
            }
            break;
        case OP_ZFB: // zero flag B
            set_flags(bf, 0);
            break;
        case OP_ZFA: // zero flag A
            set_flags(af, 0);
            break;
        case OP_TFB: // test flag B
            test_flags(bf);
            break;
        case OP_TFA: // test flag A
            test_flags(af);
            break;
        case OP_FFB: // flip flag B
            set_flags(bf, -1 /* flip */);
            break;
        case OP_FFA: // flip flag A
            set_flags(af, -1 /* flip */);
            break;
        case OP_CF: // compare flags
            compare_flags(af, bf);
            break;
        case OP_NOP30: // no operation
            break;
        case OP_EXF: // exchange flags
            exchange(af, bf);
            break;
        default:
            throw std::runtime_error("Bad class2 instruction");
        }
    } else if (class_bits == 0) {
        // jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET
        if (!condition_code) {
            next_pc = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump

    } else if (class_bits == 1) {
        // jump if set: BID, BIO, BILT, BIC, BINE
        if (condition_code) {
            next_pc = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump

    } else if ((instruction >> 7) == 8) {
        // Jump if key down on KO (BKO)
        if (keyStrobeKO()) {
            next_pc = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump

    } else if ((instruction >> 7) == 9) {
        // Jump if key down on KP (BKP) - unused
        if (keyStrobeKP()) {
            next_pc = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump

    } else {
        throw std::runtime_error("Bad instruction");
    }

    if (trace_flag) {
        trace_state();
    }

    program_counter = next_pc;
    if (program_counter == 0) {
        throw std::runtime_error("Unexpected Reset");
    }

    // Update D state.
    d_phase += 1;
    if (d_phase > REG_LEN - 2) {
        d_phase = 0;
    }
}

//
// Run the code until stopped on wait.
//
void Calculator::run()
{
    if (key_pressed == 'C' || key_pressed == 'c') {
        // Reset the calculator: start from address 0.
        program_counter = 0;
        key_pressed = 0;
        d_phase = 0;
    }

    unsigned last_addr = program_counter;
    unsigned same_addr_count = 0;
    do {
        step();

        if (program_counter == 34 && key_pressed) {
            // Release key to avoid infinite loop in debouncer.
            key_pressed = 0;
        }

        // Stop when waiting on the same address for too long.
        if (program_counter == last_addr) {
            same_addr_count++;
        } else {
            same_addr_count = 0;
            last_addr = program_counter;
        }
    } while (same_addr_count < REG_LEN);
}
