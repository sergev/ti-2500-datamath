#include <gtest/gtest.h>
#include "calculator.h"

//
// Fixture with preallocated calculator.
//
// For details, see: https://github.com/google/googletest/blob/main/docs/primer.md
//
class calc_test : public ::testing::Test {

protected:
    Calculator calc;

    void SetUp() override
    {
        // Enable trace.
        calc.set_trace(true);
        calc.run();
    }

    //
    // Send input, check output.
    //
    void send_expect(const std::string &input, const std::string &expect)
    {
        calc.send(input);
        EXPECT_EQ(calc.get_display(), expect);
    }
};
