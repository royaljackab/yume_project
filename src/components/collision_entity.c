

#include "components/collision_entity.h"
#include "flags.h"
#include <stdio.h>






bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags) {
  /**
   * Verifie si l'enité est touchés par une collision circulaire ayant un des tag de BulletTypes
   */
  bool is_hit = false;
  Collision_circleManager *collision_circle_manager = &p->collision_circle;
  PositionManager *positionManager = &p->position;

  Position * pos;
  Vector2 entityPos = Position_get_pos(Position_get(positionManager, entity));
  Collision_circle * collision;
  float entityRadius = Collision_circle_get_radius(Collision_circle_get(collision_circle_manager, entity));
  int lookup;
  for (int i = 0; i < collision_circle_manager->count; i++) {
    lookup = collision_circle_manager->entity_lookup[i];
    collision = Collision_circle_get(collision_circle_manager, lookup);
    pos = Position_get(positionManager, lookup);
    
    if (Entity_has_flag_in_list(p, lookup, BulletFlags)) {
        if (CheckCollisionCircles(entityPos, entityRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
            DrawText("Entité touché cercle", 50, 500, 50, YELLOW);
            printf("touché cercle\n");
            is_hit = true;
        }
    }
  }
  return is_hit;
}



bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags){
    /**
     * Verifie si l'enité est touchés par une collision rectangulaire ayant un des tag de LaserTypes
     */
    bool is_hit = false;
    Collision_rectangleManager *collision_rectangle_manager = &p->collision_rectangle;
    PositionManager *positionManager = &p->position;
    
    Position * pos;
    Vector2 entityPos = Position_get_pos(Position_get(positionManager, entity));
    Collision_rectangle * collision;
    float entityRadius = Collision_circle_get_radius(Collision_circle_get(&p->collision_circle, entity));
    int lookup;
    
    for (int i = 0; i < collision_rectangle_manager->count; i++) {
        collision = &collision_rectangle_manager->dense[i];
        lookup = collision_rectangle_manager->entity_lookup[i];
        pos = Position_get(positionManager, lookup);
        if (Entity_has_flag_in_list(p, lookup, laserFlags)) {
            if (CheckCircleRotatedRect(entityPos, entityRadius, Position_get_pos(pos), Collision_rectangle_get_width(collision), Collision_rectangle_get_length(collision), Position_get_angle(pos))){
                DrawText("Entité touché rectangle", 50, 500, 50, RED);
                is_hit = true;
            }
        }
    }
    return is_hit;
}


extern bool Entity_is_hit(Pool *p, Entity entity, flagList * flags){
    /**
     * Verifie si l'entité est touchés par une attaque d'un des types spécifiés dans bulletTypes
     */
    return Entity_is_hit_by_circle(p, entity, flags) || Entity_is_hit_by_rectangle(p, entity, flags); 
}




