#pragma once
/**
 * Red-Black tree interface and implementation for a tree of type T with allocator.
 * It's a self-balancing binary search tree.
 */

#include <stddef.h>
#include "common/allocator.h"
#include "common/common.h"

_BEGIN_EXTERN_C

typedef int(*cmp_fn)(void *a, void *b);

#define RB_RED 1
#define RB_BLACK 0

typedef struct RbNode RbNode;
extern RbNode RB_NIL;

COMMON_API void rb_insert_fixup_impl(void *tree, void *node);
COMMON_API void rb_delete_impl(void *tree, void *node);
COMMON_API void rb_free(void *tree);

#define DECL_RB_TREE(T)                                                     \
typedef struct T##RbNode T##RbNode;                                         \
typedef struct T##RbTree T##RbTree;                                         \
COMMON_API T##RbTree make_##T##RbTree(Allocator *alloc, cmp_fn cmp);        \
COMMON_API int T##RbTree##_insert(T##RbTree *tree, T value);                \
COMMON_API T##RbNode *T##RbTree##_search(T##RbTree *tree, T value);         \
COMMON_API T T##RbTree##_delete(T##RbTree *tree, T##RbNode *node);          \
COMMON_API void T##RbTree##_range_query(T##RbTree *tree, T min, T max, void (*callback)(T)); \

#define IMPL_RB_TREE(T)                                                     \
typedef struct T##RbNode {                                                  \
    struct T##RbNode *left;                                                 \
    struct T##RbNode *right;                                                \
    struct T##RbNode *parent;                                               \
    unsigned char color;                                                    \
    T value;                                                                \
} T##RbNode;                                                                \
                                                                            \
typedef struct T##RbTree {                                                  \
    Allocator *alloc;                                                       \
    cmp_fn cmp;                                                             \
    T##RbNode *root;                                                        \
} T##RbTree;                                                                \
                                                                            \
static T##RbNode *T##RbNIL = (T##RbNode *)&RB_NIL;                          \
                                                                            \
T##RbTree make_##T##RbTree(Allocator *alloc, cmp_fn cmp) {                  \
    return (T##RbTree){.alloc=alloc, .cmp=cmp, .root=T##RbNIL};             \
}                                                                           \
                                                                            \
int T##RbTree##_insert(T##RbTree *tree, T value) {                          \
    T##RbNode *z = ALLOC_T(tree->alloc, T##RbNode);                         \
    if (!z) return 0;                                                       \
    z->left = T##RbNIL;                                                     \
    z->right = T##RbNIL;                                                    \
    z->parent = T##RbNIL;                                                   \
    z->color = RB_RED;                                                      \
    z->value = value;                                                       \
    T##RbNode *y = T##RbNIL;                                                \
    T##RbNode *x = tree->root;                                              \
                                                                            \
    while (x != T##RbNIL) {                                                 \
        y = x;                                                              \
        if (tree->cmp(&z->value, &x->value) < 0)                            \
            x = x->left;                                                    \
        else                                                                \
            x = x->right;                                                   \
    }                                                                       \
                                                                            \
    z->parent = y;                                                          \
                                                                            \
    if (y == T##RbNIL)                                                      \
        tree->root = z;                                                     \
    else if (tree->cmp(&z->value, &y->value) < 0)                           \
        y->left = z;                                                        \
    else                                                                    \
        y->right = z;                                                       \
                                                                            \
    rb_insert_fixup_impl(tree, z);                                               \
    return 1;                                                               \
}                                                                           \
                                                                            \
T##RbNode *T##RbTree##_search(T##RbTree *tree, T value) {                   \
    T##RbNode *cur = tree->root;                                            \
    while (cur != T##RbNIL && tree->cmp(&value, &cur->value) != 0) {        \
        if (tree->cmp(&value, &cur->value) < 0)                             \
            cur = cur->left;                                                \
        else                                                                \
            cur = cur->right;                                               \
    }                                                                       \
    return cur;                                                             \
}                                                                           \
                                                                            \
T T##RbTree##_delete(T##RbTree *tree, T##RbNode *node) {                    \
    if (tree->root == T##RbNIL || node == T##RbNIL || node == NULL) return (T){0}; \
    T value = node->value;                                                  \
    rb_delete_impl(tree, node);                                                  \
    FREE(tree->alloc, node);                                                \
    return value;                                                           \
}                                                                           \
                                                                            \
void T##RbTree##_range_query(T##RbTree *tree, T min, T max, void (*callback)(T)) { \
    if (tree->root == T##RbNIL) return; \
    T##RbNode *stack[100]; \
    int stack_size = 0; \
    T##RbNode *current = tree->root; \
 \
    while (current != T##RbNIL || stack_size > 0) { \
        while (current != T##RbNIL) { \
            stack[stack_size++] = current; \
            current = current->left; \
        } \
 \
        current = stack[--stack_size]; \
 \
        int cmp_min = tree->cmp(&current->value, &min); \
        int cmp_max = tree->cmp(&current->value, &max); \
 \
        if (cmp_min >= 0 && cmp_max <= 0) { \
            callback(current->value); \
        } \
 \
        current = current->right; \
    } \
} \

_END_EXTERN_C
