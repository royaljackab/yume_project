#include "components/bullet.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"

Entity Bullet_spawn(Pool *p, float x, float y, float speed, float angle,
                    TextureID texture) {

  Entity e = pool_create_entity(p);

  Position pos = {{x, y}, angle};
  Physics phy = Physics_create_speed(speed);
  Sprite sprite;
  Sprite_set_texture(&sprite, texture);

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprite);

  return e;
}
