#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>
#include <raymath.h>

#include "components/sprite.h"

// Audio : background music for now
typedef enum {
  BGM_FAST_DANGER,
  BGM_QUIRKY_GOOFY,
  BGM_TO_BE_NAMED, // maybe won't be used
  BGM_WAITING,
  MAX_BGM // Pour savoir combien on en a
} BGMId;

typedef enum {
  BULLET_SPRITESHEET,
  BOSS_SPRITESHEET,
  ANIM_TEST_SPRITESHEET,
  REIMU_SPRITESHEET,
  HITBOX_SPRITESHEET,
  FAIRY_BLUE_STILL_SPRITESHEET,  // ajout 1
  MAX_TEXTURES
} TextureID;

typedef enum {
  BALL_M_BLACK,
  ANIM_TEST,

  REIMU_IDLE,
  REIMU_RIGHT,
  REIMU_LEFT,
  REIMU_PINK_AMULET,
  FAIRY_BLUE_STILL, // ajout 2
  MAX_SPRITES
} SpriteID;

extern void AssetsLoad();
extern void AssetsUnload();
extern void SpritesLoad();

extern Texture2D textures[MAX_TEXTURES];
extern Sprite sprites[MAX_SPRITES];
extern Music playlist[MAX_BGM];             

#endif
