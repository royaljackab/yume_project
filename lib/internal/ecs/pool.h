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

#include "background.h"
#include "bullet.h"
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
#include "flags.h"

/**
 * @brief La pool est la structure qui contient l'Entity Component System
 * Elle gêre les différentes composantes grâce à ses champs de type
 * "manager" et assure la création d'Identifiants cohérents pour les nouvelles
 * entités grâce au tableau "free_indices".
 * Enfin, les entités à détruire son ajouté à sa kill_queue, qui est vidée
 * à la fin de chaque frame.
 * 
 */
typedef struct Pool {
  //common
  PositionManager position;
  TagManager tag;
  UidManager uid;
  OwnerManager owner;

  // TimerManager timer;
  PhysicsManager physics;
  SpriteManager sprite;
  CondensationManager condensation;

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

  flagListManager flagList;

  BackgroundManager background;

  // Nouvelle gestion des indices
  Entity free_indices[MAX_ENTITIES]; // Le tableau qui stocke les IDs dispos
  int free_top; // L'index du sommet de la pile (si il est a 5000 la pile est
                // pleine et si il est a zero jeu saturé)

  //Toute entité devant être détruite est ajoutée à cette file d'attente. L'entité est détruite en fin de tour du game loop.
  Entity kill_queue[MAX_ENTITIES];
  int kill_count;
} Pool;

/**
 * @brief Initialise la pool (ECS) et tous les managers de composants.
 *
 * Initialise chaque ComponentManager présent dans la pool et prépare
 * la pile free_indices contenant les identifiants libres.
 *
 * @param p pool à initialiser
 */
extern void pool_init(Pool *p);

/**
 * @brief Crée une nouvelle entité et retourne son identifiant.
 *
 * Récupère un identifiant libre depuis free_indices et enregistre un Uid
 * unique
 *
 * @param p pool dans laquelle créé l'entité.
 * @return l'Identifiant de la nouvelle entité ou `NULL_INDEX` si erreur.
 */
extern Entity pool_create_entity(Pool *p);

/**
 * @brief Détruit toutes les entités présentes dans la kill_queue.
 *
 * Parcourt kill_queue, retire chaque entité de tous les managers
 * (components) et remet son identifiant dans la pile free_indices.
 * Doit être appelé en fin de frame.
 *
 * @param p pool courante
 */
extern void pool_kill_convicts(Pool *p);

/**
 * @brief Marque une entité pour destruction en fin de frame.
 *
 * L'entité est ajoutée à kill_queue et sera effectivement détruite
 * par pool_kill_convicts lors du traitement en fin de frame.
 * Si l'entité est déjà dans la file, la fonction ne fait rien.
 *
 * @param p pool courante
 * @param e Identifiant de l'entité à détruire.
 */
extern void pool_kill_entity(Pool *p, Entity e);

/**
 * @brief Encapsule une Entity dans un BoxedEntity contenant son UID.
 *
 * Utilisé pour transporter une référence sûre à une entité
 *
 * @param p pool courante
 * @param ent Entité à encapsuler.
 * @return BoxedEntity Boîte contenant l'entité et son unique_id, ou une
 *         boîte nulle si invalide.
 */
extern BoxedEntity entity_box(Pool *p, Entity ent);

/**
 * @brief Récupère l'Id d'une entité à partir d'un BoxedEntity si toujours valide.
 *
 *
 * @param p pool courante
 * @param box Boîte contenant l'entité à extraire.
 * @return Entity L'Id de l'entité si valide, sinon NULL_INDEX.
 */
extern Entity entity_unbox(Pool *p, BoxedEntity box);
