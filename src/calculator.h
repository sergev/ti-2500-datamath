#include <string>
#include <cstdint>

#define NUMBER_OF_MASKS 16
#define MASK_LENGTH 12

// to use with keyPressed
#define DK 1

// ORIGINAL SINCLAIR DATA
class Calculator_t {
public:
    bool sinclair{};

    // Important: Array order matches display order, not bit order.
    // I.e. a[0] is high-order digit S10, a[10] is low-order digit S0.
    int8_t a[11]{};  // Register A
    int8_t b[11]{};  // Register B
    int8_t c[11]{};  // Register C
    int8_t af[11]{}; // Flags A
    int8_t bf[11]{}; // Flags A

    int8_t d[11] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    uint8_t dActive{ 1 }; // Currently active D value 1-11. d[dActive-1] == 0
    int8_t cc{};
    char keyPressed{}; // Which key is currently pressed

    uint8_t keyStrobeKN{}; //
    uint8_t keyStrobeKO{}; //
    uint8_t keyStrobeKP{}; //

    unsigned int address{}; // Program Counter
    uint8_t display_on{};   // Flag for display on
    int8_t mask[MASK_LENGTH]{};

    bool resetinprogress{};

    // Get contents of the display.
    std::string get_display() const;

    // Run the code until stopped on wait.
    void run();
};

extern const unsigned int objectCode[320];

extern const char masks[NUMBER_OF_MASKS][MASK_LENGTH];

extern int lastSelectDigit;

// OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
// const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
static const unsigned long LISTOPSWITHK = 1007135334;

void displayInstruction(unsigned instructionid);

uint8_t readKey(Calculator_t &calc);

void step(Calculator_t &calc);
