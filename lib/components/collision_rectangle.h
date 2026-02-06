#pragma once
#include "../component.h"

typedef struct {
    float width;
    float length;
} Collision_rectangle;


DEFINE_COMPONENT_MANAGER(Collision_rectangle, MAX_ENTITIES);
DECLARE_SETTER_GETTER(Collision_rectangle, float, width);
DECLARE_SETTER_GETTER(Collision_rectangle, float, length);
