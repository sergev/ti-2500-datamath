#include <string>
#include <cstdint>

#define NUMBER_OF_MASKS 16
#define MASK_LENGTH 12

// to use with keyPressed
#define DK 1

// ORIGINAL SINCLAIR DATA
class Calculator {
private:
    bool sinclair{};

    // Important: Array order matches display order, not bit order.
    // I.e. a[0] is high-order digit S10, a[10] is low-order digit S0.
    int a[11]{};  // Register A
    int b[11]{};  // Register B
    int c[11]{};  // Register C
    int af[11]{}; // A flags
    int bf[11]{}; // B flags

    unsigned dActive{ 1 }; // Currently active D value 1-11. d[dActive-1] == 0
    int cc{};
    char keyPressed{}; // Which key is currently pressed

    unsigned int address{}; // Program Counter
    int mask[MASK_LENGTH]{};

public:
    // Get contents of the display.
    std::string get_display() const;

    void displayInstruction(unsigned instructionid);

    // Execute one instruction.
    void step();

    // Run the code until stopped on wait.
    void run();

    // Press key on keyboard.
    void press_key(char k);

    // Poll keyboard
    bool keyStrobeKN() const; // Keys 1 2 3 4 5 6 7 8 9
    bool keyStrobeKO() const; // Keys 0 . = + - * / C CE
    bool keyStrobeKP() const; // Unused

private:
    // Update mask[] and return a pointer to it.
    int *getMask();

    // Arithmetic.
    void add(int src1[], int src2[], int dst[], bool hex = false);
    void sub(int src1[], int src2[], int dst[], bool hex = false);
    void compare(int src1[], int src2[]);
    void copy(int src[], int dst[]);
    void sll(int src[]);
    void srl(int src[]);
    void writeFlag(int dest[], int val);
    void compareFlags(int src1[], int src2[]);
    void exchange(int src1[], int src2[]);
    void testFlag(int src[]);
};

extern const unsigned int objectCode[320];

extern const char masks[NUMBER_OF_MASKS][MASK_LENGTH];
