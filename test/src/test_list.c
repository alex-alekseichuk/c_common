#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include <common/malloc.h>
#include <common/allocator.h>
#include <common/list.h>

static Allocator allocator;

void setUp() {
    allocator = make_malloc_allocator();
}
void tearDown() {
}

// .h
typedef struct Struct1 {
    int key;
} Struct1;
DECL_LIST_T(Struct1)

// .c
IMPL_LIST_T(Struct1)

void test_list_reverse(void) {
    Struct1List list = make_Struct1List(&allocator);

    Struct1 s, s0 = {0}, s1 = {.key=1}, s2 = {.key=2}, s3 = {.key=3};

    Struct1List_append(&list, s1, NULL);
    Struct1List_append(&list, s2, NULL);
    Struct1List_append(&list, s3, NULL);

    list_reverse((List *)&list);

    Struct1ListNode *node = list.tail->next;
    TEST_ASSERT_EQUAL_MEMORY(&s3, &(node->value), sizeof(Struct1));
    node = node->next;
    TEST_ASSERT_EQUAL_MEMORY(&s2, &(node->value), sizeof(Struct1));
    node = node->next;
    TEST_ASSERT_EQUAL_MEMORY(&s1, &(node->value), sizeof(Struct1));

    list_free((List *)&list);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_list_reverse);

    return UNITY_END();
}
