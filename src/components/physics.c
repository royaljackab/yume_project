#include "physics.h"
#include "common.h"
#include <raylib.h>
#include <raymath.h>

/* Static Functions */
static void update_velocity(Position *pos, Physics *phy) {
  /***
   * Met a jour la vélocité (repère orienté vers le bas)
   */
  phy->velocity =
      Vector2Scale((Vector2){cosf(pos->angle), cosf(pos->angle)}, phy->speed);
}

static void update_accel(Physics *phy) {}
