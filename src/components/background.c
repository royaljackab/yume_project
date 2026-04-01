#include "background.h"

Entity background_create_layer_speed(Pool *p, float speed, SpriteID graphic, bgScrollType scroll, backgroundID bgID) {
  Entity e = pool_create_entity(p);

  Position pos = {{0, 0}, 0};
  Physics phy = Physics_create_speed(speed);

  if (scroll == BG_SCROLL_HORIZONTAL) pos.angle = 90;
  else if (scroll == BG_SCROLL_VERTICAL) pos.angle = 0;
  else if (scroll == BG_SCROLL_NONE) phy.speed = 0;

  Background bg = {scroll, bgID};

  Position_add(&p->position, e, pos);
  Physics_add(&p->physics, e, phy);
  Sprite_add(&p->sprite, e, sprites[graphic]);
  Tag_add(&p->tag, e, ENT_BACKGROUND);
  flagList_attach_first_flag(p, e, FLAG_OUT_OF_BOUNDS_ALIVE);
  Background_add(&p->background, e, bg);
  return e;
}

Entity background_duplicate_at(Pool *p, Entity original, float x, float y) {
    Entity e = pool_create_entity(p);
    Position newPos = {{x, y}, Position_get(&p->position, original)->angle};
    Physics newPhy = Physics_create_speed(Physics_get(&p->physics, original)->speed);
    Sprite newSprite = *Sprite_get(&p->sprite, original);
    Background newBg = *Background_get(&p->background, original);

    Position_add(&p->position, e, newPos);
    Physics_add(&p->physics, e, newPhy);
    Sprite_add(&p->sprite, e, newSprite);
    Tag_add(&p->tag, e, ENT_BACKGROUND);
    flagList_attach_first_flag(p, e, FLAG_OUT_OF_BOUNDS_ALIVE);
    Background_add(&p->background, e, newBg);
    
    return e;
}

Entity background_create_layer(Pool *p, SpriteID graphic, bgScrollType scroll, backgroundID bgID) {
    float speed = sprites[graphic].renderPriority + 1;
    return background_create_layer_speed(p, speed, graphic, scroll, bgID);
}

bool background_delete_id(Pool *p, backgroundID bgID){
/**
 * supprime tous les backgrounds qui ont l'id passé en paramètre
 * retourne vrai si un background a été supprimé, faux sinon
 */

    BackgroundManager *bgManager = &p->background;
    if (bgID < 0 || bgID >= MAX_ENTITIES) return false;

    // Remove the background entity from the manager
    for (int i = 0; i < MAX_ENTITIES; i++){
        int lookup = Background_get_id(bgManager, i);
        if (Background_get_bgID(lookup) == bgID) {
            pool_kill_entity(bgManager, lookup);
            return true;
        }
    }

    return false;
}

bool background_delete_other(Pool *p, backgroundID bgID){
/**supprime tous les backgrounds qui n'ont pas l'id passé en paramètre
 * retourne vrai si un background a été supprimé, faux sinon
 */
    BackgroundManager *bgManager = &p->background;
    if (bgID < 0 || bgID >= MAX_ENTITIES) return false;

    // Remove the background entity from the manager
    for (int i = 0; i < MAX_ENTITIES; i++){
        int lookup = Background_get_id(bgManager, i);
        if (Background_get_bgID(lookup) != bgID) {
            pool_kill_entity(bgManager, lookup);
            return true;
        }
    }

    return false;
}



void background_update_bounds(Pool *p, Entity e){
/**
 * Si le background ne couvre plus l'integralité du terrain, crée un nouveau background pour que la superposition des 2 le fasse
 * Si le background n'est plus du tous sur l'ecran le tue
 */

    Vector2 pos = Position_get_pos(Position_get(&p->position, e));
    Background bg = *Background_get(&p->background, e);
    Sprite sprite = *Sprite_get(&p->sprite, e);
    if (bg.scroll == BG_SCROLL_HORIZONTAL){
        int spriteWidth = sprites[sprite.textureID].srcRect.width;

        if (pos.x > PANEL_RIGHT){
            //Crée un nv bg a gauche
            background_duplicate_at(p, e, pos.x - spriteWidth, pos.y);

        }
        else if (pos.x < PANEL_LEFT){
            //Crée un nv bg a droite
            background_duplicate_at(p, e, pos.x + spriteWidth, pos.y);
        }

        if (pos.x < PANEL_LEFT - spriteWidth || pos.x > PANEL_RIGHT + spriteWidth) {
            // supprime le bg actuelle
            pool_kill_entity(p, e);
        }

    } else if (bg.scroll == BG_SCROLL_VERTICAL){
        int spriteHeight = sprites[sprite.textureID].srcRect.height;

        if (pos.y > PANEL_DOWN){
            //Crée un nv bg en haut
            background_duplicate_at(p, e, pos.x, pos.y - spriteHeight);
        }
        else if (pos.y < PANEL_UP){
            //Crée un nv bg en bas
            background_duplicate_at(p, e, pos.x, pos.y + spriteHeight);
        }

        if (pos.y < PANEL_UP - spriteHeight || pos.y > PANEL_DOWN + spriteHeight) {
            // supprime le bg actuelle
            pool_kill_entity(p, e);
        }

    }
    //Rien besoin de faire si le bg ne scroll pas

    
}


void background_update_all_bounds(Pool *p){
/** 
 * Utilise background_update_bounds pour mettre à jour tous les backgrounds
*/
    BackgroundManager *bgManager = &p->background;
    for (int i = 0; i < bgManager->count; i++){
        int lookup = Background_get_id(bgManager, i);
        background_update_bounds(p, lookup);
    }
}

void background_set_moriya(Pool *p){
    background_delete_other(p, MORIYA);
    background_create_layer(p, BACKGROUND_FLOWER, BG_SCROLL_NONE, MORIYA);
    background_create_layer(p, BACKGROUND_GRAYCIRCLES, BG_SCROLL_HORIZONTAL, MORIYA);
}

