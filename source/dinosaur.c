#include "dinosaur.h"

#include <string.h>
#include <tonc.h>

#include "dino.h"
#include "tonc_video.h"
#include "world.h"

#define DINO_BASE_Y (FLOOR_Y - 24 - 2)
#define INITIAL_VELOCITY 1.1

struct Dinosaur init_dino(void) {
  struct Dinosaur dino;

  memcpy(&tile_mem[4][0], dinoTiles, dinoTilesLen);
  memcpy(pal_obj_mem, dinoPal, dinoPalLen);

  // Dereference the first object in the buffer.
  OBJ_ATTR *object_attributes = &object_buffer[0];
  obj_set_attr(object_attributes,
               ATTR0_SQUARE | ATTR0_4BPP | ATTR0_Y(0), // Square sprites...
               ATTR1_SIZE_32,                          // of size 32x32p...
               ATTR2_BUILD(0, /*palbank=*/0,
                           /*prio=*/0) // from palbank 0, and tile index 0.
  );
  dino.attributes = object_attributes;

  dino.state = Running;
  dino.frames_in_state = 1;

  struct Position position;
  position.x = (SCREEN_WIDTH / 2) - 16;
  position.y = DINO_BASE_Y;
  dino.position = position;

  return dino;
};

static int state_for_input(int state) {
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

// Y modifier for jump: y = v₀*t - ½*g*t²
// y is the height, v₀ is the initial vertical velocity, g is the
// acceleration due to gravity, and t is the time of flight.
static int calculate_y_for_jump(int frame) {
  const float gravity = 0.123;
  return INITIAL_VELOCITY * frame - 0.5f * gravity * frame * frame;
}

void update_dino(struct Dinosaur *dino) {
  int state_for_key = state_for_input(dino->state);
  if (dino->state != state_for_key) {
    dino->state = state_for_key;
    dino->frames_in_state = 1;
  } else {
    dino->frames_in_state++;
  }

  if (dino->state == Jumping) {
    dino->position.y -= calculate_y_for_jump(dino->frames_in_state);

    // Reset the state once we hit the floor.
    if (dino->position.y >= DINO_BASE_Y) {
      dino->position.y = DINO_BASE_Y;
      dino->state = Running;
    }
  }

  // Update animation.

  ATTR2_BUILD(0, /*palbank=*/0,
              /*prio=*/0);
}
