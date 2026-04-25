#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include <common/allocator.h>
#include <common/str.h>

static Allocator allocator;

void setUp() {
    allocator = sys_alloc;
}
void tearDown() {
}

void test_str(void) {
    char *s = make_str();
    s = str_cat(s, "Hello");
    s = str_cat(s, ", ");
    s = str_cat(s, "World!");

    const char s_hello[] = "Hello, World!";

    TEST_ASSERT_EQUAL_STRING(s, s_hello);
    TEST_ASSERT_EQUAL(str_len(s), strlen(s_hello));

    s = str_cpy(s, "New String");
    TEST_ASSERT_EQUAL_STRING(s, "New String");

    str_free(s);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_str);

    return UNITY_END();
}
