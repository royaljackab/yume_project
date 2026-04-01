#pragma once

#include "common.h"
#include "physics.h"
#include "sprite.h"
#include "core/screen.h"
#include "ecs/component.h"
#include "assets.h"

typedef struct Pool Pool;

typedef enum {
  BG_SCROLL_NONE,
  BG_SCROLL_HORIZONTAL,
  BG_SCROLL_VERTICAL
} bgScrollType;
typedef enum {
    MORIYA = 0,
} backgroundID;


typedef struct {
  bgScrollType scroll;
  backgroundID bgID; //permet de gerer plusieurs background de manière commune (layers)
} Background;


DEFINE_COMPONENT_MANAGER(Background, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Background, bgScrollType, scroll)
DECLARE_SETTER_GETTER(Background, backgroundID, bgID)


Entity background_create_layer(Pool *p, SpriteID graphic, bgScrollType scroll, backgroundID bgID);
bool background_delete_id(Pool *p, backgroundID bgID);
bool background_delete_other(Pool *p, backgroundID bgID);
void background_update_all_bounds(Pool *p);
void background_update_bounds(Pool *p, Entity e);
void background_set_moriya(Pool *p);
