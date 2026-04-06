#include "components/sprite.h"
#include "collision_circle.h"
#include "components/common.h"
#include "content/assets.h"
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
  sprite->scale = (Vector2){1.7, 1.7};
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

void Sprite_draw_sprite(Sprite *sprite, Position *pos, Tag *tag) {
  /**
   * Dessine un sprite a une position donnée en utilisant DrawTexturePro de
   * raylib
   */
  Texture2D tex = textures[sprite->textureID];

  Rectangle destRec = {pos->pos.x, pos->pos.y,
                       sprite->srcRect.width * sprite->scale.x,
                       sprite->srcRect.height * sprite->scale.y};

  Vector2 scaled_center = {sprite->center.x * sprite->scale.x, sprite->center.y * sprite->scale.y};

  // TODO: Tag pour enlever la rotation par défaut
  if ( tag == NULL || (*tag != ENT_ENEMY && *tag != ENT_BOSS) )
    sprite->rotation = pos->angle;

  DrawTexturePro(tex, sprite->srcRect, destRec, scaled_center,
                 sprite->rotation, sprite->color);

}

static bool IsOutOfDrawBounds(Position pos, Sprite sprite) {
  /***
   * Retourne si le sprite au vector2 pos est hors de la fenêtre de dessin ou
   * pas
   */

  return (pos.pos.x + sprite.srcRect.width * sprite.scale.x < PANEL_LEFT ||
          pos.pos.x - sprite.srcRect.width * sprite.scale.x > PANEL_LEFT + PANEL_WIDTH ||
          pos.pos.y + sprite.srcRect.height * sprite.scale.y < PANEL_UP ||
          pos.pos.y - sprite.srcRect.height * sprite.scale.y > PANEL_UP + PANEL_HEIGHT);
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
  TagManager *tagManager = &p->tag;

  Position *pos;
  Entity e;
  Sprite *sprite;
  Tag *tag;
  for (int layer = MIN_LAYER; layer <= MAX_LAYER; layer++) {
    for (int i = 0; i < spriteManager->count; i++) {
      sprite = &spriteManager->dense[i];
      e = Sprite_get_entity(spriteManager, i);
      pos = Position_get(positionManager, e);
      tag = Tag_get(tagManager, e);


      if (IsOutOfDrawBounds(*pos, *sprite)) {
        continue;
      }

      if (sprite->renderPriority == layer) {

        if (sprite->display) {
          Sprite_draw_sprite(sprite, pos, tag);
          if (sprite->isAnimated) {
            UpdateAnimation(sprite);
          }

          Collision_circle *hitbox = Collision_circle_get(&p->collision_circle, e);
          if (hitbox) {
            DrawCircle(pos->pos.x, pos->pos.y, hitbox->radius, RED);
          }
        }
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
        pos->pos.x,
        pos->pos.y,
        laser->laserWidth,
        laser->laserLength
    };

    Vector2 origin = {laser->laserWidth/2.0, 0};

    DrawTexturePro(textures[textureID], source, dest, origin, pos->angle, sprite->color);
}

void drawAllStraightLasers(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager) {
    /**
     * Affiche tous les lasers droits actifs
     */
    Straight_laser *laser;
    Position *pos;
    Sprite *sprite;
    Entity e;
    for (int i=0; i < laserManager->count; i++) {
        laser = &laserManager->dense[i];
        e = Straight_laser_get_entity(laserManager, i);
        pos = Position_get(positionManager, e);
        sprite = Sprite_get(spriteManager, e);

        drawStraightLaser(laser, pos, sprite);
    }
}
