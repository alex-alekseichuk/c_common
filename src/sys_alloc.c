#include <stdlib.h>
#include "common/allocator.h"

static void* _malloc_alloc(Allocator *ctx, size_t size) {
    (void)ctx;

    size_t real_size = size + SIZE_T;
    void *p = malloc(real_size);
    if (p == NULL) return NULL;
    *((size_t *)p) = size;

    return ((size_t *)p) + 1;
}

static void *_malloc_realloc(Allocator *ctx, void *ptr, size_t size) {
    (void)ctx;

    size_t real_size = size + SIZE_T;
    void *p = realloc(((size_t *)ptr) - 1, real_size);
    if (p == NULL) return NULL;
    *((size_t *)p) = size;

    return (((size_t *)p) + 1);
}

static void _malloc_free(Allocator *ctx, void *ptr) {
    (void)ctx;
    free(((size_t *)ptr) - 1);
}

static size_t _malloc_sizeof(Allocator *ctx, void *ptr) {
    (void)ctx;
    return *(((size_t *)ptr) - 1);
}

AllocVTable sys_alloc_vTable = {
    .alloc = _malloc_alloc,
    .realloc = _malloc_realloc,
    .free  = _malloc_free,
    ._sizeof = _malloc_sizeof,
};

Allocator sys_alloc = {
    .vTable = &sys_alloc_vTable,
};
