/**
 * @file bullet.h
 * @brief Fonctions qui permettent de tirer des bullets de différentes manières
 * 
 * Une bullet est une entité ECS ayant les composantes:
 * - Position, Physics, Sprite, Life, Collision_circle, Tag, flagList
 */

#pragma once

#include "component.h"
#include "content/assets.h"
#include "ecs/ecs.h"
#include "common.h"
#include "flags.h"

typedef struct Pool Pool;
typedef struct GameContext GameContext;

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
 * 
 * La bullet dispose du tag ENT_ENEMY_SHOT et du flag FLAG_PROJECTILE_ENEMY
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param x Coord x initiale
 * @param y Coord y initiale
 * @param speed Vitesse initiale
 * @param angle Angle init (en degré)
 * @param texture Id de la texture a utiliser
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


                    
/**
 * @brief Spawn un bullet avec une position, un angle et une vitesse initiale
 * 
 * La bullet dispose du tag ENT_PLAYER_SHOT et des flag FLAG_BULLET_PLAYER et FLAG_PROJECTILE_PLAYER
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param x Coord x initiale
 * @param y Coord y initiale
 * @param speed Vitesse initiale
 * @param angle Angle init (en degré)
 * @param texture Id de la texture a utiliser
 */
Entity Bullet_player_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic);


/**
 * @brief Spawn un bullet avec une position, un angle et une vitesse initiale
 * @param p pool courante (toutes les données de l'ECS)
 * @param x Coord x initiale
 * @param y Coord y initiale
 * @param speed Vitesse initiale
 * @param angle Angle init (en degré)
 * @param texture Id de la texture a utiliser
 */
Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle, EntityType tag,
                    SpriteID graphic);


/**
 * @brief Spawn un bullet avec une position, un angle et une vitesse initiale.
 * La bullet réalise une animation de condensation d'une durée spécifiée.
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param x Coord x initiale
 * @param y Coord y initiale
 * @param speed 
 * @param angle 
 * @param graphic 
 * @param delay 
 * @return Entity 
 */
Entity Bullet_enemy_spawn_delayed(Pool *p, float x, float y, float speed, float angle, SpriteID graphic, int delay);

/**
 * @brief Affiche la condensation de toutes les entitées, ou la supprime si elle a expiré
 * 
 * @param p pool courante (toutes les données de l'ECS)
 */
void Condensation_update_all(Pool *p);

/**
 * @brief Détruit une bullet -> effet visuels
 */
void Bullet_kill(GameContext *ctx, Entity bullet);

/**
 * @brief Détruit toutes les bullets
 * 
 * @param p pool courante (toutes les données de l'ECS)
 */
void Bullet_clear_bullets(GameContext *ctx);