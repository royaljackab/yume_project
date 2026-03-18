#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"

Entity Particle_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic) {
  
  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);
  Tag tag = ENT_PARTICLE;

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, tag);

  return e;
}

void Particle_bind(Pool *p, Entity PositionId, Entity ParticleId){
    /**
     * Cette fonction permet d'ajouter un owner à une entité, notamment une particule.
     */
    Owner_add(&p->owner,ParticleId,PositionId);
}

void Owner_update(Pool *p){
  for(int i; i< p->owner.count; i++){
    Entity ownedId = Owner_get_entity(&p->owner, i); //l'id de la propriété
    Entity ownerId = p->owner.dense[i]; //l'id du propriétaire
    *Position_get(&p->position, ownedId) = * Position_get(&p->position, ownerId); //la position de la propriété copie tout le temps celle de l'owner
  }
}