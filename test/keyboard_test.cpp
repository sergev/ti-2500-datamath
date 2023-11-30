#include "fixture.h"

TEST_F(calc_test, initial_state)
{
    EXPECT_EQ(calc.get_display(), "0.");
}

TEST_F(calc_test, key_1)
{
    calc.press_key('1');
    EXPECT_EQ(calc.get_display(), "1.");
}

TEST_F(calc_test, key_2)
{
    calc.press_key('2');
    EXPECT_EQ(calc.get_display(), "2.");
}

TEST_F(calc_test, key_3)
{
    calc.press_key('3');
    EXPECT_EQ(calc.get_display(), "3.");
}

TEST_F(calc_test, key_4)
{
    calc.press_key('4');
    EXPECT_EQ(calc.get_display(), "4.");
}

TEST_F(calc_test, key_5)
{
    calc.press_key('5');
    EXPECT_EQ(calc.get_display(), "5.");
}

TEST_F(calc_test, key_6)
{
    calc.press_key('6');
    EXPECT_EQ(calc.get_display(), "6.");
}

TEST_F(calc_test, key_7)
{
    calc.press_key('7');
    EXPECT_EQ(calc.get_display(), "7.");
}

TEST_F(calc_test, key_8)
{
    calc.press_key('8');
    EXPECT_EQ(calc.get_display(), "8.");
}

TEST_F(calc_test, key_9)
{
    calc.press_key('9');
    EXPECT_EQ(calc.get_display(), "9.");
}
