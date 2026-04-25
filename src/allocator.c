#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include "common/allocator.h"

const size_t SIZE_T = sizeof(size_t); // 8
const size_t ALIGN_SIZE = sizeof(max_align_t); // 32

static void* _dummy_alloc(void *ctx, size_t size) {(void)ctx; (void)size; return NULL;}
static void* dummy_realloc(void *ctx, void *ptr, size_t size) {(void)ctx; (void)ptr; (void)size; return NULL;}
static void dummy_free(void *ctx, void *ptr) {(void)ctx; (void)ptr;}
static size_t dummy_sizeof(void *ctx, void *ptr) {(void)ctx; (void)ptr; return 0;}

Allocator dummy_alloc = {
    .alloc = _dummy_alloc,
    .realloc = dummy_realloc,
    .free = dummy_free,
    ._sizeof = dummy_sizeof,
    .ctx = NULL,
};

void* alloc_zero(Allocator *a, size_t size) {
    void *p = a->alloc(a->ctx, size);
    if (p) memset(p, 0, size);
    return p;
}

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
