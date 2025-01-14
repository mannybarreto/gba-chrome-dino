#include "world.h"

void init_world(int SBB) {
  // Calculate the width of the screenblock matrix row in tiles.
  int floor_width_tiles =
      sizeof(se_mat[SBB][FLOOR_TY]) / sizeof(se_mat[SBB][FLOOR_TY][0]);
  for (int i = 0; i < floor_width_tiles; i++) {
    se_mat[SBB][FLOOR_TY][i] = 1;
  }
}
