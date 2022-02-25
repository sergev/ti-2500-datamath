#include <iostream>
#include "globals.h"

// These are the 320 instructions that performed all the tasks on the Sinclair Scientific.
// the rest of this program is to execute and interface these instructions to todays hardware

const unsigned int objectCode[320] = {
    1408, 1392, 1792, 1824, 1860, 1808, 1360, 1376, 518,  1319, 1360, 1376, 9,    1360, 1908, 1072,
    1083, 1075, 1121, 1129, 1073, 1069, 1051, 1840, 1955, 1840, 516,  1425, 552,  1430, 33,   1792,
    1398, 1631, 1920, 1683, 34,   2003, 1540, 4,    1399, 1858, 1872, 1538, 4,    1329, 1335, 4,
    1349, 1347, 4,    1443, 676,  1431, 57,   1559, 4,    1553, 59,   1443, 677,  1839, 1632, 2018,
    65,   2023, 1719, 72,   1730, 71,   1840, 1666, 1751, 587,  1840, 1754, 78,   1840, 1718, 594,
    1924, 78,   2017, 1713, 89,   1540, 130,  1844, 1841, 1652, 597,  130,  1730, 95,   1849, 1650,
    114,  1443, 675,  1355, 1345, 130,  1409, 1559, 105,  1443, 750,  1839, 1632, 1844, 2023, 1719,
    92,   1538, 1537, 116,  1451, 1796, 791,  1908, 1781, 637,  1722, 1540, 120,  1940, 1786, 119,
    1445, 820,  1754, 512,  1747, 145,  1860, 1751, 142,  1686, 141,  1799, 1798, 1686, 1558, 132,
    132,  1908, 1751, 662,  1686, 1686, 1558, 152,  1441, 614,  1392, 1334, 1408, 1750, 161,  1559,
    159,  1568, 4,    1351, 1355, 1686, 681,  1908, 165,  1801, 1689, 1824, 1445, 180,  1447, 179,
    1568, 1819, 185,  1565, 1820, 1924, 2011, 1693, 738,  1888, 1888, 1888, 2012, 1696, 1936, 1936,
    1936, 1872, 1872, 1872, 2012, 1584, 1724, 1920, 1920, 1920, 1445, 183,  1561, 1447, 210,  1779,
    697,  220,  1451, 727,  1732, 185,  220,  1754, 1844, 1764, 1844, 185,  1904, 1904, 1904, 1904,
    1904, 130,  1447, 233,  1616, 1600, 1808, 1411, 100,  1451, 236,  1632, 1840, 130,  1445, 251,
    1750, 760,  1751, 759,  1908, 1686, 240,  1860, 1794, 1864, 1864, 1824, 2003, 1636, 1924, 1924,
    1700, 1431, 823,  2009, 1787, 253,  1993, 2036, 1723, 1920, 1920, 1920, 1920, 1588, 1844, 1445,
    814,  1600, 1479, 1447, 765,  1572, 1796, 1806, 1764, 797,  1700, 1562, 280,  1571, 803,  1860,
    1631, 1892, 280,  1807, 1443, 808,  130,  10,   1572, 1796, 1904, 1904, 1794, 61,   1572, 1796,
    1803, 1652, 1844, 117,  1908, 1335, 251,  1693, 253,  1565, 1860, 1563, 2036, 1844, 1411, 790
};

const char masks[NUMBER_OF_MASKS][MASK_LENGTH] = {
    { "00000000000" }, // M0
    { "5          " }, // M1
    { "  00       " }, // M2
    { "    1      " }, // M3
    { "    0000000" }, // M4
    { "          1" }, // M5
    { "  01       " }, // M6
    { " 5         " }, // M7
    { "000000     " }, // M8
    { "0001       " }, // M9
    { "    0000001" }, // M10
    { "     1     " }, // M11
    { "    00005  " }, // M12
    { "    00001  " }, // M13
    { "    4      " }, // M14
    { "    0      " }, // M15
};

struct Calculator_t calc;

static const unsigned DisplayOrder[9] = { 1, 7, 8, 9, 2, 3, 4, 5, 6 };

//
// TODO: show digit and decimal point at given position.
//
void displayDigit(unsigned position, char digit, bool dp)
{
    auto &out = std::cout;

    if (digit == 0)
        digit = ' ';
    out << '[' << position << ": " << digit << (dp ? '.' : ' ') << ']' << std::endl;
}

void updateDisplay()
{
    char showdigit = ' ';
    unsigned digitpos = DisplayOrder[calc.dActive - 1];

    switch (digitpos) {
    case 1:
        showdigit = (calc.a[0] == 0) ? ' ' : '-';
        break;
    case 2:
        showdigit = calc.a[4];
        break;
    case 3:
        showdigit = calc.a[5];
        break;
    case 4:
        showdigit = calc.a[6];
        break;
    case 5:
        showdigit = calc.a[7];
        break;
    case 6:
        showdigit = calc.a[8];
        break;
    case 7:
        showdigit = (calc.a[1] == 0) ? ' ' : '-';
        break;
    case 8:
        showdigit = calc.a[2];
        break;
    case 9:
        showdigit = calc.a[3];
        break;
    }

    // SINCLAIR behavior: turn decimal point on automatically at fixed position
    // dot stays on whether the rest of the display is on or off

    // attempt to have this block take same time to run whether a digit or blank is displayed
    bool dp = false;
    if (calc.display_on) {
        if (digitpos == 2) {
            dp = true; // true or false depending on digit shown
        }
    } else {
        // SINCLAIR behavior: dot goes brighter when display is off.
        showdigit = ' ';
        if (digitpos > 2) { // allow position 1 to be read for C and position 2 to display dot
            digitpos = 2;
            dp = true;
        }
    }

    displayDigit(digitpos, showdigit, dp);
}

int main()
{
    for (;;) {
        step();

        updateDisplay();

        char key = readKey();

        if (calc.resetinprogress) {
            if (calc.address > 8) {
                calc.resetinprogress = false;
            }
        }

        if (key == 'C') {
            if (calc.resetinprogress == false) {
                calc.address = 0;
                calc.keyStrobeKN = 0;
                calc.keyStrobeKO = 0;
                calc.keyStrobeKP = 0;
                calc.dActive = 1;
                calc.resetinprogress = true;
            }
        }
    }
}
