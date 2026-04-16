/**
 * @file
 * @brief Composante de Cinématique (Mouvement)
 *
 * Ce module gère l'intégration du mouvement pour les entités.
 * Il s'occupe exclusivement du déplacement et NON des collisions.
 *
 * SYSTÈME D'UNITÉS :
 * - Distance     : Pixels
 * - Temps        : Secondes (dépend du Delta Time)
 * - Vitesse      : Pixels / Seconde
 * - Accélération : Pixels / Seconde²
 * - Angles       : Degrés (0° = Droite, 90° = Bas, Sens Horaire)
 *
 * DONNÉES GÉRÉES :
 * - velocity (Vector2) : Vecteur de déplacement calculé à chaque frame.
 * - speed (float)      : Scalaire de la vitesse
 * - min/maxSpd (float) : Bornes pour la vitesse
 * - accel (float)      : Taux de changement de la vitesse (pour
 * accél/freinage).
 * - angVel (float)     : Vitesse angulaire en degrés/seconde.
 *
 * DÉPENDANCES :
 * - [READ/WRITE] Position ( Pos(x, y), angle)
 *
 * NOTE :
 * - La vélocité scale avec la valeur absolue de speed -> speed PEUT être
 * négative Dans ce cas, l'angle est incrémenté de 180° (demi-tour)
 *
 */

#pragma once

#include "ecs/component.h"
#include <raylib.h>

#define NO_MAX_SPEED 999999
#define NO_MIN_SPEED -999999

typedef struct Pool Pool;

/**
 * @brief composante physique dans l'ECS
 * Cette composante ajoute les notions de vitesse, accélération, vélocité angulaire et force newtonienne à une entité.
 */
typedef struct Physics {
  /* Données */
  float speed;  // Vitesse, speed < 0 => angle *= -1
  float accel;  // Accéleration
  float maxSpd; // Vitesse terminale
  float minSpd; // Vitesse minimale
  float angVel; // Vélocité angulaire
  Vector2 force;

  /* Calcul */
  Vector2 velocity;
} Physics;

DEFINE_COMPONENT_MANAGER(Physics, MAX_ENTITIES)

DECLARE_SETTER_GETTER(Physics, float, speed)
DECLARE_SETTER_GETTER(Physics, Vector2, velocity)
DECLARE_SETTER_GETTER(Physics, float, accel)
DECLARE_SETTER_GETTER(Physics, float, maxSpd)
DECLARE_SETTER_GETTER(Physics, float, minSpd)
DECLARE_SETTER_GETTER(Physics, float, angVel)
DECLARE_SETTER_GETTER(Physics, Vector2, force);

/**
 * @brief Crée une composante physique avec uniquement la speed
 *
 * Toutes les autres valeurs sont initialisées à 0 et NO_MAX/MIN_SPEED
 *
 * @param speed : Vitesse
 * @return Composante physique associée
 */
extern Physics Physics_create_speed(float speed);

/**
 * @brief Met a jour toutes les composantes physiques
 *
 * @param p La pool d'objet a updater
 */
extern void Physics_update_all(Pool *p);
