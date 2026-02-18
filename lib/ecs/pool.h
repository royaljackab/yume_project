/***************************************************************
 * @file pool.h
 * @brief Structure contenant l'ECS
 *
 * UTILISATION :
 *  1) Créer les fichiers.h et .c décrivant votre composante
 *  2) Dans le .h, utiliser les macros DEFINE_COMPONENT_MANAGER
 *  et DECLARE_SETTER_GETTER dans ecs/component.h
 *  3) Rajouter votre ComponentManger ici, dans la structure Pool
 *  4) Ajouter la fonction Component_init dans Pool_init (ecs/pool.c)
 *  5) Ajouter la fonction Component_remove dans pool_kill_entity (ecs/pool.c)
 *
 ***************************************************************/

#pragma once

#include "components/collision_circle.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "components/life.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "ecs/ecs.h"

typedef struct Pool {
  PositionManager position;
  TagManager tag;
  PhysicsManager physics;
  SpriteManager sprite;
  Collision_circleManager collision_circle;
  Collision_rectangleManager collision_rectangle;
  LifeManager life;

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
