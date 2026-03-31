#pragma once
/**
 * Stack interface implementation based on Array data structure.
 */

#include <stddef.h>
#include "common/array.h"
#include "common/stack.h"

_BEGIN_EXTERN_C

#define DECL_ARRAY_STACK_T_NAME(T, Name)                            \
COMMON_API Name make_##Name(Array *array);                          \

#define DECL_ARRAY_STACK_T(T) DECL_ARRAY_STACK_T_NAME(T, T##Stack)

#define IMPL_ARRAY_STACK_T_NAME(T, StackName, ArrayName)            \
static int StackName##_push(void *ctx, T value) {                   \
    return ArrayName##_push((Array *)ctx, value);                   \
}                                                                   \
                                                                    \
static T StackName##_pop(void *ctx) {                               \
    return ArrayName##_pop((Array *)ctx);                           \
}                                                                   \
                                                                    \
static T StackName##_top(void *ctx) {                               \
    return ArrayName##_top((Array *)ctx);                           \
}                                                                   \
                                                                    \
static int StackName##_empty(void *ctx) {                           \
    return array_len((Array *)ctx) == 0;                            \
}                                                                   \
                                                                    \
StackName make_##StackName(Array *array) {                          \
    return (StackName){                                             \
        .push = StackName##_push,                                   \
        .pop = StackName##_pop,                                     \
        .top = StackName##_top,                                     \
        .empty = StackName##_empty,                                 \
        .ctx = array                                                \
    };                                                              \
}                                                                   \

#define IMPL_ARRAY_STACK_T(T) IMPL_ARRAY_STACK_T_NAME(T, T##Stack, T##Array)

/*
static int StackName##_push(void *stack, T value) {            \
    return ArrayName##_push((Array *)(StackName *stack)->ctx, value);                   \
}                                                                   \
                                                                    \
static T StackName##_pop(void *stack) {                        \
    return ArrayName##_pop((Array *)(StackName *stack)->ctx);                           \
}                                                                   \
                                                                    \
static T StackName##_top(void *stack) {                        \
    return ArrayName##_top((Array *)(StackName *stack)->ctx);                           \
}                                                                   \
                                                                    \
static int StackName##_empty(void *stack) {                    \
    return array_len((Array *)(StackName *stack)->ctx) == 0;                            \
}                                                                   \
*/

_END_EXTERN_C
