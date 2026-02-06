#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "raymath.h"

#define MAX_TEXTURES 200

enum {
    BULLET_SPRITESHEET,
    BOSS_SPRITESHEET,
    ANIM_TEST_SPRITESHEET,
    REIMU_SPRITESHEET,
    HITBOX_SPRITESHEET
};

void AssetsLoad();
void AssetsUnload();

extern Texture2D textures[MAX_TEXTURES];

#endif