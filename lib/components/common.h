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

typedef Vector2 Position;

typedef EntityType Tag;

DEFINE_COMPONENT_MANAGER(Position, MAX_ENTITIES);
DEFINE_COMPONENT_MANAGER(Tag, MAX_ENTITIES);
