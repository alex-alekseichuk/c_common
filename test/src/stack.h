#pragma once

#include <common/stack.h>
#include <common/array_stack.h>

typedef struct Struct1 {
    const char *name;
    int age;
} Struct1;

DECL_STACK_T(Struct1);

void stackfn_struct_stack(Struct1Stack *stack);

#define TEST_FN(Prefix, Name) void Prefix##Name(void) { stackfn_##Name(Prefix##stack); }

#define TEST_FNS(Prefix) \
    TEST_FN(Prefix, struct_stack);

#define RUN_TEST_FN(Prefix, Name) RUN_TEST(Prefix##Name);

#define RUN_TEST_FNS(Prefix) \
    RUN_TEST_FN(Prefix, struct_stack);
