#ifndef OBJECT_H
#define OBJECT_H
#include <tonc.h>

// Object Attribute Memory (OAM) buffer size.
#define OAM_BUFFER_SIZE 128

// With 1024 bytes at our disposal in the OAM, we have room for 128 OBJ_ATTRs
// and 32 OBJ_AFFINEs
extern OBJ_ATTR object_buffer[OAM_BUFFER_SIZE];
extern OBJ_AFFINE *object_affine_buffer;

struct Position {
  int x;
  int y;
};

extern void render_object_at_position(OBJ_ATTR *attribute,
                                      struct Position position);
#endif // OBJECT_H
