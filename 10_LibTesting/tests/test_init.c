#include "utils.h"

int main(int argc, char **argv) {
    int count_pass = 0;
    int count_fail = 0;

    /* initialization, buf_free() */
    float *a = 0;
    TEST("capacity init", buf_capacity(a) == 0);
    TEST("size init", buf_size(a) == 0);
    buf_push(a, 1.3f);
    TEST("size 1", buf_size(a) == 1);
    TEST("value", a[0] == (float)1.3f);
    buf_clear(a);
    TEST("clear", buf_size(a) == 0);
    TEST("clear not-free", a != 0);
    buf_free(a);
    TEST("free", a == 0);

    /* Clearing an NULL pointer is a no-op */
    buf_clear(a);
    TEST("clear empty", buf_size(a) == 0);
    TEST("clear no-op", a == 0);

    printf("%d fail, %d pass\n", count_fail, count_pass);
    return count_fail != 0;
}
