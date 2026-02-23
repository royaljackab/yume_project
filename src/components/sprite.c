#include "sprite.h"
#include "assets.h"
#include "screen.h"
#include "straight_laser.h"
#include "common.h"

#define MIN_LAYER 0
#define MAX_LAYER 100

void SetTexture(Sprite *sprite, int textureID) {
  sprite->textureID = textureID;
  sprite->srcRect =
      (Rectangle){0, 0, textures[textureID].width, textures[textureID].height};
  sprite->center =
      (Vector2){textures[textureID].width / 2,
                textures[textureID].height / 2}; // Par défaut, le milieu
  sprite->isAnimated = false;
  sprite->color = WHITE;
  sprite->rotation = 0;
  sprite->scale = (Vector2){1, 1};
}

void SetAnimation(Sprite *sprite, int frameCount, int delay) {
  sprite->isAnimated = true;
  sprite->animFrameCount = frameCount;
  sprite->srcRect.width = sprite->srcRect.width / frameCount;
  sprite->frameWidth = sprite->srcRect.width;
  sprite->animStart = (Vector2){sprite->srcRect.x, sprite->srcRect.y};
  sprite->center =
      (Vector2){sprite->srcRect.width / 2, sprite->srcRect.height / 2};
  sprite->animSpeed = delay;
  sprite->animTimer = 0;
  sprite->currentFrame = 0;
}

void UpdateAnimation(Sprite* sprite) {
    /***
     * Update l'animation d'un sprite a partir de son source Rectangle, et de ses informations de frame.
     */
    sprite->animTimer++;

  if (sprite->animTimer >= sprite->animSpeed) {
    sprite->animTimer = 0;
    sprite->currentFrame = (sprite->currentFrame + 1) % sprite->animFrameCount;
    sprite->srcRect.x =
        sprite->animStart.x + sprite->frameWidth * sprite->currentFrame;
  }
}

void DrawSprite(Sprite sprite, Vector2 pos) {
    /***
     * Dessine un sprite a une position donnée en utilisant DrawTexturePro de raylib
     */
    Texture2D tex = textures[sprite.textureID];

  Rectangle destRec = {pos.x, pos.y, sprite.srcRect.width * sprite.scale.x,
                       sprite.srcRect.height * sprite.scale.y};

  DrawTexturePro(tex, sprite.srcRect, destRec, sprite.center, sprite.rotation,
                 sprite.color);
}

static bool IsOutOfDrawBounds(Vector2 pos, Sprite sprite) {
    /***
     * Retourne si le sprite au vector2 pos est hors de la fenêtre de dessin ou pas
     */

    return (pos.x + sprite.srcRect.width < PANEL_LEFT || pos.x - sprite.srcRect.width > PANEL_LEFT + PANEL_WIDTH 
        || pos.y + sprite.srcRect.height < PANEL_UP || pos.y - sprite.srcRect.height > PANEL_UP + PANEL_HEIGHT);
        
}

static bool IsOutOfBounds(Vector2 pos) {
    /***
     * Retourne si le Vector2 pos est hors limites ou pas
     */

     return (pos.x < - DRAW_MARGIN || pos.x > PANEL_WIDTH + PANEL_LEFT + DRAW_MARGIN
        || pos.y < -DRAW_MARGIN || pos.y > PANEL_UP + PANEL_HEIGHT + DRAW_MARGIN);
}



void drawAll(SpriteManager * spriteManager, PositionManager * positionManager) {
    /***
     * Dessine tous les sprites du spriteManager en parametre a la position donné par le positionManager en faisant attention à la couche dont il doit être dessiné.
     * les sprites ne sont dessinés uniquement si ils doivent être afficher (sprite.display et DrawBounds). 
     */

    Vector2 pos;
    int lookup;
    Sprite sprite;
    for (int layer = MIN_LAYER; layer <= MAX_LAYER; layer++){
        for (int i=0; i < spriteManager->count; i++)
        {
            sprite = spriteManager->dense[i];
            lookup = spriteManager->entity_lookup[i];
            if (sprite.renderPriority == layer){
                pos = positionManager->dense[lookup].coord;

                if (sprite.display && IsOutOfDrawBounds(pos, sprite))         
                   DrawSprite(sprite, pos);
            }
        }
    }
}

void drawStraightLaser(Straight_laser *laser, Position * pos, Sprite * sprite){
    /**
     * Affiche un laser droit selon sa largeur et sa longueur, à sa position et avec la couleur de son sprite
     * 
     */
    int textureID = sprite->textureID;

    Rectangle source = sprite->srcRect;

    Rectangle dest = {
        pos->coord.x,
        pos->coord.y,
        laser->laserWidth,
        laser->laserLength
    };

    Vector2 origin = {laser->laserWidth/2.0, 0};

    DrawTexturePro(textures[textureID], source, dest, origin, pos->angle, sprite->color);
}

drawAllStraightLasers(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager) {
    /**
     * Affiche tous les lasers droits actifs
     */
    Straight_laser *laser;
    Position pos;
    Sprite sprite;
    for (int i=0; i < laserManager->count; i++) {
        laser = &laserManager->dense[i];
        pos = positionManager->dense[laserManager->entity_lookup[i]];
        sprite = spriteManager->dense[laserManager->entity_lookup[i]];
        drawStraightLaser(laser, &pos, &sprite);
    }
}