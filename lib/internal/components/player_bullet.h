#pragma once
#include "component.h"

/**
 * @file player_bullet.h
 * @brief Composant décrivant les tirs du joueur (dégâts).
 */

/**
 * @brief Composant PlayerBullet dans l'ECS.
 * Contient les informations liées à un projectile du joueur.
 */
typedef struct {
    int damage; /**< Dégâts infligés par la balle */
} PlayerBullet;

DEFINE_COMPONENT_MANAGER(PlayerBullet, MAX_ENTITIES)
DECLARE_SETTER_GETTER(PlayerBullet, int, damage)