
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>




void Entity_is_hit(Pool *p, Entity entity);

void Player_is_hit_by_bullet(Pool *p, Player player) {
  /**
   * Verifie si le joueur et touchés par un projectile (circulaire) ennemis
   */
  
  Player player = p->player.dense[0];


  SpriteManager *collision_circle_manager = &p->collision_circle;
  PositionManager *positionManager = &p->position;


  Position * pos;
  Vector2 * playerPos = Player_get_position(p, player);
  Collision_circle * collision;
  float playerRadius = Collision_circle_get_radius(Player_get_collision(p, player));
  int lookup;

  for (int i = 0; i < collision_circle_manager->count; i++) {
    collision = &collision_circle_manager->dense[i];
    lookup = collision_circle_manager->entity_lookup[i];
    pos = Position_get(positionManager, lookup);

    if (&pos != Player_get_position(p, player)){
        if (CheckCollisionCircles(*playerPos, playerRadius, Position_get_pos(pos), Collision_circle_get_radius(&collision)));
            DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!!", 70, 30, 50, RED);
    }
}