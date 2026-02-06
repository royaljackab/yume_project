#pragma once

#include "component.h"
#include "ecs.h"
#include "raylib.h"

typedef struct Position {
  Vector2 pos;
} Position;

DEFINE_COMPONENT_MANAGER(Position, MAX_ENTITIES);
