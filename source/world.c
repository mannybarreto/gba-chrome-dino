#include "world.h"

#include <stdlib.h>
#include <string.h>
#include <tonc.h>

#include "background.h"

void init_world(int SBB) {
  // Setting up the tiles:
  memcpy(pal_bg_mem, backgroundPal, backgroundPalLen);
  memcpy(&tile_mem[0][0], backgroundTiles, backgroundTilesLen);

  //   using charblock 0 and screenblock 31
  REG_BG0CNT = BG_CBB(0) | BG_SBB(SBB) | BG_4BPP | BG_REG_32x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
  //
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
