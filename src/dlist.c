#include "common/dlist.h"

void list_free(List *list) {
    if (list->tail == NULL) return;
    ListNode *node = list->head;
    while (node) {
        ListNode* next_node = node == node->next ? NULL : node->next;
        FREE(list->alloc, node);
        node = next_node;
    }
}

void list_insert(List *list, ListNode *new_node, ListNode *before_node) {
    if (list->head == NULL) {
        list->head = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
        return;
    }

    if (before_node == NULL) {
        before_node = list->head;
    }

    if (before_node == list->head) {
        list->head = new_node;
    }

    before_node->prev->next = new_node;
    new_node->prev = before_node->prev;
    new_node->next = before_node;
    before_node->prev = new_node;
}

void list_append(List *list, ListNode *new_node, ListNode *after_node) {
    if (list->head == NULL) {
        list->head = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
        return;
    }

    if (after_node == NULL) {
        after_node = list->head->prev;
    }

    if (after_node == list->head) {
        list->head = new_node;
    }

    after_node->prev->next = new_node;
    new_node->prev = after_node->prev;
    new_node->next = after_node;
    after_node->prev = new_node;
}

