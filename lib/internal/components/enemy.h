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
 * @brief composante ennemi dans l'ECS
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
 * @param p pool courante (toutes les données de l'ECS)
 * @param x Position x initiale
 * @param y Position y initiale
 * @param speed Vitesse initiale
 * @param angle Angle initial
 * @param life Points de vie initiaux
 * @param hitboxRadius Rayon de la hitbox circulaire
 * @param graphic l'identifiant de la texture à utiliser
 * @return L'entité créée
 */
extern Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                          int life, float hitboxRadius,
                          int score, SpriteID graphic);

/**
 * @brief Met à jour tous les ennemis
 *
 * Parcourt tous les ennemis chaque frame. Détecte si l'ennemi doit:
 * - attribuer du score à sa mort
 * - subir des dégâts
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param scoreS Système de gestion du score
 */
extern void Enemy_update_all(Pool *p, ScoreSystem *scoreS);


/** @brief spawn un enemy qui a le flag qui fait perdre des points si il est tué 
 * @param x Position x initiale
 * @param y Position y initiale
 * @param speed Vitesse initiale
 * @param angle Angle initial
 * @param life Points de vie initiaux
 * @param hitboxRadius Rayon de la hitbox circulaire
 * @param score Le score que fait perdre l'ennemi si il est tué
 * @param graphic l'identifiant de la texture à utiliser
 * @return L'entité créée
 */
Entity Enemy_spawn_score_decrease(Pool *p, float x, float y, float speed, float angle, float hitboxRadius, int score, SpriteID graphic);