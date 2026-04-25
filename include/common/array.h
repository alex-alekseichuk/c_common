#pragma once
/** Array interface and implementation for an array of type T with allocator:
 *   - static array of fixed size with dummy_alloc
 *   - dynamic array with real allocator
 */

#include <stddef.h>
#include <stdalign.h>
#include "common/allocator.h"

_BEGIN_EXTERN_C

typedef struct Array Array;

COMMON_API size_t array_size(Array *arr);
COMMON_API size_t array_len(Array *arr);
COMMON_API int array_empty(Array *arr);
COMMON_API void array_free(Array *arr);

#define DECL_ARRAY_T(T)                                              \
    COMMON_API Array static_##T##Array_len(T *data, size_t size, size_t len); \
    COMMON_API Array static_##T##Array(T *data, size_t size);        \
    COMMON_API Array make_##T##Array(Allocator *alloc, size_t size); \
    COMMON_API int T##Array##_push(Array *arr, T value);             \
    COMMON_API T T##Array##_pop(Array *arr);                         \
    COMMON_API T T##Array##_top(Array *arr);                         \
    COMMON_API T T##Array##_get(Array *arr, size_t i);               \
    COMMON_API int T##Array##_put(Array *arr, size_t i, T value);    \

_END_EXTERN_C
