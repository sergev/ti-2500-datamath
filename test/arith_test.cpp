#include "fixture.h"

TEST_F(calc_test, example_4_23_plus_4)
{
    send_expect("4.23+4=", "8.23");
}

TEST_F(calc_test, example_6_minus_1_854)
{
    send_expect("6-1.854=", "4.146");
}

TEST_F(calc_test, example_12_32_minus_7_plus_1_6)
{
    send_expect("12.32-7+1.6=", "6.92");
}

TEST_F(calc_test, example_27_2_mul_18)
{
    send_expect("27.2*18=", "489.6");
}

TEST_F(calc_test, example_12_div_5_2)
{
    send_expect("12/5.2=", "2.3076923");
}

TEST_F(calc_test, example_5_mul_4_then_6_then_7)
{
    send_expect("5*4=", "20.");
    send_expect("6=", "24.");
    send_expect("7=", "28.");
}

TEST_F(calc_test, example_pos_neg)
{
    send_expect("-125/", "-125.");
    send_expect("5+", "-25.");
    send_expect("3*", "-22.");
    send_expect("-4=", "88.");
}

TEST_F(calc_test, example_squaring)
{
    send_expect("25*=", "625.");
}

TEST_F(calc_test, example_raising_to_a_power)
{
    send_expect("3*===", "81.");
}

TEST_F(calc_test, example_5_reciprocal)
{
    send_expect("5/==", "0.2");
}

TEST_F(calc_test, example_0625_reciprocal)
{
    send_expect(".0625/==", "16.");
}

TEST_F(calc_test, example_square_root)
{
    send_expect("26/", "26.");
    send_expect("5+", "5.2"); // Initial approximation
    send_expect("5/", "10.2");
    send_expect("2=", "5.1"); // 2nd approximation

    send_expect("26/", "26.");
    send_expect("5.1+", "5.0980392"); // Re-enter 2nd approximation
    send_expect("5.1/", "10.198039");
    send_expect("2=", "5.0990195"); // Answer

    send_expect("*=", "25.999999"); // Square answer to check
}

TEST_F(calc_test, calculation_overflow)
{
    send_expect("13635*", "13635.");
    send_expect("10000=", "1.3635000"); // On TI-2500 Version 3: "u1.3635000"
}

TEST_F(calc_test, calculation_minus_overflow)
{
    send_expect("-13635*", "-13635.");
    send_expect("10000=", "-1.3635000"); // On TI-2500 Version 3: "o1.3635000"
}

TEST_F(calc_test, entry_overflow)
{
    send_expect("123456789", "12345678."); // On TI-2500 Version 3: "C12345678."
}

TEST_F(calc_test, entry_minus_overflow)
{
    send_expect("-123456789", "12345678."); // On TI-2500 Version 3: "E12345678."
}

TEST_F(calc_test, calculation_round_off)
{
    send_expect("148623/", "148623.");
    send_expect("11=", "13511.181");
}
