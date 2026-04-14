/**
 * @file background.h
 * @brief Affichage des backgrounds 2D
 */

#pragma once

#include "component.h"
#include "assets.h"

#define MAX_BACKGROUNDS 100

typedef struct Pool Pool;

/**
 * @brief Composante fond d'écran 2D.
 */
typedef struct Background {
    float scroll_speed_x;
    float scroll_speed_y;
} Background;

DEFINE_COMPONENT_MANAGER(Background, MAX_BACKGROUNDS)

DECLARE_SETTER_GETTER(Background, float, scroll_speed_x)
DECLARE_SETTER_GETTER(Background, float, scroll_speed_y)

/**
 * @brief met à jour le sprite de tous les backgrounds selon leurs vitesses de défilement.
 * @param p pool courante (toutes les données de l'ECS)
 */
void Background_update_all(Pool *p);

/**
 * @brief créé un background
 * @param p pool courante (toutes les données de l'ECS)
 * @param bgId l'identifiant de la texture associée au background
 * @param scroll_speed_x vitesse de défilement horizontal du background (positif vers la droite, négatif vers la gauche)
 * @param scroll_speed_y vitesse de défilement vertical du background (positif vers le bas, négatif vers le haut)
 * @return Id du background créé
 */
Entity Background_create(Pool *p, SpriteID bgId, float scroll_speed_x, float scroll_speed_y);