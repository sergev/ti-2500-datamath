#include "calculator.h"

//
// These are the 320 instructions that performed all the tasks on the TI-2500 calculator.
//
// 000 .CLEAR   ZFA    ALL    ; Power on, clear flags
// 001          ZFB    ALL
// 002          AKA    ALL    ; Clear A and C
// 003          AKC    ALL
// 004          BET    RJ     ; Reset condition latch
// 005 .RJ      CAK    MANT1  ; Right justify A, maybe add minu...
// 006          BILT   ZERO   ; Mantissa is zero?
// 007 .S       TFA    SIGN
// 008          BIZ    RJ1    ; A is positive
// 009          SAKAH  OV1    ; Minus sign is hex 14
// 010          SAKAH  OV1
// 011          BET    RJ1    ; Reset condition latch
// 012 .RJ1     CAK    LSD1   ; Right justify the display
// 013          BIGE   DPTPOS
// 014          CAK    EXP1
// 015          BILT   DPTPOS
// 016          SRLA   MANT   ; Shift A mantissa right
// 017          SAKA   EXP1   ; Decrement A exponent
// 018          BET    RJ1
// 019 .ZERO    AKA    ALL    ; A is zero, clear entire register
// 020          ZFA    SIGN   ; Clear A sign flag
// 021 .DPTPOS  EXAB   ALL    ; Compute decimal point position...
// 022          AKA    MANT1
// 023          AAKA   MANT1
// 024          EXAB   MANT   ; B mantissa is 2
// 025          ABOA   EXP    ; Restore A exponent
// 026 .DP1     SAKA   EXP1   ; Shift B (2) left by A exponent...
// 027          BILT   DP2
// 028          SLLB   MANT
// 029          BET    DP1
// 030 .DP2     ABOA   EXP
// 031          TFB    F10
// 032          BIO    ID1
// 033 .LOCK    SYNC          ; Debounce: wait for no key for a few cycles...
// 034          SCAN
// 035          BID    LOCK
// 036          SYNC
// 037          SCAN
// 038          BID    LOCK
// 039          ACKB   EXP
// 040 .IDLE    AKC    EXP    ; Idle loop entry
// 041 .ID1     ZFB    F10
// 042 .ID2     WAITNO ID3    ; Wait for key press or timeout
// 043 .ID4     SYNC
// 044          SCAN
// 045          BID    KEY    ; Key down
// 046 .FD1     ACKC   EXP1
// 047          CCK    TIM4   ; Check number of timeouts
// 048          BILT   FD3
// 049          AKC    EXP
// 050          SYNC
// 051          WAITDK ID5    ; Turn off display, wait for DK
// 052 .FD3     TFB    F5     ; Test overflow
// 053          BIZ    ID2
// 054          SFB    F10
// 055          AKC    MANT
// 056          SCKC   MANT1
// 057          BET    FD5    ; Reset condition latch
// 058 .FD5     ACKB   MANT
// 059          ACKC   EXP1
// 060          WAITNO FD4
// 061 .MINKEY  SFB    OP2    ; MINUS = flags II, III
// 062          TFA    F3     ; If CD is clear...
// 063          BIZ    PLSKEY ; perform subtract operation
// 064          TFA    F4     ; Or EQ is clear...
// 065          BIO    PLSKEY ; perform subtract operation
// 066          TFA    OP1    ; Or not after multi/div...
// 067          BIZ    PLSKEY ; perform subtract operation
// 068          ZFB    OP2    ; Minus key as sign after mult or divide
// 069          SFA    SIGN   ; Set sign to minus
// 070          AKA    ALL    ; Clear A
// 071          SFA    F9     ; Set ST flag (sign temp) for unary minus
// 072          AAKA   DPT7   ; Set A exponent to 7
// 073          ZFA    F3     ; Clear CD
// 074          BET    S      ; Update display
// 075 .DIVKEY  SFB    OP2    ; DIV = flags II, III
// 076 .MLTKEY  SFB    OP1    ; MULT = flags I, III
// 077 .PLSKEY  SFB    OP3    ; PLUS = flag III
// 078          TFA    F3     ; Clear display flag
// 079          BIO    KS1
// 080          TFA    F4     ; Equal flag
// 081          BIZ    KS2
// 082 .KS1     ZFA    F4
// 083          SFA    F5
// 084          BET    KS2
// 085 .EQLKEY  SFA    F4     ; F4 (EQ) flag set for equals
// 086 .KS2     CAK    MANT1
// 087          BILT   KS6    ; A mantissa == 0?
// 088 .KS3     CAK    OV1    ; Left justify A...
// 089          BIGE   KS4    ; Digit has reached left?
// 090          SLLA   MANT   ; Shift mantissa left
// 091          AAKA   EXP1   ; Increment exponent
// 092          BET    KS3
// 093 .DPTKEY  SFA    F10    ; Decimal point key...
// 094          ZFA    F9     ; Set DPT flag, clear ST flag
// 095          TFA    F3
// 096          BIZ    DPTPOS
// 097          ZFA    F3
// 098          BET    ZERO
// 099 .CEKEY   TFA    F3
// 100          BIO    DPTPOS
// 101          SFA    F3
// 102          ZFA    F10
// 103          BET    ZERO
// 104 .KEY     ABOC   EXP    ; Process key down...
// 105          TFB    F5     ; Test overflow
// 106          BIO    LOCK   ; overflow never gets cleared???
// 107          SYNC          ; Wait to debounce key
// 108          SYNC
// 109          SCAN
// 110          BIU    IDLE
// 111          AKB    ALL
// 112          SYNC
// 113          BKO    CLEAR  ; Clear key pressed?
// 114          BKO    EQLKEY ; Equal key pressed?
// 115          BKO    PLSKEY ; Plus key pressed?
// 116          BKO    MINKEY ; Minus key pressed?
// 117          BKO    MLTKEY ; Mult key pressed?
// 118          BKO    DIVKEY ; Divide key pressed?
// 119          BKO    CEKEY  ; CE key pressed?
// 120          BKO    DPTKEY ; Decimal point key pressed?
// 121          BKO    ZERKEY ; Zero key pressed?
// 122          EXAB   ALL    ; Process digit key...
// 123          AKCN   LSD1   ; Count key position into A
// 124          EXAB   ALL
// 125          BIC    DPTPOS ; No key pressed
// 126 .ZERKEY  TFA    F3     ; Clear display flag
// 127          BIZ    NUM1
// 128 .CD      ZFA    SIGN
// 129 .CD1     AKA    ALL
// 130          ZFA    F3
// 131 .NUM1    TFA    SIGN   ; Number: Digit is in B...
// 132          BIZ    NUM3   ; Branch if A SIGN clear
// 133          AAKAH  OV1    ; Test for 15 in OV1?
// 134          AAKAH  OV1
// 135          SAKAH  OV1
// 136          SAKAH  OV1
// 137          BIC    DPTPOS ; Borrow set if OV1 == 15?
// 138 .NUM2    TFA    F9     ; Test ST (sign temp) flag
// 139          BIZ    NUM4
// 140          TFA    F10
// 141          BIO    NUM7
// 142          EXAB   ALL
// 143          CAK    MANT1
// 144          EXAB   ALL
// 145          BILT   DPTPOS
// 146 .NUM7    ZFA    F9     ; Clear ST flag
// 147          BET    NUM6
// 148 .NUM3    CAK    MSD1
// 149          BIGE   DPTPOS
// 150          CAK    DPT7
// 151          BIGE   DPTPOS
// 152 .NUM4    TFA    F10
// 153          BIZ    NUM5
// 154          AAKA   EXP1
// 155 .NUM5    SLLA   MANT
// 156 .NUM6    ABOA   LSD1
// 157          BET    DPTPOS
// 158 .KS6     AKA    EXP
// 159          ZFA    SIGN
// 160          AAKA   DPT7
// 161 .KS7     ZFA    FD     ; Zeros temp(num) and dpt flag
// 162          TFA    F5     ; Post flag
// 163          BIO    POST
// 164          TFA    OP3
// 165          BIZ    POST
// 166          ACKB   ALL
// 167          TFB    F3     ; Constant flag
// 168          BIZ    KS8
// 169          ZFB    F3
// 170          EXAB   ALL
// 171          ABOC   ALL
// 172          EXF    SIGN
// 173 .KS8     TFA    OP1
// 174          BIO    MLTDIV
// 175 .ADDSUB  TFB    SIGN   ; Add / subtract...
// 176          BIZ    AS1    ; Copy B sign to TEMP...
// 177          SFA    F5
// 178 .AS1     TFA    OP2    ; Flip B sign for subtract...
// 179          BIZ    AS2
// 180          FFB    SIGN
// 181 .AS2     CAB    EXP    ; Make sure A exponent >= B exponent...
// 182          BIGE   AS3
// 183          EXAB   ALL    ; Exchange if A exponent smaller
// 184          EXF    SIGN   ; And exchange the signs
// 185 .AS3     SAKA   EXP1   ; Shift A right until exponents equal...
// 186          CAB    EXP    ; Infinite loop if B EXP == 0?
// 187          BILT   AS4
// 188          SRLA   MANT
// 189          BET    AS3
// 190 .AS4     AAKA   EXP1   ; Exponents now equal
// 191          BET    AS5    ; Reset condition
// 192 .AS5     CAB    MANT   ; Make sure A >= B...
// 193          BIGE   AS6
// 194          EXAB   ALL    ; If not, exchange exponents
// 195          EXF    SIGN   ; and exchange signs
// 196 .AS6     CF     SIGN
// 197          BIE    AS7    ; If signs equal, add...
// 198          SABA   MANT   ; Otherwise subtract
// 199          BET    AS8
// 200 .AS7     AABA   MANT   ; Add mantissas
// 201 .AS8     ZFB    SIGN   ; Restore B sign from TEMP flag...
// 202          TFA    F5
// 203          BIZ    POST
// 204          SFB    SIGN
// 205          ZFA    F5     ; And clear TEMP
// 206          BET    POST   ; Done add/sub
// 207 .MLTDIV  CF     SIGN   ; Mult / div operation...
// 208          ZFA    SIGN   ; Positive result if signs the same
// 209          BIE    MD1
// 210          SFA    SIGN   ; A, B signs different: negative result
// 211 .MD1     TFA    OP2
// 212          BIC    DIV
// 213          AAKC   ALL    ; Copy A to C
// 214          AKA    MANT   ; Clear A mantissa
// 215          AABA   EXP    ; Add A and B exponents
// 216 .M1      SCKC   LSD1   ; Repeat least-significant-digit of C times...
// 217          BILT   M2
// 218          AABA   MANT   ; Add B to A
// 219          BET    M1
// 220 .M2      SRLC   MANT   ; Shift C right
// 221          CCK    MANT1  ; Is C zero?
// 222          BILT   MD2    ; Multiply done
// 223          SRLA   MANT   ; Shift A right
// 224          SAKA   EXP1   ; And decrease exponent
// 225          BIGE   M1     ; Repeat the addition loop for the next C digit
// 226          SFB    F5     ; Set overflow to indicate exponent negative
// 227          BET    M1     ; But keep going
// 228 .DIV     CCK    MANT1  ; Divide code: A / B...
// 229          BILT   ERR    ; Divide by zero
// 230          AAKC   ALL    ; C / B, result goes in A...
// 231 .D1      AKA    MANT   ; Clear A mantissa
// 232          CCK    MANT1
// 233          BILT   MD2    ; Answer zero
// 234          SABA   EXP
// 235          BIGE   D2
// 236          SFB    F5
// 237 .D2      CCB    MANT   ; Repeatedly subtract B from C...
// 238          BILT   D3     ; Can\'t subtract any more
// 239          SCBC   MANT   ; Subtract B from C
// 240          AAKA   LSD1   ; And increment quotient in A
// 241          BET    D2
// 242 .D3      CAK    MSD1   ; Value in A\'s most significant digit?
// 243          BIGE   MD2    ; Divide done
// 244          SLLC   MANT   ; Shift C left
// 245          SLLA   MANT   ; Shift A left
// 246          AAKA   EXP1   ; Increment exponent
// 247          BINC   D2     ; If negative exponent becomes zero...
// 248          ZFB    F5     ; Clear the overflow
// 249          BET    D2     ; And keep going
// 250 .ERR     SFB    F5     ; Divide by zero - Set overflow
// 251          BET    ZERO
// 252 .MD2     ABOC   ALL    ; Done: move B to C
// 253 .POST    AKB    EXP    ; Post-operation normalization...
// 254          AKB    DPT7   ; Set B exponent to 7
// 255          CAK    OV1    ; Is A overflow (top digit) set?
// 256          BILT   P1     ; Branch if zero
// 257          SRLA   MANT
// 258          SAKA   EXP1
// 259          BIGE   P1
// 260          SFB    F5
// 261 .OVF     AABA   EXP
// 262          AAKA   EXP1
// 263          BIC    OVF1
// 264 .P1      TFB    F5     ; Test BF overflow
// 265          BIO    OVF
// 266          CAK    MANT1  ; Is A mantissa zero?
// 267          BILT   P3     ; Branch if zero?
// 268 .P2      CAK    MSD1   ; Left-justify A...
// 269          BIGE   P4
// 270          SLLA   MANT
// 271          AAKA   EXP1   ; Incrementing exponent each shift
// 272          BET    P2
// 273 .ID5     BET    ID4    ; Random branch destination from WAITDK
// 274 .P3      ABOA   EXP    ; A is zero; set A exponent to 7
// 275          ZFA    SIGN   ; And clear the sign
// 276 .P4      TFB    OP3    ; In middle of operation?
// 277          BIZ    P7     ; Branch if operation completed
// 278          SFB    F3     ; Set BF CONST
// 279 .P5      AAKC   ALL    ; Copy A to C
// 280          CF     SIGN   ; Flip B SIGN if A and B SIGN different...
// 281          BIE    P6
// 282          FFB    SIGN
// 283 .P6      EXF    OPFGS  ; Move BF op flags to AF
// 284          ZFB    OPFGS  ; and clear B op flags
// 285 .P7      SFA    F3     ; Set AF CD
// 286          ZFA    F5     ; Clear AF POST/TEMP
// 287          CAB    EXP    ; Shift A right until exponent <= 7...
// 288          BILT   RJ     ; Branch if done
// 289          SABA   EXP    ; Subtract 7 from A exponent
// 290 .P8      CAK    EXP1   ; Shift A right until exponent 0...
// 291          BILT   P9     ; Add 7 back to exponent when done...
// 292          SAKA   EXP1   ; otherwise
// 293          SRLA   MANT
// 294          BET    P8     ; keep looping
// 295 .ID3     BET    ID4
// 296 .FD4     BET    DPTPOS ; Random WAITNO jump destination
// 297 .P9      AAKA   DPT7   ; Add 7 to A exponent
// 298          BET    RJ     ; Branch
// 299 .KS4     TFA    SIGN   ; A negative?
// 300          BIZ    KS9
// 301          AKA    OV1    ; 1 -> OV1
// 302          SAKA   OV1    ; 0 -> OV1 ?
// 303          CAK    MANT1
// 304          BILT   KS6
// 305 .KS5     CAK    MSD1
// 306          BIGE   KS7
// 307          SLLA   MANT
// 308          AAKA   EXP1
// 309          BET    KS5
// 310 .KS9     SRLA   MANT
// 311          SAKA   EXP1
// 312          BET    KS7
// 313 .OVF1    TFA    SIGN
// 314          BIZ    DPTPOS
// 315          SAKAH  OV1
// 316          SAKAH  OV1
// 317          BILT   DPTPOS ; Will this ever be true???
//
const unsigned Calculator::code_tab[320] = {
    1423, 1407, 1807, 1839, 5,    1755, 531,  1446, 12,   1992, 1992, 12,   1746, 21,   1757, 533,
    1916, 1693, 12,   1807, 1414, 1855, 1803, 1563, 1852, 1598, 1693, 542,  1884, 26,   1598, 1432,
    553,  1360, 1376, 545,  1360, 1376, 545,  1646, 1838, 1400, 1319, 1360, 1376, 616,  2013, 1777,
    564,  1838, 1360, 1297, 1429, 42,   1336, 1836, 1723, 58,   1644, 2013, 1320, 1329, 1443, 77,
    1444, 589,  1440, 77,   1393, 1350, 1807, 1351, 1552, 1411, 7,    1329, 1328, 1330, 1443, 594,
    1444, 86,   1412, 1349, 86,   1348, 1755, 670,  1752, 299,  1868, 1565, 88,   1352, 1415, 1443,
    21,   1411, 19,   1443, 533,  1347, 1416, 19,   1614, 1429, 545,  1360, 1360, 1376, 40,   1823,
    1360, 1024, 1109, 1101, 1085, 1100, 1099, 1123, 1117, 1150, 1855, 1954, 1855, 533,  1443, 131,
    1414, 1807, 1411, 1446, 148,  1976, 1976, 1992, 1992, 533,  1447, 152,  1448, 658,  1855, 1755,
    1855, 533,  1415, 156,  1754, 21,   1744, 21,   1448, 155,  1565, 1868, 1586, 21,   1806, 1414,
    1552, 1418, 1445, 765,  1442, 253,  1647, 1427, 173,  1395, 1855, 1615, 1526, 1440, 719,  1430,
    178,  1349, 1441, 181,  1462, 1742, 185,  1855, 1526, 1693, 1742, 702,  1916, 185,  1565, 192,
    1740, 196,  1855, 1526, 1494, 200,  1660, 201,  1548, 1398, 1445, 253,  1334, 1413, 253,  1494,
    1414, 211,  1350, 1441, 740,  1583, 1804, 1550, 1714, 732,  1548, 216,  1948, 1787, 764,  1916,
    1693, 216,  1333, 216,  1787, 762,  1583, 1804, 1787, 764,  1662, 237,  1333, 1772, 754,  1708,
    1554, 237,  1754, 252,  1900, 1868, 1565, 237,  1397, 237,  1333, 19,   1615, 1822, 1808, 1752,
    776,  1916, 1693, 264,  1333, 1550, 1565, 825,  1429, 773,  1755, 786,  1754, 276,  1868, 1565,
    268,  43,   1598, 1414, 1426, 285,  1331, 1583, 1494, 283,  1462, 1529, 1401, 1347, 1413, 1742,
    517,  1662, 1757, 809,  1693, 1916, 290,  43,   21,   1552, 5,    1446, 310,  1800, 1688, 1755,
    670,  1754, 161,  1868, 1565, 305,  1916, 1693, 161,  1446, 21,   1992, 1992, 533,
};

const char Calculator::mask_tab[NUMBER_OF_MASKS][MASK_LENGTH] = {
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
