#pragma once

#include <stddef.h>
#include <stdalign.h>
#include "common/allocator.h"

_BEGIN_EXTERN_C

typedef struct Array {
    size_t   size;
    size_t   len;
    Allocator *alloc;
    void *data;
} Array;

COMMON_API size_t array_size(Array *arr);
COMMON_API size_t array_len(Array *arr);
COMMON_API int array_empty(Array *arr);
COMMON_API void array_free(Array *arr);

#define DEFINE_ARRAY_T(T)                               \
    COMMON_API Array static_##T##Array_len(T *data, size_t size, size_t len);       \
    COMMON_API Array static_##T##Array(T *data, size_t size);        \
    COMMON_API Array make_##T##Array(Allocator *alloc, size_t size); \
    COMMON_API int T##Array##_push(Array *arr, T value);             \
    COMMON_API T T##Array##_pop(Array *arr);                         \
    COMMON_API T T##Array##_top(Array *arr);                         \
    COMMON_API T T##Array##_get(Array *arr, size_t i);               \
    COMMON_API int T##Array##_put(Array *arr, size_t i, T value);    \

#define IMPL_ARRAY_T(T)                                 \
    Array static_##T##Array_len(T *data, size_t size, size_t len) { \
        return (Array){                                 \
            .size=size,                                 \
            .len=len,                                   \
            .alloc=&dummy_allocator,                    \
            .data=data                                  \
        };                                              \
    }                                                   \
    Array static_##T##Array(T *data, size_t size) {     \
        return (Array){                                 \
            .size=size,                                 \
            .len=0,                                     \
            .alloc=&dummy_allocator,                    \
            .data=data                                  \
        };                                              \
    }                                                   \
    Array make_##T##Array(Allocator *alloc, size_t size) { \
        return (Array){                                 \
            .size=size,                                 \
            .len=0,                                     \
            .alloc=alloc,                               \
            .data=ALLOC_N(alloc, T, size)               \
        };                                              \
    }                                                   \
    int T##Array##_push(Array *arr, T value) {          \
        if (arr->size <= arr->len) return 0;            \
        ((T *)(arr->data))[arr->len++] = value;         \
        return 1;                                       \
    }                                                   \
    T T##Array##_pop(Array *arr) {                      \
        if (arr->len == 0) return (T){0};               \
        return ((T *)(arr->data))[--arr->len];          \
    }                                                   \
    T T##Array##_top(Array *arr) {                      \
        if (arr->len == 0) return (T){0};               \
        return ((T *)(arr->data))[arr->len - 1];        \
    }                                                   \
    T T##Array##_get(Array *arr, size_t i) {            \
        if (i < arr->len)                               \
            return ((T *)(arr->data))[i];               \
        else                                            \
            return (T){0};                              \
    }                                                   \
    int T##Array##_put(Array *arr, size_t i, T value) { \
        if (i >= arr->size) return 0;                   \
        T *data = (T *)(arr->data);                     \
        while (arr->len < i - 1)                        \
            data[arr->len++] = (T){0};                  \
        data[arr->len++] = value;                       \
        return 1;                                       \
    }                                                   \

_END_EXTERN_C
