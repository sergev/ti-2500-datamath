#include <iostream>
#include "calculator.h"

void Calculator_t::displayInstruction(unsigned instructionid)
{
    auto &out = std::cout;

    if (instructionid == 68) {
        out << std::endl; // print end of line at end of void step()
        return;
    }

    out << '(' << address
        << ", " << dActive
        << ") " << instructionid << ": ";
    switch (instructionid) {
    case 1:
        out << "AABA: A+B -> A";
        break;

    case 2:
        out << "AABA: A+B -> A";
        break;

    case 3:
        out << "AAKC: A+K -> C";
        break;

    case 4:
        out << "ACBB C+B -> B";
        break;

    case 5:
        out << "ABOA: B -> A";
        break;

    case 6:
        out << "ABOC: B -> C";
        break;

    case 7:
        out << "ACKA: C+K -> A";
        break;

    case 8:
        out << "AKCB: C+K -> B";
        break;

    case 9:
        out << "SABA: A-B -> A";
        break;

    case 10:
        out << "SABC: A-B -> C";
        break;

    case 11:
        out << "SAKA: A-K -> A";
        break;

    case 12:
        out << "SCBC: C-B -> C";
        break;

    case 13:
        out << "SCKC: C-K -> C";
        break;

    case 14:
        out << "CAB: compare A-B";
        break;

    case 15:
        out << "CAK: compare A-K";
        break;

    case 16:
        out << "CCB: compare C-B";
        break;

    case 17:
        out << "CCK: compare C-K";
        break;

    case 18:
        out << "AKA: K -> A";
        break;

    case 19:
        out << "AKB: K -> B";
        break;

    case 20:
        out << "AKC: K -> C";
        break;

    case 21:
        out << "EXAB: exchange A and B";
        break;

    case 22:
        out << "SLLA: shift A left";
        break;

    case 23:
        out << "SLLB: shift B left";
        break;

    case 24:
        out << "SLLC: shift C left";
        break;

    case 25:
        out << "SRLA: shift A right";
        break;

    case 26:
        out << "SRLB: shift B right";
        break;

    case 27:
        out << "AKCN: A+K -> A until key down on N or D11 [sic] (ADVANCE)";
        break;

    case 28:
        out << "AKCN: A+K -> A until key down on N or D11 [sic] (HOLD)";
        break;

    case 29:
        out << "AKCN: A+K -> A until key down on N or D11 [sic] (D10 FALLTHROUGH)";
        break;

    case 30:
        out << "SCBA C-B -> A";
        break;

    case 31:
        out << "AAKAH A+K -> A hex";
        break;

    case 32:
        out << "SCKB C-K -> B";
        break;

    case 33:
        out << "SAKAH A-K -> A hex";
        break;

    case 34:
        out << "ACKC: C+K -> C";
        break;

    case 35:
        out << "AABC A+B -> C";
        break;

    case 36:
        out << "ACBC C+B -> C";
        break;

    case 37:
        out << "Bad instruction";
        break;

    case 38:
        out << "NOP";
        break;

    case 39:
        out << "WAITDK: wait for display key DK (JUMP)";
        break;

    case 40:
        out << "WAITDK: wait for display key DK (HOLD)";
        break;

    case 41:
        out << "WAITNO: wait for key or address register overflow (JUMP)";
        break;

    case 42:
        out << "WAITNO: wait for key or address register overflow (HOLDING)";
        break;

    case 43:
        out << "SFB: set flag B";
        break;

    case 44:
        out << "SFA: set flag A";
        break;

    case 45:
        out << "SYNC (SYNCH): hold address until end of D10";
        break;

    case 46:
        out << "SCAN (SCANNO): wait for key (KEY)";
        break;

    case 47:
        out << "SCAN (SCANNO): wait for key (NO KEY)";
        break;

    case 48:
        out << "ZFB: zero flag B";
        break;

    case 49:
        out << "ZFA: zero flag A";
        break;

    case 50:
        out << "TFB: test flag B";
        break;

    case 51:
        out << "TFA: test flag A";
        break;

    case 52:
        out << "FFB: flip flag B";
        break;

    case 53:
        out << "CF: compare flags";
        break;

    case 54:
        out << "NOP";
        break;

    case 55:
        out << "EXF: exchange flags";
        break;

    case 56:
        out << "Bad instruction";
        break;

    case 57:
        out << "Jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET ";
        break;

    case 58:
        out << "JUMP IF RESET TAKEN";
        break;

    case 59:
        out << "Jump if set: BID, BIO, BILT, BIC, BINE";
        break;

    case 60:
        out << "JUMP IF SET TAKEN";
        break;

    case 61:
        out << "Jump if key down on KO (BKO)";
        break;

    case 62:
        out << "JUMP IF KEY DOWN ON KO TAKEN";
        break;

    case 63:
        out << "Jump if key down on KP (BKP)";
        break;

    case 64:
        out << "JUMP IF KEY DOWN ON KP TAKEN";
        break;

    case 65:
        out << "Bad instruction code";
        break;

    case 66:
        out << "SRLC: shift C right";
        break;

    case 67:
        out << "FFA: flip flag A";
        break;

    default:
        out << "UNKNOWN INSTRUCTION ID";
        break;
    }
}
