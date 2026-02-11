#pragma once

#include "common.h"
#include "component.h"
#include "ecs.h"
#include "physics.h"
#include "sprite.h"
#include "ecs.h"
#include "collision_circle.h"
#include "collision_rectangle.h"
#include "player_bullet.h"
#include "life.h"
#include "straight_laser.h"

typedef struct Pool {
  PositionManager position;
  TagManager tag;
  PhysicsManager physics;
  SpriteManager sprite;
  Collision_circleManager collision_circle;
  Collision_rectangleManager collision_rectangle;
  PlayerBulletManager playerBullet;
  LifeManager life;
  Straight_laserManager straightLaser;
  TimerManager timer;

  // Nouvelle gestion des indices
  Entity free_indices[MAX_ENTITIES]; // Le tableau qui stocke les IDs dispos
  int free_top; // L'index du sommet de la pile (si il est a 5000 la pile est
                // pleine et si il est a zero jeu saturé)
} Pool;

extern void pool_init(Pool *p);
extern Entity pool_create_entity(Pool *p);
extern void pool_destroy_entity(Pool *p,
                                Entity e); // La fonction de destruction
