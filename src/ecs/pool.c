#include "../../lib/ecs/pool.h"
#include "raylib.h"
void pool_init(Pool *p) {
  p->next_id = 0;

  /* Initialisation des composantes */
  Common_init(&p->position);
  Physics_init(&p->physics);
  Sprite_init(&p->sprite);
}

Entity pool_create_entity(Pool *p) {
  // TODO: Gestion du recyclage d'indice

  return p->next_id++;
}
