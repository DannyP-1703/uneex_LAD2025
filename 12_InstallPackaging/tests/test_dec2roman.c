#include "roman_nums.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int count_pass = 0, count_fail = 0;
    TEST("dec2roman(1)", strcmp(dec2roman(1), "I") == 0);
    TEST("dec2roman(10)", strcmp(dec2roman(10), "X") == 0);
    TEST("dec2roman(50)", strcmp(dec2roman(50), "L") == 0);
    TEST("dec2roman(100)", strcmp(dec2roman(100), "C") == 0);

    TEST("dec2roman(0) (out of range)", dec2roman(0) == NULL);
    TEST("dec2roman(101) (out of range)", dec2roman(101) == NULL);

    printf("%d fail, %d pass\n", count_fail, count_pass);
    return count_fail != 0;
}
