#pragma once
/**
 * Bits operations.
 */

#include "common/common.h"

_BEGIN_EXTERN_C

#define BIT_SET(value, bit) ((value) |= (1u << (bit)))
#define BIT_CLEAR(value, bit) ((value) &= ~(1u << (bit)))
#define BIT_TOGGLE(value, bit) ((value) ^= (1u << (bit)))
#define BIT_CHECK(value, bit) (((value) >> (bit)) & 1u)

_END_EXTERN_C
