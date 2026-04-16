/**
 * @file particle.h
 * @brief fonctions d'invocation d'effets visuels
 * 
 */
#pragma once

#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"


/**
 * @brief Crée une particule
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param x coordonnée x initiale
 * @param y coordonnée y initiale
 * @param speed vitesse initiale
 * @param angle angle initial
 * @param graphic l'identifiant de la texture à utiliser
 * @return l'id de l'entité créée
 */
Entity particle_spawn(Pool *p, float x, float y, float speed, float angle, SpriteID graphic);

/**
 * @brief Crée une particule à partir d'un sprite, puis la lie à une entité
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param graphic l'identifiant de la texture à utiliser
 * @param owner l'identifiant de l'entité qui devient owner de la particule
 * @return l'id de l'entité créée
 */
Entity particle_bound(Pool *p, SpriteID graphic, Entity owner);
