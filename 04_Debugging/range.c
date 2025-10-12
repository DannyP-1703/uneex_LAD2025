#include <stdio.h>
#include <stdlib.h>

int cmp(int left, int right, int negate) {
    if (negate) {
        return left > right;
    }
    return left < right;
}

int main(int argc, char *argv[]) {
    int start = 0;
    int end = 0;
    int step = 1;

    switch (argc) {
    case 1:
        printf("Usage: %s [START] END [STEP]\n\n"
               "Print integers from START (default 0) to END (exclusive) by STEP (default 1).",
               argv[0]);
        break;
    case 2:
        if (sscanf(argv[1], "%d", &end) != 1) {
            fprintf(stderr, "Incorrect arguments");
            exit(1);
        }
        break;
    case 3:
        if (sscanf(argv[1], "%d", &start) != 1 || sscanf(argv[2], "%d", &end) != 1) {
            fprintf(stderr, "Incorrect arguments");
            exit(1);
        }
        break;
    case 4:
        if (sscanf(argv[1], "%d", &start) != 1 || sscanf(argv[2], "%d", &end) != 1 || sscanf(argv[3], "%d", &step) != 1) {
            fprintf(stderr, "Incorrect arguments");
            exit(1);
        }
        break;
    default:
        fprintf(stderr, "Incorrect arguments");
        exit(1);
    }

    if (step == 0) {
        exit(2);
    }

    if (end - start < 0 && step > 0 || end - start > 0 && step < 0) {
        exit(0);
    }

    for (int i = start; cmp(i, end, step < 0); i += step) {
        printf("%d\n", i);
    }

    return 0;
}