/**
 * @file collision_rectangle.h
 * @brief collisions rectangulaires
 */
#pragma once
#include "ecs/component.h"

/**
 * @brief composante collision rectangulaire dans l'ECS
 * 
 */
typedef struct {
  float width;
  float length;
} Collision_rectangle;


DEFINE_COMPONENT_MANAGER(Collision_rectangle, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Collision_rectangle, float, width)
DECLARE_SETTER_GETTER(Collision_rectangle, float, length)
