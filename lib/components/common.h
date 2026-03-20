#pragma once

#include "ecs/component.h"
#include "ecs/ecs.h"
#include <raylib.h>
#include <stdbool.h>

#define BOUNDS_X 100 /**< Marge pour la largeur du cadre de jeu*/
#define BOUNDS_Y 50  /**< Marge pour la hauteur du cadre de jeu*/

typedef struct Pool Pool;

// ------------ component tag ------------
typedef enum {
  ID_INVALID = -1,
  ENT_PLAYER,
  ENT_ENEMY,
  ENT_BOSS,
  ENT_ENEMY_SHOT,
  ENT_PLAYER_SHOT,
  ENT_ENEMY_LASER,
  ENT_PLAYER_LASER,
  ENT_LOOSE_LASER,
  ENT_PARTICLE,
} EntityType;
typedef EntityType Tag;
DEFINE_COMPONENT_MANAGER(Tag, MAX_ENTITIES)

// ------------ component position ------------
typedef struct Position {
  Vector2 pos;
  float angle; /**< Angle de direction, en degrés*/
} Position;
DEFINE_COMPONENT_MANAGER(Position, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Position, Vector2, pos)
DECLARE_SETTER_GETTER(Position, float, angle)


// ------------ component owner ------------
#define MAX_SERVANTS 200

typedef Entity Owner;
typedef Entity* Servants;

//Note: Pour avoir un owner, il faut que celui-ci ait une position et que l'entitée possédée ait une position (dans l'ECS);
DEFINE_COMPONENT_MANAGER(Owner, MAX_ENTITIES)

/**
 * @brief Renvoie si la position est dans le cadre du jeu
 * 
 * Le cadre du jeu est délimité par les macros PANEL définies dans core/screen.h
 * Il est aussi bounded par les macros BOUND_[X/Y] définies dans components/common.h
 *
 * @param pos 
 * @return true 
 * @return false 
 */
extern bool Position_is_out_of_bounds(Position * pos);

/**
 * @brief Met une valeur de position
 * 
 * @param pos 
 * @param x coordonnée x
 * @param y coordonnée y
 */
extern void Position_set_position(Position * pos, float x, float y);

void Owner_bind(Pool *p, Entity PositionId, Entity ParticleId);
void Owner_update(Pool *p);
