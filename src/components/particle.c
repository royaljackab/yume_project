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
  EntityType tag = ENT_PARTICLE;

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, tag);

  return e;
}
