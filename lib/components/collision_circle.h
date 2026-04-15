/**
 * @file collision_circle.h
 * @brief collisions circulaires
 */

#pragma once

#include "ecs/component.h"

/**
 * @brief composante collision circulaire dans l'ECS
 */
typedef struct {
  float radius;
} Collision_circle;

DEFINE_COMPONENT_MANAGER(Collision_circle, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Collision_circle, float, radius)

