/***************************************************************
 * @file pool.h
 * @brief Structure contenant l'ECS
 *
 * UTILISATION :
 *  1) Créer les fichiers.h et .c décrivant votre composante
 *  2) Dans le .h, utiliser les macros DEFINE_COMPONENT_MANAGER
 *  et DECLARE_SETTER_GETTER dans ecs/component.h
 *  3) Rajouter votre ComponentManager ici, dans la structure Pool
 *  4) Ajouter la fonction Component_init dans Pool_init (ecs/pool.c)
 *  5) Ajouter la fonction Component_remove dans pool_kill_entity (ecs/pool.c)
 *
 ***************************************************************/

#pragma once

#include "collision_circle.h"
#include "collision_rectangle.h"
#include "common.h"
#include "life.h"
#include "physics.h"
#include "sprite.h"
#include "ecs/ecs.h"
#include "player.h"
#include "life.h"
#include "straight_laser.h"
#include "looseLaser.h"
#include "enemy.h"

typedef struct Pool {
  //common
  PositionManager position;
  TagManager tag;
  OwnerManager owner;

  // TimerManager timer;
  PhysicsManager physics;
  SpriteManager sprite;

  EnemyManager enemy;

  //collisions
  Collision_circleManager collision_circle;
  Collision_rectangleManager collision_rectangle;

  //lasers
  Straight_laserManager straightLaser;
  Loose_laserManager looseLaser;

  LifeManager life;
  PlayerManager player;
  WeaponManager weapon;

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
