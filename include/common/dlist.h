#pragma once
/**
 * Doubly linked list interface and implementation for a list of type T with allocator.
 * TODO: in progress
 */

#include <stddef.h>
#include "common/allocator.h"
#include "common/common.h"

_BEGIN_EXTERN_C

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct List {
    Allocator *alloc;
    ListNode *head;
} List;

COMMON_API void list_free(List *list);
COMMON_API void list_insert(List *list, ListNode *new_node, ListNode *before_node);

#define DECL_LIST(T)                                                       \
typedef struct T##Node {                                                     \
    struct T##Node *next;                                                    \
    struct T##Node *prev;                                                    \
    T value;                                                                 \
} T##Node;                                                                   \
                                                                             \
typedef struct T##List {                                                     \
    Allocator *alloc;                                                        \
    T##Node *head;                                                           \
} T##List;                                                                   \
                                                                             \
COMMON_API T##List make _##T##List(Allocator *alloc);                        \

#define IMPL_LIST(T)                                                         \
T##List make _##T##List(Allocator *alloc) {                                  \
    return (T##List){.alloc=alloc,.head=NULL};                               \
}                                                                            \
                                                                             \
int T##List##_insert(T##List *list, T value, ListNode *before_node) {        \
    T##Node *new_node = ALLOC_T(list->alloc, T);                             \
    if (!new_node) return 0;                                                 \
    new_node->value = value;                                                 \
    list_insert(list, new_node, before_node);                                \
    return 1;                                                                \
}                                                                            \
                                                                             \
T T##List##_pop(T##List *list) {                                             \
    if (list->head == NULL) return (T){0};                                   \
    T##Node *node = list->head;                                              \
    list->head = node->next;                                                 \
    T value = node->value;                                                   \
    FREE(list->alloc, node);                                                 \
    return value;                                                            \
}                                                                            \
                                                                             \
T T##List##_top(T##List *list) {                                             \
    if (list->head == NULL) return (T){0};                                   \
    return list->head->value;                                                \
}                                                                            \
                                                                             \

_END_EXTERN_C
