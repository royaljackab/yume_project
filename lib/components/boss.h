/**
 * @file boss.h
 * @brief Fonctions spécifiques aux boss
 */

#pragma once

#include "components/life.h"
#include "cotask.h"
#include "obj.h"
#include "pool.h"
#include "tasks.h"

/**
 * @brief Affiche une barre de vie de boss
 * 
 * @param life les points de vies maximum et actuels à représenter dans la barre de vie
 */
void bossbar_draw(Life * life);

/**
 * @brief Trouve et affiche toutes les barres de vies des boss.
 * @note On suppose qu'il n'y a qu'un boss à la fois
 * 
 * @param p pool courante (toutes les données de l'ECS)
 */
void bossbar_draw_all(Pool * p);

/**
 * @brief Crée une entité boss, affiche ses particules visuelles passives.
 * 
 * @param pool pool courante (toutes les données de l'ECS)
 * @param x coordonnée abscisse
 * @param y coordonnée ordonnée
 * @param life le nombre de points de vie du boss
 * @param hitboxRadius la taille de la hitbox du boss (celle-ci est circulaire)
 * @param score le score octroyé lorsque le boss est vaincu
 * @param graphic l'identifiant de la texture à utiliser
 * @return Id du boss
 */
Entity Boss_spawn(Pool *pool, float x, float y, int life, float hitboxRadius, int score, SpriteID graphic);

/**
 * @brief Infoque les effets de particules liés au boss
 * 
 * @param pool la pool (toutes les données de l'ECS)
 * @param boss l'ID du boss à traiter
 * @param lens_center 
 * @param lens_radius
 * @param lens_strength
 */
void Boss_fight_begin(Pool *pool, Entity boss, Vector2 *lens_center, float *lens_radius, float *lens_strength);