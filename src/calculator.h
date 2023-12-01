#include <string>
#include <cstdint>

//
// Number of digits in calculator registers.
//
const unsigned REG_LEN = 11;

//
// A register is represented as array of digits.
//
using Reg = std::array<int8_t, REG_LEN>;

class Calculator {
private:
    // Hardware registers.
    Reg a{};    // Register A
    Reg b{};    // Register B
    Reg c{};    // Register C
    Reg af{};   // A flags
    Reg bf{};   // B flags
    Reg mask{}; // Mask value obtained from instruction opcode

    unsigned dActive{ 1 }; // Currently active D value 1-11. d[dActive-1] == 0
    int cc{};

    // Which key is currently pressed.
    char keyPressed{};

    // Program counter.
    unsigned int address{};

    // Sinclair Scientific mode.
    bool sinclair{};

    // Executable code.
    static const unsigned code_tab[320];

public:
    // Get contents of the display.
    std::string get_display() const;

    // Execute one instruction.
    void step();

    // Run the code until stopped on wait.
    void run();

    // Press key on keyboard.
    void press_key(char k);

private:
    // Update mask and return a pointer to it.
    void update_mask(unsigned instruction);

    // Arithmetic.
    void add(const Reg &src1, const Reg &src2, Reg &dst, bool hex = false);
    void sub(const Reg &src1, const Reg &src2, Reg &dst, bool hex = false);
    void compare(const Reg &src1, const Reg &src2);
    void copy(const Reg &src, Reg &dst);
    void sll(Reg &x);
    void srl(Reg &x);
    void exchange(Reg &x, Reg &y);
    void writeFlag(Reg &dest, int val);
    void compareFlags(const Reg &src1, const Reg &src2);
    void testFlag(const Reg &src);

    // Poll keyboard
    bool keyStrobeKN() const; // Keys 1 2 3 4 5 6 7 8 9
    bool keyStrobeKO() const; // Keys 0 . = + - * / C CE
    bool keyStrobeKP() const; // Unused

    void displayInstruction(unsigned instructionid);
};
