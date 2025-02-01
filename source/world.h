#ifndef WORLD_H
#define WORLD_H
#include <tonc.h>

// Tile y in tiles.
#define FLOOR_TY (SCREEN_HEIGHT_T - 1)

#define FLOOR_Y (8 * FLOOR_TY)

// Initializes tiles representing the world in provided screen block
extern void init_world();
#endif // WORLD_H
