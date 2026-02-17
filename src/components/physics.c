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

  phy->velocity = Vector2Scale((Vector2){cosf(rad), sinf(rad)}, phy->speed);
}

static void update_speed(Physics *phy, Position *pos) {
  /***
   * Met a jour la vitesse :
   *  - Acceleration
   *  - MaxSpeed
   *  - MinSpeed
   */

  // Acceleration
  phy->speed += phy->accel;

  // On s'assure que la vitesse reste positive
  if (phy->speed < 0) {
    phy->speed *= -1;
    // On change de sens
    pos->angle += 180.0f;
  }

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
