/**
 * @file enemy.h
 * @brief Composante et fonctions pour les ennemis
 */

#pragma once

#include "ecs/component.h"
#include "core/assets.h"

typedef struct Pool Pool;

typedef enum {
    ENEMY_TYPE_FAIRY,
    ENEMY_TYPE_BUTTERFLY,
    ENEMY_TYPE_BIG_FAIRY
} EnemyType;

typedef struct {
    EnemyType type;
} Enemy;

DEFINE_COMPONENT_MANAGER(Enemy, MAX_ENTITIES)

/**
 * @brief Spawn un ennemi dans la pool
 * @param p Pool courante
 * @param x Position x initiale
 * @param y Position y initiale
 * @param speed Vitesse initiale
 * @param angle Angle initial (en degrés)
 * @param life Points de vie
 * @param hitboxRadius Rayon de la hitbox
 * @param type Type d'ennemi
 * @param graphic Sprite à utiliser
 * @return L'entité créée
 */
extern Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                          int life, float hitboxRadius,
                          EnemyType type, SpriteID graphic);

/**
 * @brief Vérifie si des ennemis sont morts et les tue
 * @param p Pool courante
 */
extern void Enemy_update_all(Pool *p);