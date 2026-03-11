#pragma once

#include "ecs/component.h"
#include "ecs/ecs.h"
#include <raylib.h>

typedef enum {
  ID_INVALID = -1,
  ENT_PLAYER,
  ENT_ENEMY,
  ENT_BOSS,
  ENT_ENEMY_SHOT,
  ENT_PLAYER_SHOT,
  ENT_ENEMY_LASER,
  ENT_PLAYER_LASER,
  ENT_PARTICLE,
  ENT_LOOSE_LASER
} EntityType;

typedef struct Position {
  Vector2 pos;
  float angle;
} Position;

typedef EntityType Tag;


DEFINE_COMPONENT_MANAGER(Position, MAX_ENTITIES);
DECLARE_SETTER_GETTER(Position, Vector2, pos);
DECLARE_SETTER_GETTER(Position, float, angle);

DEFINE_COMPONENT_MANAGER(Tag, MAX_ENTITIES)


