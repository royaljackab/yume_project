#include "background.h"

#include "assets.h"
#include "obj.h"
#include "pool.h"

Entity background_create_layer_speed(Pool *p, float speed, SpriteID graphic, bgScrollType scroll, backgroundID bgID) {
  Entity e = pool_create_entity(p);

  // 1. On centre parfaitement le fond dans le terrain !
  Position pos = {
      { PANEL_LEFT + (PANEL_WIDTH / 2.0f), PANEL_UP + (PANEL_HEIGHT / 2.0f) },
      0 
  };
  Physics phy = Physics_create_speed(speed);

  // 2. On corrige les angles de direction
  if (scroll == BG_SCROLL_HORIZONTAL) {
      pos.angle = 180; // 180 = Défile vers la GAUCHE (comme dans les jeux classiques)
  }
  else if (scroll == BG_SCROLL_VERTICAL) {
      pos.angle = 90;  // 90 = Défile vers le BAS
  }
  else if (scroll == BG_SCROLL_NONE) {
      phy.speed = 0;
  }

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
    BackgroundManager *bgManager = &p->background;
    bool deleted_something = false;

    // CORRECTION : On s'arrête à 'count', pas à MAX_ENTITIES !
    for (int i = 0; i < bgManager->count; i++){
        Entity lookup = bgManager->entity_lookup[i];
        if (Background_get_bgID(Background_get(bgManager, lookup)) == bgID) {
            pool_kill_entity(p, lookup);
            deleted_something = true;
            i--; // CORRECTION CRUCIALE : On recule d'un pas car l'ECS vient de boucher le trou !
        }
    }
    return deleted_something;
}

bool background_delete_other(Pool *p, backgroundID bgID){
    BackgroundManager *bgManager = &p->background;
    bool deleted_something = false;

    for (int i = 0; i < bgManager->count; i++){
        Entity lookup = bgManager->entity_lookup[i];
        if (Background_get_bgID(Background_get(bgManager, lookup)) != bgID) {
            pool_kill_entity(p, lookup);
            deleted_something = true;
            i--; 
        }
    }
    return deleted_something;
}

void background_update_bounds(Pool *p, Entity e){
    Position *pos = Position_get(&p->position, e);
    Background *bg = Background_get(&p->background, e);
    Sprite *sprite = Sprite_get(&p->sprite, e);
    
    if (!pos || !bg || !sprite) return;

    // Le vrai point central du terrain
    float centerX = PANEL_LEFT + (PANEL_WIDTH / 2.0f);
    float centerY = PANEL_UP + (PANEL_HEIGHT / 2.0f);

    if (bg->scroll == BG_SCROLL_HORIZONTAL){
        float width = sprite->srcRect.width * sprite->scale.x;

        // La boucle parfaite : on saute dès qu'on a parcouru exactement la taille d'une image
        if (pos->pos.x <= centerX - width){
            pos->pos.x += width * 2.0f; 
        }
        else if (pos->pos.x >= centerX + width){
            pos->pos.x -= width * 2.0f;
        }

    } else if (bg->scroll == BG_SCROLL_VERTICAL){
        float height = sprite->srcRect.height * sprite->scale.y;

        if (pos->pos.y <= centerY - height){
            pos->pos.y += height * 2.0f;
        }
        else if (pos->pos.y >= centerY + height){
            pos->pos.y -= height * 2.0f;
        }
    }
}

void background_update_all_bounds(Pool *p){
/** 
 * Utilise background_update_bounds pour mettre à jour tous les backgrounds
*/
    BackgroundManager *bgManager = &p->background;
    for (int i = 0; i < bgManager->count; i++){
        int lookup = Background_get_entity(bgManager, i);
        background_update_bounds(p, lookup);
    }
}

void background_set_moriya(Pool *p){
    background_delete_other(p, MORIYA);
    
    // Le fond fixe
    Entity bg0 = background_create_layer_speed(p, 0.0f, BACKGROUND_FLOWER, BG_SCROLL_NONE, MORIYA);
    obj_SetScale(p, bg0, 2.5, 2.5);
    
    // LE FOND GRIS (Une seule entité !)
    // On met sa vitesse physique à 0.0f car l'entité ne bougera pas, c'est l'image qui glissera.
    Entity bg1 = background_create_layer_speed(p, 1.0f, BACKGROUND_GRAYCIRCLES, BG_SCROLL_HORIZONTAL, MORIYA);
    obj_SetAlpha(p, bg1, 120);
    
    Sprite *spr1 = Sprite_get(&p->sprite, bg1);
    spr1->scale = (Vector2){1.0f, 1.0f}; // L'image reste nette et bien zoomée !
}
