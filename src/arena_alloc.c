#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common/common.h"
#include "common/allocator.h"
#include "common/allocator_impl.h"
#include "common/arena_alloc.h"

typedef struct Arena {
    Allocator base;
    uint8_t *buffer;
    size_t   size;
    size_t   offset;
} Arena;

static void* _arena_alloc(Allocator *alloc, size_t size) {
    Arena *a = container_of(alloc, Arena, base);

    size_t real_size = size + SIZE_T;
    real_size = ALIGN(real_size);

    if (a->offset + real_size > a->size)
        return NULL;

    void *ptr = a->buffer + a->offset;
    a->offset += real_size;

    *((size_t *)ptr) = size;

    return ((size_t *)ptr) + 1;
}

static void* _arena_realloc(Allocator *a, void *ptr, size_t size) {
    size_t actual_size = *(((size_t *)ptr) - 1);
    if (actual_size >= size) {
        return ptr;
    }

    void *p = _arena_alloc(a, size);
    if (p == NULL) return NULL;
    
    memcpy(p, ptr, actual_size);
    
    return p;
}

static void _arena_free(Allocator *a, void *ptr) {
    (void)a;
    (void)ptr;
}

static size_t _arena_sizeof(Allocator *a, void *ptr) {
    (void)a;
    return *(((size_t *)ptr) - 1);
}

AllocVTable arena_alloc_vTable = {
    .alloc = _arena_alloc,
    .realloc = _arena_realloc,
    .free = _arena_free,
    ._sizeof = _arena_sizeof,
};

static void _Arena_init(Arena *ctx) {
    Allocator_init(&ctx->base, &arena_alloc_vTable);
}

Arena *make_buffer_arena(uint8_t *buffer, size_t size) {
    size_t n = ALIGN(sizeof(Arena));
    if (n >= size) return NULL;

    Arena *a = (void *)buffer;

    a->buffer = buffer + n;
    a->size = size - n;
    a->offset = 0;

    _Arena_init(a);
    return a;
}

Arena *make_malloc_arena(size_t size) {
    Arena *a = malloc(sizeof(Arena));
    if (!a) return NULL;

    a->buffer = malloc(size);
    if (!a->buffer) {
        free(a);
        return NULL;
    }

    a->size = size;
    a->offset = 0;

    _Arena_init(a);
    return a;
}

void arena_free(Arena *a) {
    free(a->buffer);
    free(a);
}

void arena_reset(Arena *a) {
    a->offset = 0;
}

// Checkpoint / rollback
size_t arena_mark(Arena *a) {
    return a->offset;
}

void arena_rewind(Arena *a, size_t mark) {
    a->offset = mark;
}
