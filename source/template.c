#include "dino.h"
#include "world.h"
#include <string.h>
#include <tonc.h>

// --- Constants ---

// Object Attribute Memory (OAM) buffer size.
#define OAM_BUFFER_SIZE 128

// Main screenblock.
#define SBB 31

// Screen height in tiles.
#define TILE_DIMENSION 8

// Screen height in tiles.
#define SCREEN_TH (160 / TILE_DIMENSION)

// Tile y in tiles.
#define FLOOR_TY (SCREEN_TH - 1)

// --- Globals ---

// With 1024 bytes at our disposal in the OAM, we have room for 128 OBJ_ATTRs
// and 32 OBJ_AFFINEs
OBJ_ATTR object_buffer[OAM_BUFFER_SIZE];
OBJ_AFFINE *object_affine_buffer = (OBJ_AFFINE *)object_buffer;

// Convert tile coordinates to pixel coordinates.
int tile_to_pixel_coordinate(int tile) { return TILE_DIMENSION * tile; }

// Y modifier for jump: y = v₀*t - ½*g*t²
// y is the height, v₀ is the initial vertical velocity, g is the
// acceleration due to gravity, and t is the time of flight.
int calculate_y_for_jump(float initial_velocity, int frame) {
  const float gravity = 0.15;
  return initial_velocity * frame - 0.5f * gravity * frame * frame;
}

enum DinoState { Running, Jumping };

void render_character(int y) {
  int x = 100;

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

void init_map() {
  // Calculate the width of the screenblock matrix row in tiles.
  int floor_width_tiles =
      sizeof(se_mat[SBB][FLOOR_TY]) / sizeof(se_mat[SBB][FLOOR_TY][0]);
  for (int i = 0; i < floor_width_tiles; i++) {
    se_mat[SBB][FLOOR_TY][i] = 1;
  }
}

int state_for_input(int state) {
  switch (state) {
  case (Jumping): {
    return Jumping;
  }
  default: {
    if (key_hit(KEY_A)) {
      return Jumping;
    }
    return Running;
  }
  }
}

int main(void) {
  // Setting up the tiles:
  memcpy(pal_bg_mem, worldPal, worldPalLen);
  memcpy(&tile_mem[0][0], worldTiles, worldTilesLen);

  // set up BG0 for a 4bpp 32x32t map, using
  //   using charblock 0 and screenblock 31
  REG_BG0CNT = BG_CBB(0) | BG_SBB(SBB) | BG_4BPP | BG_REG_32x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

  // Setting up sprites:
  // Sprites are placed in block 4 (lower) and 5 (higher).
  // Place the glyphs of the 4bpp boxed character sprite into LOW obj memory
  // (cbb == 4).
  memcpy(&tile_mem[4][0], dinoTiles, dinoTilesLen);
  memcpy(pal_obj_mem, dinoPal, dinoPalLen);

  // Hide all the sprites.
  oam_init(object_buffer, OAM_BUFFER_SIZE);

  // Enabling sprites in the display control.
  REG_DISPCNT |=
      DCNT_OBJ |
      DCNT_OBJ_1D; // Enables rendering of sprites & Object mapping mode.

  init_map();

  int character_floor_position = tile_to_pixel_coordinate(FLOOR_TY) - 32;
  int frames_in_state = 1;
  int dino_state = Running;
  int y = character_floor_position;
  while (1) {
    vid_vsync();

    // Accept input
    key_poll();
    int state_for_key = state_for_input(dino_state);

    // Update
    if (dino_state != state_for_key) {
      dino_state = state_for_key;
      frames_in_state = 1;
    } else {
      frames_in_state++;
    }

    if (dino_state == Jumping) {
      y -= calculate_y_for_jump(1.1, frames_in_state);

      // Reset the state once we hit the floor.
      if (y >= character_floor_position) {
        y = character_floor_position;
        dino_state = Running;
      }
    }

    // Render
    render_character(y);
  }

  return 0;
}
