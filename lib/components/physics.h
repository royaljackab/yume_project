#pragma once

#include "ecs/component.h"
#include <raylib.h>

#define NO_MAX_SPEED 999999
#define NO_MIN_SPEED -999999

typedef struct Pool Pool;

typedef struct Physics {
  /* Données */
  float speed;  // Vitesse, speed < 0 => angle *= -1
  float accel;  // Accéleration
  float maxSpd; // Vitesse terminale
  float minSpd; // Vitesse minimale
  float angVel; // Vélocité angulaire

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

/**
 * @brief Met a jour toutes les composantes physiques
 *
 * @param p La pool d'objet a updater
 */
extern void Physics_update_all(Pool *p);
