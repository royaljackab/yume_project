
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>
#include "components/collision.h"


extern bool Entity_is_hit(Pool *p, Entity entity, flagList * flags);
bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags);
bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags);

extern Collision_circle * Entity_get_collision(Pool *p,  Entity entity);
extern Position * Entity_get_position(Pool *p, Entity entity);
