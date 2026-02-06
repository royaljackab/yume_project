#pragma once

#include "common.h"
#include "component.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "ecs.h"

typedef struct Pool {
  CommonManager position;
  PhysicsManager physics;
  SpriteManager sprite;

  Entity next_id;
} Pool;

extern void pool_init(Pool *p);
extern Entity pool_create_entity(Pool *p);
