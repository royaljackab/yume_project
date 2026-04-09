/**
 * @file enemy.h
 * @brief Composant et fonctions de gestion des ennemis
 *
 * Ce module gère la création et la mise à jour des entités ennemies.
 * Un ennemi est une entité ECS avec les composants suivants :
 * - Position, Physics, Sprite, Life, Collision_circle, Tag, Enemy
 *
 * @author @amyelalem
 */

#pragma once

#include "ecs/component.h"
#include "content/assets.h"
#include "systems/score.h"

typedef struct Pool Pool;

/**
 * @brief Composant ennemi
 *
 * Contient les informations spécifiques à un ennemi.
 * Les données de vie, hitbox et physique sont dans leurs composants respectifs.
 */
typedef struct {
    unsigned int score; /**< Score à attribuer au joueur lors de la destruction de cet ennemi */
} Enemy;

DEFINE_COMPONENT_MANAGER(Enemy, MAX_ENTITIES)

/**
 * @brief Crée un ennemi dans la pool
 *
 * Attache les composants Position, Physics, Sprite, Life,
 * Collision_circle, Tag et Enemy à une nouvelle entité.
 *
 * @param p Pool courante
 * @param x Position x initiale (en pixels)
 * @param y Position y initiale (en pixels)
 * @param speed Vitesse initiale (en pixels/frame)
 * @param angle Angle initial (en degrés)
 * @param life Points de vie initiaux
 * @param hitboxRadius Rayon de la hitbox circulaire (en pixels)
 * @param type Type de l'ennemi (EnemyType)
 * @param graphic Sprite à utiliser (SpriteID défini dans assets.h)
 * @return L'entité créée
 */
extern Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                          int life, float hitboxRadius,
                          int score, SpriteID graphic);

/**
 * @brief Met à jour tous les ennemis et supprime ceux à 0 PV
 *
 * Parcourt tous les ennemis chaque frame.
 * Si un ennemi n'a plus de points de vie, il est ajouté à la kill queue.
 *
 * @param p Pool courante
 * @param scoreS Système de gestion du score
 */
extern void Enemy_update_all(Pool *p, ScoreSystem *scoreS);
