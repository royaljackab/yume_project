#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"

Entity Bullet_enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic) {
  Bullet_spawn(p, x, y, speed, angle, ENT_ENEMY, graphic);
}

Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic) {
  Bullet_spawn(p, x, y, speed, angle, ENT_PLAYER_SHOT, graphic);
}

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, Tag tag, 
                    SpriteID graphic) {
  
  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, tag);

  return e;
}