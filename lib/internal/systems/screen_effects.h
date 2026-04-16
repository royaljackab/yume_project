#pragma once

#include <raylib.h>

#include "core/coroutine/tasks.h"

extern Vector2 invert_centers[4];
extern float invert_radius;

DECLARE_EXTERN_TASK(player_hit_effect, {Pool *pool; Entity player;});