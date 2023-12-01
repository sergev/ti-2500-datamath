#include <iostream>
#include "calculator.h"

static bool opsWithK(unsigned opcode)
{
    // OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
    // const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
    static const unsigned long LISTOPSWITHK = 1007135334;

    return (LISTOPSWITHK >> opcode) & 1;
}

void Calculator::update_mask(unsigned instruction)
{
    unsigned classBits = instruction >> 9;
    unsigned opcode    = (instruction >> 4) & 0x1f;

    if (classBits == 3 || (classBits == 2 && opcode > 18 && opcode != 21 && opcode != 22)) {
        unsigned maskno = instruction & 0x0f;

        for (unsigned i = 0; i <= 10; i++) {
            char maskdigit = masks[maskno][i];

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

void Calculator::add(int src1[], int src2[], int dst[], bool hex)
{
    unsigned carry = 0;
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            int result = src1[i] + src2[i] + carry;
            if (!hex && result >= 10) {
                result -= 10;
                carry = 1;
            } else if (hex && result >= 16) {
                result -= 16;
                carry = 1;
            } else {
                carry = 0;
            }
            dst[i] = result;
        }
    }
    if (carry) {
        cc = carry;
        // ccMeaning = carry ? 'overflow' : 'no overflow';
    }
}

void Calculator::sub(int src1[], int src2[], int dst[], bool hex)
{
    unsigned borrow = 0;
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            int result = src1[i] - src2[i] - borrow;
            if (result < 0) {
                result += hex ? 16 : 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            dst[i] = result;
        }
    }
    if (borrow) {
        cc = borrow;
        // ccMeaning = borrow ? 'borrow' : 'no borrow';
    }
}

void Calculator::compare(int src1[], int src2[])
{
    int tmp[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    sub(src1, src2, tmp);
    // Compare sets condition if not borrow
    // ccMeaning = cc ? "less than" : "not less than";
}

void Calculator::copy(int src[], int dst[])
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            dst[i] = src[i];
        }
    }
}

void Calculator::sll(int src[])
{
    int digit = 0;
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            int newdigit = src[i];
            src[i] = digit;
            digit = newdigit;
        }
    }
}

void Calculator::srl(int src[])
{
    int digit = 0;
    for (int i = 0; i <= 10; i++) {
        if (mask[i] != ' ') {
            int newdigit = src[i];
            src[i] = digit;
            digit = newdigit;
        }
    }
}

void Calculator::exchange(int src1[], int src2[])
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            int t = src1[i];
            src1[i] = src2[i];
            src2[i] = t;
        }
    }
}

void Calculator::writeFlag(int dest[], int val)
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            // Flip dst if val == -1, otherwise set to val
            dest[i] = (val < 0) ? (1 - dest[i]) : val;
        }
    }
}

void Calculator::compareFlags(int src1[], int src2[])
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (src1[i] != src2[i]) {
                cc = 1;
                // ccMeaning = 'flags not equal';
            }
        }
    }
}

void Calculator::testFlag(int src[])
{
    for (int i = 10; i >= 0; i--) {
        if (mask[i] != ' ') {
            if (src[i]) {
                cc = 1;
                // ccMeaning = 'flag set';
            }
        }
    }
}

void Calculator::step()
{
    unsigned instruction = objectCode[address];
    unsigned classBits = instruction >> 9;
    unsigned opcode = (instruction >> 4) & 0x1f;
    unsigned nextAddress = address + 1;

    // Serial.print(F(" data:"));
    // Serial.print(opcode);
    // Serial.print(F(" classbits:"));
    // Serial.print(classBits);
    update_mask(instruction);

    if (classBits == 3) {
        // Register instruction
        switch (opcode) {
        case 0: // AABA: A+B -> A
            displayInstruction(1);
            add(a, b, a);
            break;
        case 1: // AAKA: A+K -> A
            displayInstruction(2);
            add(a, mask, a);
            break;
        case 2: // AAKC: A+K -> C
            displayInstruction(3);
            add(a, mask, c);
            break;
        case 3:
            if (sinclair) { // ACBB C+B -> B
                displayInstruction(4);
                add(c, b, b);
            } else { // ABOA: B -> A
                displayInstruction(5);
                copy(b, a);
            }
            break;
        case 4: // ABOC: B -> C
            displayInstruction(6);
            copy(b, c);
            break;
        case 5: // ACKA: C+K -> A
            displayInstruction(7);
            add(c, mask, a);
            break;
        case 6: // AKCB: C+K -> B
            displayInstruction(8);
            add(c, mask, b);
            break;
        case 7: // SABA: A-B -> A
            displayInstruction(9);
            sub(a, b, a);
            break;
        case 8: // SABC: A-B -> C
            displayInstruction(10);
            sub(a, b, c);
            break;
        case 9: // SAKA: A-K -> A
            displayInstruction(11);
            sub(a, mask, a);
            break;
        case 10: // SCBC: C-B -> C
            displayInstruction(12);
            sub(c, b, c);
            break;
        case 11: // SCKC: C-K -> C
            displayInstruction(13);
            sub(c, mask, c);
            break;
        case 12: // CAB: compare A-B
            displayInstruction(14);
            compare(a, b);
            break;
        case 13: // CAK: compare A-K
            displayInstruction(15);
            compare(a, mask);
            break;
        case 14: // CCB: compare C-B
            displayInstruction(16);
            compare(c, b);
            break;
        case 15: // CCK: compare C-K
            displayInstruction(17);
            compare(c, mask);
            break;
        case 16: // AKA: K -> A
            displayInstruction(18);
            copy(mask, a);
            break;
        case 17: // AKB: K -> B
            displayInstruction(19);
            copy(mask, b);
            break;
        case 18: // AKC: K -> C
            displayInstruction(20);
            copy(mask, c);
            break;
        case 19: // EXAB: exchange A and B
            displayInstruction(21);
            exchange(a, b);
            break;
        case 20: // SLLA: shift A left
            displayInstruction(22);
            sll(a);
            break;
        case 21: // SLLB: shift B left
            displayInstruction(23);
            sll(b);
            break;
        case 22: // SLLC: shift C left
            displayInstruction(24);
            sll(c);
            break;
        case 23: // SRLA: shift A right
            displayInstruction(25);
            srl(a);
            break;
        case 24: // SRLB: shift B right
            displayInstruction(26);
            srl(b);
            break;
        case 25: // SRLC: shift C right
            displayInstruction(66);
            srl(c);
            break;
        case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
            // Patent says sets condition if key down, but real behavior
            // is to set condition if addition overflows (i.e. no key down)
            add(a, mask, a);
            if (keyStrobeKN()) {
                displayInstruction(27);
                // Advance to next instruction
            } else if (dActive != 10) {
                displayInstruction(28);
                // Hold at current instruction and continue scan
                nextAddress = address;
            } else {
                displayInstruction(29);
                // For state d10, fall through
            }
            break;
        case 27:
            if (sinclair) { // SCBA C-B -> A
                displayInstruction(30);
                sub(c, b, a);
            } else { // AAKAH A+K -> A hex
                displayInstruction(31);
                add(a, mask, a, 1 /* hex */);
                cc = 0;
                // ccMeaning = '';
            }
            break;
        case 28:
            if (sinclair) { // SCKB C-K -> B
                displayInstruction(32);
                sub(c, mask, b);
            } else { // SAKAH A-K -> A hex
                displayInstruction(33);
                sub(a, mask, a, 1 /* hex */);
                cc = 0;
                // ccMeaning = '';
            }
            break;
        case 29: // ACKC: C+K -> C
            displayInstruction(34);
            add(c, mask, c);
            break;
        case 30:
            if (sinclair) { // AABC A+B -> C
                displayInstruction(35);
                add(a, b, c);
                break;
            }
        case 31:
            if (sinclair) { // ACBC C+B -> C
                displayInstruction(36);
                add(c, b, c);
                break;
            }
        default:
            // bad instruction
            // alert('Bad instruction ' + instruction);
            displayInstruction(37);
            break;
        }
    } else if ((instruction >> 8) == 5) {
        // Flag instruction
        switch (opcode) {
        case 16: // NOP
            displayInstruction(38);
            break;
        case 17: // WAITDK: wait for display key
            if (keyPressed == DK) {
                // Jump
                displayInstruction(39);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until DK pressed
                displayInstruction(40);
                nextAddress = address;
            }
            break;
        case 18: // WAITNO: wait for key or address register overflow
            if (keyStrobeKO() || keyStrobeKN() || keyStrobeKP()) {
                // Jump
                displayInstruction(41);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until key pressed or address overflow (TODO)
                displayInstruction(42);
                nextAddress = address;
            }
            break;
        case 19: // SFB: set flag B
            displayInstruction(43);
            writeFlag(bf, 1);
            break;
        case 20: // SFA: set flag A
            displayInstruction(44);
            writeFlag(af, 1);
            break;
        case 21: // SYNC (SYNCH): hold address until end of D10
            displayInstruction(45);
            if (dActive != 10) {
                nextAddress = address;
            }
            cc = 0;
            // ccMeaning = '';
            break;
        case 22:                      // SCAN (SCANNO): wait for key
            if (keyStrobeKO() || keyStrobeKN() || keyStrobeKP()) {
                displayInstruction(46);
                cc = 1;
                // ccMeaning = 'key';
            } else {
                displayInstruction(47);
                cc = 0;
                // ccMeaning = 'no key';
                if (dActive != 10) {
                    // Hold address until end of D10
                    nextAddress = address;
                }
            }
            break;
        case 23: // ZFB: zero flag B
            displayInstruction(48);
            writeFlag(bf, 0);
            break;
        case 24: // ZFA: zero flag A
            displayInstruction(49);
            writeFlag(af, 0);
            break;
        case 25: // TFB: test flag B
            displayInstruction(50);
            testFlag(bf);
            break;
        case 26: // TFA: test flag A
            displayInstruction(51);
            testFlag(af);
            break;
        case 27: // FFB: flip flag B
            displayInstruction(52);
            writeFlag(bf, -1 /* flip */);
            break;
        case 28: // FFA: flip flag A
            displayInstruction(67);
            writeFlag(af, -1 /* flip */);
            break;
        case 29: // CF: compare flags
            displayInstruction(53);
            compareFlags(af, bf);
            break;
        case 30: // NOP
            displayInstruction(54);
            break;
        case 31: // EXF: exchange flags
            displayInstruction(55);
            exchange(af, bf);
            break;
        default:
            // bad instruction
            // alert('Bad instruction ' + instruction);
            displayInstruction(56);
            break;
        }
    } else if (classBits == 0) {
        // jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET
        displayInstruction(57);
        if (cc == 0) {
            displayInstruction(58);
            nextAddress = instruction & 0x1ff;
        }
        cc = 0; // Clear after jump
                             // ccMeaning = '';
    } else if (classBits == 1) {
        // jump if set: BID, BIO, BILT, BIC, BINE
        displayInstruction(59);
        if (cc == 1) {
            displayInstruction(60);
            nextAddress = instruction & 0x1ff;
        }
        cc = 0; // Clear after jump
                             // ccMeaning = '';
    } else if ((instruction >> 7) == 8) {
        // Jump if key down on KO (BKO)
        displayInstruction(61);
        if (keyStrobeKO()) {
            displayInstruction(62);
            nextAddress = instruction & 0x1ff;
        }
        cc = 0; // Clear after jump
                             // ccMeaning = '';
    } else if ((instruction >> 7) == 9) {
        // Jump if key down on KP (BKP)
        displayInstruction(63);
        if (keyStrobeKP()) {
            displayInstruction(64);
            nextAddress = instruction & 0x1ff;
        }
        cc = 0; // Clear after jump
                             // ccMeaning = '';
    } else {
        displayInstruction(65);
        // bad instruction
        // alert('Bad instruction code ' + instruction);
    }
    address = nextAddress;

    // Update D state.
    dActive += 1;
    if (dActive > 10) {
        dActive = 1;
    }

    displayInstruction(68); // if printing is enabled, do a println after executing a line of code
}

//
// Run the code until stopped on wait.
//
void Calculator::run()
{
    if (keyPressed == 'C') {
        // Reset the calculator: start from address 0.
        address = 0;
        keyPressed = 0;
        dActive = 1;
    }

    unsigned last_addr = address;
    unsigned same_addr_count = 0;
    do {
        step();

        if (address == 34 && keyPressed) {
            // Release keyboard.
            std::cout << "--- Release key '" << keyPressed << "'" << std::endl;
            keyPressed = 0;
        }

        // Stop when waiting on the same address for too long.
        if (address == last_addr) {
            same_addr_count++;
        } else {
            same_addr_count = 0;
            last_addr = address;
        }
    } while (same_addr_count < 11);
}
