#pragma once

#include "common.h"
#include "component.h"
#include "ecs.h"

typedef struct Pool {
  PositionManager position;

  Entity next_id;
} Pool;

extern void pool_init(Pool *p);
extern Entity pool_create_entity(Pool *p);
