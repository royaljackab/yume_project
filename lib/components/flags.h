/**
 * @file flags.h
 * @brief Composant Flag pour identifier le type d'une entité
 */

#pragma once

#include "ecs/component.h"

/* Types de flags pour les entités */
typedef enum {
    FLAG_NONE = 0,
    FLAG_PLAYER,
    FLAG_ENEMY,
    FLAG_BOSS,
    FLAG_BULLET_PLAYER,
    FLAG_BULLET_ENEMY,
    FLAG_POWERUP,
    FLAG_WALL
} FlagType;

/* Composant Flag */
typedef struct {
    FlagType type;
} Flag;

/* Génération automatique du FlagManager et des fonctions init/add/get/remove */
DEFINE_COMPONENT_MANAGER(Flag, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Flag, FlagType, type)