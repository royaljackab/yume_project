
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>
#include "components/collision.h"



bool Player_is_hit(Pool *p, Player entity){
    /**
     * Verifie si le joueur est touchés par une balle ennemis ou un laser droit
     */
    return Player_is_hit_by_bullet(p, entity) || Player_is_hit_by_staight_laser(p, entity);

}


bool Player_is_hit_by_bullet(Pool *p, Player player) {
  /**
   * Verifie si le joueur est touchés par une balle ennemis
   */
  bool is_hit = false;

  Collision_circleManager *collision_circle_manager = &p->collision_circle;
  PositionManager *positionManager = &p->position;
  TagManager * tagManager = &p->tag;

  Position * pos;
  Vector2 playerPos = Position_get_pos(Player_get_position(p, player));
  Collision_circle * collision;
  float playerRadius = Collision_circle_get_radius(Player_get_collision(p, player));
  int lookup;

  for (int i = 0; i < collision_circle_manager->count; i++) {
    collision = &collision_circle_manager->dense[i];
    lookup = collision_circle_manager->entity_lookup[i];
    pos = Position_get(positionManager, lookup);

    if (*Tag_get(tagManager, lookup) == ENT_ENEMY_SHOT){
        if (CheckCollisionCircles(playerPos, playerRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
            DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!! (laser straight)", 70, 30, 50, RED);
            is_hit = true;
        }
    }
  }
  return is_hit;

}


bool Player_is_hit_by_staight_laser(Pool *p, Player player){
  /**
   * Verifie si le joueur est touchés par un laser droit ennemis
   */

  bool is_hit = false;

  Straight_laserManager *laser_manager = &p->straightLaser;
  PositionManager *positionManager = &p->position;
  TagManager * tagManager = &p->tag;
  Collision_rectangleManager * rectangleManager = &p->collision_rectangle;

  Position * pos;
  Vector2 playerPos = Position_get_pos(Player_get_position(p, player));
  Collision_rectangle * collision;
  Rectangle rect;
  float playerRadius = Collision_circle_get_radius(Player_get_collision(p, player));
  int lookup;

  for (int i = 0; i < laser_manager->count; i++) {

    lookup = laser_manager->entity_lookup[i];
    collision = Collision_rectangle_get(rectangleManager, lookup);
    pos = Position_get(positionManager, lookup);
    //rect = {}
    if (*Tag_get(tagManager, lookup) == ENT_ENEMY_LASER){
        //if (CheckCollisionCircles(playerPos, playerRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
          //CheckCollisionCircleRec(*playerPos, playerRadius, rect);
            DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!! (laser straight)", 70, 30, 50, RED);
            is_hit = true;
       // }
    }
  }
  return is_hit;

}
