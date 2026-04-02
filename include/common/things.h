#pragma once
/**
 * TODO: tree of objects and their children in game scene w/o allocation.
 */

 #include <stddef.h>
#include "common/common.h"
#include "common/int.h"

_BEGIN_EXTERN_C

typedef enum {
    NONE,
    SCENE,
    PLAYER
} ThingType;

typedef struct {
    u32 width;
    u32 height;
} Scene;

typedef struct {
    u32 x;
    u32 y;
} Player;

COMMON_API void init_things(void);

_END_EXTERN_C
