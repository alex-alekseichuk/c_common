#include <stddef.h>
#include <string.h>
#include "common/allocator.h"

static void* _dummy_alloc(Allocator *ctx, size_t size) {(void)ctx; (void)size; return NULL;}
static void* dummy_realloc(Allocator *ctx, void *ptr, size_t size) {(void)ctx; (void)ptr; (void)size; return NULL;}
static void dummy_free(Allocator *ctx, void *ptr) {(void)ctx; (void)ptr;}
static size_t dummy_sizeof(Allocator *ctx, void *ptr) {(void)ctx; (void)ptr; return 0;}

AllocVTable dummy_alloc_vTable = {
    .alloc = _dummy_alloc,
    .realloc = dummy_realloc,
    .free = dummy_free,
    ._sizeof = dummy_sizeof,
};

Allocator dummy_alloc = {
    .vTable = &dummy_alloc_vTable
};
