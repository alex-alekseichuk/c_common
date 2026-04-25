#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common/allocator.h"
#include "common/arena_alloc.h"

typedef struct ArenaCtx {
    uint8_t *buffer;
    size_t   size;
    size_t   offset;
} ArenaCtx;

static void* _arena_alloc(void *ctx, size_t size) {
    ArenaCtx *a = (ArenaCtx*)ctx;

    size_t real_size = size + SIZE_T;
    real_size = ALIGN(real_size);

    if (a->offset + real_size > a->size)
        return NULL;

    void *ptr = a->buffer + a->offset;
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
    if (p == NULL) return NULL;
    
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

static Allocator _make_arena(ArenaCtx *ctx) {
    return (Allocator){
        .alloc = _arena_alloc,
        .realloc = _arena_realloc,
        .free = _arena_free,
        ._sizeof = _arena_sizeof,
        .ctx = ctx
    };
}

Allocator make_buffer_arena(uint8_t *buffer, size_t size) {
    size_t n = ALIGN(sizeof(ArenaCtx));
    if (n >= size) return (Allocator){0};

    ArenaCtx *a = (void *)buffer;

    a->buffer = buffer + n;
    a->size = size - n;
    a->offset = 0;

    return _make_arena(a);
}

Allocator make_malloc_arena(size_t size) {
    ArenaCtx *a = malloc(sizeof(ArenaCtx));
    if (!a) return (Allocator){0};

    a->buffer = malloc(size);
    if (!a->buffer) {
        free(a);
        return (Allocator){0};
    }

    a->size = size;
    a->offset = 0;

    return _make_arena(a);
}

void arena_free(Allocator *alloc) {
    ArenaCtx *a = (ArenaCtx*)alloc->ctx;
    free(a->buffer);
    free(a);
    alloc->ctx = NULL;
}

void arena_reset(Allocator *alloc) {
    ArenaCtx *a = (ArenaCtx*)alloc->ctx;
    a->offset = 0;
}

// Checkpoint / rollback
size_t arena_mark(Allocator *alloc) {
    ArenaCtx *a = (ArenaCtx*)alloc->ctx;
    return a->offset;
}

void arena_rewind(Allocator *alloc, size_t mark) {
    ArenaCtx *a = (ArenaCtx*)alloc->ctx;
    a->offset = mark;
}
