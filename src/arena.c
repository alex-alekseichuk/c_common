#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "common/allocator.h"
#include "common/arena.h"

static void* _arena_alloc(void *ctx, size_t size) {
    Arena *a = (Arena*)ctx;

    size_t real_size = size + SIZE_T;
    real_size = (real_size + ALIGN - 1) & ~(ALIGN - 1);

    if (a->offset + real_size > a->size)
        return NULL;

    void *ptr = a->base + a->offset;
    a->offset += real_size;

    *((size_t *)ptr) = size;

    return ((size_t *)ptr) + 1;
}

static void* _arena_realloc(void *ctx, void *ptr, size_t size) {
    size_t actual_size = *(((size_t *)ptr) - 1);
    if (actual_size >= size) {
        return ptr;
    }

    void *p = _arena_alloc(ctx, size);
    memcpy(p, ptr, actual_size);
    
    return p;
}

static void _arena_free(void *ctx, void *ptr) {
    (void)ctx;
    (void)ptr;
}

static size_t _arena_sizeof(void *ctx, void *ptr) {
    (void)ctx;
    return *(((size_t *)ptr) - 1);
}

Allocator static_arena_allocator(Arena *a) {
    return (Allocator){
        .alloc = _arena_alloc,
        .realloc = _arena_realloc,
        .free  = _arena_free,
        ._sizeof = _arena_sizeof,
        .ctx   = a
    };
}

Allocator make_arena_allocator(size_t size) {
    Arena *a = malloc(sizeof(Arena));
    if (!a) return (Allocator){0};

    a->base = malloc(size);
    if (!a->base) {
        free(a);
        return (Allocator){0};
    }

    a->size = size;
    a->offset = 0;

    return (Allocator){
        .alloc = _arena_alloc,
        .realloc = _arena_realloc,
        .free  = _arena_free,
        ._sizeof = _arena_sizeof,
        .ctx   = a
    };
}

void arena_free(Allocator *alloc) {
    Arena *a = (Arena*)alloc->ctx;
    free(a->base);
    free(a);
    alloc->ctx = NULL;
}

void arena_reset(Allocator *alloc) {
    Arena *a = (Arena*)alloc->ctx;
    a->offset = 0;
}

// Checkpoint / rollback
size_t arena_mark(Allocator *alloc) {
    Arena *a = (Arena*)alloc->ctx;
    return a->offset;
}

void arena_rewind(Allocator *alloc, size_t mark) {
    Arena *a = (Arena*)alloc->ctx;
    a->offset = mark;
}
