#include <iostream>
#include "calculator.h"

static bool opsWithK(unsigned opcode)
{
    // OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
    // const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
    static const unsigned long LISTOPSWITHK = 1007135334;

    return (LISTOPSWITHK >> opcode) & 1;
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
    unsigned classBits = instruction >> 9;
    unsigned opcode    = (instruction >> 4) & 0x1f;

    if (classBits == 3 || (classBits == 2 && opcode > 18 && opcode != 21 && opcode != 22)) {
        unsigned maskno = instruction & 0x0f;

        for (unsigned i = 0; i < REG_LEN; i++) {
            char maskdigit = mask_tab[maskno][i];

            if (maskdigit == ' ') {
                mask[i] = maskdigit;
            } else if (classBits == 3 && opsWithK(opcode)) {
                // Register instruction
                mask[i] = maskdigit - '0';
            } else {
                mask[i] = '*';
            }
        }
    }
}

void Calculator::add(const Reg &x, const Reg &y, Reg &z, bool hex_flag)
{
    bool carry = false;

    for (int i = 10; i >= 0; i--) {
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
        // ccMeaning = carry ? 'overflow' : 'no overflow';
    }
}

void Calculator::sub(const Reg &x, const Reg &y, Reg &z, bool hex_flag)
{
    bool borrow = false;

    for (int i = 10; i >= 0; i--) {
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
        // ccMeaning = borrow ? 'borrow' : 'no borrow';
    }
}

void Calculator::compare(const Reg &x, const Reg &y)
{
    Reg tmp{};

    sub(x, y, tmp);
    // Compare sets condition if not borrow
    // ccMeaning = condition_code ? "less than" : "not less than";
}

void Calculator::copy(const Reg &x, Reg &z)
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            z[i] = x[i];
        }
    }
}

void Calculator::sll(Reg &x)
{
    int digit = 0;
    for (int i = 10; i >= 0; i--) {
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
    for (int i = 0; i <= 10; i++) {
        if (mask[i] != ' ') {
            int newdigit = x[i];
            x[i] = digit;
            digit = newdigit;
        }
    }
}

void Calculator::exchange(Reg &x, Reg &y)
{
    for (int i = 10; i >= 0; i--) {
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
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            x[i] = (val < 0) ? (1 - x[i]) : val;
        }
    }
}

void Calculator::compare_flags(const Reg &x, const Reg &y)
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (x[i] != y[i]) {
                condition_code = true;
                // ccMeaning = 'flags not equal';
            }
        }
    }
}

void Calculator::test_flags(const Reg &x)
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (x[i]) {
                condition_code = true;
                // ccMeaning = 'flag set';
            }
        }
    }
}

void Calculator::step()
{
    unsigned instruction = code_tab[program_counter];
    unsigned classBits = instruction >> 9;
    unsigned opcode = (instruction >> 4) & 0x1f;
    unsigned nextAddress = program_counter + 1;

    decode_mask(instruction);
    trace_instruction(instruction);

    if (classBits == 3) {
        // Register instruction
        switch (opcode) {
        case 0: // AABA: A+B -> A
            //displayInstruction(1);
            add(a, b, a);
            break;
        case 1: // AAKA: A+K -> A
            //displayInstruction(2);
            add(a, mask, a);
            break;
        case 2: // AAKC: A+K -> C
            //displayInstruction(3);
            add(a, mask, c);
            break;
        case 3:
            if (sinclair_flag) { // ACBB C+B -> B
                //displayInstruction(4);
                add(c, b, b);
            } else { // ABOA: B -> A
                //displayInstruction(5);
                copy(b, a);
            }
            break;
        case 4: // ABOC: B -> C
            //displayInstruction(6);
            copy(b, c);
            break;
        case 5: // ACKA: C+K -> A
            //displayInstruction(7);
            add(c, mask, a);
            break;
        case 6: // AKCB: C+K -> B
            //displayInstruction(8);
            add(c, mask, b);
            break;
        case 7: // SABA: A-B -> A
            //displayInstruction(9);
            sub(a, b, a);
            break;
        case 8: // SABC: A-B -> C
            //displayInstruction(10);
            sub(a, b, c);
            break;
        case 9: // SAKA: A-K -> A
            //displayInstruction(11);
            sub(a, mask, a);
            break;
        case 10: // SCBC: C-B -> C
            //displayInstruction(12);
            sub(c, b, c);
            break;
        case 11: // SCKC: C-K -> C
            //displayInstruction(13);
            sub(c, mask, c);
            break;
        case 12: // CAB: compare A-B
            //displayInstruction(14);
            compare(a, b);
            break;
        case 13: // CAK: compare A-K
            //displayInstruction(15);
            compare(a, mask);
            break;
        case 14: // CCB: compare C-B
            //displayInstruction(16);
            compare(c, b);
            break;
        case 15: // CCK: compare C-K
            //displayInstruction(17);
            compare(c, mask);
            break;
        case 16: // AKA: K -> A
            //displayInstruction(18);
            copy(mask, a);
            break;
        case 17: // AKB: K -> B
            //displayInstruction(19);
            copy(mask, b);
            break;
        case 18: // AKC: K -> C
            //displayInstruction(20);
            copy(mask, c);
            break;
        case 19: // EXAB: exchange A and B
            //displayInstruction(21);
            exchange(a, b);
            break;
        case 20: // SLLA: shift A left
            //displayInstruction(22);
            sll(a);
            break;
        case 21: // SLLB: shift B left
            //displayInstruction(23);
            sll(b);
            break;
        case 22: // SLLC: shift C left
            //displayInstruction(24);
            sll(c);
            break;
        case 23: // SRLA: shift A right
            //displayInstruction(25);
            srl(a);
            break;
        case 24: // SRLB: shift B right
            //displayInstruction(26);
            srl(b);
            break;
        case 25: // SRLC: shift C right
            //displayInstruction(66);
            srl(c);
            break;
        case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
            // Patent says sets condition if key down, but real behavior
            // is to set condition if addition overflows (i.e. no key down)
            add(a, mask, a);
            if (keyStrobeKN()) {
                //displayInstruction(27);
                // Advance to next instruction
            } else if (d_phase != REG_LEN - 2) {
                //displayInstruction(28);
                // Hold at current instruction and continue scan
                nextAddress = program_counter;
            } else {
                //displayInstruction(29);
                // For state d10, fall through
            }
            break;
        case 27:
            if (sinclair_flag) { // SCBA C-B -> A
                //displayInstruction(30);
                sub(c, b, a);
            } else { // AAKAH A+K -> A hex
                //displayInstruction(31);
                add(a, mask, a, true /* hex */);
                condition_code = false;
                // ccMeaning = '';
            }
            break;
        case 28:
            if (sinclair_flag) { // SCKB C-K -> B
                //displayInstruction(32);
                sub(c, mask, b);
            } else { // SAKAH A-K -> A hex
                //displayInstruction(33);
                sub(a, mask, a, true /* hex */);
                condition_code = false;
                // ccMeaning = '';
            }
            break;
        case 29: // ACKC: C+K -> C
            //displayInstruction(34);
            add(c, mask, c);
            break;
        case 30:
            if (sinclair_flag) { // AABC A+B -> C
                //displayInstruction(35);
                add(a, b, c);
                break;
            }
        case 31:
            if (sinclair_flag) { // ACBC C+B -> C
                //displayInstruction(36);
                add(c, b, c);
                break;
            }
        default:
            // bad instruction
            // alert('Bad instruction ' + instruction);
            //displayInstruction(37);
            break;
        }
    } else if ((instruction >> 8) == 5) {
        // Flag instruction
        switch (opcode) {
        case 16: // NOP
            //displayInstruction(38);
            break;
        case 17: // WAITDK: wait for display key
            if (key_pressed == 1) { // TODO: what does this mean?
                // Jump
                //displayInstruction(39);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until DK pressed
                //displayInstruction(40);
                nextAddress = program_counter;
            }
            break;
        case 18: // WAITNO: wait for key or address register overflow
            if (key_pressed) {
                // Jump
                //displayInstruction(41);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until key pressed or address overflow (TODO)
                //displayInstruction(42);
                nextAddress = program_counter;
            }
            break;
        case 19: // SFB: set flag B
            //displayInstruction(43);
            set_flags(bf, 1);
            break;
        case 20: // SFA: set flag A
            //displayInstruction(44);
            set_flags(af, 1);
            break;
        case 21: // SYNC (SYNCH): hold address until end of D10
            //displayInstruction(45);
            if (d_phase != REG_LEN - 2) {
                nextAddress = program_counter;
            }
            condition_code = false;
            // ccMeaning = '';
            break;
        case 22:                      // SCAN (SCANNO): wait for key
            if (key_pressed) {
                //displayInstruction(46);
                condition_code = true;
                // ccMeaning = 'key';
            } else {
                //displayInstruction(47);
                condition_code = false;
                // ccMeaning = 'no key';
                if (d_phase != REG_LEN - 2) {
                    // Hold address until end of D10
                    nextAddress = program_counter;
                }
            }
            break;
        case 23: // ZFB: zero flag B
            //displayInstruction(48);
            set_flags(bf, 0);
            break;
        case 24: // ZFA: zero flag A
            //displayInstruction(49);
            set_flags(af, 0);
            break;
        case 25: // TFB: test flag B
            //displayInstruction(50);
            test_flags(bf);
            break;
        case 26: // TFA: test flag A
            //displayInstruction(51);
            test_flags(af);
            break;
        case 27: // FFB: flip flag B
            //displayInstruction(52);
            set_flags(bf, -1 /* flip */);
            break;
        case 28: // FFA: flip flag A
            //displayInstruction(67);
            set_flags(af, -1 /* flip */);
            break;
        case 29: // CF: compare flags
            //displayInstruction(53);
            compare_flags(af, bf);
            break;
        case 30: // NOP
            //displayInstruction(54);
            break;
        case 31: // EXF: exchange flags
            //displayInstruction(55);
            exchange(af, bf);
            break;
        default:
            // bad instruction
            // alert('Bad instruction ' + instruction);
            //displayInstruction(56);
            break;
        }
    } else if (classBits == 0) {
        // jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET
        //displayInstruction(57);
        if (!condition_code) {
            //displayInstruction(58);
            nextAddress = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump
        // ccMeaning = '';
    } else if (classBits == 1) {
        // jump if set: BID, BIO, BILT, BIC, BINE
        //displayInstruction(59);
        if (condition_code) {
            //displayInstruction(60);
            nextAddress = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump
        // ccMeaning = '';
    } else if ((instruction >> 7) == 8) {
        // Jump if key down on KO (BKO)
        //displayInstruction(61);
        if (keyStrobeKO()) {
            //displayInstruction(62);
            nextAddress = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump
        // ccMeaning = '';
    } else if ((instruction >> 7) == 9) {
        // Jump if key down on KP (BKP)
        //displayInstruction(63);
        if (keyStrobeKP()) {
            //displayInstruction(64);
            nextAddress = instruction & 0x1ff;
        }
        condition_code = false; // Clear after jump
        // ccMeaning = '';
    } else {
        //displayInstruction(65);
        // bad instruction
        // alert('Bad instruction code ' + instruction);
    }
    program_counter = nextAddress;

    // Update D state.
    d_phase += 1;
    if (d_phase > REG_LEN - 2) {
        d_phase = 0;
    }

    //displayInstruction(68); // if printing is enabled, do a println after executing a line of code
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
            std::cout << "--- Release key '" << key_pressed << "'" << std::endl;
            key_pressed = 0;
        }

        // Stop when waiting on the same address for too long.
        if (program_counter == last_addr) {
            same_addr_count++;
        } else {
            same_addr_count = 0;
            last_addr = program_counter;
        }
    } while (same_addr_count < 11);
}
