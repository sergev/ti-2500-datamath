#include <array>
#include <string>
#include <cstdint>

//
// Model of TI-2500 calculator.
//
class Calculator {
public:
    // Get contents of the display.
    std::string get_display() const;

    // Execute one instruction.
    void step();

    // Run the code until stopped on wait.
    void run();

    // Press key on keyboard.
    void press_key(char k);

    // Send a string of input.
    void send(const std::string &input);

private:
    // Number of digits in calculator registers.
    static const unsigned REG_LEN = 11;

    // A register is represented as array of digits.
    using Reg = std::array<int8_t, REG_LEN>;

    // Hardware registers.
    Reg a{};    // Register A
    Reg b{};    // Register B
    Reg c{};    // Register C
    Reg af{};   // A flags
    Reg bf{};   // B flags
    Reg mask{}; // Mask value obtained from instruction opcode

    // Flag for conditional branch.
    bool condition_code{};

    // Which key is currently pressed.
    char key_pressed{};

    // Program counter.
    unsigned program_counter{};

    // D counter in range 0-9.
    unsigned d_phase{};

    // Executable code.
    static const unsigned code_tab[];

    // Sinclair Scientific mode.
    const bool sinclair_flag{};

    // Trace mode.
    bool trace_flag{ true };

    // Previous state for tracing.
    Reg prev_a{};
    Reg prev_b{};
    Reg prev_c{};
    Reg prev_af{};
    Reg prev_bf{};
    bool prev_condition_code{};
    char prev_key_pressed{};

private:
    // Set mask value from bits 3:0 of instruction.
    void decode_mask(unsigned instruction);

    static bool op_with_mask(unsigned class_bits, unsigned opcode);
    static bool op_with_k(unsigned opcode);

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

    void trace_instruction(unsigned instruction) const;
    void trace_keyboard();
    void trace_state();
    std::string disassemble_opcode(unsigned class_bits, unsigned opcode) const;
    std::string disassemble_mask() const;

    friend std::ostream &operator<<(std::ostream &out, const Reg &x);
};

std::ostream &operator<<(std::ostream &out, const Calculator::Reg &x);
