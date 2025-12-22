/** @mainpage Guess the number
 * @copydetails guesser
 */

/** @file guesser.c
 * Source code of the guess the number game.
 *
 * All the logic is here.
 *
 */

#include "roman_nums.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)

#ifndef LOCALEDIR
#define LOCALEDIR "../share/locale"
#endif /* LOCALEDIR */

#ifndef TEXTDOMAIN
#define TEXTDOMAIN "guesser"
#endif /* TEXTDOMAIN */

/** @page guesser
 *
 * Guess the number game with roman numeral support
 *
 * @section SYNOPSIS
 * `guesser [-r] [-h | --help]`
 *
 * @section DESCRIPTION
 * This interactive program asks you questions and guesses the number that you have thought of (between 1 and 100) using
 * binary search.
 * The game supports decimal and roman numeral modes.
 *
 * The game is translated to English and Russian languages.
 *
 * @b -h, @b --help
 * @n show the program's help and exit
 *
 * @b -r
 * @n play in roman numerals mode
 */
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain(TEXTDOMAIN, LOCALEDIR);
    textdomain(TEXTDOMAIN);

    char *help_msg = _("Usage: guesser [-h] [-r]\n\nPlay a guess the number game with this program!\n\n"
                       "Options:\n-h, --help\tshow this message and exit\n-r, --roman\tplay in roman numerals mode");
    int roman_flag = 0;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            puts(help_msg);
            return 0;
        }
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--roman") == 0) {
            roman_flag = 1;
            continue;
        }
        puts(_("Wrong arguments\n"));
        puts(help_msg);
        return 1;
    }

    int l = 1, r = 100;

    const char *positive = _("yes");
    const char *negative = _("no");

    printf(_("Think of a number in range "));
    if (roman_flag) {
        printf("%s..%s\n\n", dec2roman(l), dec2roman(r));
    } else {
        printf("%d..%d\n\n", l, r);
    }

    while (l < r) {
        int m = (l + r) / 2;

        printf(_("Is your number greater than "));
        if (roman_flag) {
            fputs(dec2roman(m), stdout);
        } else {
            printf("%d", m);
        }
        printf("? - %s/%s\n", positive, negative);
        printf("> ");

        char response[8];
        if (scanf("%8s", response) == EOF) {
            puts("");
            return 2;
        }

        if (strcmp(response, positive) == 0) {
            l = m + 1;
        } else if (strcmp(response, negative) == 0) {
            r = m;
        } else {
            printf(_("Please answer with '%s' or '%s'\n"), positive, negative);
        }
        puts("");
    }
    printf(_("Your number is "));
    if (roman_flag) {
        printf("%s\n", dec2roman(l));
    } else {
        printf("%d\n", l);
    }
    return 0;
}