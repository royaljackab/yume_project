#include "../../lib/components/common.h"
#include "physics.h"
#include <raylib.h>
#include <raymath.h>

void Physics_update_position(Physics *physics, Position *position) {
  float rad = physics->angle * DEG2RAD;
  Vector2 velocity =
      Vector2Scale((Vector2){cosf(rad), sinf(rad)}, physics->speed);
  position->pos = Vector2Add(position->pos, velocity);
}
