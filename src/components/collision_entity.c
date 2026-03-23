

#include "components/collision_entity.h"







bool Entity_is_hit_by_circle(Pool *p, Entity entity, Tag * BulletTypes) {
  /**
   * Verifie si l'enité est touchés par une collision circulaire ayant un des tag de BulletTypes
   */
  bool is_hit = false;

  Collision_circleManager *collision_circle_manager = &p->collision_circle;
  PositionManager *positionManager = &p->position;
  TagManager * tagManager = &p->tag;

  Position * pos;
  Vector2 entityPos = Position_get_pos(Entity_get_position(p, entity));
  Collision_circle * collision;
  float entityRadius = Collision_circle_get_radius(Entity_get_collision(p, entity));
  int lookup;

  for (int i = 0; i < collision_circle_manager->count; i++) {
    collision = &collision_circle_manager->dense[i];
    lookup = collision_circle_manager->entity_lookup[i];
    pos = Position_get(positionManager, lookup);
    if (Tag_in_array(Tag_get(tagManager, lookup), BulletTypes, sizeof(BulletTypes)/sizeof(Tag))) {
        if (CheckCollisionCircles(entityPos, entityRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
            DrawText("TU ES TOUCH2S MON DIEU 9A MARCHe!!!!! (bullet)", 50, 500, 50, RED);
            is_hit = true;
        }
    }
  }
  return is_hit;
}



bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, Tag * LaserTypes){
    /**
     * Verifie si l'enité est touchés par une collision rectangulaire ayant un des tag de LaserTypes
     */
    bool is_hit = false;
    
    Collision_rectangleManager *collision_rectangle_manager = &p->collision_rectangle;
    PositionManager *positionManager = &p->position;
    TagManager * tagManager = &p->tag;
    
    Position * pos;
    Vector2 entityPos = Position_get_pos(Entity_get_position(p, entity));
    Collision_rectangle * collision;
    float entityRadius = Collision_circle_get_radius(Entity_get_collision(p, entity));
    int lookup;
    
    for (int i = 0; i < collision_rectangle_manager->count; i++) {
        collision = &collision_rectangle_manager->dense[i];
        lookup = collision_rectangle_manager->entity_lookup[i];
        pos = Position_get(positionManager, lookup);
        if (Tag_in_array(Tag_get(tagManager, lookup), LaserTypes, sizeof(LaserTypes)/sizeof(Tag))) {
            if (CheckCircleRotatedRect(entityPos, entityRadius, Position_get_pos(pos), Collision_rectangle_get_width(collision), Collision_rectangle_get_length(collision), Position_get_angle(pos))){
                DrawText("Entité touchés rectangle", 50, 500, 50, RED);
                is_hit = true;
            }
        }
    }
    return is_hit;
}









extern bool Entity_is_hit(Pool *p, Entity entity, Tag * BulletTypes){
    /**
     * Verifie si l'entité est touchés par une attaque d'un des types spécifiés dans bulletTypes
     */
    return Entity_is_hit_circle(p, entity, BulletTypes) || Entity_is_hit_by_rectangle(p, entity, BulletTypes); 
}






extern Position * Entity_get_position(Pool *p, Entity entity){
    /**
     * Récupère la position a partir d'une entité
     */
  PositionManager *positionManager = &p->position;
  int lookup = positionManager->entity_lookup[(int)entity];
  return &positionManager->dense[lookup];

}

extern Collision_circle * Entity_get_collision(Pool *p,  Entity entity){
    /**
     * Récupère le cercle de collision a partir d'une entité
     */
  Collision_circleManager *collision_circleManager = &p->collision_circle;
  int lookup = collision_circleManager->entity_lookup[(int)entity];
  return &collision_circleManager->dense[lookup];
}