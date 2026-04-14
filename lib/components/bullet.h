/**
 * @file bullet.h
 * @brief Fonctions qui permettent de tirer des bullets de différentes manières
 */

#pragma once

#include "component.h"
#include "content/assets.h"
#include "ecs/ecs.h"
#include "common.h"
#include "flags.h"

typedef struct Pool Pool;

/**
 * @brief composante condensation dans l'ECS
 * @details Avant l'apparition d'une balle, une courte animation appellée condensation préviens le joueur visuellement du danger
 */
typedef struct Condensation {
    int timer;
    int max_time;
    float target_speed;
    Tag target_tag;
    Vector2 target_scale;
} Condensation;

DEFINE_COMPONENT_MANAGER(Condensation, MAX_ENTITIES)

/**
 * @brief Spawn un bullet avec une position, un angle et une vitesse initiale
 * @param p : Pool courrante
 * @param x : Coord x initiale
 * @param y : Coord y initiale
 * @param speed : Vitesse initiale
 * @param angle : Angle init (en degré)
 * @param texture : Id de la texture a utiliser
 */
Entity Bullet_enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic);

                    
Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic);

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, EntityType tag,
                    SpriteID graphic);

Entity Bullet_enemy_spawn_delayed(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, int delay);

void Condensation_update_all(Pool *p);

void Bullet_clear_bullets(Pool *p);