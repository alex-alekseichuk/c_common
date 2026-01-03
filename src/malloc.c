#include <stdlib.h>
#include "allocator.h"

void* malloc_alloc(void *ctx, size_t size) {
    (void)ctx;
    return malloc(size);
}

void malloc_free(void *ctx, void *ptr) {
    (void)ctx;
    free(ptr);
}

Allocator malloc_allocator_make(void) {
    return (Allocator){
        .alloc = malloc_alloc,
        .free  = malloc_free,
        .ctx   = NULL
    };
}
