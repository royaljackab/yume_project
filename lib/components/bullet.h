#pragma once

#include "core/assets.h"
#include "ecs/ecs.h"
#include "pool.h"

extern Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle,
                           TextureID texture);
