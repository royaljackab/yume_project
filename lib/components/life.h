#pragma once
#include "component.h"

typedef struct{
    float life;
    float maxLife;
} Life;

DEFINE_COMPONENT_MANAGER(Life, MAX_ENTITIES)