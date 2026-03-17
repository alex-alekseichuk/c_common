#pragma once

#include <stddef.h>
#include <stdarg.h>
#include <common/common.h>
#include <common/allocator.h>

COMMON_API size_t utf8_strlen(const char *s);
COMMON_API char* malloc_printf(const char *fmt, ...);

typedef struct Str Str;
