#include <cstdint>

#define NUMBER_OF_MASKS 16
#define MASK_LENGTH 12

// to use with keyPressed
#define DK 1

// ORIGINAL SINCLAIR DATA
struct Calculator_t {
    bool sinclair = false;

    // Important: Array order matches display order, not bit order.
    // I.e. a[0] is high-order digit S10, a[10] is low-order digit S0.
    int8_t a[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // Register A
    int8_t b[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // Register B
    int8_t c[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // Register C
    int8_t af[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Flags A
    int8_t bf[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Flags A

    int8_t d[11] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    uint8_t dActive = 1; // Currently active D value 1-11. d[dActive-1] == 0
    int8_t cc = 0;
    uint8_t keyPressed = 0;

    uint8_t keyStrobeKN = 0; //
    uint8_t keyStrobeKO = 0; // '
    uint8_t keyStrobeKP = 0; // '

    unsigned int address = 0; // PROGRAM COUNTER
    uint8_t display_on = 0;   // Flag for display on
    int8_t mask[MASK_LENGTH];

    bool resetinprogress = false;
};

extern struct Calculator_t calc;

extern const unsigned int objectCode[320];

extern const char masks[NUMBER_OF_MASKS][MASK_LENGTH];

extern int lastSelectDigit;

// OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
// const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
static const unsigned long LISTOPSWITHK = 1007135334;

void displayInstruction(unsigned instructionid);

uint8_t readKey();

void step();
