#include <stdio.h>
#include <string.h>

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain("guesser", "locale");
    textdomain("guesser");

    int l = 0, r = 100;

    const char *positive = _("yes");
    const char *negative = _("no");

    printf(_("Think of a number between %d and %d\n\n"), l, r);
    while (l < r) {
        int m = (l + r) / 2;

        printf(_("Is your number greater than %d? - %s/%s\n"), m, positive, negative);
        printf("> ");

        char response[8];
        if (scanf("%8s", response) == EOF) {
            puts("");
            return 1;
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
    printf(_("Your number is %d\n"), l);
    return 0;
}