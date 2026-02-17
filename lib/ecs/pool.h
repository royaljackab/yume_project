#pragma once

#include "components/collision_circle.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "ecs/component.h"
#include "ecs/ecs.h"

typedef struct Pool {
  PositionManager position;
  TagManager tag;
  PhysicsManager physics;
  SpriteManager sprite;
  Collision_circleManager collision_circle;
  Collision_rectangleManager collision_rectangle;

  // Nouvelle gestion des indices
  Entity free_indices[MAX_ENTITIES]; // Le tableau qui stocke les IDs dispos
  int free_top; // L'index du sommet de la pile (si il est a 5000 la pile est
                // pleine et si il est a zero jeu saturé)

  Entity kill_queue[MAX_ENTITIES];
  int kill_count;
} Pool;

extern void pool_init(Pool *p);
extern Entity pool_create_entity(Pool *p);
extern void pool_kill_convicts(Pool *p);
extern void pool_kill_entity(Pool *p, Entity e);
