#include "roman_nums.h"
#include <stdlib.h>
#include <string.h>

/** @file roman_nums.c
 * Convert between decimal and roman numerals
 */

/** Preprocessed array of roman numerals from 1 to 100 */
static const char *roman_range[] = {
    "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
    "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
    "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
    "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL",
    "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L",
    "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", "LX",
    "LXI", "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", "LXIX", "LXX",
    "LXXI", "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", "LXXVIII", "LXXIX", "LXXX",
    "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", "LXXXVII", "LXXXVIII", "LXXXIX", "XC",
    "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", "XCVII", "XCVIII", "XCIX", "C"};

/** Convert decimal number @p num (1 <= @p num <= 100) into its roman representation.
 *
 * @param num number to convert
 *
 * @return string with roman representation, or NULL on error
 */
const char *dec2roman(int num) {
    if (num <= 0 || num > 100) {
        return NULL;
    }
    return roman_range[num - 1];
}

/** Convert roman number @p num (I <= @p num <= C) into its decimal representation.
 *
 * @param num string with roman representation of the number
 *
 * @return decimal number, or -1 on error
 */
int roman2dec(char *num) {
    for (int i = 0; i < 100; ++i) {
        if (strcmp(num, roman_range[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
}
