#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"
#include "flags.h"

#include <stdio.h>

Entity Bullet_enemy_spawn(Pool *p, float x, float y, float speed, float angle, 
                    SpriteID graphic) {
  Entity e = Bullet_spawn(p, x, y, speed, angle, ENT_ENEMY_SHOT, graphic);

  FlagType * flagTypeList = malloc(sizeof(FlagType) * MAX_FLAGS);
  flagList flagList = {.flags = flagTypeList, .size = 1};
  flagList_add_element(&flagList, FLAG_PROJECTILE_ENEMY);
  flagList_add(&p->flagList, e, flagList);
  return e;
}

Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic) {
  Entity e = Bullet_spawn(p, x, y, speed, angle, ENT_PLAYER_SHOT, graphic);

  FlagType * flagTypeList = malloc(sizeof(FlagType) * MAX_FLAGS);
  flagList flagList = {.flags = flagTypeList, .size = 1};
  flagList_add_element(&flagList, FLAG_PROJECTILE_PLAYER);
  flagList_add(&p->flagList, e, flagList);
  return e;


}

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, EntityType tag,
                    SpriteID graphic) {
  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);
  Collision_circle collision = {15.0};

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, tag);

  Collision_circle_add(&p->collision_circle, e, collision);
  return e;
}