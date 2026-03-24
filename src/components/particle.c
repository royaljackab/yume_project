#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"

Entity particle_spawn(Pool *p, float x, float y, float speed, float angle,
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

Entity particle_bound(Pool *p, SpriteID graphic, Entity owner){
    Entity e = pool_create_entity(p);
    Position pos;
    Tag tag = ENT_PARTICLE;

    Position_add(&p->position, e, pos);
    Sprite_add(&p->sprite, e, sprites[graphic]);
    Tag_add(&p->tag, e, tag);
    Owner_add(&p->owner,e , owner);
    return e;
}


