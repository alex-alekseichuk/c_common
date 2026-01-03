#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

typedef struct Arena {
    uint8_t *base;
    size_t   size;
    size_t   offset;
} Arena;

static void* arena_alloc(void *ctx, size_t size) {
    Arena *a = (Arena*)ctx;

    const size_t ALIGN = sizeof(max_align_t); // 8
    size = (size + ALIGN - 1) & ~(ALIGN - 1);

    if (a->offset + size > a->size)
        return NULL;

    void *ptr = a->base + a->offset;
    a->offset += size;
    return ptr;
}

static void arena_free(void *ctx, void *ptr) {
    (void)ctx;
    (void)ptr;
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
        .alloc = arena_alloc,
        .free  = arena_free,
        .ctx   = a
    };
}

void free_arena_allocator(Allocator *alloc) {
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

