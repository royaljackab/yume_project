/**
 * @file
 *
 * @brief Composante des bullets
 *
 * Fonctions qui permettent de tirer des bullets de différentes manières
 */

#pragma once

#include "component.h"
#include "content/assets.h"
#include "ecs/ecs.h"
#include "common.h"
#include "flags.h"

typedef struct Pool Pool;

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

/**
 * @brief Spawn bullet après un certain delay. Durant ce delay, le sprite est affiché en agrandissant le sprite de base
 * 
 * @param p 
 * @param x 
 * @param y 
 * @param speed 
 * @param angle 
 * @param graphic 
 * @param delay 
 * @return Entity 
 */
Entity Bullet_enemy_spawn_delayed(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, int delay);

/**
 * @brief Spawn une bullet à un rayon autour d'un point
 * 
 * @param p 
 * @param x 
 * @param y 
 * @param speed 
 * @param angle 
 * @param radius 
 * @param graphic 
 * @return Entity 
 */
Entity Bullet_enemy_spawn_radius(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphic);

Entity Bullet_enemy_spawn_radius_delayed(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphic, int delay);

Entity Bullet_spawn_accelerate(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, float accel, int accel_delay);

Entity Bullet_spawn_radius_accelerate(Pool *p, float x, float y, float speed, float angle, float radius, SpriteID graphice, float accel, int accel_delay);

Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic);

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, EntityType tag,
                    SpriteID graphic);



void Condensation_update_all(Pool *p);

void Bullet_clear_bullets(Pool *p);