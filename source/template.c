#include "cacti.h"
#include "dino.h"
#include "dinosaur.h"
#include "object.h"
#include "tonc_memdef.h"
#include "world.h"
#include <string.h>
#include <tonc.h>

int main(void) {
  init_world();

  memcpy(&tile_mem[5][0], cactiTiles, cactiTilesLen);
  memcpy(pal_obj_mem + dinoPalLen, cactiPal, cactiPalLen);

  // Hide all the sprites.
  oam_init(object_buffer, OAM_BUFFER_SIZE);

  // Enabling sprites in the display control.
  REG_DISPCNT |=
      DCNT_OBJ |
      DCNT_OBJ_1D; // Enables rendering of sprites & Object mapping mode.
  struct Dinosaur dinosaur = init_dino();

  float scroll_velocity = 0.5;
  float scroll_offset = 0;

  // TODO: Create a vector of these we can move through and translate as
  // needed.
  OBJ_ATTR *cactus = &object_buffer[1];

  // TODO: There's probably a better constant for this?
  int cactus_tile_index = sizeof(tile_mem[4]) / sizeof(tile_mem[4][0]);

  obj_set_attr(cactus,
               ATTR0_TALL | ATTR0_4BPP | ATTR0_Y(0), // Rectangle sprite...
               ATTR1_SIZE_16x32,                     // of size 16x32p...
               ATTR2_BUILD(cactus_tile_index, /*palbank=*/0,
                           /*prio=*/0) // from palbank 0, and tile index 0.
  );
  while (1) {
    vid_vsync();

    // Accept input
    key_poll();

    // Update
    scroll_offset += scroll_velocity;
    update_dino(&dinosaur);

    // Render
    REG_BG0HOFS = scroll_offset;

    render_object_at_position(dinosaur.attributes, dinosaur.position);

    // Position sprite.
    obj_set_pos(cactus, dinosaur.position.x + 40, FLOOR_Y - 26);
    oam_copy(oam_mem, object_buffer, 2); // Update the object buffer.
  }

  return 0;
}
