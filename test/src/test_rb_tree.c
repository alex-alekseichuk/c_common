#include <unity.h>
#include <string.h>
 
#include <common/rb_tree.h>
#include <common/allocator.h>
 
DECL_RB_TREE(int)
IMPL_RB_TREE(int)
 
static int int_cmp(void *a, void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}
 
static unsigned char node_color(const intRbNode *node) {
    return (node == NULL || node == intRbNIL) ? RB_BLACK : node->color;
}
 
static int rb_check_black_height_and_no_red_red(const intRbNode *node) {
    if (node == intRbNIL) return 1; // NIL is black
 
    if (node->color == RB_RED) {
        TEST_ASSERT_EQUAL(RB_BLACK, node_color(node->left));
        TEST_ASSERT_EQUAL(RB_BLACK, node_color(node->right));
    }
 
    // Parent pointers should be consistent for non-NIL children.
    if (node->left != intRbNIL) TEST_ASSERT_EQUAL_PTR(node, node->left->parent);
    if (node->right != intRbNIL) TEST_ASSERT_EQUAL_PTR(node, node->right->parent);
 
    int lh = rb_check_black_height_and_no_red_red(node->left);
    int rh = rb_check_black_height_and_no_red_red(node->right);
    TEST_ASSERT_EQUAL(lh, rh);
 
    return lh + (node->color == RB_BLACK ? 1 : 0);
}
 
static void rb_assert_valid(intRbTree *tree) {
    TEST_ASSERT_NOT_NULL(tree);
    TEST_ASSERT_NOT_NULL(tree->root);
 
    if (tree->root == intRbNIL) return;
    TEST_ASSERT_EQUAL(RB_BLACK, tree->root->color);
    (void)rb_check_black_height_and_no_red_red(tree->root);
}
 
static void rb_assert_order_duplicates_right(
    const intRbNode *node,
    int hasMin,
    int minInclusive,
    int hasMax,
    int maxExclusive
) {
    if (node == intRbNIL) return;
 
    if (hasMin) TEST_ASSERT_EQUAL(1, node->value >= minInclusive);
    if (hasMax) TEST_ASSERT_EQUAL(1, node->value <= maxExclusive);
 
    // Left: strictly less than node->value.
    rb_assert_order_duplicates_right(node->left, hasMin, minInclusive, 1, node->value);
 
    // Right: greater than or equal to node->value.
    rb_assert_order_duplicates_right(node->right, 1, node->value, hasMax, maxExclusive);
}
 
static intRbNode *find_by_value(intRbTree *tree, int value) {
    intRbNode *n = intRbTree_search(tree, value);
    return (n == intRbNIL) ? NULL : n;
}
 
static intRbNode *find_any_leaf(intRbNode *node) {
    if (node == intRbNIL) return NULL;
    if (node->left == intRbNIL && node->right == intRbNIL) return node;
 
    intRbNode *left = find_any_leaf(node->left);
    if (left) return left;
    return find_any_leaf(node->right);
}
 
static intRbNode *find_any_one_child(intRbNode *node) {
    if (node == intRbNIL) return NULL;
 
    int left_nil = (node->left == intRbNIL);
    int right_nil = (node->right == intRbNIL);
    if (left_nil != right_nil) return node;
 
    intRbNode *left = find_any_one_child(node->left);
    if (left) return left;
    return find_any_one_child(node->right);
}
 
static intRbNode *find_any_two_child(intRbNode *node) {
    if (node == intRbNIL) return NULL;
    if (node->left != intRbNIL && node->right != intRbNIL) return node;
 
    intRbNode *left = find_any_two_child(node->left);
    if (left) return left;
    return find_any_two_child(node->right);
}
 
void setUp(void) {}
void tearDown(void) {}
 
void test_rb_insert_sorted_keeps_valid(void) {
    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);
 
    for (int i = 1; i <= 64; i++) {
        TEST_ASSERT_EQUAL(1, intRbTree_insert(&tree, i));
        rb_assert_valid(&tree);
    }
 
    rb_assert_order_duplicates_right(tree.root, 0, 0, 0, 0);
}
 
void test_rb_search_finds_inserted_and_nil_for_missing(void) {
    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);
 
    int values[] = {10, 5, 15, 3, 7, 12, 18};
    int n = (int)(sizeof(values) / sizeof(values[0]));
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL(1, intRbTree_insert(&tree, values[i]));
    }
 
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_NOT_NULL(find_by_value(&tree, values[i]));
    }
 
    TEST_ASSERT_NULL(find_by_value(&tree, 999));
    TEST_ASSERT_NULL(find_by_value(&tree, -1));
 
    rb_assert_valid(&tree);
}
 
void test_rb_delete_leaf_one_child_two_child_keeps_valid(void) {
    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);
 
    for (int i = 1; i <= 63; i++) {
        TEST_ASSERT_EQUAL(1, intRbTree_insert(&tree, i));
    }
    rb_assert_valid(&tree);
 
    // Remove a leaf
    intRbNode *leaf = find_any_leaf(tree.root);
    TEST_ASSERT_NOT_NULL(leaf);
    int leaf_value = leaf->value;
    TEST_ASSERT_EQUAL(leaf_value, intRbTree_delete(&tree, leaf));
    TEST_ASSERT_NULL(find_by_value(&tree, leaf_value));
    rb_assert_valid(&tree);
 
    // Remove a one-child node
    intRbNode *one_child = find_any_one_child(tree.root);
    TEST_ASSERT_NOT_NULL(one_child);
    int one_child_value = one_child->value;
    TEST_ASSERT_EQUAL(one_child_value, intRbTree_delete(&tree, one_child));
    TEST_ASSERT_NULL(find_by_value(&tree, one_child_value));
    rb_assert_valid(&tree);
 
    // Remove a two-child node
    intRbNode *two_child = find_any_two_child(tree.root);
    TEST_ASSERT_NOT_NULL(two_child);
    int two_child_value = two_child->value;
    TEST_ASSERT_EQUAL(two_child_value, intRbTree_delete(&tree, two_child));
    TEST_ASSERT_NULL(find_by_value(&tree, two_child_value));
    rb_assert_valid(&tree);
 
    rb_assert_order_duplicates_right(tree.root, 0, 0, 0, 0);
}
 
void test_rb_duplicates_allowed_and_valid(void) {
    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);
 
    int values[] = {10, 10, 10, 5, 15, 10, 7, 10};
    int n = (int)(sizeof(values) / sizeof(values[0]));
 
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL(1, intRbTree_insert(&tree, values[i]));
        rb_assert_valid(&tree);
    }
 
    rb_assert_order_duplicates_right(tree.root, 0, 0, 0, 0);
    rb_assert_valid(&tree);
}

static int range_query_sum;

void add_value(int value) {
    range_query_sum += value;
}

void test_rb_range_query(void) {
    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);

    // Insert some values into the tree
    int values[] = {10, 5, 15, 3, 7, 12, 18};
    int n = (int)(sizeof(values) / sizeof(values[0]));
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL(1, intRbTree_insert(&tree, values[i]));
    }

    // Perform range queries
    int min = 5;
    int max = 12;
    range_query_sum = 0;
    intRbTree_range_query(&tree, min, max, add_value);

    TEST_ASSERT_EQUAL(5 + 7 + 10 + 12, range_query_sum);

    rb_assert_valid(&tree);
}

int main(void) {
    UNITY_BEGIN();
 
    RUN_TEST(test_rb_insert_sorted_keeps_valid);
    RUN_TEST(test_rb_search_finds_inserted_and_nil_for_missing);
    RUN_TEST(test_rb_delete_leaf_one_child_two_child_keeps_valid);
    RUN_TEST(test_rb_duplicates_allowed_and_valid);
    RUN_TEST(test_rb_range_query);
 
    return UNITY_END();
}
