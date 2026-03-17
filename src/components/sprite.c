#include "components/sprite.h"
#include "components/common.h"
#include "core/assets.h"
#include "core/screen.h"
#include "ecs/pool.h"
#include <raylib.h>



#define MIN_LAYER 0
#define MAX_LAYER 100

void Sprite_set_texture(Sprite *sprite, int renderPriority, int textureID) {
  sprite->textureID = textureID;
  sprite->srcRect =
      (Rectangle){0, 0, textures[textureID].width, textures[textureID].height};
  sprite->center =
      (Vector2){(float)textures[textureID].width / 2,
                (float)textures[textureID].height / 2}; // Par défaut, le milieu
  sprite->isAnimated = false;
  sprite->color = WHITE;
  sprite->rotation = 0;
  sprite->renderPriority = renderPriority;
  sprite->scale = (Vector2){1, 1};
  sprite->display = true;
}

void Sprite_set_animation(Sprite *sprite, int frameCount, int delay) {
/*
Crée l'animation pour un spritesheet de type sprite.
Prend le nombre de frame de l'animation et le delai en frame de jeu entre chaque frame de l'animation 
*/
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

void Sprite_set_SourceRect(Sprite *sprite, float x, float y, float width, float height) {
    //x, y le coin en haut a gauche
  sprite->srcRect = (Rectangle){x,y,width,height};
  sprite->center = (Vector2){sprite->srcRect.width / 2, sprite->srcRect.height / 2};
}

void UpdateAnimation(Sprite *sprite) {
  /***
   * Update l'animation d'un sprite a partir de son source Rectangle, et de ses
   * informations de frame.
   */
  sprite->animTimer++;

  if (sprite->animTimer >= sprite->animSpeed) {
    sprite->animTimer = 0;
    sprite->currentFrame = (sprite->currentFrame + 1) % sprite->animFrameCount;
    sprite->srcRect.x = sprite->animStart.x + sprite->frameWidth * sprite->currentFrame;
  }
}

static void Sprite_draw_sprite(Sprite *sprite, Position *pos) {
  /**
   * Dessine un sprite a une position donnée en utilisant DrawTexturePro de
   * raylib
   */
  Texture2D tex = textures[sprite->textureID];

  Rectangle destRec = {pos->pos.x, pos->pos.y,
                       sprite->srcRect.width * sprite->scale.x,
                       sprite->srcRect.height * sprite->scale.y};

  // TODO: Tag pour enlever la rotation par défaut
  sprite->rotation = pos->angle;

  DrawTexturePro(tex, sprite->srcRect, destRec, sprite->center,
                 sprite->rotation, sprite->color);
}

static bool IsOutOfDrawBounds(Position pos, Sprite sprite) {
  /***
   * Retourne si le sprite au vector2 pos est hors de la fenêtre de dessin ou
   * pas
   */

  return (pos.pos.x + sprite.srcRect.width < PANEL_LEFT ||
          pos.pos.x - sprite.srcRect.width > PANEL_LEFT + PANEL_WIDTH ||
          pos.pos.y + sprite.srcRect.height < PANEL_UP ||
          pos.pos.y - sprite.srcRect.height > PANEL_UP + PANEL_HEIGHT);
}

void Sprite_draw_all(Pool *p) {
  /**
   * Dessine tous les sprites du spriteManager en parametre a la position donné
   * par le positionManager en faisant attention à la couche dont il doit être
   * dessiné. les sprites ne sont dessinés uniquement si ils doivent être
   * afficher (sprite.display et DrawBounds).
   */
  SpriteManager *spriteManager = &p->sprite;
  PositionManager *positionManager = &p->position;

  Position *pos;
  int lookup;
  Sprite *sprite;
  for (int layer = MIN_LAYER; layer <= MAX_LAYER; layer++) {
    for (int i = 0; i < spriteManager->count; i++) {
      sprite = &spriteManager->dense[i];
      lookup = spriteManager->entity_lookup[i];
      pos = Position_get(positionManager,lookup);


      if (IsOutOfDrawBounds(*pos, *sprite)) {
        continue;
      }

      if (sprite->renderPriority == layer) {

        if (sprite->display) {
          Sprite_draw_sprite(sprite, pos);
          if (sprite->isAnimated) {
            UpdateAnimation(sprite);
          }
        }
      }
    }
  }
}

