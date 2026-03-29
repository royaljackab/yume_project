
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>
#include "components/collision.h"


bool Player_is_hit(Pool *p, Player entity){
    /**
     * Verifie si le joueur est touchés par une attaque ennemis
     */
    return Player_is_hit_by_bullet(p, entity) || Player_is_hit_by_staight_laser(p, entity) || Player_is_hit_by_loose_laser(p, entity);
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
            DrawText("touchés (bullet)", 30, 500, 20, RED);
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

    if (*Tag_get(tagManager, lookup) == ENT_ENEMY_LASER){
      collision = Collision_rectangle_get(rectangleManager, lookup);
      pos = Position_get(positionManager, lookup);
      if (CheckCircleRotatedRect(playerPos, playerRadius, Position_get_pos(pos), Collision_rectangle_get_width(collision), Collision_rectangle_get_length(collision), Position_get_angle(pos)) ){
          DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!! (laser straight)", 30, 600, 20, RED);
          is_hit = true;
      }

    }
  }
  return is_hit;

}



bool CheckCircleRotatedRect(Vector2 cPos, float radius,
                            Vector2 rPos, float w, float h, float angle)
{
    /**
    * Fonction pour verifier la collision entre un rectangle avec un angle et un cercle
    */

    float rad = -angle * DEG2RAD;

    // déplacer le cercle dans l'espace du rectangle
    float dx = cPos.x - rPos.x;
    float dy = cPos.y - rPos.y;

    // rotation inverse
    float localX = dx * cosf(rad) - dy * sinf(rad);
    float localY = dx * sinf(rad) + dy * cosf(rad);

    // clamp au rectangle (centré)
    float closestX = fmaxf(-w/2, fminf(localX, w/2));
    float closestY = fmaxf(-h/2, fminf(localY, h/2));

    float distX = localX - closestX;
    float distY = localY - closestY;

    return (distX*distX + distY*distY) <= radius*radius;
}



bool Player_is_hit_by_loose_laser(Pool *p, Player player){
  /**
   * Verifie si le joueur est touchés par un laser loose ennemis
   */

  bool is_hit = false;

  Loose_laserManager *laser_manager = &p->looseLaser;
  PositionManager *positionManager = &p->position;
  TagManager * tagManager = &p->tag;
  Collision_circleManager * circleManager = &p->collision_circle;

  Position * pos;
  Vector2 playerPos = Position_get_pos(Player_get_position(p, player));
  Collision_circle * collision;
  Rectangle rect;
  Loose_laser *laser;
  float playerRadius = Collision_circle_get_radius(Player_get_collision(p, player));
  int lookup;
  int NodeCount;

  for (int i = 0; i < laser_manager->count; i++) {
    lookup = laser_manager->entity_lookup[i];
    laser = Loose_laser_get(laser_manager, lookup);
    if (*Tag_get(tagManager, lookup) == ENT_LOOSE_LASER){

      collision = Collision_circle_get(circleManager, lookup);
      NodeCount = Loose_laser_get_looseNodeCount(laser);
      for (int j = 0; j < NodeCount; j++){
        pos = Position_get(positionManager, lookup);
        if (CheckCollisionCircles(playerPos, playerRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
            DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!! (laser loose)", 50, 700, 20, RED);
            is_hit = true;
        }
      }

    }
  }
  return is_hit;

}
