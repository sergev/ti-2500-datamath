#include "globals.h"

bool opsWithK(uint8_t opcode)
{
    return (LISTOPSWITHK & (1UL << opcode));
}

unsigned int getInstruction(unsigned int PC)
{
    return objectCode[PC];
}

uint8_t getMaskNum()
{
    return getInstruction(calc.address) & 0x0f;
}

int8_t *getMask()
{
    unsigned int instruction = getInstruction(calc.address);
    uint8_t classBits = instruction >> 9;
    uint8_t opcode = (instruction >> 4) & 0x1f;

    if (classBits == 3 || (classBits == 2 && opcode > 18 && opcode != 21 && opcode != 22)) {
        uint8_t maskno = getMaskNum();

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

void add(int8_t src1[], int8_t src2[], int8_t dst[], bool hex = false)
{
    uint8_t carry = 0;
    getMask();
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

void sub(int8_t src1[], int8_t src2[], int8_t dst[], bool hex = false)
{
    uint8_t borrow = 0;
    getMask();
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

void compare(int8_t src1[], int8_t src2[])
{
    int8_t tmp[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    sub(src1, src2, tmp);
    // Compare sets condition if not borrow
    // calc.ccMeaning = calc.cc ? "less than" : "not less than";
}

void copy(int8_t src[], int8_t dst[])
{
    getMask();
    for (int8_t i = 10; i >= 0; i--) {
        if (calc.mask[i] == ' ') {
            // masked out
            // continue;
        } else {
            dst[i] = src[i];
        }
    }
}

void sll(int8_t src[])
{
    getMask();
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

void srl(int8_t src[])
{
    getMask();
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

void writeFlag(int8_t dest[], int8_t val)
{
    getMask();
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

void compareFlags(int8_t src1[], int8_t src2[])
{
    int8_t cc = 0;
    getMask();
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

void exchange(int8_t src1[], int8_t src2[])
{
    getMask();
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

void testFlag(int8_t src[])
{
    int8_t cc = 0;
    getMask();
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

void updateD()
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

void step()
{
    // Serial.print(F("addr:"));
    // Serial.print(calc.address);

    unsigned int instruction = getInstruction(calc.address);
    uint8_t classBits = instruction >> 9;
    uint8_t opcode = (instruction >> 4) & 0x1f;
    unsigned int nextAddress = calc.address + 1;

    // Serial.print(F(" data:"));
    // Serial.print(opcode);
    // Serial.print(F(" classbits:"));
    // Serial.print(classBits);

    if (classBits == 3) {
        // Register instruction
        switch (opcode) {
        case 0: // AABA: A+B -> A
            displayInstruction(1);
            add(calc.a, calc.b, calc.a);
            break;
        case 1: // AAKA: A+K -> A
            displayInstruction(2);
            add(calc.a, getMask(), calc.a);
            break;
        case 2: // AAKC: A+K -> C
            displayInstruction(3);
            add(calc.a, getMask(), calc.c);
            break;
        case 3:
            if (calc.sinclair) { // ACBB C+B -> B
                displayInstruction(4);
                add(calc.c, calc.b, calc.b);
            } else { // ABOA: B -> A
                displayInstruction(5);
                copy(calc.b, calc.a);
            }
            break;
        case 4: // ABOC: B -> C
            displayInstruction(6);
            copy(calc.b, calc.c);
            break;
        case 5: // ACKA: C+K -> A
            displayInstruction(7);
            add(calc.c, getMask(), calc.a);
            break;
        case 6: // AKCB: C+K -> B
            displayInstruction(8);
            add(calc.c, getMask(), calc.b);
            break;
        case 7: // SABA: A-B -> A
            displayInstruction(9);
            sub(calc.a, calc.b, calc.a);
            break;
        case 8: // SABC: A-B -> C
            displayInstruction(10);
            sub(calc.a, calc.b, calc.c);
            break;
        case 9: // SAKA: A-K -> A
            displayInstruction(11);
            sub(calc.a, getMask(), calc.a);
            break;
        case 10: // SCBC: C-B -> C
            displayInstruction(12);
            sub(calc.c, calc.b, calc.c);
            break;
        case 11: // SCKC: C-K -> C
            displayInstruction(13);
            sub(calc.c, getMask(), calc.c);
            break;
        case 12: // CAB: compare A-B
            displayInstruction(14);
            compare(calc.a, calc.b);
            break;
        case 13: // CAK: compare A-K
            displayInstruction(15);
            compare(calc.a, getMask());
            break;
        case 14: // CCB: compare C-B
            displayInstruction(16);
            compare(calc.c, calc.b);
            break;
        case 15: // CCK: compare C-K
            displayInstruction(17);
            compare(calc.c, getMask());
            break;
        case 16: // AKA: K -> A
            displayInstruction(18);
            copy(getMask(), calc.a);
            break;
        case 17: // AKB: K -> B
            displayInstruction(19);
            copy(getMask(), calc.b);
            break;
        case 18: // AKC: K -> C
            displayInstruction(20);
            copy(getMask(), calc.c);
            break;
        case 19: // EXAB: exchange A and B
            displayInstruction(21);
            exchange(calc.a, calc.b);
            break;
        case 20: // SLLA: shift A left
            displayInstruction(22);
            sll(calc.a);
            break;
        case 21: // SLLB: shift B left
            displayInstruction(23);
            sll(calc.b);
            break;
        case 22: // SLLC: shift C left
            displayInstruction(24);
            sll(calc.c);
            break;
        case 23: // SRLA: shift A right
            displayInstruction(25);
            srl(calc.a);
            break;
        case 24: // SRLB: shift B right
            displayInstruction(26);
            srl(calc.b);
            break;
        case 25: // SRLC: shift C right
            displayInstruction(66);
            srl(calc.c);
            break;
        case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
            // Patent says sets condition if key down, but real behavior
            // is to set condition if addition overflows (i.e. no key down)
            // calc.display_on = 0; //comment this line to glitch the display when a number key
            // is pressed (SINCLAIR behavior: actual hardware behavior)
            add(calc.a, getMask(), calc.a);
            if (calc.keyStrobeKN) {
                displayInstruction(27);
                // Advance to next instruction
            } else if (calc.dActive != 10) {
                displayInstruction(28);
                // Hold at current instruction and continue scan
                nextAddress = calc.address;
            } else {
                displayInstruction(29);
                // For state d10, fall through
            }
            break;
        case 27:
            if (calc.sinclair) { // SCBA C-B -> A
                displayInstruction(30);
                sub(calc.c, calc.b, calc.a);
            } else { // AAKAH A+K -> A hex
                displayInstruction(31);
                add(calc.a, getMask(), calc.a, 1 /* hex */);
                calc.cc = 0;
                // calc.ccMeaning = '';
            }
            break;
        case 28:
            if (calc.sinclair) { // SCKB C-K -> B
                displayInstruction(32);
                sub(calc.c, getMask(), calc.b);
            } else { // SAKAH A-K -> A hex
                displayInstruction(33);
                sub(calc.a, getMask(), calc.a, 1 /* hex */);
                calc.cc = 0;
                // calc.ccMeaning = '';
            }
            break;
        case 29: // ACKC: C+K -> C
            displayInstruction(34);
            add(calc.c, getMask(), calc.c);
            break;
        case 30:
            if (calc.sinclair) { // AABC A+B -> C
                displayInstruction(35);
                add(calc.a, calc.b, calc.c);
                break;
            }
        case 31:
            if (calc.sinclair) { // ACBC C+B -> C
                displayInstruction(36);
                add(calc.c, calc.b, calc.c);
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
            calc.display_on = 0;
            if (calc.keyPressed == DK) {
                // Jump
                displayInstruction(39);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until DK pressed
                displayInstruction(40);
                nextAddress = calc.address;
            }
            break;
        case 18: // WAITNO: wait for key or address register overflow
            if (calc.keyStrobeKO || calc.keyStrobeKN || calc.keyStrobeKP) {
                // Jump
                displayInstruction(41);
                nextAddress = instruction & 0x1ff;
            } else {
                // Hold address until key pressed or address overflow (TODO)
                displayInstruction(42);
                nextAddress = calc.address;
            }
            break;
        case 19: // SFB: set flag B
            displayInstruction(43);
            writeFlag(calc.bf, 1);
            break;
        case 20: // SFA: set flag A
            displayInstruction(44);
            writeFlag(calc.af, 1);
            break;
        case 21: // SYNC (SYNCH): hold address until end of D10
            displayInstruction(45);
            if (calc.dActive != 10) {
                nextAddress = calc.address;
            }
            calc.cc = 0;
            // calc.ccMeaning = '';
            break;
        case 22:                      // SCAN (SCANNO): wait for key
            calc.display_on = 1; // Reset display power off latch
            if (calc.keyStrobeKO || calc.keyStrobeKN || calc.keyStrobeKP) {
                displayInstruction(46);
                calc.cc = 1;
                // calc.ccMeaning = 'key';
            } else {
                displayInstruction(47);
                calc.cc = 0;
                // calc.ccMeaning = 'no key';
                if (calc.dActive != 10) {
                    // Hold address until end of D10
                    nextAddress = calc.address;
                }
            }
            break;
        case 23: // ZFB: zero flag B
            displayInstruction(48);
            writeFlag(calc.bf, 0);
            break;
        case 24: // ZFA: zero flag A
            displayInstruction(49);
            writeFlag(calc.af, 0);
            break;
        case 25: // TFB: test flag B
            displayInstruction(50);
            testFlag(calc.bf);
            break;
        case 26: // TFA: test flag A
            displayInstruction(51);
            testFlag(calc.af);
            break;
        case 27: // FFB: flip flag B
            displayInstruction(52);
            writeFlag(calc.bf, -1 /* flip */);
            break;
        case 28: // FFA: flip flag A
            displayInstruction(67);
            writeFlag(calc.af, -1 /* flip */);
            break;
        case 29: // CF: compare flags
            displayInstruction(53);
            compareFlags(calc.af, calc.bf);
            break;
        case 30: // NOP
            displayInstruction(54);
            break;
        case 31: // EXF: exchange flags
            displayInstruction(55);
            exchange(calc.af, calc.bf);
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
        if (calc.cc == 0) {
            displayInstruction(58);
            nextAddress = instruction & 0x1ff;
        }
        calc.cc = 0; // Clear after jump
                             // calc.ccMeaning = '';
    } else if (classBits == 1) {
        // jump if set: BID, BIO, BILT, BIC, BINE
        displayInstruction(59);
        if (calc.cc == 1) {
            displayInstruction(60);
            nextAddress = instruction & 0x1ff;
        }
        calc.cc = 0; // Clear after jump
                             // calc.ccMeaning = '';
    } else if ((instruction >> 7) == 8) {
        // Jump if key down on KO (BKO)
        displayInstruction(61);
        if (calc.keyStrobeKO) {
            calc.display_on = 0;
            displayInstruction(62);
            nextAddress = instruction & 0x1ff;
        }
        calc.cc = 0; // Clear after jump
                             // calc.ccMeaning = '';
    } else if ((instruction >> 7) == 9) {
        // Jump if key down on KP (BKP)
        displayInstruction(63);
        if (calc.keyStrobeKP) {
            calc.display_on = 0;
            displayInstruction(64);
            nextAddress = instruction & 0x1ff;
        }
        calc.cc = 0; // Clear after jump
                             // calc.ccMeaning = '';
    } else {
        displayInstruction(65);
        // bad instruction
        // alert('Bad instruction code ' + instruction);
    }
    calc.address = nextAddress;
    // Put the mask for the next instruction in the model for display
    // calc.mask = getMask();
    getMask();
    // Update D state
    updateD();

    displayInstruction(68); // if printing is enabled, do a println after executing a line of code
}
