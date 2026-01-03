#pragma once

#include "allocator.h"

extern Allocator malloc_allocator_make(void);

/*
#include "allocator.h"
#include "malloc.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node* list_push(Allocator *a, Node *head, int value) {
    Node *n = a->alloc(a->ctx, sizeof(Node));
    if (!n) return head;

    n->value = value;
    n->next  = head;
    return n;
}

Allocator a = malloc_allocator_make();

Node *list = NULL;
list = list_push(&a, list, 1);
list = list_push(&a, list, 2);
list = list_push(&a, list, 3);
*/
