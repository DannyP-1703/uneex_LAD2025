#include "roman_nums.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int count_pass = 0, count_fail = 0;

    for (int i = 1; i <= 100; ++i) {
        const char *roman = dec2roman(i);
        int dec = roman2dec((char *)roman);
        TEST("round convertion", i == dec);
    }

    printf("%d fail, %d pass\n", count_fail, count_pass);
    return count_fail != 0;
}
