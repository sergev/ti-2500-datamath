#include <gtest/gtest.h>
#include "calculator.h"

//
// Fixture with preallocated calculator.
//
// For details, see: https://github.com/google/googletest/blob/main/docs/primer.md
//
class calc_test : public ::testing::Test {

protected:
    Calculator_t calc;

    void SetUp() override
    {
        calc.run();
    }

    void send_expect(const std::string &send, const std::string &expect)
    {
        for (char ch : send) {
            calc.press_key(ch);
        }
        EXPECT_EQ(calc.get_display(), expect);
    }
};
