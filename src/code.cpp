#include "calculator.h"

//
// These are the 320 instructions that performed all the tasks on the TI-2500 calculator.
//
const unsigned Calculator::code_tab[320] = {
    //
    // Program Listing
    //
    0b10'11000'1111, // 000 .CLEAR  ZFA     ALL ; Power on, clear flags
    0b10'10111'1111, // 001         ZFB     ALL
    0b11'10000'1111, // 002         AKA     ALL ; Clear A and C
    0b11'10010'1111, // 003         AKC     ALL
    0b00'00000'0101, // 004         BET     RJ ; Reset condition latch
    0b11'01101'1011, // 005 .RJ     CAK     MANT1 ; Right justify A, maybe add minu...
    0b01'00001'0011, // 006         BILT    ZERO ; Mantissa is zero?
    0b10'11010'0110, // 007 .S      TFA     SIGN
    0b00'00000'1100, // 008         BIZ     RJ1 ; A is positive
    0b11'11100'1000, // 009         SAKAH   OV1 ; Minus sign is hex 14
    0b11'11100'1000, // 010         SAKAH   OV1
    0b00'00000'1100, // 011         BET     RJ1 ; Reset condition latch
    0b11'01101'0010, // 012 .RJ1    CAK     LSD1 ; Right justify the display
    0b00'00001'0101, // 013         BIGE    DPTPOS
    0b11'01101'1101, // 014         CAK     EXP1
    0b01'00001'0101, // 015         BILT    DPTPOS
    0b11'10111'1100, // 016         SRLA    MANT ; Shift A mantissa right
    0b11'01001'1101, // 017         SAKA    EXP1 ; Decrement A exponent
    0b00'00000'1100, // 018         BET     RJ1
    0b11'10000'1111, // 019 .ZERO   AKA     ALL ; A is zero, clear entire register
    0b10'11000'0110, // 020         ZFA     SIGN ; Clear A sign flag
    0b11'10011'1111, // 021 .DPTPOS EXAB    ALL ; Compute decimal point position...
    0b11'10000'1011, // 022         AKA     MANT1
    0b11'00001'1011, // 023         AAKA    MANT1
    0b11'10011'1100, // 024         EXAB    MANT ; B mantissa is 2
    0b11'00011'1110, // 025         ABOA    EXP ; Restore A exponent
    0b11'01001'1101, // 026 .DP1    SAKA    EXP1 ; Shift B (2) left by A exponent...
    0b01'00001'1110, // 027         BILT    DP2
    0b11'10101'1100, // 028         SLLB    MANT
    0b00'00001'1010, // 029         BET     DP1
    0b11'00011'1110, // 030 .DP2    ABOA    EXP
    0b10'11001'1000, // 031         TFB     F10
    0b01'00010'1001, // 032         BIO     ID1
    0b10'10101'0000, // 033 .LOCK   SYNC ; Debounce: wait for no key for a few cycles...
    0b10'10110'0000, // 034         SCAN
    0b01'00010'0001, // 035         BID     LOCK
    0b10'10101'0000, // 036         SYNC
    0b10'10110'0000, // 037         SCAN
    0b01'00010'0001, // 038         BID     LOCK
    0b11'00110'1110, // 039         ACKB    EXP
    0b11'10010'1110, // 040 .IDLE   AKC     EXP ; Idle loop entry
    0b10'10111'1000, // 041 .ID1    ZFB     F10
    0b10'10010'0111, // 042 .ID2    WAITNO  ID3  ; Wait for key press or timeout
    0b10'10101'0000, // 043 .ID4    SYNC
    0b10'10110'0000, // 044         SCAN
    0b01'00110'1000, // 045         BID     KEY ; Key down
    0b11'11101'1101, // 046 .FD1    ACKA    EXP1
    0b11'01111'0001, // 047         CCK     TIM4 ; Check number of timeouts
    0b01'00011'0100, // 048         BILT    FD3
    0b11'10010'1110, // 049         AKC     EXP
    0b10'10101'0000, // 050         SYNC
    0b10'10001'0001, // 051         WAITDK  ID5 ; Turn off display, wait for DK
    0b10'11001'0101, // 052 .FD3    TFB     F5 ;
    0b00'00010'1010, // 053         BIZ     ID2
    0b10'10011'1000, // 054         SFB     F10
    0b11'10010'1100, // 055         AKC     MANT
    0b11'01011'1011, // 056         SCKC    MANT1
    0b00'00011'1010, // 057         BET     FD5 ; Reset condition latch
    0b11'00110'1100, // 058 .FD5    ACKB    MANT
    0b11'11101'1101, // 059         ACKC    EXP1
    0b10'10010'1000, // 060         WAITNO  FD4
    0b10'10011'0001, // 061 .MINKEY SFB     OP2 ; MINUS = flags II, III
    0b10'11010'0011, // 062         TFA     F3 ; If CD is clear...
    0b00'00100'1101, // 063         BIZ     PLSKEY ; perform subtract operation
    0b10'11010'0100, // 064         TFA     F4 ; Or EQ is clear...
    0b01'00100'1101, // 065         BIO     PLSKEY ; perform subtract operation
    0b10'11010'0000, // 066         TFA     OP1 ; Or not after multi/div...
    0b00'00100'1101, // 067         BIZ     PLSKEY ; perform subtract operation
    0b10'10111'0001, // 068         ZFB     OP2 ; Minus key as sign after mult or divide
    0b10'10100'0110, // 069         SFA     SIGN ; Set sign to minus
    0b11'10000'1111, // 070         AKA     ALL ; Clear A
    0b10'10100'0111, // 071         SFA     F9 ; Set ST flag (sign temp) for unary minus
    0b11'00001'0000, // 072         AAKA    DPT7 ; Set A exponent to 7
    0b10'11000'0011, // 073         ZFA     F3 ; Clear CD
    0b00'00000'0111, // 074         BET     S ; Update display
    0b10'10011'0001, // 075 .DIVKEY SFB     OP2 ; DIV = flags II, III
    0b10'10011'0000, // 076 .MLTKEY SFB     OP1 ; MULT = flags I, III
    0b10'10011'0010, // 077 .PLSKEY SFB     OP3 ; PLUS = flag III
    0b10'11010'0011, // 078         TFA     F3 ; Clear display flag
    0b01'00101'0010, // 079         BIO     KS1
    0b10'11010'0100, // 080         TFA     F4 ; Equal flag
    0b00'00101'0110, // 081         BIZ     KS2
    0b10'11000'0100, // 082 .KS1    ZFA     F4
    0b10'10100'0101, // 083         SFA     F5
    0b00'00101'0110, // 084         BET     KS2
    0b10'10100'0100, // 085 .EQLKEY SFA     F4 ; F4 (EQ) flag set for equals
    0b11'01101'1011, // 086 .KS2    CAK     MANT1
    0b01'01001'1110, // 087         BILT    KS6 ; A mantissa == 0?
    0b11'01101'1000, // 088 .KS3    CAK     OV1 ; Left justify A...
    0b00'10010'1011, // 089         BIGE    KS4 ; Digit has reached left?
    0b11'10100'1100, // 090         SLLA    MANT ; Shift mantissa left
    0b11'00001'1101, // 091         AAKA    EXP1 ; Increment exponent
    0b00'00101'1000, // 092         BET     KS3
    0b10'10100'1000, // 093 .DPTKEY SFA     F10 ; Decimal point key...
    0b10'11000'0111, // 094         ZFA     F9 ; Set DPT flag, clear ST flag
    0b10'11010'0011, // 095         TFA     F3
    0b00'00001'0101, // 096         BIZ     DPTPOS
    0b10'11000'0011, // 097         ZFA     F3
    0b00'00001'0011, // 098         BET     ZERO
    0b10'11010'0011, // 099 .CEKEY  TFA     F3
    0b01'00001'0101, // 100         BIO     DPTPOS
    0b10'10100'0011, // 101         SFA     F3
    0b10'11000'1000, // 102         ZFA     F10
    0b00'00001'0011, // 103         BET     ZERO
    0b11'00100'1110, // 104 .KEY    ABOC    EXP ; Process key down...
    0b10'11001'0101, // 105         TFB     F5
    0b01'00010'0001, // 106         BIO     LOCK
    0b10'10101'0000, // 107         SYNC ; Wait to debounce key
    0b10'10101'0000, // 108         SYNC
    0b10'10110'0000, // 109         SCAN
    0b00'00010'1000, // 110         BIU     IDLE
    0b11'10001'1111, // 111         AKB     ALL
    0b10'10101'0000, // 112         SYNC
    0b10'00000'0000, // 113         BKO     CLEAR  ; Clear key pressed?
    0b10'00101'0101, // 114         BKO     EQLKEY ; Equal key pressed?
    0b10'00100'1101, // 115         BKO     PLSKEY ; Plus key pressed?
    0b10'00011'1101, // 116         BKO     MINKEY ; Minus key pressed?
    0b10'00100'1100, // 117         BKO     MLTKEY ; Mult key pressed?
    0b10'00100'1011, // 118         BKO     DIVKEY ; Divide key pressed?
    0b10'00110'0011, // 119         BKO     CEKEY ; CE key pressed?
    0b10'00101'1101, // 120         BKO     DPTKEY ; Decimal point key pressed?
    0b10'00111'1110, // 121         BKO     ZERKEY ; Zero key pressed?
    //
    // Digit entry
    //
    0b11'10011'1111, // 122         EXAB    ALL ; Process digit key...
    0b11'11010'0010, // 123         AKCM    LSD1 ; Count key position into A
    0b11'10011'1111, // 124         EXAB    ALL
    0b01'00001'0101, // 125         BIC     DPTPOS ; No key pressed
    0b10'11010'0011, // 126 .ZERKEY TFA     F3 ; Clear display flag
    0b00'01000'0011, // 127         BIZ     NUM1
    0b10'11000'0110, // 128 .CD     ZFA     SIGN
    0b11'10000'1111, // 129 .CD1    AKA     ALL
    0b10'11000'0011, // 130         ZFA     F3
    0b10'11010'0110, // 131 .NUM1   TFA     SIGN ; Number: Digit is in B...
    0b00'01001'0100, // 132         BIZ     NUM3 ; Branch if A SIGN clear
    0b11'11011'1000, // 133         AAKAH   OV1 ; Test for 15 in OV1?
    0b11'11011'1000, // 134         AAKAH   OV1
    0b11'11100'1000, // 135         SAKAH   OV1
    0b11'11100'1000, // 136         SAKAH   OV1
    0b01'00001'0101, // 137         BIC     DPTPOS ; Borrow set if OV1 == 15?
    0b10'11010'0111, // 138 .NUM2   TFA     F9 ; Test ST (sign temp) flag
    0b00'01001'1000, // 139         BIZ     NUM4
    0b10'11010'1000, // 140         TFA     F10
    0b01'01001'0010, // 141         BIO     NUM7
    0b11'10011'1111, // 142         EXAB    ALL
    0b11'01101'1011, // 143         CAK     MANT1
    0b11'10011'1111, // 144         EXAB    ALL
    0b01'00001'0101, // 145         BILT    DPTPOS
    0b10'11000'0111, // 146 .NUM7   ZFA     F9 ; Clear ST flag
    0b00'01001'1100, // 147         BET     NUM6
    0b11'01101'1010, // 148 .NUM3   CAK     MSD1
    0b00'00001'0101, // 149         BIGE    DPTPOS
    0b11'01101'0000, // 150         CAK     DPT7
    0b00'00001'0101, // 151         BIGE    DPTPOS
    0b10'11010'1000, // 152 .NUM4   TFA     F10
    0b00'01001'1011, // 153         BIZ     NUM5
    0b11'00001'1101, // 154         AAKA    EXP1
    0b11'10100'1100, // 155 .NUM5   SLLA    MANT
    0b11'00011'0010, // 156 .NUM6   ABOA    LSD1
    0b00'00001'0101, // 157         BET     DPTPOS
    0b11'10000'1110, // 158 .KS6    AKA     EXP
    0b10'11000'0110, // 159         ZFA     SIGN
    0b11'00001'0000, // 160         AAKA    DPT7
    0b10'11000'1010, // 161 .KS7    ZFA     FD ; Zeros temp(num) and dpt flag
    0b10'11010'0101, // 162         TFA     F5 ; Post flag
    0b01'01111'1101, // 163         BIO     POST
    0b10'11010'0010, // 164         TFA     OP3
    0b00'01111'1101, // 165         BIZ     POST
    0b11'00110'1111, // 166         ACKB    ALL
    0b10'11001'0011, // 167         TFB     F3 ; Constant flag
    0b00'01010'1101, // 168         BIZ     KS8
    0b10'10111'0011, // 169         ZFB     F3
    0b11'10011'1111, // 170         EXAB    ALL
    0b11'00100'1111, // 171         ABOC    ALL
    0b10'11111'0110, // 172         EXF     SIGN
    0b10'11010'0000, // 173 .KS8    TFA     OP1
    0b01'01100'1111, // 174         BIO     MLTDIV
    //
    // Add subtract routine
    //
    0b10'11001'0110, // 175 .ADDSUB TFB     SIGN ; Add / subtract...
    0b00'01011'0010, // 176         BIZ     AS1 ; Copy B sign to TEMP...
    0b10'10100'0101, // 177         SFA     F5
    0b10'11010'0001, // 178 .AS1    TFA     OP2 ; Flip B sign for subtract...
    0b00'01011'0101, // 179         BIZ     AS2
    0b10'11011'0110, // 180         FFB     SIGN
    0b11'01100'1110, // 181 .AS2    CAB     EXP ; Make sure A exponent >= B exponent...
    0b00'01011'1001, // 182         BIGE    AS3
    0b11'10011'1111, // 183         EXAB    ALL ; Exchange if A exponent smaller
    0b10'11111'0110, // 184         EXF     SIGN ; And exchange the signs
    0b11'01001'1101, // 185 .AS3    SAKA    EXP1 ; Shift A right until exponents equal...
    0b11'01100'1110, // 186         CAB     EXP ; Infinite loop if B EXP == 0?
    0b01'01011'1110, // 187         BILT    AS4
    0b11'10111'1100, // 188         SRLA    MANT
    0b00'01011'1001, // 189         BET     AS3
    0b11'00001'1101, // 190 .AS4    AAKA    EXP1 ; Exponents now equal
    0b00'01100'0000, // 191         BET     AS5 ; Reset condition
    0b11'01100'1100, // 192 .AS5    CAB     MANT ; Make sure A >= B...
    0b00'01100'0100, // 193         BIGE    AS6
    0b11'10011'1111, // 194         EXAB    ALL ; If not, exchange exponents
    0b10'11111'0110, // 195         EXF     SIGN ; and exchange signs
    0b10'11101'0110, // 196 .AS6    CF      SIGN
    0b00'01100'1000, // 197         BIE     AS7 ; If signs equal, add...
    0b11'00111'1100, // 198         SABA    MANT ; Otherwise subtract
    0b00'01100'1001, // 199         BET     AS8
    0b11'00000'1100, // 200 .AS7    AABA    MANT ; Add mantissas
    0b10'10111'0110, // 201 .AS8    ZFB     SIGN ; Restore B sign from TEMP flag...
    0b10'11010'0101, // 202         TFA     F5
    0b00'01111'1101, // 203         BIZ     POST
    0b10'10011'0110, // 204         SFB     SIGN
    0b10'11000'0101, // 205         ZFA     F5 ; And clear TEMP
    0b00'01111'1101, // 206         BET     POST ; Done add/sub
    0b10'11101'0110, // 207 .MLTDIV CF      SIGN ; Mult / div operation...
    0b10'11000'0110, // 208         ZFA     SIGN ; Positive result if signs the same
    0b00'01101'0011, // 209         BIE     MD1
    0b10'10100'0110, // 210         SFA     SIGN ; A, B signs different: negative result
    0b10'11010'0001, // 211 .MD1    TFA     OP2
    0b01'01110'0100, // 212         BIC     DIV
    //
    // Multiply
    //
    0b11'00010'1111, // 213         AAKC    ALL ; Copy A to C
    0b11'10000'1100, // 214         AKA     MANT ; Clear A mantissa
    0b11'00000'1110, // 215         AABA    EXP ; Add A and B exponents
    0b11'01011'0010, // 216 .M1     SCKC    LSD1 ; Repeat least-significant-digit of C times...
    0b01'01101'1100, // 217         BILT    M2
    0b11'00000'1100, // 218         AABA    MANT ; Add B to A
    0b00'01101'1000, // 219         BET     M1
    0b11'11001'1100, // 220 .M2     SRLC    MANT ; Shift C right
    0b11'01111'1011, // 221         CCK     MANT1 ; Is C zero?
    0b01'01111'1100, // 222         BILT    MD2 ; Multiply done
    0b11'10111'1100, // 223         SRLA    MANT ; Shift A right
    0b11'01001'1101, // 224         SAKA    EXP1 ; And decrease exponent
    0b00'01101'1000, // 225         BIGE    M1 ; Repeat the addition loop for the next C digit
    0b10'10011'0101, // 226         SFB     F5 ; Set overflow to indicate exponent negative
    0b00'01101'1000, // 227         BET     M1 ; But keep going
    //
    // Divide
    //
    0b11'01111'1011, // 228 .DIV    CCK     MANT1 ; Divide code: A / B...
    0b01'01111'1010, // 229         BILT    ERR ; Divide by zero
    0b11'00010'1111, // 230         AAKC    ALL ; C / B, result goes in A...
    0b11'10000'1100, // 231 .D1     AKA     MANT ; Clear A mantissa
    0b11'01111'1011, // 232         CCK     MANT1
    0b01'01111'1100, // 233         BILT    MD2 ; Answer zero
    0b11'00111'1110, // 234         SABA    EXP
    0b00'01110'1101, // 235         BIGE    D2
    0b10'10011'0101, // 236         SFB     F5
    0b11'01110'1100, // 237 .D2     CCB     MANT ; Repeatedly subtract B from C...
    0b01'01111'0010, // 238         BILT    D3 ; Can't subtract any more
    0b11'01010'1100, // 239         SCBC    MANT ; Subtract B from C
    0b11'00001'0010, // 240         AAKA    LSD1 ; And increment quotient in A
    0b00'01110'1101, // 241         BET     D2
    0b11'01101'1010, // 242 .D3     CAK     MSD1 ; Value in A's most significant digit?
    0b00'01111'1100, // 243         BIGE    MD2 ; Divide done
    0b11'10110'1100, // 244         SLLC    MANT ; Shift C left
    0b11'10100'1100, // 245         SLLA    MANT ; Shift A left
    0b11'00001'1101, // 246         AAKA    EXP1 ; Increment exponent
    0b00'01110'1101, // 247         BINC    D2 ; If negative exponent becomes zero...
    0b10'10111'0101, // 248         ZFB     F5 ; Clear the
    0b00'01110'1101, // 249         BET     D2 ; And keep going
    0b10'10011'0101, // 250 .ERR    SFB     F5 ; Divide by zero - Set overflow
    0b00'00001'0011, // 251         BET     ZERO
    0b11'00100'1111, // 252 .MD2    ABOC    ALL ; Done: move B to C
    //
    // Post norm
    //
    0b11'10001'1110, // 253 .POST   AKB     EXP ; Post-operation normalization...
    0b11'10001'0000, // 254         AKB     DPT7 ; Set B exponent to 7
    0b11'01101'1000, // 255         CAK     OV1 ; Is A overflow (top digit) set?
    0b01'10000'1000, // 256         BILT    P1 ; Branch if zero
    0b11'10111'1100, // 257         SRLA    MANT
    0b11'01001'1101, // 258         SAKA    EXP1
    0b00'10000'1000, // 259         BIGE    P1
    0b10'10011'0101, // 260         SFB     F5
    0b11'00000'1110, // 261 .OVF    AABA    EXP
    0b11'00001'1101, // 262         AAKA    EXP1
    0b01'10011'1001, // 263         BIC     OVF1
    0b10'11001'0101, // 264 .P1     TFB     F5 ; Test BF overflow
    0b01'10000'0101, // 265         BIO     OVF
    0b11'01101'1011, // 266         CAK     MANT1 ; Is A mantissa zero?
    0b01'10001'0010, // 267         BILT    P3 ; Branch if zero?
    0b11'01101'1010, // 268 .P2     CAK     MSD1 ; Left-justify A...
    0b00'10001'0100, // 269         BIGE    P4
    0b11'10100'1100, // 270         SLLA    MANT
    0b11'00001'1101, // 271         AAKA    EXP1 ; Incrementing exponent each shift
    0b00'10000'1100, // 272         BET     P2
    0b00'00010'1011, // 273 .ID5    BET     ID4 ; Random branch destination from WAITDK
    0b11'00011'1110, // 274 .P3     ABOA    EXP ; A is zero; set A exponent to 7
    0b10'11000'0110, // 275         ZFA     SIGN ; And clear the sign
    0b10'11001'0010, // 276 .P4     TFB     OP3 ; In middle of operation?
    0b00'10001'1101, // 277         BIZ     P7 ; Branch if operation completed
    0b10'10011'0011, // 278         SFB     F3 ; Set BF CONST
    0b11'00010'1111, // 279 .P5     AAKC    ALL ; Copy A to C
    0b10'11101'0110, // 280         CF      SIGN ; Flip B SIGN if A and B SIGN different...
    0b00'10001'1011, // 281         BIE     P6
    0b10'11011'0110, // 282         FFB     SIGN
    0b10'11111'1001, // 283 .P6     EXF     OPFGS ; Move BF op flags to AF
    0b10'10111'1001, // 284         ZFB     OPFGS ; and clear B op flags
    0b10'10100'0011, // 285 .P7     SFA     F3 ; Set AF CD
    0b10'11000'0101, // 286         ZFA     F5 ; Clear AF POST/TEMP
    0b11'01100'1110, // 287         CAB     EXP ; Shift A right until exponent <= 7...
    0b01'00000'0101, // 288         BILT    RJ ; Branch if done
    0b11'00111'1110, // 289         SABA    EXP ; Subtract 7 from A exponent
    0b11'01101'1101, // 290 .P8     CAK     EXP1 ; Shift A right until exponent 0...
    0b01'10010'1001, // 291         BILT    P9 ; Add 7 back to exponent when done...
    0b11'01001'1101, // 292         SAKA    EXP1 ; otherwise
    0b11'10111'1100, // 293         SRLA    MANT
    0b00'10010'0010, // 294         BET     P8 ; keep looping
    0b00'00010'1011, // 295 .ID3    BET     ID4
    0b00'00001'0101, // 296 .FD4    BET     DPTPOS ; Random WAITNO jump destination
    0b11'00001'0000, // 297 .P9     AAKA    DPT7 ; Add 7 to A exponent
    0b00'00000'0101, // 298         BET     RJ ; Branch
    0b10'11010'0110, // 299 .KS4    TFA     SIGN ; A negative?
    0b00'10011'0110, // 300         BIZ     KS9
    0b11'10000'1000, // 301         AKA     OV1 ; 1 -> OV1
    0b11'01001'1000, // 302         SAKA    OV1 ; 0 -> OV1 ?
    0b11'01101'1011, // 303         CAK     MANT1
    0b01'01001'1110, // 304         BILT    KS6
    0b11'01101'1010, // 305 .KS5    CAK     MSD1
    0b00'01010'0001, // 306         BIGE    KS7
    0b11'10100'1100, // 307         SLLA    MANT
    0b11'00001'1101, // 308         AAKA    EXP1
    0b00'10011'0001, // 309         BET     KS5
    0b11'10111'1100, // 310 .KS9    SRLA    MANT
    0b11'01001'1101, // 311         SAKA    EXP1
    0b00'01010'0001, // 312         BET     KS7
    0b10'11010'0110, // 313 .OVF1   TFA     SIGN
    0b00'00001'0101, // 314         BIZ     DPTPOS
    0b11'11100'1000, // 315         SAKAH   OV1
    0b11'11100'1000, // 316         SAKAH   OV1
    0b00'00001'0101, // 317         BET     DPTPOS
};
