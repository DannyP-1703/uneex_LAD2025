#include "roman_nums.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int count_pass = 0, count_fail = 0;
    TEST("roman2dec(I)", roman2dec("I") == 1);
    TEST("roman2dec(X)", roman2dec("X") == 10);
    TEST("roman2dec(L)", roman2dec("L") == 50);
    TEST("roman2dec(C)", roman2dec("C") == 100);

    TEST("roman2dec() (empty)", roman2dec("") == -1);
    TEST("roman2dec(\"ABC\") (inval)", roman2dec("ABC") == -1);
    TEST("dec2roman(\"CI\") (out of range)", roman2dec("CI") == -1);

    printf("%d fail, %d pass\n", count_fail, count_pass);
    return count_fail != 0;
}