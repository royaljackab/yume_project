

#include "components/collision_entity.h"
#include "flags.h"
#include <stdio.h>
#include "core/screen.h"





static bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags, Entity foundCollisions[], int * nbcollisions) {
  /**
   * Verifie si l'entité est touchée par une collision circulaire ayant un des tag de BulletTypes
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
    if ((Entity)lookup != entity) {
        collision = Collision_circle_get(collision_circle_manager, lookup);
        pos = Position_get(positionManager, lookup);
        if (Entity_has_flag_in_list(p, lookup, BulletFlags)) {
            if (CheckCollisionCircles(entityPos, entityRadius, Position_get_pos(pos), Collision_circle_get_radius(collision))){
                DrawText("Entité touché cercle", PANEL_WIDTH + 100, 500, 50, YELLOW);

                if (nbcollisions && *nbcollisions  < MAX_COLLISIONS){
                    foundCollisions[(*nbcollisions)++] = lookup; //Tout ID en collision avec entity est mis dans ce tableau
                }
                
                is_hit = true;
            }
        }
    }
  }
  return is_hit;
}



static bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags, Entity foundCollisions[], int * nbcollisions){
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
                DrawText("Entité touché rectangle", PANEL_WIDTH + 100, 500, 50, RED);

                if (nbcollisions && *nbcollisions  < MAX_COLLISIONS){
                    foundCollisions[(*nbcollisions)++] = lookup; //Tout ID en collision avec entity est mis dans ce tableau
                }

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
    return Entity_is_hit_by_rectangle(p, entity, flags, NULL, NULL) || Entity_is_hit_by_circle(p, entity, flags, NULL, NULL); 
}


extern bool Entity_find_hitters(Pool *p, Entity entity, flagList * flags, Entity foundCollisions[], int *nbCollisions){
    /**
     * @brief Rempli le tableau d'ID passé en paramètre avec les ID des entités ayant les flags spécifiés et en collision avec entity.
     */
    Entity_is_hit_by_rectangle(p, entity, flags, foundCollisions, nbCollisions);
    Entity_is_hit_by_circle(p, entity, flags, foundCollisions, nbCollisions);
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

static bool CheckCircleRotatedRect(Vector2 cPos, float radius, Vector2 rPos, float w, float h, float angle) {

    // 1. Calcul de l'angle, décalage de -90 degrés (jsp pk mais sans probleme)
    float rad = (angle - 90.0f) * DEG2RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    // Vecteurs de direction unitaires
    Vector2 dirX = { cosA, sinA };
    Vector2 dirY = { -sinA, cosA };

    // 2. Calcul des 4 sommets
    // Le rectangle s'étend de -w/2 à w/2 latéralement (dirX)
    // et de 0 à h en profondeur (dirY).
    float hw = w / 2.0f;

    Vector2 corners[4];
    
    // Haut-Gauche : rPos - (largeur/2 * dirX)
    corners[0] = Vector2Subtract(rPos, Vector2Scale(dirX, hw));
    
    // Haut-Droite : rPos + (largeur/2 * dirX)
    corners[1] = Vector2Add(rPos, Vector2Scale(dirX, hw));
    
    // Bas-Droite : Haut-Droite + (hauteur * dirY)
    corners[2] = Vector2Add(corners[1], Vector2Scale(dirY, h));
    
    // Bas-Gauche : Haut-Gauche + (hauteur * dirY)
    corners[3] = Vector2Add(corners[0], Vector2Scale(dirY, h));

    // 3. Boucle de collision par segments
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        //DrawLineV(corners[i], corners[(i + 1) % 4], GREEN);  //affiche si besoin les lignes
        // Test de collision avec le segment actuel
        if (CheckCollisionCircleLine(cPos, radius, corners[i], corners[(i + 1) % 4])) {
            collision = true;
        }
    }

    // 4. Test d'inclusion (si le centre du cercle est à l'intérieur du rectangle)
    if (!collision) {
        // Projection du vecteur (Cercle - rPos) sur les axes locaux
        Vector2 d = Vector2Subtract(cPos, rPos);
        float projX = d.x * cosA + d.y * sinA;     // Axe de la largeur
        float projY = -d.x * sinA + d.y * cosA;    // Axe de la hauteur

        // Vérification des limites : x entre [-w/2, w/2] et y entre [0, h]
        if (projX >= -hw && projX <= hw && projY >= 0 && projY <= h) {
            collision = true;
        }
    }
    return collision;
}


extern bool collision_circle_add_scaled_with_sprite(Pool *p, Entity entity){
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

