#include "fixture.h"

TEST_F(calc_test, initial_state)
{
    EXPECT_EQ(calc.get_display(), "[        0]");
}
