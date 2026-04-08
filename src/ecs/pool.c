#include "ecs/pool.h"
#include "background.h"
#include "bullet.h"
#include "components/common.h"
#include "ecs/ecs.h"
#include "life.h"
#include "player.h"
#include <stdint.h>

static uint32_t uid_counter = 0;

void pool_init(Pool *p) {
  /* Initialisation des composantes */
  // NOTE: Pour chaque composante, ajouter le init ici
  Position_init(&p->position);
  Tag_init(&p->tag);
  Uid_init(&p->uid);
  Owner_init(&p->owner);

  Physics_init(&p->physics);
  Sprite_init(&p->sprite);
  Condensation_init(&p->condensation);

  Collision_circle_init(&p->collision_circle);
  Collision_rectangle_init(&p->collision_rectangle);
  
  Life_init(&p->life);
  Player_init(&p->player);
  Weapon_init(&p->weapon);

  Enemy_init(&p->enemy);

  Straight_laser_init(&p->straightLaser);
  Loose_laser_init(&p->looseLaser);
  
  flagList_init(&p->flagList);

  Background_init(&p->background);

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
  Entity id = p->free_indices[--p->free_top];
  Uid uid = {
    .unique_id = ++uid_counter,
  };
  Uid_add(&p->uid, id, uid);
  return id;
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
    Uid_remove(&p->uid, e);
    Owner_remove(&p->owner, e);

    Physics_remove(&p->physics, e);
    Sprite_remove(&p->sprite, e);
    Condensation_remove(&p->condensation, e);

    //collisions
    Collision_circle_remove(&p->collision_circle, e);
    Collision_rectangle_remove(&p->collision_rectangle, e);
    Life_remove(&p->life, e);
    Player_remove(&p->player, e);
    Weapon_remove(&p->weapon, e);

    //lasers
    Straight_laser_remove(&p->straightLaser, e);
    Loose_laser_destroy(p, e);
    Life_remove(&p->life, e);

    Enemy_remove(&p->enemy, e);
    Background_remove(&p->background, e);
    flagList_destroy(p, e);

    // Ajout de l'entité dans la pile libre
    p->free_indices[p->free_top++] = e;
  }

  p->kill_count = 0;
}

void pool_kill_entity(Pool *p, Entity e) {

  // On vérifie que l'objet n'est pas déjà dans la kill queue
  for (int i = 0; i < p->kill_count; i++) {
    if (p->kill_queue[i] == e) {
      return; 
    }
  }

  /***
   * Ajoute l'entité dans le kill_queue
   */
  if (p->kill_count < MAX_ENTITIES) {
    p->kill_queue[p->kill_count++] = e;
  }
}

BoxedEntity entity_box(Pool *p, Entity ent) {
  if (ent == NULL_INDEX) {
    return (BoxedEntity){ .id = NULL_INDEX, .unique_id = 0};
  }

  Uid *uid_comp = Uid_get(&p->uid, ent);
  if (!uid_comp)
    return (BoxedEntity){.id=NULL_INDEX, .unique_id = 0};

  return (BoxedEntity){
    .id = ent,
    .unique_id = uid_comp->unique_id,
  };
}

Entity entity_unbox(Pool *p, BoxedEntity box) {
  if (box.id == NULL_INDEX || box.unique_id == 0) return NULL_INDEX;

  Uid *uid_comp = Uid_get(&p->uid, box.id);
  if (uid_comp && uid_comp->unique_id == box.unique_id)
    return box.id;

  return NULL_INDEX;
}
