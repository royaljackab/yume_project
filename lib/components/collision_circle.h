#pragma once
#include "../component.h"

typedef struct {
    float radius;
} Collision_circle;

DEFINE_COMPONENT_MANAGER(Collision_circle, MAX_ENTITIES);
DECLARE_SETTER_GETTER(Collision_circle, float, radius);
