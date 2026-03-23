#pragma once

#include <stddef.h>
#include <stdalign.h>
#include <stdint.h>
#include "common/common.h"
#include "common/allocator.h"

_BEGIN_EXTERN_C

typedef struct Arena {
    uint8_t *base;
    size_t   size;
    size_t   offset;
} Arena;

COMMON_API Allocator make_arena_allocator(size_t size);
COMMON_API Allocator static_arena_allocator(Arena *a);
COMMON_API void arena_free(Allocator *alloc);
COMMON_API void arena_reset(Allocator *alloc);
COMMON_API size_t arena_mark(Allocator *alloc);
COMMON_API void arena_rewind(Allocator *alloc, size_t mark);

#define STATIC_ARENA(NAME, SIZE)                            \
    static BUFFER(NAME##_buffer, uint8_t, SIZE)             \
    static Arena NAME = {                                   \
        .base = NAME##_buffer,                              \
        .size = (SIZE),                                     \
        .offset = 0                                         \
    };                                                      \

_END_EXTERN_C
