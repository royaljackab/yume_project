#pragma once

#include "component.h"

typedef struct Physics {
  float speed;
  float angle; // En degrés
  float accel;
  float maxSpd;
  float angVel;
} Physics;

DEFINE_COMPONENT_MANAGER(Physics, MAX_ENTITIES)

DECLARE_SETTER_GETTER(Physics, float, speed)
DECLARE_SETTER_GETTER(Physics, float, angle)
