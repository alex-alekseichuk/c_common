#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include "allocator.h"

char* alloc_printf(Allocator *a, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    if (n < 0) return NULL;

    char *buf = a->alloc(a->ctx, (size_t)n + 1);
    if (!buf) return NULL;

    va_start(ap, fmt);
    vsnprintf(buf, (size_t)n + 1, fmt, ap);
    va_end(ap);

    return buf;
}
