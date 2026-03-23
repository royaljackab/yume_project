
#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"
#include "enemy.h"

Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle, int healthPoints, Weapon weapon,
                    SpriteID graphic) {
  
  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);
  Life life = {.life = healthPoints, .maxLife = healthPoints };
  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, ENT_ENEMY);
  Life_add(&p->life, e, life);
  Weapon_add(&p->weapon, e, weapon);
  return e;
}