#include "pool.h"
#include "common.h"
#include "raylib.h"

void pool_init(Pool *p) {
  /* Initialisation des composantes */
  Position_init(&p->position);
  Tag_init(&p->tag);
  Physics_init(&p->physics);

  /* Remplissage de la pile d'indices libres */
  for (int i = 0; i < MAX_ENTITIES; i++) {
    p->free_indices[i] = (Entity)(MAX_ENTITIES - 1 - i);
  }
  p->free_top = MAX_ENTITIES; // La pile est pleine au début
}

Entity pool_create_entity(Pool *p) {
  if (p->free_top <= 0) {
    // Plus d'indices disponibles
    return NULL_INDEX;
  }

  // On décrémente et on rend l'ID qui était au sommet
  return p->free_indices[--p->free_top];
}

void pool_destroy_entity(Pool *p, Entity e) {
  if (p->free_top < MAX_ENTITIES) {
    // On remet l'ID dans la pile pour le réutiliser plus tard
    p->free_indices[p->free_top++] = e;
  }
}
