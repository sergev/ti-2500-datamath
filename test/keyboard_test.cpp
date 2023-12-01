#include "fixture.h"

TEST_F(calc_test, initial_state)
{
    EXPECT_EQ(calc.get_display(), "0.");
}

TEST_F(calc_test, key_1)
{
    send_expect("1", "1.");
}

TEST_F(calc_test, key_2)
{
    send_expect("2", "2.");
    send_expect("2", "22.");
}

TEST_F(calc_test, key_3)
{
    send_expect("3", "3.");
    send_expect("3", "33.");
    send_expect("3", "333.");
}

TEST_F(calc_test, key_4)
{
    send_expect("4", "4.");
    send_expect("4", "44.");
    send_expect("4", "444.");
    send_expect("4", "4444.");
}

TEST_F(calc_test, key_5)
{
    send_expect("5", "5.");
    send_expect("5", "55.");
    send_expect("5", "555.");
    send_expect("5", "5555.");
    send_expect("5", "55555.");
}

TEST_F(calc_test, key_6)
{
    send_expect("6", "6.");
    send_expect("6", "66.");
    send_expect("6", "666.");
    send_expect("6", "6666.");
    send_expect("6", "66666.");
    send_expect("6", "666666.");
}

TEST_F(calc_test, key_7)
{
    send_expect("7", "7.");
    send_expect("7", "77.");
    send_expect("7", "777.");
    send_expect("7", "7777.");
    send_expect("7", "77777.");
    send_expect("7", "777777.");
    send_expect("7", "7777777.");
}

TEST_F(calc_test, key_8)
{
    send_expect("8", "8.");
    send_expect("8", "88.");
    send_expect("8", "888.");
    send_expect("8", "8888.");
    send_expect("8", "88888.");
    send_expect("8", "888888.");
    send_expect("8", "8888888.");
    send_expect("8", "88888888.");
}

TEST_F(calc_test, key_9)
{
    send_expect("9", "9.");
    send_expect("9", "99.");
    send_expect("9", "999.");
    send_expect("9", "9999.");
    send_expect("9", "99999.");
    send_expect("9", "999999.");
    send_expect("9", "9999999.");
    send_expect("9", "99999999.");
    //TODO: send_expect("9", "C99999999.");
}

TEST_F(calc_test, key_0)
{
    send_expect("1", "1.");
    send_expect("0", "10.");
    send_expect("4", "104.");
    send_expect("0", "1040.");
    send_expect("8", "10408.");
    send_expect("0", "104080.");
    send_expect("3", "1040803.");
    send_expect("0", "10408030.");
}

//
// Check '.' key - decimal point.
// Note that the behavior is different from TI-2500 Vertion 3 model.
//
TEST_F(calc_test, key_decimal_point)
{
    send_expect("1.", "1.");
    send_expect("4", "1.4");
    send_expect(".", "1.4");   // On TI-2500 Vertion 3 model: "14."
    send_expect("8", "1.48");  // On TI-2500 Vertion 3 model: "14.8"
    send_expect(".", "1.48");  // On TI-2500 Vertion 3 model: "148."
    send_expect("3", "1.483"); // On TI-2500 Vertion 3 model: "148.3"
    send_expect(".", "1.483"); // On TI-2500 Vertion 3 model: "1483."
}

TEST_F(calc_test, key_plus)
{
    send_expect("7+", "7.");
    send_expect("2", "2.");
    send_expect("+", "9.");
}

TEST_F(calc_test, key_minus)
{
    send_expect("7-", "7.");
    send_expect("2", "2."); // On TI-2500 Vertion 3 model: "-2."
    send_expect("-", "5.");
}

TEST_F(calc_test, key_mult)
{
    send_expect("7*", "7.");
    send_expect("2", "2.");
    send_expect("*", "14.");
}

TEST_F(calc_test, key_divide)
{
    send_expect("7/", "7.");
    send_expect("2", "2.");
    send_expect("/", "3.5");
}

TEST_F(calc_test, key_equal)
{
    send_expect("7+", "7.");
    send_expect("2", "2.");
    send_expect("=", "9.");
}

TEST_F(calc_test, key_ce)
{
    send_expect("7+", "7.");
    send_expect("2", "2.");
    send_expect("e", "0.");
    send_expect("8", "8.");
    send_expect("=", "15.");
}

TEST_F(calc_test, key_clear)
{
    send_expect("7+", "7.");
    send_expect("2", "2.");
    send_expect("c", "0.");
    send_expect("8", "8.");
    send_expect("=", "8.");
}
