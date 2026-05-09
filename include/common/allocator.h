#pragma once
/**
 * Allocator interface for memory allocation.
 * An entity of the interface can allocate and free memory.
 * It has abstract context implementation.
 * It's used in data structures.
 * Allocated chunk of memory has size property and the buffer.
 * It has implementations:
 *   - sys_alloc instance uses system malloc/free/realloc
 *       - not just the alias for system
 *   - arena_alloc type uses pre-allocated buffer.
 *       - use already existing buffer on stack/static/global/heap
 *       - or malloc buffer in runtime
 *   - dummy_alloc instance allocates nothing
 *       - safe to be used with static memory;
 * 
 */

#include <stddef.h>
#include "common/common.h"

_BEGIN_EXTERN_C

typedef struct Allocator Allocator;

typedef void* (*alloc_fn)(Allocator *a, size_t size);
typedef void* (*realloc_fn)(Allocator *a, void *ptr, size_t size);
typedef void (*free_fn)(Allocator *a, void *ptr);
typedef size_t (*sizeof_fn)(Allocator *a, void *ptr);

// allocator interface
typedef struct AllocVTable {
    alloc_fn alloc;
    realloc_fn realloc;
    free_fn free;
    sizeof_fn _sizeof;
} AllocVTable;

// base allocator class
typedef struct Allocator {
    const AllocVTable *vTable;
} Allocator;

extern Allocator dummy_alloc;
extern Allocator sys_alloc;

#define ALLOC(a, size) ((a)->vTable->alloc((a), (size)))

// allocate a struct
// Node *n = ALLOC_T(&a, Node);
#define ALLOC_T(a, T) ((T*)(a)->vTable->alloc((a), sizeof(T)))

// allocate an array of n elements
#define ALLOC_N(a, T, n) ((T*)(a)->vTable->alloc((a), sizeof(T) * (n)))

void* alloc_zero(Allocator *a, size_t size);

// allocate severtal bytes and zero the memory
#define ALLOC_ZERO(a, size) alloc_zero((a), (size))

// allocate a struct and zero the memory
// Node *n = ALLOC_ZERO_T(&alloc, Node);
#define ALLOC_ZERO_T(a, T) \
    ((T*)alloc_zero((a), sizeof(T)))

// allocate an array of n elements and zero the memory
// int *arr = ALLOC_ZERO_N(&alloc, int, 128);
#define ALLOC_ZERO_N(a, T, n) \
    ((T*)alloc_zero((a), sizeof(T) * (n)))

#define REALLOC(a, ptr, size) (a)->vTable->realloc((a), (ptr), (size))
#define REALLOC_N(a, ptr, T, n) ((T*)(a)->vTable->realloc((a), (ptr), sizeof(T) * (n)))

// free memory
#define FREE(a, ptr) (a)->vTable->free((a), (ptr))

#define SIZEOF(a, ptr) (a)->vTable->_sizeof((a), (ptr))

// allocate a string and format it
COMMON_API char* alloc_printf(Allocator *a, const char *fmt, ...);

_END_EXTERN_C
