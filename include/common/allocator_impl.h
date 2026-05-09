#pragma once

#include "common/common.h"
#include "common/allocator.h"

_BEGIN_EXTERN_C

COMMON_API void Allocator_init(Allocator *alloc, const AllocVTable *vtable);

_END_EXTERN_C
