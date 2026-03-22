
#include "components/common.h"
#include "components/collision_rectangle.h"
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>
#include "components/collision.h"






bool Entity_is_hit_by_straight_laser(Pool *p, Entity entity, Tag * LaserTypes);
extern Collision_circle * Entity_get_collision(Pool *p,  Entity entity);
extern Position * Entity_get_position(Pool *p, Entity entity);
bool Entity_is_hit_by_bullet(Pool *p, Entity entity, Tag * BulletTypes);

bool Tag_in_array(Tag * tag, Tag * array, int size) {
    for (int i = 0; i < size; i++) {
        if (*tag == array[i]) {
            return true;
        }
    }
    return false;
}

bool Entity_is_hit(Pool *p, Entity entity, Tag * bulletTypes){
    /**
     * Verifie si le joueur est touchés par une attaque d'un des types spécifiés dans bulletTypes
     */
    return Entity_is_hit_by_bullet(p, entity, bulletTypes);  //|| Entity_is_hit_by_staight_laser(p, entity) || Entity_is_hit_by_loose_laser(p, entity);
}


bool Entity_is_hit_by_bullet(Pool *p, Entity entity, Tag * BulletTypes) {
  /**
   * Verifie si le joueur est touchés par une balle ennemis
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



bool Entity_is_hit_by_staight_laser(Pool *p, Entity entity, Tag * LaserTypes);
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