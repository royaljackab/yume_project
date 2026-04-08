#pragma once

#include "component.h"
#include "assets.h"

#define MAX_BACKGROUNDS 100

typedef struct Pool Pool;

typedef struct Background {
    float scroll_speed_x;
    float scroll_speed_y;
} Background;

DEFINE_COMPONENT_MANAGER(Background, MAX_BACKGROUNDS)

DECLARE_SETTER_GETTER(Background, float, scroll_speed_x)
DECLARE_SETTER_GETTER(Background, float, scroll_speed_y)

void Background_update_all(Pool *p);
Entity Background_create(Pool *p, SpriteID bgId, float scroll_speed_x, float scroll_speed_y);