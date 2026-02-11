#pragma once

#include "component.h"
#include "ecs.h"
#include "raylib.h"

typedef enum {
    ID_INVALID = -1,
    ENT_PLAYER,
    ENT_ENEMY,
    ENT_BOSS,
    ENT_ENEMY_SHOT,
    ENT_PLAYER_SHOT,
    ENT_ENEMY_LASER,
    ENT_PLAYER_LASER,
    ENT_LOOSE_LASER
} EntityType;

typedef struct Common {
  Vector2 pos;
  EntityType type;
} Common;

DEFINE_COMPONENT_MANAGER(Common, MAX_ENTITIES);
