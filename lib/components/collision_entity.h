
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>
#include "components/collision.h"



bool Entity_is_hit_by_bullet(Pool *p, Entity entity, Tag * BulletTypes);

bool Entity_is_hit_by_straight_laser(Pool *p, Entity entity, Tag * LaserTypes);
extern Collision_circle * Entity_get_collision(Pool *p,  Entity entity);
extern Position * Entity_get_position(Pool *p, Entity entity);