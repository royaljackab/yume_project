#pragma once

#include "ecs/component.h"
#include "ecs/ecs.h"
#include <raylib.h>
#include <stdbool.h>

#define BOUNDS_X 100 /**< Marge pour la largeur du cadre de jeu*/
#define BOUNDS_Y 50  /**< Marge pour la hauteur du cadre de jeu*/

typedef enum {
  ID_INVALID = -1,
  ENT_PLAYER,
  ENT_ENEMY,
  ENT_BOSS,
  ENT_ENEMY_SHOT,
  ENT_PLAYER_SHOT,
  ENT_ENEMY_LASER,
  ENT_PLAYER_LASER,
  ENT_LOOSE_LASER
} EntityType;

typedef struct Position {
  Vector2 pos;
  float angle; /**< Angle de direction, en degrés*/
} Position;

typedef EntityType Tag;
typedef unsigned long Timer;

DEFINE_COMPONENT_MANAGER(Position, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Position, Vector2, pos)
DECLARE_SETTER_GETTER(Position, float, angle)

DEFINE_COMPONENT_MANAGER(Tag, MAX_ENTITIES)
DEFINE_COMPONENT_MANAGER(Timer, MAX_ENTITIES)

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