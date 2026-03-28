

#include "components/collision_entity.h"
#include "flags.h"
#include <stdio.h>
#include "core/screen.h"





bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags) {
  /**
   * Verifie si l'enité est touchés par une collision circulaire ayant un des tag de BulletTypes
   */
  if (Entity_has_flag(p, entity, FLAG_INVINCIBLE)) {
    return false; //ne rentre pas dans les calculs si l'entité est invincible
  }
  
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
    if (lookup != entity) {
        collision = Collision_circle_get(collision_circle_manager, lookup);
        pos = Position_get(positionManager, lookup);
        if (Entity_has_flag_in_list(p, lookup, BulletFlags)) {
            if (CheckCollisionCircles(entityPos, entityRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
                DrawText("Entité touché cercle", PANEL_WIDTH + 100, 500, 50, YELLOW);
                is_hit = true;
            }
        }
    }
  }
  return is_hit;
}



bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags){
    /**
     * Verifie si l'enité est touchés par une collision rectangulaire ayant un des tag de LaserTypes
     */
    if (Entity_has_flag(p, entity, FLAG_INVINCIBLE)) {
        return false; //ne rentre pas dans les calculs si l'entité est invincible 
    }

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
                DrawText("Entité touché rectangle", PANEL_WIDTH + 100, 500, 50, RED);
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
    return Entity_is_hit_by_rectangle(p, entity, flags) || Entity_is_hit_by_circle(p, entity, flags); 
}



extern bool Damage_entity_by_enemy_projectile(Pool *p, Entity entity){
    /**
     * Verifie si l'entité (surtout le joueur) est touchée par un projectile ennemi, et lui inflige des dégâts
     */
    flagList bulletFlags = {.flags = (FlagType[]){FLAG_PROJECTILE_ENEMY}, .size = 1};
    if(Entity_is_hit(p, entity, &bulletFlags)){
        Life *life = Life_get(&p->life, entity);
        if (!life) return false;
        Life_damage(life, 1);
        return true;
    }
    return false;
}

extern bool Damage_player_by_enemy_projectile(Pool *p, Entity player){
    /**
     * Verifie si l'entité (surtout le joueur) est touchée par un projectile ennemi, et lui inflige des dégâts
     */
    flagList bulletFlags = {.flags = (FlagType[]){FLAG_PROJECTILE_ENEMY}, .size = 1};
    if(Entity_is_hit(p, player, &bulletFlags)){
        Damage_player(p, player);
        return true;
    }
    return false;
}


bool CheckCircleRotatedRect(Vector2 cPos, float radius,
                            Vector2 rPos, float w, float h, float angle)
{
    /**
    * Fonction pour verifier la collision entre un rectangle avec un angle et un cercle
    */    
    angle += 90;   

    float rad = angle * DEG2RAD; //necessaire car cosf et sinf prennent des randians

    // déplacer le cercle dans l'espace du rectangle
    float dx = cPos.x - rPos.x;
    float dy = cPos.y - rPos.y;

    // rotation inverse
    float localX = dx * cosf(rad) + dy * sinf(rad);
    float localY = dx * sinf(rad) + dy * cosf(rad);

    // clamp au rectangle (centré)
    float closestX = fmaxf(-w/2, fminf(localX, w/2));
    float closestY = fmaxf(-h/2, fminf(localY, h/2));

    float distX = localX - closestX;
    float distY = localY - closestY;
    bool collision = (distX * distX + distY * distY) <= (radius * radius);
    
    DrawRectanglePro((Rectangle){rPos.x, rPos.y, w, h}, (Vector2){w/2, h}, angle, collision ? RED : GREEN);
    

    return collision;
}




bool collision_circle_add_scaled_with_sprite(Pool *p, Entity entity){
    /**
     * Ajoute une collision circulaire à une entité, avec un rayon basé sur la taille de son sprite
     */
    Sprite *sprite = Sprite_get(&p->sprite, entity);
    if (!sprite) return false;

    float radius = (sprite->srcRect.height) / 2.0; // approximation du rayon à partir de la taille du sprite
    Collision_circle collision = {radius};
    Collision_circle_add(&p->collision_circle, entity, collision);
    return true;
}

