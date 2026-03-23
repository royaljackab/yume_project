
#pragma once
#include "ecs/component.h"
#include "player.h"
#define MAX_SIZE_NAME 30



typedef struct {
  char * name;
} Enemy;


DEFINE_COMPONENT_MANAGER(Enemy, MAX_ENTITIES);
DECLARE_SETTER_GETTER(Enemy, char * , name);


Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle, int healthPoints, Weapon weapon,
                    SpriteID graphic);