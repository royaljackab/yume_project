#pragma once
#include "component.h"

typedef struct{
    float hp;
    float maxHp;
} Life;

DEFINE_COMPONENT_MANAGER(Life, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Life, float, hp)
DECLARE_SETTER_GETTER(Life, float, maxHp)