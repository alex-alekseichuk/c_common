#pragma once
/**
 * Stack interface implementation based on Array data structure.
 */

#include <stddef.h>
#include "common/common.h"
#include "common/array.h"
#include "common/stack.h"

_BEGIN_EXTERN_C

#define DECL_ARRAY_STACK_T_NAME(T, StackName)                            \
typedef struct Array##StackName { \
    StackName base; \
    Array *array; \
} Array##StackName; \
COMMON_API Array##StackName make_Array##StackName(Array *array);                          \

#define DECL_ARRAY_STACK_T(T) DECL_ARRAY_STACK_T_NAME(T, T##Stack)

#define IMPL_ARRAY_STACK_T_NAME(T, StackName, ArrayName)            \
static int StackName##_push(StackName *s, T value) {                   \
    return ArrayName##_push(container_of(s, Array##StackName, base)->array, value);                   \
}                                                                   \
                                                                    \
static T StackName##_pop(StackName *s) {                               \
    return ArrayName##_pop(container_of(s, Array##StackName, base)->array);                           \
}                                                                   \
                                                                    \
static T StackName##_top(StackName *s) {                               \
    return ArrayName##_top(container_of(s, Array##StackName, base)->array);                           \
}                                                                   \
                                                                    \
static int StackName##_empty(StackName *s) {                           \
    return array_len(container_of(s, Array##StackName, base)->array) == 0;                            \
}                                                                   \
                                                                    \
static StackName##VTable vTable_##StackName = {                     \
    .push = StackName##_push,                               \
    .pop = StackName##_pop,                                 \
    .top = StackName##_top,                                 \
    .empty = StackName##_empty                              \
}; \
Array##StackName make_Array##StackName(Array *array) {                          \
    return (Array##StackName){                                         \
        .base={.vTable = &vTable_##StackName},                      \
        .array = array                                              \
    };                                                              \
}                                                                   \

#define IMPL_ARRAY_STACK_T(T) IMPL_ARRAY_STACK_T_NAME(T, T##Stack, T##Array)

_END_EXTERN_C
