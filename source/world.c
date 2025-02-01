#include "world.h"
#include <stdlib.h>

void init_world(int SBB) {
  // Calculate the width of the screenblock matrix row in tiles.
  int floor_width_tiles =
      sizeof(se_mat[SBB][FLOOR_TY]) / sizeof(se_mat[SBB][FLOOR_TY][0]);
  int num_tiles = 10;
  for (int i = 0; i < floor_width_tiles; i++) {
    // Generate a random digit between 1 and the length of the amount of tiles
    // in world.
    int tile = rand() % (num_tiles - 1) + 1;
    se_mat[SBB][FLOOR_TY][i] = tile;
  }
}
