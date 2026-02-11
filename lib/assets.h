#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "raymath.h"

// Audio : background music for now
typedef enum {
    BGM_FAST_DANGER,
    BGM_QUIRKY_GOOFY,
    BGM_TO_BE_NAMED, // maybe won't be used
    BGM_WAITING,
    MAX_BGM // Pour savoir combien on en a
} BGMId;

enum {
    BULLET_SPRITESHEET,
    BOSS_SPRITESHEET,
    ANIM_TEST_SPRITESHEET,
    REIMU_SPRITESHEET,
    HITBOX_SPRITESHEET
};

void AssetsLoad();
void AssetsUnload();

#endif