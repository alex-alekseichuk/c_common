#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "common/allocator.h"

void Allocator_init(Allocator *alloc, const AllocVTable *vtable) {
    alloc->vTable = vtable;
}

const size_t SIZE_T = sizeof(size_t); // 8
const size_t ALIGN_SIZE = sizeof(max_align_t); // 32

void* alloc_zero(Allocator *a, size_t size) {
    void *p = a->vTable->alloc(a, size);
    if (p) memset(p, 0, size);
    return p;
}

char* alloc_printf(Allocator *a, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    if (n < 0) return NULL;

    char *buf = a->vTable->alloc(a, (size_t)n + 1);
    if (!buf) return NULL;

    va_start(ap, fmt);
    vsnprintf(buf, (size_t)n + 1, fmt, ap);
    va_end(ap);

    return buf;
}
