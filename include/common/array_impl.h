#pragma once
/**
 * Details of array structure.
 * It's used in the array implementation.
 * It's not a part of Array interface.
 */
#include <stddef.h>
#include <stdalign.h>
#include "common/array.h"

_BEGIN_EXTERN_C

typedef struct Array {
    size_t   size;
    size_t   len;
    Allocator *alloc;
    void *data;
} Array;

#define IMPL_ARRAY_T(T)                                 \
    Array static_##T##Array_len(T *data, size_t size, size_t len) { \
        return (Array){                                 \
            .size=size,                                 \
            .len=len,                                   \
            .alloc=&dummy_alloc,                    \
            .data=data                                  \
        };                                              \
    }                                                   \
    Array static_##T##Array(T *data, size_t size) {     \
        return (Array){                                 \
            .size=size,                                 \
            .len=0,                                     \
            .alloc=&dummy_alloc,                    \
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
        if (arr->size <= arr->len) {                    \
            size_t size = arr->size < 256 ? (2 * arr->size) : (arr->size + ((arr->size + 768) >> 2)); \
            void *data = REALLOC_N(arr->alloc, arr->data, T, size); \
            if (!data) return 0;                        \
            arr->data = data;                           \
            arr->size = size;                           \
        }                                               \
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
        if (i >= arr->size) {                           \
            size_t size = arr->size < 256 ? (2 * arr->size) : (arr->size + ((arr->size + 768) >> 2)); \
            if (size <= i) size = i + 1;                \
            void *data = REALLOC_N(arr->alloc, arr->data, T, size); \
            if (!data) return 0;                        \
            arr->data = data;                           \
            arr->size = size;                           \
        }                                               \
        T *data = (T *)(arr->data);                     \
        while (arr->len <= i - 1)                       \
            data[arr->len++] = (T){0};                  \
        data[arr->len++] = value;                       \
        return 1;                                       \
    }                                                   \

_END_EXTERN_C
