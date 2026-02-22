/**
 * @file
 *
 * @brief Composante des bullets
 *
 * Fonctions qui permettent de tirer des bullets de différentes manières
 */

#pragma once

#include "core/assets.h"
#include "ecs/ecs.h"
#include "pool.h"

/**
 * @brief Spawn un bullet avec une position, un angle et une vitesse initiale
 * @param p : Pool courrante
 * @param x : Coord x initiale
 * @param y : Coord y initiale
 * @param speed : Vitesse initiale
 * @param angle : Angle init (en degré)
 * @param texture : Id de la texture a utiliser
 */
// TODO: J'ai mis TextureID prc que le systeme de sprite est codé avec le cul!
// A changer au plus vite svp
extern Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle,
                           TextureID texture);
