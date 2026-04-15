/**
 * @file
 * @brief Composante de l'HP
 *
 * Ce module s'occupe de la gestion des points de vie SIMPLE des objets.
 *
 * DONNÉES GÉRÉES :
 *  - maxLife (int) : Points de vie initiale
 *  - life (int)    : Points de vie courrants
 *
 */

#pragma once

#include "ecs/component.h"
#include "player.h"
#include "systems/score.h"
#include <stdbool.h>

typedef struct Life {
  int maxLife;
  int life;
} Life;

typedef struct Pool Pool;

DEFINE_COMPONENT_MANAGER(Life, MAX_ENTITIES)

DECLARE_SETTER_GETTER(Life, int, maxLife)
DECLARE_SETTER_GETTER(Life, int, life)

/**
 * @brief Applique des dégats
 * @param life Composante HP
 * @param damage Dégâts à appliquer
 */
extern void Life_damage(Life *life, int damage);

/**
 * @brief Rajoute de l'HP
 * @param life Composante HP
 * @param heal Soin à appliquer
 */
extern void Life_heal(Life *life, int heal);

/**
 * @brief Renvoie si l'objet est mort <=> HP <= 0
 * @param life Composante HP
 * @return true s'il est mort, false sinon
 */
extern bool Life_is_dead(Life *life);

/**
 * @brief  tue / effectue un traitement adapté à tout les objets ayant 0 HP
 * @param p pool
 * @param scoreS manager de score
 */
extern void Life_update_all(GameContext *ctx, ScoreSystem * scoreS);