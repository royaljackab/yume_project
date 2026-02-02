#include "../../lib/pool.h"

void pool_init(Pool *p) {
  p->next_id = 0;

  /* Initialisation des composantes */
  Position_init(&p->position);
}

Entity pool_create_entity(Pool *p) {
  // TODO: Gestion du recyclage d'indice

  return p->next_id++;
}
