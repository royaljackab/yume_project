#pragma once
#include "ecs/component.h"

typedef enum { 
    CENTER,
    TOP
} Collision_rectangle_origin;

typedef struct {
  float width;
  float length;
  Collision_rectangle_origin origin;
} Collision_rectangle;


DEFINE_COMPONENT_MANAGER(Collision_rectangle, MAX_ENTITIES);
DECLARE_SETTER_GETTER(Collision_rectangle, float, width);
DECLARE_SETTER_GETTER(Collision_rectangle, float, length);
DECLARE_SETTER_GETTER(Collision_rectangle, Collision_rectangle_origin, origin);
