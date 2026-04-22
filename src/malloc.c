#include <stdlib.h>
#include "common/malloc.h"

static void* _malloc_alloc(void *ctx, size_t size) {
    (void)ctx;

    size_t real_size = size + SIZE_T;
    void *p = malloc(real_size);
    if (p == NULL) return NULL;
    *((size_t *)p) = size;

    return ((size_t *)p) + 1;
}

static void *_malloc_realloc(void *ctx, void *ptr, size_t size) {
    (void)ctx;

    size_t real_size = size + SIZE_T;
    void *p = realloc(((size_t *)ptr) - 1, real_size);
    if (p == NULL) return NULL;
    *((size_t *)p) = size;

    return (((size_t *)p) + 1);
}

static void _malloc_free(void *ctx, void *ptr) {
    (void)ctx;
    free(((size_t *)ptr) - 1);
}

static size_t _malloc_sizeof(void *ctx, void *ptr) {
    (void)ctx;
    return *(((size_t *)ptr) - 1);
}

Allocator make_malloc_allocator() {
    return (Allocator){
        .alloc = _malloc_alloc,
        .realloc = _malloc_realloc,
        .free  = _malloc_free,
        ._sizeof = _malloc_sizeof,
        .ctx   = NULL
    };
}
