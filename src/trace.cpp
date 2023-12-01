#include <iostream>
#include <iomanip>
#include <sstream>
#include "calculator.h"

std::string Calculator::disassemble_opcode(unsigned class_bits, unsigned opcode) const
{
    switch (class_bits) {
    default:
    case 0:
        return "jump.on.false";
    case 1:
        return "jump.on.true";
    case 2:
        // Operations on flags.
        switch (opcode) {
        default:
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            return "jump.key.o";
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
            return "jump.key.p";
        case 16:
            return "nop";
        case 17:
            return "wait.display.key"; // WAITDK: wait for display key
        case 18:
            return "wait.key"; // WAITNO: wait for key or address register overflow
        case 19:
            return "set.bf"; // SFB: set flag B
        case 20:
            return "set.af"; // SFA: set flag A
        case 21:
            return "sync"; // SYNC (SYNCH): hold address until end of D10
        case 22:
            return "scan"; // SCAN (SCANNO): wait for key
        case 23:
            return "zero.bf"; // ZFB: zero flag B
        case 24:
            return "zero.af"; // ZFA: zero flag A
        case 25:
            return "test.bf"; // TFB: test flag B
        case 26:
            return "test.af"; // TFA: test flag A
        case 27:
            return "flip.bf"; // FFB: flip flag B
        case 28:
            return "flip.af"; // FFA: flip flag A
        case 29:
            return "compare.af.bf"; // CF: compare flags
        case 30:
            return "nop";
        case 31:
            return "exchange.af.bf"; // EXF: exchange flags
        }
    case 3:
        // Arithmetic operations.
        switch (opcode) {
        default:
        case 0:
            return "add.ab.a"; // AABA: A+B -> A
        case 1:
            return "add.ak.a"; // AAKA: A+K -> A
        case 2:
            return "add.ak.c"; // AAKC: A+K -> C
        case 3:
            if (sinclair_flag) {
                return "add.cb.b"; // ACBB C+B -> B
            } else {
                return "copy.b.a"; // ABOA: B -> A
            }
        case 4:
            return "copy.b.c"; // ABOC: B -> C
        case 5:
            return "add.ck.a"; // ACKA: C+K -> A
        case 6:
            return "add.ck.b"; // AKCB: C+K -> B
        case 7:
            return "sub.ab.a"; // SABA: A-B -> A
        case 8:
            return "sub.ab.c"; // SABC: A-B -> C
        case 9:
            return "sub.ak.a"; // SAKA: A-K -> A
        case 10:
            return "sub.cb.c"; // SCBC: C-B -> C
        case 11:
            return "sub.ck.c"; // SCKC: C-K -> C
        case 12:
            return "compare.ab"; // CAB: compare A-B
        case 13:
            return "compare.ak"; // CAK: compare A-K
        case 14:
            return "compare.cb"; // CCB: compare C-B
        case 15:
            return "compare.ck"; // CCK: compare C-K
        case 16:
            return "copy.k.a"; // AKA: K -> A
        case 17:
            return "copy.k.b"; // AKB: K -> B
        case 18:
            return "copy.k.c"; // AKC: K -> C
        case 19:
            return "exchange.a.b"; // EXAB: exchange A and B
        case 20:
            return "shift.a.left"; // SLLA: shift A left
        case 21:
            return "shift.b.left"; // SLLB: shift B left
        case 22:
            return "shift.c.left"; // SLLC: shift C left
        case 23:
            return "shift.a.right"; // SRLA: shift A right
        case 24:
            return "shift.b.right"; // SRLB: shift B right
        case 25:
            return "shift.c.right"; // SRLC: shift C right
        case 26:
            return "add.ak.a.key.n"; // AKCN: A+K -> A until key down on N or D11 [sic]
        case 27:
            if (sinclair_flag) {
                return "sub.cb.a"; // SCBA C-B -> A
            } else {
                return "hexadd.ak.a"; // AAKAH A+K -> A hex
            }
        case 28:
            if (sinclair_flag) {
                return "sub.ck.b"; // SCKB C-K -> B
            } else {
                return "hexsub.ak.a"; // SAKAH A-K -> A hex
            }
        case 29:
            return "add.ck.c"; // ACKC: C+K -> C
        case 30:
            return "add.ab.c"; // AABC A+B -> C
        case 31:
            return "add.cb.c"; // ACBC C+B -> C
        }
    }
}

std::string Calculator::disassemble_mask() const
{
    std::stringstream out;

    for (unsigned i = 0; i < REG_LEN; i++) {
        char ch = mask[i];

        if (ch > ' ') {
            out << ch;
        } else if (ch == ' ') {
            out << '.';
        } else {
            out << (unsigned)ch;
        }
    }
    return out.str();
}

void Calculator::trace_instruction(unsigned instruction) const
{
    unsigned class_bits = instruction >> 9;
    unsigned opcode     = (instruction >> 4) & 0x1f;
    unsigned maskno     = instruction & 0x0f;

    std::string name = disassemble_opcode(class_bits, opcode);

    std::string operand;
    if (op_with_mask(class_bits, opcode)) {
        // Note: mask is already decoded.
        operand = disassemble_mask();
    }

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
