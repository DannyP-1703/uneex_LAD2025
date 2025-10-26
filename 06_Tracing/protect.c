#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

typedef int (*unlink_fn_t)(const char *);

int unlink(const char *pathname) {
    static unlink_fn_t real_unlink = NULL;
    if (!real_unlink)
        real_unlink = (unlink_fn_t)dlsym(RTLD_NEXT, "unlink");
    if (pathname && strstr(pathname, "PROTECT")) {
        errno = EPERM;
        return -1;
    }
    return real_unlink(pathname);
}
