#include "fixture.h"

TEST_F(calc_test, initial_state)
{
    calc.run();
    EXPECT_EQ(calc.get_display(), "[        0.]");
}

TEST_F(calc_test, key_1)
{
    calc.run();
    EXPECT_EQ(calc.get_display(), "[        0.]");
    calc.press_key('1');
    EXPECT_EQ(calc.get_display(), "[        1.]");
    calc.press_key(0);
    EXPECT_EQ(calc.get_display(), "[        1.]");
}
