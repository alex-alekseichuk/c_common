#include "common/things.h"

#define THINGS_MAX 1024
#define NIL 0

typedef u32 thing_i;

typedef struct {
    ThingType type;
    thing_i parent;
    thing_i first;
    thing_i next; 
    union {
        Scene scene;
        Player player;
    } d;
} Thing;

static Thing[THINGS_MAX] things;
static thing_i next_free_i = 1;

void init_things(void) {
    for (int i = 0; i < THINGS_MAX; i++) {
        things[i].type = NONE;
        things[i].parent = NIL;
        things[i].first = NIL;
        things[i].next = NIL;
    }
}

// TODO: move things from c_common to c_game
