#pragma once

#include <stddef.h>

#define LIST(Typename)                                                     \
typedef struct Node_##Typename {                                            \
    Typename value;                                                          \
    struct Node_##Typename *next;                                            \
} Node_##Typename;                                                           \
                                                                             \
static Node_##Typename* node_##Typename##_create(Allocator *A, Typename val) { \
    Node_##Typename *n = (Node_##Typename*)A->alloc(A->ctx, sizeof(Node_##Typename)); \
    if (!n) return NULL;                                                     \
    n->value = val;                                                          \
    n->next = NULL;                                                          \
    return n;                                                                \
}                                                                            \
                                                                             \
static Node_##Typename* list_##Typename##_push_front(Allocator *A, Node_##Typename* head, Typename val) { \
    Node_##Typename *n = node_##Typename##_create(A, val);                   \
    if (!n) return head;                                                     \
    n->next = head;                                                          \
    return n;                                                                \
}                                                                            \
                                                                             \
static Node_##Typename* list_##Typename##_push_back(Allocator *A, Node_##Typename* head, Typename val) { \
    Node_##Typename *n = node_##Typename##_create(A, val);                   \
    if (!n) return head;                                                     \
    if (!head) return n;                                                     \
    Node_##Typename *cur = head;                                             \
    while (cur->next) cur = cur->next;                                       \
    cur->next = n;                                                           \
    return head;                                                             \
}                                                                            \
                                                                             \
static void list_##Typename##_free(Allocator *A, Node_##Typename* head) {   \
    while (head) {                                                           \
        Node_##Typename* tmp = head;                                         \
        head = head->next;                                                   \
        A->free(A->ctx, tmp);                                                \
    }                                                                        \
}

/*
#include <stdio.h>

LIST(int)

int main(void) {
    STATIC_ARENA_ALLOCATOR(my_arena, 1024);

    Node_int *list = NULL;
    list = list_int_push_front(&my_arena, list, 10);
    list = list_int_push_front(&my_arena, list, 20);
    list = list_int_push_back(&my_arena, list, 30);

    for (Node_int *cur = list; cur; cur = cur->next)
        printf("%d -> ", cur->value);
    printf("NULL\n");

    list_int_free(&my_arena, list);
    arena_reset(&my_arena);
    return 0;
}
*/
