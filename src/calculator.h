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

//
// Model of TI-2500 calculator.
//
class Calculator {
private:
    // Hardware registers.
    Reg a{};    // Register A
    Reg b{};    // Register B
    Reg c{};    // Register C
    Reg af{};   // A flags
    Reg bf{};   // B flags
    Reg mask{}; // Mask value obtained from instruction opcode

    unsigned d_phase{}; // D counter in range 0-9.

    // Flag for conditional branch.
    bool condition_code{};

    // Which key is currently pressed.
    char key_pressed{};

    // Program counter.
    unsigned program_counter{};

    // Executable code.
    static const unsigned code_tab[];

    // Sinclair Scientific mode.
    const bool sinclair_flag{};

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
    // Set mask value from bits 3:0 of instruction.
    void update_mask(unsigned instruction);

    // Arithmetic.
    void add(const Reg &x, const Reg &y, Reg &z, bool hex_flag = false);
    void sub(const Reg &x, const Reg &y, Reg &z, bool hex_flag = false);
    void compare(const Reg &x, const Reg &y);
    void copy(const Reg &x, Reg &z);
    void sll(Reg &x);
    void srl(Reg &x);
    void exchange(Reg &x, Reg &y);
    void set_flags(Reg &x, int val);
    void compare_flags(const Reg &x, const Reg &y);
    void test_flags(const Reg &x);

    // Poll keyboard.
    bool keyStrobeKN() const; // Keys 1 2 3 4 5 6 7 8 9
    bool keyStrobeKO() const; // Keys 0 . = + - * / C CE
    bool keyStrobeKP() const; // Unused

    void displayInstruction(unsigned instructionid); // TODO: remove
};
