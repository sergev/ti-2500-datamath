#include "calculator.h"

bool opsWithK(uint8_t opcode)
{
    return (LISTOPSWITHK & (1UL << opcode));
}

unsigned int getInstruction(unsigned int PC)
{
    return objectCode[PC];
}

uint8_t getMaskNum(const Calculator_t &calc)
{
    return getInstruction(calc.address) & 0x0f;
}

int8_t *getMask(Calculator_t &calc)
{
    unsigned int instruction = getInstruction(calc.address);
    uint8_t classBits = instruction >> 9;
    uint8_t opcode = (instruction >> 4) & 0x1f;

    if (classBits == 3 || (classBits == 2 && opcode > 18 && opcode != 21 && opcode != 22)) {
        uint8_t maskno = getMaskNum(calc);

        for (uint8_t i = 0; i <= 10; i++) {
            char maskdigit = masks[maskno][i];

            if (maskdigit == ' ') {
                calc.mask[i] = maskdigit;
            } else if (classBits == 3 && opsWithK(opcode)) {
                // Register instruction
                calc.mask[i] = maskdigit - '0';
                ;
            } else {
                calc.mask[i] = '*';
            }
        }
    } else {
        calc.mask[0] = 0;
    }

    return calc.mask;
}

void add(Calculator_t &calc, int8_t src1[], int8_t src2[], int8_t dst[], bool hex = false)
{
    uint8_t carry = 0;
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            int8_t result = src1[i] + src2[i] + carry;
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
        calc.cc = carry;
        // calc.ccMeaning = carry ? 'overflow' : 'no overflow';
    }
}

void sub(Calculator_t &calc, int8_t src1[], int8_t src2[], int8_t dst[], bool hex = false)
{
    uint8_t borrow = 0;
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            int8_t result = src1[i] - src2[i] - borrow;
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
        calc.cc = borrow;
        // calc.ccMeaning = borrow ? 'borrow' : 'no borrow';
    }
}

void compare(Calculator_t &calc, int8_t src1[], int8_t src2[])
{
    int8_t tmp[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    sub(calc, src1, src2, tmp);
    // Compare sets condition if not borrow
    // calc.ccMeaning = calc.cc ? "less than" : "not less than";
}

void copy(Calculator_t &calc, int8_t src[], int8_t dst[])
{
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            dst[i] = src[i];
        }
    }
}

void sll(Calculator_t &calc, int8_t src[])
{
    getMask(calc);
    int8_t digit = 0;
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            int8_t newdigit = src[i];
            src[i] = digit;
            digit = newdigit;
        }
    }
}

void srl(Calculator_t &calc, int8_t src[])
{
    getMask(calc);
    int8_t digit = 0;
    for (int8_t i = 0; i <= 10; i++) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            int8_t newdigit = src[i];
            src[i] = digit;
            digit = newdigit;
        }
    }
}

void writeFlag(Calculator_t &calc, int8_t dest[], int8_t val)
{
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            // Flip dst if val == -1, otherwise set to val
            dest[i] = (val < 0) ? (1 - dest[i]) : val;
        }
    }
}

void compareFlags(Calculator_t &calc, int8_t src1[], int8_t src2[])
{
    int8_t cc = 0;
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            if (src1[i] != src2[i]) {
                cc = 1;
            }
        }
    }
    if (cc) {
        calc.cc = 1;
        // calc.ccMeaning = 'flags not equal';
    }
}

void exchange(Calculator_t &calc, int8_t src1[], int8_t src2[])
{
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            int8_t t = src1[i];
            src1[i] = src2[i];
            src2[i] = t;
        }
    }
}

void testFlag(Calculator_t &calc, int8_t src[])
{
    int8_t cc = 0;
    getMask(calc);
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            if (src[i]) {
                cc = 1;
            }
        }
    }
    /* Only update cc if bit set */
    if (cc) {
        calc.cc = cc;
        // calc.ccMeaning = 'flag set';
    }
}

void updateD(Calculator_t &calc)
{
    for (int8_t i = 10; i >= 0; i--) {
        calc.d[i] = 1;
    }

    calc.dActive += 1;
    if (calc.dActive > 10) {
        calc.dActive = 1;
    }
    calc.d[calc.dActive - 1] = 0;
}

void Calculator_t::step()
{
    unsigned int instruction = getInstruction(address);
    uint8_t classBits = instruction >> 9;
    uint8_t opcode = (instruction >> 4) & 0x1f;
    unsigned int nextAddress = address + 1;

    // Serial.print(F(" data:"));
    // Serial.print(opcode);
    // Serial.print(F(" classbits:"));
    // Serial.print(classBits);

    if (classBits == 3) {
        // Register instruction
        switch (opcode) {
        case 0: // AABA: A+B -> A
            displayInstruction(1);
            add(*this, a, b, a);
            break;
        case 1: // AAKA: A+K -> A
            displayInstruction(2);
            add(*this, a, getMask(*this), a);
            break;
        case 2: // AAKC: A+K -> C
            displayInstruction(3);
            add(*this, a, getMask(*this), c);
            break;
        case 3:
            if (sinclair) { // ACBB C+B -> B
                displayInstruction(4);
                add(*this, c, b, b);
            } else { // ABOA: B -> A
                displayInstruction(5);
                copy(*this, b, a);
            }
            break;
        case 4: // ABOC: B -> C
            displayInstruction(6);
            copy(*this, b, c);
            break;
        case 5: // ACKA: C+K -> A
            displayInstruction(7);
            add(*this, c, getMask(*this), a);
            break;
        case 6: // AKCB: C+K -> B
            displayInstruction(8);
            add(*this, c, getMask(*this), b);
            break;
        case 7: // SABA: A-B -> A
            displayInstruction(9);
            sub(*this, a, b, a);
            break;
        case 8: // SABC: A-B -> C
            displayInstruction(10);
            sub(*this, a, b, c);
            break;
        case 9: // SAKA: A-K -> A
            displayInstruction(11);
            sub(*this, a, getMask(*this), a);
            break;
        case 10: // SCBC: C-B -> C
            displayInstruction(12);
            sub(*this, c, b, c);
            break;
        case 11: // SCKC: C-K -> C
            displayInstruction(13);
            sub(*this, c, getMask(*this), c);
            break;
        case 12: // CAB: compare A-B
            displayInstruction(14);
            compare(*this, a, b);
            break;
        case 13: // CAK: compare A-K
            displayInstruction(15);
            compare(*this, a, getMask(*this));
            break;
        case 14: // CCB: compare C-B
            displayInstruction(16);
            compare(*this, c, b);
            break;
        case 15: // CCK: compare C-K
            displayInstruction(17);
            compare(*this, c, getMask(*this));
            break;
        case 16: // AKA: K -> A
            displayInstruction(18);
            copy(*this, getMask(*this), a);
            break;
        case 17: // AKB: K -> B
            displayInstruction(19);
            copy(*this, getMask(*this), b);
            break;
        case 18: // AKC: K -> C
            displayInstruction(20);
            copy(*this, getMask(*this), c);
            break;
        case 19: // EXAB: exchange A and B
            displayInstruction(21);
            exchange(*this, a, b);
            break;
        case 20: // SLLA: shift A left
            displayInstruction(22);
            sll(*this, a);
            break;
        case 21: // SLLB: shift B left
            displayInstruction(23);
            sll(*this, b);
            break;
        case 22: // SLLC: shift C left
            displayInstruction(24);
            sll(*this, c);
            break;
        case 23: // SRLA: shift A right
            displayInstruction(25);
            srl(*this, a);
            break;
        case 24: // SRLB: shift B right
            displayInstruction(26);
            srl(*this, b);
            break;
        case 25: // SRLC: shift C right
            displayInstruction(66);
            srl(*this, c);
            break;
        case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
            // Patent says sets condition if key down, but real behavior
            // is to set condition if addition overflows (i.e. no key down)
            // display_on = 0; //comment this line to glitch the display when a number key
            // is pressed (SINCLAIR behavior: actual hardware behavior)
            add(*this, a, getMask(*this), a);
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
                sub(*this, c, b, a);
            } else { // AAKAH A+K -> A hex
                displayInstruction(31);
                add(*this, a, getMask(*this), a, 1 /* hex */);
                cc = 0;
                // ccMeaning = '';
            }
            break;
        case 28:
            if (sinclair) { // SCKB C-K -> B
                displayInstruction(32);
                sub(*this, c, getMask(*this), b);
            } else { // SAKAH A-K -> A hex
                displayInstruction(33);
                sub(*this, a, getMask(*this), a, 1 /* hex */);
                cc = 0;
                // ccMeaning = '';
            }
            break;
        case 29: // ACKC: C+K -> C
            displayInstruction(34);
            add(*this, c, getMask(*this), c);
            break;
        case 30:
            if (sinclair) { // AABC A+B -> C
                displayInstruction(35);
                add(*this, a, b, c);
                break;
            }
        case 31:
            if (sinclair) { // ACBC C+B -> C
                displayInstruction(36);
                add(*this, c, b, c);
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
            display_on = 0;
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
            writeFlag(*this, bf, 1);
            break;
        case 20: // SFA: set flag A
            displayInstruction(44);
            writeFlag(*this, af, 1);
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
            display_on = 1; // Reset display power off latch
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
            writeFlag(*this, bf, 0);
            break;
        case 24: // ZFA: zero flag A
            displayInstruction(49);
            writeFlag(*this, af, 0);
            break;
        case 25: // TFB: test flag B
            displayInstruction(50);
            testFlag(*this, bf);
            break;
        case 26: // TFA: test flag A
            displayInstruction(51);
            testFlag(*this, af);
            break;
        case 27: // FFB: flip flag B
            displayInstruction(52);
            writeFlag(*this, bf, -1 /* flip */);
            break;
        case 28: // FFA: flip flag A
            displayInstruction(67);
            writeFlag(*this, af, -1 /* flip */);
            break;
        case 29: // CF: compare flags
            displayInstruction(53);
            compareFlags(*this, af, bf);
            break;
        case 30: // NOP
            displayInstruction(54);
            break;
        case 31: // EXF: exchange flags
            displayInstruction(55);
            exchange(*this, af, bf);
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
            display_on = 0;
            displayInstruction(62);
            nextAddress = instruction & 0x1ff;
        }
        cc = 0; // Clear after jump
                             // ccMeaning = '';
    } else if ((instruction >> 7) == 9) {
        // Jump if key down on KP (BKP)
        displayInstruction(63);
        if (keyStrobeKP()) {
            display_on = 0;
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
    // Put the mask for the next instruction in the model for display
    // mask = getMask(*this);
    getMask(*this);
    // Update D state
    updateD(*this);

    displayInstruction(68); // if printing is enabled, do a println after executing a line of code
}

//
// Run the code until stopped on wait.
//
void Calculator_t::run()
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

        // Stop when waiting on the same address for too long.
        if (address == last_addr) {
            same_addr_count++;
        } else {
            same_addr_count = 0;
            last_addr = address;
        }
    } while (same_addr_count < 11);
}
