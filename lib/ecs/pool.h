#pragma once

#include "collision_circle.h"
#include "collision_rectangle.h"
#include "common.h"
#include "component.h"
#include "ecs.h"
#include "physics.h"
#include "sprite.h"
#include "player_bullet.h"
#include "life.h"
#include "straight_laser.h"
#include "looseLaser.h"

typedef struct Pool {
  //common
  PositionManager position;
  TagManager tag;
  TimerManager timer;

  PhysicsManager physics;
  SpriteManager sprite;

  //collisions
  Collision_circleManager collision_circle;
  Collision_rectangleManager collision_rectangle;

  //lasers
  Straight_laserManager straightLaser;
  Loose_laserManager looseLaser;

  PlayerBulletManager playerBullet;
  LifeManager life;
  




  // Nouvelle gestion des indices
  Entity free_indices[MAX_ENTITIES]; // Le tableau qui stocke les IDs dispos
  int free_top; // L'index du sommet de la pile (si il est a 5000 la pile est
                // pleine et si il est a zero jeu saturé)

  //Toute entité devant être détruite est ajoutée à cette file d'attente. L'entité est détruite en fin de tour du game loop.
  Entity kill_queue[MAX_ENTITIES];
  int kill_count;
} Pool;

extern void pool_init(Pool *p);
extern Entity pool_create_entity(Pool *p);
extern void pool_kill_convicts(Pool *p);
extern void pool_kill_entity(Pool *p, Entity e);
