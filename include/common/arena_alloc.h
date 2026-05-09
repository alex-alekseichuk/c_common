#pragma once
/**
 * Arena allocator allocates memory in a pre-allocated buffer.
 * It can be instantiated dynamically in runtime by make_arena_allocator(),
 * or statically in compile time: define by STATIC_ARENA macro, then init by static_arena_allocator().
 *
 * It's useful for allocating many small objects, as it reduces the number of system calls.
 * Also, it can be used to allocate memory in a fixed-size buffer, which is useful for embedded systems.
 */

#include <stddef.h>
#include <stdint.h>
#include "common/common.h"
#include "common/allocator.h"

_BEGIN_EXTERN_C

typedef struct Arena Arena;

COMMON_API Arena *make_buffer_arena(uint8_t *buffer, size_t size);

COMMON_API Arena *make_malloc_arena(size_t size);
COMMON_API void arena_free(Arena *a);

COMMON_API void arena_reset(Arena *a);

COMMON_API size_t arena_mark(Arena *a);
COMMON_API void arena_rewind(Arena *a, size_t mark);

_END_EXTERN_C
