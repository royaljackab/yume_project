#include "../../lib/pool.h"
#include "raylib.h"
void pool_init(Pool *p) {
  p->next_id = 0;

  /* Initialisation des composantes */
  Position_init(&p->position);
  Physics_init(&p->physics);
  Sprite_init(&p->sprite);
  Collision_circle_init(&p->collision_circle);
  Collision_rectangle_init(&p->collision_circle);
}

Entity pool_create_entity(Pool *p) {
  // TODO: Gestion du recyclage d'indice

  return p->next_id++;
}
