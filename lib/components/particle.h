#include "components/bullet.h"
#include "common.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"

typedef Pool Pool;

typedef Entity Owner;

//Note: Pour avoir un owner, il faut que celui-ci ait une position et que l'entitée possédée ait une position (dans l'ECS);
DEFINE_COMPONENT_MANAGER(Owner, MAX_ENTITIES)

Entity Particle_spawn(Pool *p, float x, float y, float speed, float angle,
                    SpriteID graphic);