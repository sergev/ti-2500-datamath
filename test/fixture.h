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
        //TODO
    }
};
