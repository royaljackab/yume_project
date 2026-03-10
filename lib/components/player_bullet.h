#pragma once
#include "component.h"

typedef struct{
    int damage;
} PlayerBullet;

DEFINE_COMPONENT_MANAGER(PlayerBullet, MAX_ENTITIES)
DECLARE_SETTER_GETTER(PlayerBullet, int, damage)