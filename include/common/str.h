#pragma once

#include <stddef.h>
#include <stdarg.h>
#include "common/allocator.h"

extern size_t utf8_strlen(const char *s);
extern char* malloc_printf(const char *fmt, ...);

typedef struct Str Str;
