#include "object.h"

OBJ_ATTR object_buffer[128];
OBJ_AFFINE *object_affine_buffer = (OBJ_AFFINE *)object_buffer;

void render_object_at_position(OBJ_ATTR *object, struct Position position) {
  // Position sprite.
  obj_set_pos(object, position.x, position.y);
  oam_copy(oam_mem, object_buffer, 1);
}
