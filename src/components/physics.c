#include "components/physics.h"
#include "components/common.h"
#include "ecs/pool.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
/* Static Functions */
static void update_velocity(Physics *phy, Position *pos) {
  /***
   * Met a jour la vélocité (repère orienté vers le bas)
   */
  float rad = DEG2RAD * pos->angle;

  phy->velocity =
      Vector2Scale((Vector2){cosf(rad), sinf(rad)}, fabsf(phy->speed));
}

static void update_speed(Physics *phy, Position *pos) {
  /***
   * Met a jour la vitesse :
   *  - Acceleration
   *  - MaxSpeed
   *  - MinSpeed
   */

  // Acceleration
  float new_speed = phy->speed + phy->accel;

  // cohérence pour la vitesse et l'angle
  if (phy->speed * new_speed < 0) {
    // On change de sens
    pos->angle += 180.0f;
  }

  phy->speed = new_speed;

  // MaxSpeed
  if (phy->maxSpd != NO_MAX_SPEED && phy->speed > phy->maxSpd) {
    phy->speed = phy->maxSpd;
  }

  // MinSpeed
  if (phy->minSpd != NO_MIN_SPEED && phy->speed < phy->minSpd) {
    phy->speed = phy->minSpd;
  }
}

static void update_angle(Physics *phy, Position *pos) {
  /***
   * Met a jour l'angle :
   *  - Vélocité Angulaire
   */
  pos->angle += phy->angVel;
}

static void update_pos(Physics *phy, Position *pos) {
  /**
   * Met a jour la position :
   * - Vélocité
   */
  pos->pos = Vector2Add(pos->pos, phy->velocity);
}

static void Physics_update(Physics *phy, Position *pos) {
  /***
   * Met a jour une composante physique pour une entité
   */
  update_angle(phy, pos);
  update_speed(phy, pos);
  update_velocity(phy, pos);
  update_pos(phy, pos);
}

/* Extern functions */
Physics Physics_create_speed(float speed) {
  Physics phy = {speed, 0, NO_MAX_SPEED, NO_MIN_SPEED, 0, {0, 0}};
  return phy;
}

void Physics_update_all(Pool *p) {
  /***
   * Met a jour la physique pour la Pool
   */
  PhysicsManager *pm = &p->physics;
  Physics *phy;
  Position *pos;

  // On boucle sur la physique
  for (int i = 0; i < pm->count; i++) {
    phy = &pm->dense[i];
    pos = Position_get(&p->position, pm->entity_lookup[i]);

    Physics_update(phy, pos);
  }
}
