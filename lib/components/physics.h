#pragma once

#include "component.h"
#include <raylib.h>

typedef struct Physics {
  float speed; // Vitesse

  Vector2 velocity;

  float accel;  // Accéleration
  float maxSpd; // Vitesse terminale
  float angVel; // Vélocité angulaire
} Physics;

DEFINE_COMPONENT_MANAGER(Physics, MAX_ENTITIES)

DECLARE_SETTER_GETTER(Physics, float, speed)
DECLARE_SETTER_GETTER(Physics, Vector2, velocity)
DECLARE_SETTER_GETTER(Physics, float, accel)
DECLARE_SETTER_GETTER(Physics, float, maxSpd)
DECLARE_SETTER_GETTER(Physics, float, angVel)
