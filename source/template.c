#include "dino.h"
#include "tonc_memdef.h"
#include <string.h>
#include <tonc.h>
#include <world.h>

// With 1024 bytes at our disposal in the OAM, we have room for 128 OBJ_ATTRs
// and 32 OBJ_AFFINEs
OBJ_ATTR object_buffer[128];
OBJ_AFFINE *object_affine_buffer = (OBJ_AFFINE *)object_buffer;

void render_character() {
  int x = 96, y = 32;

  // Dereference the first object in the buffer.
  OBJ_ATTR *character = &object_buffer[0];

  // Easy object initializer
  obj_set_attr(character,
               ATTR0_SQUARE | ATTR0_4BPP | ATTR0_Y(0), // Square sprites...
               ATTR1_SIZE_32,                          // of size 32x32p...
               ATTR2_BUILD(0, /*palbank=*/0,
                           /*prio=*/0) // from palbank 0, and tile index 0.
  );

  // Position sprite.
  obj_set_pos(character, x, y);
  oam_copy(oam_mem, object_buffer, 1); // Update the object buffer.
}

int main(void) {
  // Setting up the tiles:
  memcpy(pal_bg_mem, worldPal, worldPalLen);
  memcpy(&tile_mem[0][0], worldTiles, worldTilesLen);

  // set up BG0 for a 4bpp 32x32t map, using
  //   using charblock 0 and screenblock 31
  REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_4BPP | BG_REG_32x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

  // Setting up sprites:
  // Sprites are placed in block 4 (lower) and 5 (higher).
  // Place the glyphs of the 4bpp boxed character sprite into LOW obj memory
  // (cbb == 4).
  memcpy(&tile_mem[4][0], dinoTiles, dinoTilesLen);
  memcpy(pal_obj_mem, dinoPal, dinoPalLen);

  // Hide all the sprites.
  oam_init(object_buffer, 128);

  // Enabling sprites in the display control.
  REG_DISPCNT |=
      DCNT_OBJ |
      DCNT_OBJ_1D; // Enables rendering of sprites & Object mapping mode.

  while (1) {
    vid_vsync();
    key_poll();
    // accept_input();
    render_character();
  }

  return 0;
}
