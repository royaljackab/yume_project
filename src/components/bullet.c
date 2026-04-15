#include "components/bullet.h"
#include "common.h"
#include "obj.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"
#include "tasks.h"
#include "life.h"
#include "tasks.h"
#include "common_task.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

Entity Bullet_enemy_spawn(Pool *p, float x, float y, float speed, float angle, 
                    SpriteID graphic) {
  Entity e = Bullet_spawn(p, x, y, speed, angle, ENT_ENEMY_SHOT, graphic);
  flagList_attach_first_flag(p, e, FLAG_PROJECTILE_ENEMY);
  return e;
}

Entity Bullet_enemy_spawn_radius(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphic) {
  float rad_angle = angle * DEG2RAD;
  float new_x = x + radius * cosf(rad_angle);
  float new_y = y + radius * sinf(rad_angle);

  return Bullet_enemy_spawn(p, new_x, new_y, speed, angle, graphic);
}

Entity Bullet_spawn_radius_accelerate(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphic, float accel, int accel_delay) {
  Entity bullet = Bullet_enemy_spawn_radius(p, x, y, speed, angle, radius, graphic);
  INVOKE_TASK(Bullet_spawn_accel, p, bullet, speed, accel, accel_delay);
  return bullet;
}

Entity Bullet_spawn_accelerate(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, float accel, int accel_delay) {
  Entity bullet = Bullet_enemy_spawn(p, x, y, speed, angle, graphic);
  INVOKE_TASK(Bullet_spawn_accel, p, bullet, speed, accel, accel_delay);
  return bullet;
}

Entity Bullet_enemy_spawn_radius_delayed(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphic, int delay) {
  float rad_angle = angle * DEG2RAD;
  float new_x = x + radius * cosf(rad_angle);
  float new_y = y + radius * sinf(rad_angle);

  return Bullet_enemy_spawn_delayed(p, new_x, new_y, speed, angle, graphic, delay);
}

Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic) {
  Entity e = Bullet_spawn(p, x, y, speed, angle, ENT_PLAYER_SHOT, graphic);
  flagList_attach_first_flag(p, e, FLAG_PROJECTILE_PLAYER);
  flagList_add_element(flagList_get(&p->flagList, e), FLAG_BULLET_PLAYER);
  obj_SetHitboxRadius(p, e, 30);
                    
  obj_SetAlpha(p, e, 170);
  obj_SetRenderPriority(p, e, 9);
  return e;

}

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, EntityType tag,
                    SpriteID graphic) {
  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);
  Collision_circle collision = {sprites[graphic].frameWidth / 2.0};
  Life life = {1,1};
  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, tag);
  Life_add(&p->life, e, life);

  obj_SetRenderPriority(p, e, RENDER_PRIO_BULLET);


  Collision_circle_add(&p->collision_circle, e, collision);
  return e;
}

Entity Bullet_enemy_spawn_delayed(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, int delay) {
  Entity e = Bullet_enemy_spawn(p, x, y, 0, angle, graphic);

  Condensation cond = {
    .timer = delay,
    .max_time = delay,
    .target_speed = speed,
    .target_tag = ENT_ENEMY_SHOT,
    .target_scale = obj_GetScale(p, e)
  };
  Condensation_add(&p->condensation, e, cond);

  obj_SetScale(p, e, cond.target_scale.x * 3.0, cond.target_scale.y * 3.0);
  obj_SetAlpha(p, e, 0.0f);

  return e;
}

void Condensation_update_all(Pool *p) {
  for (int i=0; i < p->condensation.count; ++i) {
    Entity e = Condensation_get_entity(&p->condensation, i);
    Condensation *cond = &p->condensation.dense[i];

    cond->timer--;

    float progression = 1.0 - ((float)cond->timer / (float)cond->max_time);

    float current_scaleX = (cond->target_scale.x * 3.0) - ((cond->target_scale.x * 2.0) * progression);
    float current_scaleY = (cond->target_scale.y * 3.0) - ((cond->target_scale.y * 2.0) * progression);
    obj_SetScale(p, e, current_scaleX, current_scaleY);

    obj_SetAlpha(p, e, 255.0 * progression);

    if (cond->timer <= 0) {
      obj_SetScale(p, e, cond->target_scale.x, cond->target_scale.y);
      obj_SetAlpha(p, e, 255);

      obj_SetSpeed(p, e, cond->target_speed);
      obj_SetTag(p, e, cond->target_tag);

      Condensation_remove(&p->condensation, e);
      i--;
    }
  }
}

void Bullet_clear_bullets(Pool *p) {
  for (int i=0; i < p->tag.count; ++i) {
    Entity e = Tag_get_entity(&p->tag, i);
    Tag tag = p->tag.dense[i];

    if (tag == ENT_ENEMY_SHOT || tag == ENT_ENEMY_LASER) {
      pool_kill_entity(p, e);
    }
  }
}

