#include "ecs/pool.h"
#include "components/common.h"
#include "ecs/ecs.h"
#include "life.h"
#include "player.h"

void pool_init(Pool *p) {
  /* Initialisation des composantes */
  // NOTE: Pour chaque composante, ajouter le init ici
  Position_init(&p->position);
  Tag_init(&p->tag);
  Timer_init(&p->timer);

  Physics_init(&p->physics);
  Sprite_init(&p->sprite);

  Collision_circle_init(&p->collision_circle);
  Collision_rectangle_init(&p->collision_rectangle);
  
  Life_init(&p->life);
  Player_init(&p->player);
  Weapon_init(&p->weapon);

  Straight_laser_init(&p->straightLaser);
  Loose_laser_init(&p->looseLaser);
  

  /* Remplissage de la pile d'indices libres */
  for (int i = 0; i < MAX_ENTITIES; i++) {
    p->free_indices[i] = (Entity)(MAX_ENTITIES - 1 - i);
  }
  p->free_top = MAX_ENTITIES; // La pile est pleine au début
  p->kill_count = 0;          // Personne a tuer
}

Entity pool_create_entity(Pool *p) {
  if (p->free_top <= 0) {
    // Plus d'indices disponibles
    return NULL_INDEX;
  }

  // On décrémente et on rend l'ID qui était au sommet
  return p->free_indices[--p->free_top];
}

// NOTE: Cette fonction DOIT être utilisée dans le while du main lorsqu'il
// utilise une pool
void pool_kill_convicts(Pool *p) {
  /***
   * Détruit toutes les entités dans la kill_queue
   */
  Entity e;
  for (int i = 0; i < p->kill_count; i++) {
    // NOTE: Pour chaque composante, ajouter le destroy ici
    e = p->kill_queue[i];

    //commons
    Position_remove(&p->position, e);
    Tag_remove(&p->tag, e);
    Timer_remove(&p->timer, e);

    Physics_remove(&p->physics, e);
    Sprite_remove(&p->sprite, e);

    //collisions
    Collision_circle_remove(&p->collision_circle, e);
    Collision_rectangle_remove(&p->collision_rectangle, e);
    Life_remove(&p->life, e);
    Player_remove(&p->player, e);
    Weapon_remove(&p->weapon, e);

    //lasers
    Straight_laser_remove(&p->straightLaser, e);
    Loose_laser_destroy(&p->looseLaser, e);
    Life_remove(&p->life, e);



    
    // Ajout de l'entité dans la pile libre
    p->free_indices[p->free_top++] = e;
  }

  p->kill_count = 0;
}

void pool_kill_entity(Pool *p, Entity e) {
  /***
   * Ajoute l'entité dans le kill_queue
   */
  if (p->kill_count < MAX_ENTITIES) {
    p->kill_queue[p->kill_count++] = e;
  }
}
