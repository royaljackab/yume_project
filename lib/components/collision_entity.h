#pragma once
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>

#define MAX_COLLISIONS 100

extern bool Entity_find_hitters(Pool *p, Entity entity, flagList * flags, Entity foundCollisions[], int *nbCollisions);
extern bool Entity_is_hit(Pool *p, Entity entity, flagList * flags);

extern bool Damage_entity_by_enemy_projectile(Pool *p, Entity entity);
extern bool Damage_player_by_enemy_projectile(Pool *p, Entity player);

extern bool collision_circle_add_scaled_with_sprite(Pool *p, Entity entity);


static bool CheckCircleRotatedRect(Vector2 cPos, float radius, Vector2 rPos, float w, float h, float angle);
static bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags, Entity foundCollisions[], int * nbcollisions);
static bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags, Entity foundCollisions[], int * nbcollisions);