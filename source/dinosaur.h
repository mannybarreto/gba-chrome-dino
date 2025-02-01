#ifndef DINO_H
#define DINO_H
#include <tonc.h>

#include "object.h"

enum State { Running, Jumping };

struct Dinosaur {
  OBJ_ATTR *attributes;

  enum State state;
  int frames_in_state;

  struct Position position;

  int frames_in_sprite;
  int sprite_index;
};

struct Dinosaur init_dino(void);

extern void update_dino(struct Dinosaur *dino);
#endif // DINO_H
