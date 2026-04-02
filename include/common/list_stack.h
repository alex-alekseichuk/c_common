#pragma once
/**
 * Stack interface implementation based on List data structure.
 */

#include <stddef.h>
#include "common/list.h"
#include "common/stack.h"

_BEGIN_EXTERN_C

#define DECL_LIST_STACK_T_NAME(T, StackName, ListName)              \
COMMON_API StackName make_##StackName(ListName *list);              \

#define DECL_LIST_STACK_T(T) DECL_LIST_STACK_T_NAME(T, T##Stack, T##List)

#define IMPL_LIST_STACK_T_NAME(T, StackName, ListName)              \
static int StackName##_push(void *ctx, T value) {                   \
    return ListName##_insert_head((ListName *)ctx, value);          \
}                                                                   \
                                                                    \
static T StackName##_pop(void *ctx) {                               \
    return ListName##_remove_head((ListName *)ctx);                 \
}                                                                   \
                                                                    \
static T StackName##_top(void *ctx) {                               \
    return ListName##_head((ListName *)ctx);                        \
}                                                                   \
                                                                    \
static int StackName##_empty(void *ctx) {                           \
    return ((ListName *)ctx)->tail == NULL;                         \
}                                                                   \
                                                                    \
StackName make_##StackName(ListName *list) {                        \
    return (StackName){                                             \
        .push = StackName##_push,                                   \
        .pop = StackName##_pop,                                     \
        .top = StackName##_top,                                     \
        .empty = StackName##_empty,                                 \
        .ctx = list                                                 \
    };                                                              \
}                                                                   \

#define IMPL_LIST_STACK_T(T) IMPL_LIST_STACK_T_NAME(T, T##Stack, T##List)

_END_EXTERN_C
