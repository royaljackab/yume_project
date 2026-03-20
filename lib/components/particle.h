#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"



Entity particle_spawn(Pool *p, float x, float y, float speed, float angle, SpriteID graphic);
Entity particle_bound(Pool *p, SpriteID graphic, Entity owner);
